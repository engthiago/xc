# -*- coding: utf-8 -*-
''' Example from page 4.7 from the book "Structural Wood Design: A 
    Practice-Oriented Approach Using the ASD Method. Abi Aghayere and 
    Jason Vigil Copyright © 2007 John Wiley & Sons, Inc. 2007
    ISBN: 978-0-470-05678-3
'''

from __future__ import print_function
from __future__ import division

__author__= "Luis Claudio Pérez Tato (LCPT)"
__copyright__= "Copyright 2022, LCPT"
__license__= "GPL"
__version__= "3.0"
__email__= "l.pereztato@gmail.com"

import math
import os
import geom
import xc
from materials.awc_nds import structural_panels
from model import predefined_spaces
from actions import load_cases
from actions import combinations as combs
from postprocess.config import default_config
from postprocess import limit_state_data as lsd
from materials.awc_nds import AWCNDS_limit_state_checking as nds

# Units
inchToMeter= 2.54/100.0
footToMeter= 0.3048
poundToN= 4.44822
psiToPa= 6894.76
psfToPa= 47.88026

# Problem type
steelBeam= xc.FEProblem()
steelBeam.title= 'Example 4.7 Structural Wood Design'
preprocessor= steelBeam.getPreprocessor
nodes= preprocessor.getNodeHandler
modelSpace= predefined_spaces.StructuralMechanics3D(nodes)

#Materials
## Materials LVL 2900Fb (page 4 of the PDF document from "SolidStart")
beamSection= structural_panels.LVLHeaderSections['3.5x11-1/4']
xcSection= beamSection.defElasticShearSection3d(preprocessor)

# Model geometry

## Points.
span= 15*footToMeter
pointHandler= preprocessor.getMultiBlockTopology.getPoints
p0= pointHandler.newPoint(geom.Pos3d(0.0,0.0,0.0))
p1= pointHandler.newPoint(geom.Pos3d(span,0.0,0.0))

## Lines
lineHandler= preprocessor.getMultiBlockTopology.getLines
l1= lineHandler.newLine(p0.tag,p1.tag)
l1.nDiv= 10

# Mesh generation.
modelSpace= predefined_spaces.StructuralMechanics3D(nodes)
trfs= preprocessor.getTransfCooHandler
lin= trfs.newLinearCrdTransf3d('lin')
lin.xzVector= xc.Vector([0,1,0])
seedElemHandler= preprocessor.getElementHandler.seedElemHandler
seedElemHandler.defaultMaterial= xcSection.name
seedElemHandler.defaultTransformation= lin.name
elem= seedElemHandler.newElement("ElasticBeam3d")

xcTotalSet= modelSpace.getTotalSet()
mesh= xcTotalSet.genMesh(xc.meshDir.I)

# Constraints
modelSpace.fixNode('000_0FF',p0.getNode().tag)
modelSpace.fixNode('F00_FFF',p1.getNode().tag)

# Actions
carriedSpan= 16/2.0*footToMeter
deadLoad= 15*psfToPa*carriedSpan
liveLoad= 40*psfToPa*carriedSpan

loadCaseManager= load_cases.LoadCaseManager(preprocessor)
loadCaseNames= ['deadLoad','liveLoad']
loadCaseManager.defineSimpleLoadCases(loadCaseNames)

## Dead load.
deadLoadVector= xc.Vector([0.0,0.0,-deadLoad])
cLC= loadCaseManager.setCurrentLoadCase('deadLoad')
for e in xcTotalSet.elements:
    e.vector3dUniformLoadGlobal(deadLoadVector)
  
## Live load.
liveLoadVector= xc.Vector([0.0,0.0,-liveLoad])
cLC= loadCaseManager.setCurrentLoadCase('liveLoad')
for e in xcTotalSet.elements:
    e.vector3dUniformLoadGlobal(liveLoadVector)

## Load combinations
combContainer= combs.CombContainer()
### Ultimate limit state.
combContainer.ULS.perm.add('combULS01','1.0*deadLoad+1.0*liveLoad')
### Dump combination definition into XC.
combContainer.dumpCombinations(preprocessor)

# Compute internal forces.

## Setup working directory.
cfg= default_config.get_temporary_env_config()
lsd.LimitStateData.envConfig= cfg

## Set combinations to compute.
loadCombinations= preprocessor.getLoadHandler.getLoadCombinations

## Limit states to calculate internal forces for.
limitStates= [nds.normalStressesResistance, # Normal stresses resistance.
nds.shearResistance, # Shear stresses resistance (IS THE SAME AS NORMAL STRESSES, THIS IS WHY IT'S COMMENTED OUT).
]

## Create NDS Member objects.
ndsCalcSet= modelSpace.defSet('ndsCalcSet') # Elements to be checked as NDS members.
ndsMembers= list() # NDS members list.
for l in xcTotalSet.getLines:
    Lx= 0.5 # continuously braced.
    member= nds.Member(name= l.name, section= beamSection, unbracedLengthX= Lx, lstLines= [l])
    #member.setControlPoints()
    member.installULSControlRecorder(recorderType="element_prop_recorder", calcSet= ndsCalcSet)
    ndsMembers.append(member)
    
## Compute internal forces for each combination
for ls in limitStates:
    ls.analyzeLoadCombinations(combContainer, ndsCalcSet)
# Limit state to check.
limitState= limitStates[1]
# Build controller.
controller= limitState.getController()
# Perform checking.
average= limitState.check(setCalc=ndsCalcSet, appendToResFile='Y', listFile='N', calcMeanCF='Y', controller= controller)

ratio= ((average[0]-0.22055129655618932)/0.22055129655618932)**2
ratio+= ((average[1]-0.22055129655619)/0.22055129655619)**2
ratio0= math.sqrt(ratio)

# Label to get the property that contains the control vars.
label1= controller.limitStateLabel+controller.getSectionLabel(0)
label2= controller.limitStateLabel+controller.getSectionLabel(1)
maxCF= 0.0
# Reference value obtained from verification test: lvl_beam_test_01.py 
refMaxCF= 0.441117667235691
for e in ndsCalcSet.elements:
    CF= e.getProp(label1).CF
    maxCF= max(maxCF, CF)
    CF= e.getProp(label2).CF
    maxCF= max(maxCF, CF)

ratio1= abs(maxCF-refMaxCF)/refMaxCF
'''
print('average= ', average)
print('ratio0= ', ratio0)
print('max. CF:', maxCF)
print('ref. max. CF:', refMaxCF)
print('ratio1= ', ratio1)
'''

cfg.cleandirs() # Clean after yourself.
from misc_utils import log_messages as lmsg
fname= os.path.basename(__file__)
if((ratio0<1e-8) and (ratio1<1e-4)):
    print('test '+fname+': ok.')
else:
    lmsg.error(fname+' ERROR.')
    
# #########################################################
# # Graphic stuff.
# from postprocess import output_handler
# oh= output_handler.OutputHandler(modelSpace)

# oh.displayBeamResult(attributeName= limitState.label, itemToDisp='CF', beamSetDispRes=ndsCalcSet, setToDisplay=xcTotalSet)

