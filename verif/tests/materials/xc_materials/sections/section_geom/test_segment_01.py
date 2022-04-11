# -*- coding: utf-8 -*-
''' Home made test.'''
from __future__ import print_function

import geom
import xc
from model import predefined_spaces
from materials import typical_materials
import math

__author__= "Luis C. Pérez Tato (LCPT) and Ana Ortega (AOO)"
__copyright__= "Copyright 2015, LCPT and AOO"
__license__= "GPL"
__version__= "3.0"
__email__= "l.pereztato@gmail.com"

fy= 2600 # Yield stress of the material expressed in kp/cm2.
E= 2.1e6 # Young modulus of the material en kp/cm2.
xA= 1/3.0
yA= 3/4.0
feProblem= xc.FEProblem()
preprocessor=  feProblem.getPreprocessor
epp= typical_materials.defElasticPPMaterial(preprocessor, "epp",E,fy,-fy)
sectionGeometryTest= preprocessor.getMaterialHandler.newSectionGeometry("sectionGeometryTest")
sectionGeometryTest.tagSpot= 1
spot1= sectionGeometryTest.newSpot(geom.Pos2d(xA,yA))
x1= spot1.pos.x
y1= spot1.pos.y
spot2= sectionGeometryTest.newSpot(geom.Pos2d(0,0))
spot3= sectionGeometryTest.newSpot(geom.Pos2d(10,10))
dist= sectionGeometryTest.distSpots(1,2)
linea1= sectionGeometryTest.newSegment(2,1)
lengthL1= linea1.getLength()
linea2= sectionGeometryTest.newSegment(1,3)
nl1= spot1.numLines
nl2= spot2.numLines
nl3= spot3.numLines


ratio1= ((xA-x1)/xA)
ratio2= ((yA-y1)/yA)
ratio3= (dist-math.sqrt((x1)**2+(y1)**2))
ratio4= (dist-lengthL1)
ratio5= (nl1-2)
ratio6= (nl2-1)
ratio7= (nl3-1)

'''
print("xA= ", xA )
print("x1= ", x1 )
print("ratio1= ",ratio1)
print("ratio2= ",ratio2)
print("ratio3= ",ratio3)
'''

import os
from misc_utils import log_messages as lmsg
fname= os.path.basename(__file__)
if (abs(ratio1)<1e-12) & (abs(ratio2)<1e-12) & (abs(ratio3)<1e-12) & (abs(ratio4)<1e-12) & (abs(ratio5)<1e-12) & (abs(ratio6)<1e-12) & (abs(ratio7)<1e-12):
    print('test '+fname+': ok.')
else:
    lmsg.error(fname+' ERROR.')
