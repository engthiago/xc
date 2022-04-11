# -*- coding: utf-8 -*-
from __future__ import print_function

import geom
import xc
import math
import os

__author__= "Luis C. Pérez Tato (LCPT)"
__copyright__= "Copyright 2014, LCPT"
__license__= "GPL"
__version__= "3.0"
__email__= "l.pereztato@gmail.com"

feProblem= xc.FEProblem()
preprocessor=  feProblem.getPreprocessor
# Defining nodes

points= preprocessor.getMultiBlockTopology.getPoints
pt1= points.newPoint(geom.Pos3d(1.0, 0.0, 0.0) )
pt2= points.newPoint(geom.Pos3d(0.0, 1.0, 0.0) )
pt3= points.newPoint(geom.Pos3d(0.0, 0.0, 1.0) )
plane= geom.Plane3d(pt1.getPos,pt2.getPos,pt3.getPos)

reflex1= xc.Reflection(geom.Reflection3d(plane))

pt4= points.newPoint(3, reflex1.getTrfPos(geom.Pos3d(2/3.0, 2/3.0, 2/3.0)))

x4= pt4.getPos.x
y4= pt4.getPos.y
z4= pt4.getPos.z

#print("x4= ", x4)

import os
from misc_utils import log_messages as lmsg
fname= os.path.basename(__file__)
if (abs(x4)<1e-12) & (abs(y4)<1e-12) & (abs(z4)<1e-12):
    print('test '+fname+': ok.')
else:
    lmsg.error(fname+' ERROR.')
