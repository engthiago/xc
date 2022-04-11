# -*- coding: utf-8 -*-
# Home made test

from __future__ import print_function

__author__= "Luis C. Pérez Tato (LCPT) and Ana Ortega (AO_O)"
__copyright__= "Copyright 2015, LCPT and AO_O"
__license__= "GPL"
__version__= "3.0"
__email__= "l.pereztato@ciccp.es ana.ortega@ciccp.es"

import geom
import xc
from model import predefined_spaces
from materials import typical_materials
import math

Es= 2.1e11
n= 15.0
Ec= Es/n
areaFi16= 2.01e-4

feProblem= xc.FEProblem()
preprocessor=  feProblem.getPreprocessor

steel= typical_materials.defElasticMaterial(preprocessor, "steel",Es)

sectionGeometryTest= preprocessor.getMaterialHandler.newSectionGeometry("sectionGeometryTest")
reinforcement= sectionGeometryTest.getReinfLayers
reinforcementA= reinforcement.newStraightReinfLayer("steel")
reinforcementA.numReinfBars= 2
reinforcementA.barDiam= 16e-3
reinforcementA.barArea= areaFi16
reinforcementA.p1= geom.Pos2d(0.05,0.95) # bottom layer.
reinforcementA.p2= geom.Pos2d(0.05,0.05)
reinforcementB= reinforcement.newStraightReinfLayer("steel")
reinforcementB.numReinfBars= 2
reinforcementB.barDiam= 16e-3
reinforcementB.barArea= areaFi16
reinforcementB.p1= geom.Pos2d(0.95,0.95) # bottom layer.
reinforcementB.p2= geom.Pos2d(0.95,0.05)

nRebars= reinforcement.getNumReinfBars
area= sectionGeometryTest.getAreaHomogenizedSection(Ec)
G= sectionGeometryTest.getCenterOfMassHomogenizedSection(Ec)
Iy= sectionGeometryTest.getIyHomogenizedSection(Ec)
Iz= sectionGeometryTest.getIzHomogenizedSection(Ec)
Pyz= sectionGeometryTest.getPyzHomogenizedSection(Ec)

areaTeor= (n*4*areaFi16)
yGTeor= 0.5
zGTeor= 0.5
iTeor= (n*4*math.pi/4*(8e-3)**4+(0.45)**2*areaTeor)

ratio1= ((area-areaTeor)/areaTeor)
ratio2= ((G[0]-yGTeor)/yGTeor)
ratio3= ((G[1]-zGTeor)/zGTeor)
ratio4= ((Iy-iTeor)/iTeor)
ratio5= ((Iz-iTeor)/iTeor)
ratio6= ((nRebars-4)/4)

''' 
print("area= ",area)
print("areaTeor= ",areaTeor)
print("G= ",G)
print("Iy= ",Iy)
print("Iz= ",Iz)
print("iTeor= ",iTeor)
print("Pyz= ",Pyz)
print("ratio1= ",ratio1)
print("ratio2= ",ratio2)
print("ratio3= ",ratio3)
print("ratio4= ",ratio4)
print("ratio5= ",ratio5)
 '''

import os
from misc_utils import log_messages as lmsg
fname= os.path.basename(__file__)
if (abs(ratio1)<1e-15) & (abs(ratio2)<1e-15) & (abs(ratio3)<1e-15) & (abs(ratio4)<1e-7) & (abs(ratio5)<1e-7) & (abs(ratio6)<1e-17) :
    print("test_geom_caract_mec_02: ok.")
else:
    print("test_geom_caract_mec_02: ERROR.")
