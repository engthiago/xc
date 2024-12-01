# -*- coding: utf-8 -*-
from __future__ import print_function

'''Limit state controller for shear.'''

__author__= "Luis C. Pérez Tato (LCPT) and Ana Ortega (AO_O)"
__copyright__= "Copyright 2015, LCPT and AO_O"
__license__= "GPL"
__version__= "3.0"
__email__= "l.pereztato@gmail.com ana.ortega@ciccp.es"

import math
import xc
from solution import predefined_solutions
from model import predefined_spaces
from materials.aci import ACI_materials
from materials.aci import ACI_limit_state_checking
from materials.sections import section_properties
from actions import combinations as combs
from postprocess import limit_state_data as lsd
from postprocess import RC_material_distribution
from postprocess.config import default_config
from materials.sections.fiber_section import def_simple_RC_section
from postprocess import element_section_map
import os
import logging

from misc_utils import log_messages as lmsg

#Hide INFO messages from modules.
rootLogger = logging.getLogger()
rootLogger.setLevel(logging.ERROR)

# Geometry
L= 1.0 # Bar length (m)

feProblem= xc.FEProblem()
preprocessor=  feProblem.getPreprocessor
nodes= preprocessor.getNodeHandler

# Materials
sectionGeometry= section_properties.RectangularSection("test",b=.3,h=.5)
concr= ACI_materials.c4000
concr.gmmC= 1/0.75
section= concr.defElasticShearSection3d(preprocessor, sectionGeometry)

# Problem type
modelSpace= predefined_spaces.StructuralMechanics3D(nodes)

#Mesh.
n1= nodes.newNodeXYZ(0,0.0,0.0)
n2= nodes.newNodeXYZ(L/2.0,0.0,0.0)
n3= nodes.newNodeXYZ(L,0.0,0.0)

lin= modelSpace.newLinearCrdTransf("lin",xc.Vector([0,1,0]))

elements= preprocessor.getElementHandler
elements.defaultTransformation= lin.name
elements.defaultMaterial= section.name
e1= elements.newElement("ElasticBeam3d",xc.ID([n1.tag,n2.tag]))
e2= elements.newElement("ElasticBeam3d",xc.ID([n2.tag,n3.tag]))

#Constraints.
modelSpace.fixNode000_000(n1.tag)

#Loads.
Fx= -400e3 # Axial force for shear checking.
Fz= 1e3 # Bending moment force for shear checking.
Fy= 1e5 # Bending moment force for shear checking.
loadHandler= preprocessor.getLoadHandler
lPatterns= loadHandler.getLoadPatterns
#Load modulation.
ts= lPatterns.newTimeSeries("constant_ts","ts")
lPatterns.currentTimeSeries= "ts"
#Load case definition
lp0= lPatterns.newLoadPattern("default","lp0")
lp0.newNodalLoad(n3.tag,xc.Vector([Fx,Fy,Fz,0,0,0]))
#We add the load case to domain.
lPatterns.addToDomain(lp0.getName())

# # Solution
# analysis= predefined_solutions.simple_static_linear(feProblem)
# result= analysis.analyze(1)

# Load combinations
combContainer= combs.CombContainer()
combContainer.ULS.perm.add('allLoads', '1.0*lp0')
totalSet= preprocessor.getSets.getSet('total')
cfg= default_config.get_temporary_env_config()
lsd.LimitStateData.envConfig= cfg
# Limit state to check.
limitState= ACI_limit_state_checking.shearResistance
## Save internal forces.
limitState.analyzeLoadCombinations(combContainer,totalSet) 

# Define available sections for the elements (spatial distribution of RC sections).
# It refers to the reinforced concrete sections associated with the element
# (i.e. for shell elements we typically define two RC sections, one for each
# main direction; in the case of beam elements the most common way is to define
# RC sections in the front and back ends of the elements)
reinfConcreteSectionDistribution= RC_material_distribution.RCMaterialDistribution()
sections= reinfConcreteSectionDistribution.sectionDefinition #creates an RC sections container

#Generic layers (rows of rebars). Other instance variables that we can define
#for ReinfRows are coverLat and nRebars.If we define nRebars that
#value overrides the rebarsSpacing
barArea= 4e-4
barDiameter= math.sqrt(barArea)/math.pi

reinfLayer= def_simple_RC_section.ReinfRow(rebarsDiam= barDiameter,areaRebar= barArea,rebarsSpacing=0.075,width=0.25,nominalCover=0.050)

#instances of element_section_map.RCSlabBeamSection that defines the
#variables that make up THE TWO reinforced concrete sections in the two
#reinforcement directions of a slab or the front and back ending sections
#of a beam element
reinfSteel= ACI_materials.A615G60
reinfSteel.gammaS= 1.0/0.75
beamRCsect= element_section_map.RCSlabBeamSection(name='beamRCsect',sectionDescr='beam section',concrType=concr, reinfSteelType=reinfSteel,width= sectionGeometry.b,depth= sectionGeometry.h)
beamRCsect.dir1PositvRebarRows= def_simple_RC_section.LongReinfLayers([reinfLayer])
beamRCsect.dir1NegatvRebarRows= def_simple_RC_section.LongReinfLayers([reinfLayer])
beamRCsect.dir2PositvRebarRows= def_simple_RC_section.LongReinfLayers([reinfLayer])
beamRCsect.dir2NegatvRebarRows= def_simple_RC_section.LongReinfLayers([reinfLayer])
sections.append(beamRCsect)

# Spatial distribution of reinforced concrete
# sections (assign RC sections to elements).
reinfConcreteSectionDistribution.assign(elemSet=totalSet.getElements,setRCSects=beamRCsect)

# Checking shear.
## Build controller.
controller= limitState.getController()

limitState.outputDataBaseFileName= 'resVerif'
## Perform checking.
feProblem.errFileName= "/tmp/erase.err" # Ignore warning messagess about maximum error in computation of the interaction diagram.
meanCFs= limitState.check(setCalc= None, crossSections= reinfConcreteSectionDistribution, listFile='N',calcMeanCF='Y',threeDim= True, controller= controller)
feProblem.errFileName= "cerr" # From now on display errors if any.

# Check results.
meanCF0Teor= 0.932218036004
ratio1= abs(meanCFs[0]-meanCF0Teor)/meanCF0Teor
meanCF1Teor= 0.932218036004
ratio2= abs(meanCFs[1]-meanCF1Teor)/meanCF1Teor

'''
print("meanCFs[0]= ", meanCFs[0])
print("ratio1= ",ratio1)
print("meanCFs[1]= ", meanCFs[1])
print("ratio2= ",ratio2)
'''

cfg.cleandirs() # Clean after yourself.
# Show logging messages.
fname= os.path.basename(__file__)
if((ratio1<0.01) and (ratio2<0.01)):
    print('test '+fname+': ok.')
else:
    lmsg.error(fname+' ERROR.')
    
# Report sections.
# reinfConcreteSectionDistribution.pdfReport(preprocessor= preprocessor, showPDF= True, keepPDF= False)
