# -*- coding: utf-8 -*-
''' Trivial verification test for sliding vector system 
    load distribution.'''

from __future__ import print_function

__author__= "Luis C. Pérez Tato (LCPT) and Ana Ortega (AO_O)"
__copyright__= "Copyright 2015, LCPT and AO_O"
__license__= "GPL"
__version__= "3.0"
__email__= "l.pereztato@ciccp.es ana.ortega@ciccp.es"

import geom
import math

# Sliding vector system.
O= geom.Pos3d(0.0,1.0,0.0)
V= geom.Vector3d(1.0,0.0,0.0)
M= geom.Vector3d(0.0,0.0,0.0)
svs1= geom.SlidingVectorsSystem3d(O,V,M)

# Points and weights.
points= [geom.Pos3d(-1.0,0.0,0.0), geom.Pos3d(1.0,0.0,0.0)]
weights= [1.0,1.0]

vectors1= svs1.distribute(points,weights)

error= (vectors1[0].getVector3d()-geom.Vector3d(0.5,0.5,0.0)).getModulus()
error+= (vectors1[1].getVector3d()-geom.Vector3d(0.5,-0.5,0.0)).getModulus()

vectors2= svs1.distribute(points)
error+= (vectors2[0].getVector3d()-geom.Vector3d(0.5,0.5,0.0)).getModulus()
error+= (vectors2[1].getVector3d()-geom.Vector3d(0.5,-0.5,0.0)).getModulus()


'''
print(vectors1[0])
print(vectors1[1])
print(vectors2[0])
print(vectors2[1])
print(points)
print(weights, len(weights))
print(error)
'''

import os
from misc_utils import log_messages as lmsg
fname= os.path.basename(__file__)
if error<1e-12:
    print('test: '+fname+': ok.')
else:
    lmsg.error('test: '+fname+' ERROR.')

