# -*- coding: utf-8 -*-
''' Home made test.'''

from __future__ import print_function

__author__= "Luis C. Pérez Tato (LCPT) and Ana Ortega (AOO)"
__copyright__= "Copyright 2015, LCPT and AOO"
__license__= "GPL"
__version__= "3.0"
__email__= "l.pereztato@gmail.com"

E= 2.1e6 # Young modulus of the steel en kg/cm2.
nu= 0.3 # Poisson's ratio.
h= 0.1 # Thickness.
dens= 1.33 # Density kg/m2.

import geom
import xc
from model import predefined_spaces
from materials import typical_materials

feProblem= xc.FEProblem()
preprocessor=  feProblem.getPreprocessor
nodes= preprocessor.getNodeHandler


delta= 10.1
p1= geom.Pos3d(0+delta,0+delta,-5.0)
p2= geom.Pos3d(1+delta,0+delta,5.0)
p3= geom.Pos3d(1+delta,1+delta,5.0)
p4= geom.Pos3d(0+delta,1+delta,-5.0)

# Problem type
modelSpace= predefined_spaces.StructuralMechanics3D(nodes)
n1= nodes.newNodeXYZ(p1.x,p1.y,p1.z)
n2= nodes.newNodeXYZ(p2.x,p2.y,p2.z)
n3= nodes.newNodeXYZ(p3.x,p3.y,p3.z)
n4= nodes.newNodeXYZ(p4.x,p4.y,p4.z)

# Materials definition
memb1= typical_materials.defElasticMembranePlateSection(preprocessor, "memb1",E,nu,dens,h)

elements= preprocessor.getElementHandler
elements.defaultMaterial= memb1.name
elem= elements.newElement("ShellMITC4",xc.ID([n4.tag,n3.tag,n2.tag,n1.tag]))

c= elem.getCoordTransf.getVPosCentroide
O= geom.Pos3d(c[0],c[1],c[2])
elem.computeBasis()
On= elem.getNaturalCoordinates(O, True)
p1n= elem.getNaturalCoordinates(p1, True)
p2n= elem.getNaturalCoordinates(p2, True)
p3n= elem.getNaturalCoordinates(p3, True)
p4n= elem.getNaturalCoordinates(p4, True)
vv= elem.getCoordTransf.getG3Vector+0.02*elem.getCoordTransf.getG2Vector
p= O+10.0*geom.Vector3d(vv[0],vv[1],vv[2])
pp= elem.getCoordTransf.getProj(p)
ppn= elem.getNaturalCoordinates(pp, True)

ratio= 0.0
ratio1= abs(On.r)
ratio= max(ratio,ratio1)
ratio2= abs(On.s)
ratio= max(ratio,ratio2)
ratio3= abs(p1n.r+1.0)
ratio= max(ratio,ratio3)
ratio4= abs(p1n.s-1.0)
ratio= max(ratio,ratio4)
ratio5= abs(p2n.r-1.0)
ratio= max(ratio,ratio5)
ratio6= abs(p2n.s-1.0)
ratio= max(ratio,ratio6)
ratio7= abs(p3n.r-1.0)
ratio= max(ratio,ratio7)
ratio8= abs(p3n.s+1.0)
ratio= max(ratio,ratio8)
ratio9= abs(p4n.r+1.0)
ratio= max(ratio,ratio9)
ratio10= abs(p4n.s+1.0)
ratio= max(ratio,ratio10)

'''
print(O)
print("Centroid natural coordinates: ", On)
print("ratio1= ",ratio1)
print("ratio2= ",ratio2)
print(p1)
print(p1n)
print("ratio3= ", ratio3)
print("ratio4= ", ratio4)
print(p2)
print(p2n)
print("ratio5= ", ratio5)
print("ratio6= ", ratio6)
print(p3)
print(p3n)
print("ratio7= ",ratio7)
print("ratio8= ",ratio8)
print(p4)
print(p4n)
print("ratio9= ",ratio9)
print("ratio10= ",ratio10)
print("p=", p)
print("pp=", pp)
print("ppn=", ppn)
'''

import os
from misc_utils import log_messages as lmsg
fname= os.path.basename(__file__)
if (ratio < 1e-14):
    print('test '+fname+': ok.')
else:
    lmsg.error(fname+' ERROR.')

