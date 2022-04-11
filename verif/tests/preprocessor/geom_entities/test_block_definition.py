# -*- coding: utf-8 -*-
'''Home cooked test.'''
from __future__ import print_function

__author__= "Luis C. Pérez Tato (LCPT) and Ana Ortega (AOO)"
__copyright__= "Copyright 2015, LCPT and AOO"
__license__= "GPL"
__version__= "3.0"
__email__= "l.pereztato@gmail.com"

import geom
import xc
from model import predefined_spaces
from materials import typical_materials
# from postprocess import output_handler

# Problem type
feProblem= xc.FEProblem()
preprocessor=  feProblem.getPreprocessor

# Geometry

width= 6*0.0254
length= 4*0.3048
height= 12*0.0254

## Block topology

points= preprocessor.getMultiBlockTopology.getPoints
pt1= points.newPoint(geom.Pos3d(0.0,0.0,0.0))
pt2= points.newPoint(geom.Pos3d(width,0.0,0.0))
pt3= points.newPoint(geom.Pos3d(width,length,0.0))
pt4= points.newPoint(geom.Pos3d(0.0,length,0.0))
pt5= points.newPoint(geom.Pos3d(0.0,0.0,height))
pt6= points.newPoint(geom.Pos3d(width,0.0,height))
pt7= points.newPoint(geom.Pos3d(width,length,height))
pt8= points.newPoint(geom.Pos3d(0.0,length,height))

bodies= preprocessor.getMultiBlockTopology.getBodies
b1= bodies.newBlockPts(pt1.tag,pt2.tag,pt3.tag,pt4.tag,pt5.tag,pt6.tag,pt7.tag,pt8.tag)

k_points_ref= [(1, [0,3,2,1]),(2, [0,1,5,4]),(3, [1,2,6,5]),(4, [2,3,7,6]),(5, [0,4,7,3]),(6, [4,5,6,7])]
k_points= list()

for i in range(1,7):
    face= b1.getFace(i)
    k_points.append((i,face.getSurface().getKPoints()))

err= 0
for kp,kp_ref in zip(k_points,k_points_ref):
    err+= (kp[0]-kp_ref[0])**2
    tags= kp[1]
    tags_ref= kp_ref[1]
    for t,t_ref in zip(tags,tags_ref):
        err+= (t-t_ref)**2
    #print(err)


import os
from misc_utils import log_messages as lmsg
fname= os.path.basename(__file__)
if(err==0) :
    print('test '+fname+': ok.')
else:
    lmsg.error(fname+' ERROR.')
