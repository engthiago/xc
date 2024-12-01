# -*- coding: utf-8 -*-
# Home made test.

from __future__ import division
from __future__ import print_function

__author__= "Ana Ortega (AO_O) and Luis C. Pérez Tato (LCPT)"
__copyright__= "Copyright 2020, AO_O and LCPT"
__license__= "GPL"
__version__= "3.0"
__email__= " ana.Ortega.Ort@gmail.com, l.pereztato@gmail.com"

import math
import os
import geom
import xc
from materials.astm_aisc import ASTM_materials
from materials.astm_aisc import AISC_limit_state_checking as aisc
from model import predefined_spaces
from actions import load_cases
from actions import combinations as combs
# from postprocess import output_handler
from postprocess import limit_state_data as lsd
from postprocess.config import default_config

from misc_utils import units_utils


MPa2ksi= 0.145038
kN2kips= 0.2248
kip2kN= 1.0/kN2kips

m2Toin2= 1.0/units_utils.inchToMeter**2

# Problem type
steelBeam= xc.FEProblem()
steelBeam.title= 'Example H.3'
preprocessor= steelBeam.getPreprocessor
nodes= preprocessor.getNodeHandler

#Materials
## Steel material
steel= ASTM_materials.A500
steel.gammaM= 1.00
## Profile geometry
shape= ASTM_materials.HSSShape(steel,'HSS6X4X3/8')
xcSection= shape.defElasticSection1d(preprocessor)

# Model geometry

## Points.
span= 14.0*units_utils.footToMeter
pointHandler= preprocessor.getMultiBlockTopology.getPoints
p0= pointHandler.newPoint(geom.Pos3d(0.0,0.0,0.0))
p1= pointHandler.newPoint(geom.Pos3d(span,0.0,0.0))

## Lines
lineHandler= preprocessor.getMultiBlockTopology.getLines
l1= lineHandler.newLine(p0.tag,p1.tag)
l1.nDiv= 1

# Mesh
modelSpace= predefined_spaces.StructuralMechanics3D(nodes)
trfs= preprocessor.getTransfCooHandler
seedElemHandler= preprocessor.getElementHandler.seedElemHandler
seedElemHandler.defaultMaterial= xcSection.name
seedElemHandler.dimElem= 3 #Bars defined in a three dimensional space.
elem= seedElemHandler.newElement("TrussSection")

xcTotalSet= preprocessor.getSets.getSet('total')
mesh= xcTotalSet.genMesh(xc.meshDir.I)

# Constraints
modelSpace.fixNode('000_000',p0.getNode().tag)
modelSpace.fixNode('F00_000',p1.getNode().tag)

# Actions
loadCaseManager= load_cases.LoadCaseManager(preprocessor)
loadCaseNames= ['deadLoad','liveLoad']
loadCaseManager.defineSimpleLoadCases(loadCaseNames)

## Dead load.
cLC= loadCaseManager.setCurrentLoadCase('deadLoad')
p1.getNode().newLoad(xc.Vector([379.58e3,0.0,0.0,0.0,0.0,0.0]))
  
## Live load.
cLC= loadCaseManager.setCurrentLoadCase('liveLoad')
p1.getNode().newLoad(xc.Vector([427.03e3,0.0,0.0,0.0,0.0,0.0]))

## Load combinations
combContainer= combs.CombContainer()

### Serviceability limit states.
combContainer.SLS.qp.add('combSLS01', '1.0*liveLoad')

### Ultimate limit state.
combContainer.ULS.perm.add('combULS01','1.2*deadLoad+1.6*liveLoad')

### Dump combination definition into XC.
combContainer.dumpCombinations(preprocessor)

xcTotalSet= modelSpace.getTotalSet()

# Compute internal forces.

## Setup working directory.
cfg= default_config.get_temporary_env_config()
lsd.LimitStateData.envConfig= cfg

## Set combinations to compute.
loadCombinations= preprocessor.getLoadHandler.getLoadCombinations

## Limit states to calculate internal forces for.
limitStates= [aisc.normalStressesResistance, # Normal stresses resistance.
aisc.shearResistance, # Shear stresses resistance (IS THE SAME AS NORMAL STRESSES, THIS IS WHY IT'S COMMENTED OUT).
] 

## Create AISC Member objects.
lines= [l1]
Lx= l1.getLength()
member= aisc.Member(name= l1.name, section= shape,unbracedLengthX= Lx, lstLines= [l1])

aiscCalcSet= modelSpace.defSet('aiscCalcSet') # Elements to be checked as AISC members.
aiscMembers= [member] # AISC members.
for m in aiscMembers:
    m.installULSControlRecorder(recorderType="element_prop_recorder", calcSet= aiscCalcSet)
    
## Compute internal forces for each combination
for ls in limitStates:
    ls.analyzeLoadCombinations(combContainer,aiscCalcSet, bucklingMembers= aiscMembers)

# Check limit state.
## Limit state to check.
limitState= limitStates[0]
## Build controller.
controller= limitState.getController()
## Perform checking.
average= limitState.check(setCalc=aiscCalcSet, appendToResFile='Y', listFile='N', calcMeanCF='Y', controller= controller)

# Check results.
ratio= ((average[0]-1.0067001969482556)/1.0067001969482556)**2
ratio+= ((average[1]-1.0067001969482556)/1.0067001969482556)**2
ratio= math.sqrt(ratio)

'''
print('average= ', average)
print('ratio= ', ratio)
'''

cfg.cleandirs() # Clean after yourself.
from misc_utils import log_messages as lmsg
fname= os.path.basename(__file__)
if(ratio<1e-8):
    print('test '+fname+': ok.')
else:
    lmsg.error(fname+' ERROR.')

# #########################################################
# # Graphic stuff.
# oh= output_handler.OutputHandler(modelSpace)

# oh.displayElementValueDiagram('chiN', setToDisplay= aiscCalcSet)

# oh.displayBeamResult(attributeName= limitState.label, itemToDisp='CF', beamSetDispRes=aiscCalcSet, setToDisplay=xcTotalSet)
