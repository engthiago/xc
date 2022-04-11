# -*- coding: utf-8 -*-

from __future__ import print_function

__author__= "Luis C. Pérez Tato (LCPT) and Ana Ortega (AO_O)"
__copyright__= "Copyright 2015, LCPT and AO_O"
__license__= "GPL"
__version__= "3.0"
__email__= "l.pereztato@ciccp.es ana.ortega@ciccp.es"

import geom
import math

u=geom.Vector2d(1,0)
v=geom.Vector2d(1,1)
alpha= v.getAngle(u)

ratio1=math.degrees(alpha)-45

v.x=0
alpha= v.getAngle(u)
ratio2=math.degrees(alpha)-90
v.x=-1
alpha= v.getAngle(u)
ratio3=math.degrees(alpha)-135
v.y=0
alpha= v.getAngle(u)
ratio4=math.degrees(alpha)-180
v.y=-1
alpha= v.getAngle(u)
ratio5=math.degrees(alpha)-225
v.x=0 
alpha= v.getAngle(u)
ratio6=math.degrees(alpha)-270
v.x=1 
alpha= v.getAngle(u)
ratio7=math.degrees(alpha)-315
v.y=0 
alpha= v.getAngle(u)
ratio8=math.degrees(alpha)

import os
from misc_utils import log_messages as lmsg
fname= os.path.basename(__file__)
if math.fabs(ratio1)<1e-10 and math.fabs(ratio2)<1e-10 and math.fabs(ratio3)<1e-10 and math.fabs(ratio4)<1e-10 and math.fabs(ratio5)<1e-10 and math.fabs(ratio6)<1e-10 and math.fabs(ratio7)<1e-10 and math.fabs(ratio8)<1e-10:
    print('test: '+fname+': ok.')
else:
    lmsg.error('test: '+fname+' ERROR.')











