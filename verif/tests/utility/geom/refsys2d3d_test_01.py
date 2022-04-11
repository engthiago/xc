# -*- coding: utf-8 -*-
from __future__ import print_function

__author__= "Luis C. Pérez Tato (LCPT) and Ana Ortega (AO_O)"
__copyright__= "Copyright 2015, LCPT and AO_O"
__license__= "GPL"
__version__= "3.0"
__email__= "l.pereztato@ciccp.es ana.ortega@ciccp.es"

import math
import geom
p1=geom.Pos3d(1,1,1)
vI=geom.Vector3d(1,1,1)
ref= geom.Ref2d3d(p1,vI)


v= geom.Vector3d(1,1,1)
vTrf= ref.getLocalCoordinates(v)
vTrfTeor= geom.Vector2d(math.sqrt(3.0),0)
ratio= (vTrf-vTrfTeor).getModulus()
p0= ref.getGlobalPosition(geom.Pos2d(0.0,0.0))
ratio2= (p1-p0).getModulus()

p0= geom.Pos3d(0,0,0)
pA= geom.Pos3d(1,0,0)
pB= geom.Pos3d(0,1,0)

ref= geom.Ref2d3d(p0,pA,pB)
ref.Org+=geom.Vector3d(1,1,1)
ratio3= p1.dist(ref.Org)

'''
print("vTrf= ", vTrf)
print("ratio= ", ratio)
print("p0= ", p0)
print("ratio2= ", ratio2)
print(ref.Org)
print("ratio3= ", ratio3)
'''

import os
from misc_utils import log_messages as lmsg
fname= os.path.basename(__file__)
if (abs(ratio)<1e-12 and abs(ratio2)<1e-12 and abs(ratio3)<1e-15):
    print('test: '+fname+': ok.')
else:
    lmsg.error('test: '+fname+' ERROR.')










