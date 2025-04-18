# -*- coding: utf-8 -*-
'''Verification test of the addFiber function.'''

from __future__ import print_function

__author__= "Luis C. Pérez Tato (LCPT) and Ana Ortega (A_OO)"
__copyright__= "Copyright 2015, LCPT and AO_O"
__license__= "GPL"
__version__= "3.0"
__email__= "l.pereztato@gmail.com ana.ortega.ort@gmal.com"

import xc
from model import predefined_spaces
from materials import typical_materials

yF= 2.0
zF= 3.0
F= 2100
MomY= F*zF
MomZ= F*yF
MomYFibersDq= 0.0
MomZFibersDq= 0.0
Es= 2.1e6
Area= 1.0
eps= F/(Area*Es)

# Problem type
feProblem= xc.FEProblem()
feProblem.logFileName= "/tmp/erase.log" # Ignore warning messages
preprocessor=  feProblem.getPreprocessor
nodes= preprocessor.getNodeHandler
modelSpace= predefined_spaces.StructuralMechanics3D(nodes)


# Define materials
A= 0.0
yG= 0.0
zG= 0.0

IEA= 0.0; IK12= 0.0; IK13= 0.0
IK21= 0.0; IEIy= 0.0; IK23= 0.0
IK31= 0.0; IK32= 0.0; IEIz= 0.0

TEA= 0.0; TK12= 0.0; TK13= 0.0
TK21= 0.0; TEIy= 0.0; TK23= 0.0
TK31= 0.0; TK32= 0.0; TEIz= 0.0

elast0= typical_materials.defElasticMaterial(preprocessor, "elast0",Es)
# Sections
fiberSectionTest= preprocessor.getMaterialHandler.newMaterial("fiber_section_3d","fiberSectionTest")
fiberSectionTest.addFiber("elast0",Area,xc.Vector([yF,zF]))

A= fiberSectionTest.getArea
yG= fiberSectionTest.getCenterOfMassY()
zG= fiberSectionTest.getCenterOfMassZ()
initialTangentStiffness= fiberSectionTest.getInitialTangentStiffness()
IEA= initialTangentStiffness.at(1,1)
IK12= initialTangentStiffness.at(1,2)
IK13= initialTangentStiffness.at(1,3)
IK21= initialTangentStiffness.at(2,1)
IEIy= initialTangentStiffness.at(2,2)
IK23= initialTangentStiffness.at(2,3)
IK31= initialTangentStiffness.at(3,1)
IK32= initialTangentStiffness.at(3,2)
IEIz= initialTangentStiffness.at(3,3)

fiberSectionTest.setTrialSectionDeformation(xc.Vector([eps,0.0,0.0]))
tangentStiffness= fiberSectionTest.getTangentStiffness()
TEA=  tangentStiffness.at(1,1)
TK12=  tangentStiffness.at(1,2)
TK13=  tangentStiffness.at(1,3)
TK21=  tangentStiffness.at(2,1)
TEIy=  tangentStiffness.at(2,2)
TK23=  tangentStiffness.at(2,3)
TK31=  tangentStiffness.at(3,1)
TK32=  tangentStiffness.at(3,2)
TEIz=  tangentStiffness.at(3,3)

R= fiberSectionTest.getStressResultant()
fibers= fiberSectionTest.getFibers()
MomYFibersDq= fibers.getMy(0.0)
MomZFibersDq= fibers.getMz(0.0)


ratio1= R[0]-F
ratio2= R[1]-MomZ
ratio3= R[2]-MomY
ratio4= R[1]-MomZFibersDq
ratio5= R[2]-MomYFibersDq

import os
from misc_utils import log_messages as lmsg
fname= os.path.basename(__file__)
if (abs(ratio1)<1e-15) & (abs(ratio2)<1e-15) & (abs(ratio3)<1e-15) & (abs(ratio4)<1e-15) & (abs(ratio5)<1e-15) :
    print('test '+fname+': ok.')
else:
    lmsg.error(fname+' ERROR.')
