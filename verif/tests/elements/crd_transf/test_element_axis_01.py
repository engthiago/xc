# -*- coding: utf-8 -*-
from __future__ import print_function
from __future__ import division

import xc
from model import predefined_spaces
from materials import typical_materials
import math

__author__= "Luis C. Pérez Tato (LCPT) and Ana Ortega (AOO)"
__copyright__= "Copyright 2015, LCPT and AOO"
__license__= "GPL"
__version__= "3.0"
__email__= "l.pereztato@gmail.com"

b= 0.4
h= 0.8
A= b*h
E= 200000*9.81/1e-4 # Estimated concrete elastic modulus.
nu= 0.3 # Poisson's ratio
G= E/(2*(1+nu)) # Shear modulus
Iy= (1/12.0*h*b**3) # Cross section moment of inertia (m4)
Iz= (1/12.0*b*h**3) # Cross section moment of inertia (m4)
J= 0.721e-8 # Cross section torsion constant (m4)
L= 1 # Element length expressed in meters.

# Problem type
feProblem= xc.FEProblem()
preprocessor=  feProblem.getPreprocessor
nodes= preprocessor.getNodeHandler
modelSpace= predefined_spaces.StructuralMechanics3D(nodes)
n1= nodes.newNodeXYZ(0.0,0.0,0.0)
n2= nodes.newNodeXYZ(L,0.0,0)

lin= modelSpace.newLinearCrdTransf("lin",xc.Vector([0,1,0]))

# Materials
sectionProperties= xc.CrossSectionProperties3d()
sectionProperties.A= A; sectionProperties.E= E; sectionProperties.G= G
sectionProperties.Iz= Iz; sectionProperties.Iy= Iy; sectionProperties.J= J
section= typical_materials.defElasticSectionFromMechProp3d(preprocessor, "section",sectionProperties)

# Elements definition
elements= preprocessor.getElementHandler
elements.defaultTransformation= lin.name
elements.defaultMaterial= section.name
beam3d= elements.newElement("ElasticBeam3d",xc.ID([n1.tag,n2.tag]))


strong= beam3d.getVDirStrongAxisGlobalCoord(True) # initialGeometry= True
weak= beam3d.getVDirWeakAxisGlobalCoord(True) # initialGeometry= True
sectionAngle= beam3d.getStrongAxisAngle()
ratio1= ((strong[0])**2+(strong[2])**2)
ratio2= ((weak[0])**2+(weak[1])**2)
ratio3= abs(sectionAngle-math.pi/2)/(math.pi/2)



''' 
print("strong axis: ", strong)
print("ratio1= ",ratio1)
print("weak axis: ", weak)
print("ratio2= ",ratio2)
print("ratio3= ",ratio3)
  '''

import os
from misc_utils import log_messages as lmsg
fname= os.path.basename(__file__)
if (ratio1 < 1e-15) & (ratio2 < 1e-15) & (ratio3 < 1e-15):
    print('test '+fname+': ok.')
else:
    lmsg.error(fname+' ERROR.')
