# -*- coding: utf-8 -*-
from __future__ import print_function
from __future__ import division

import xc
from model import predefined_spaces
from materials import typical_materials

__author__= "Luis C. Pérez Tato (LCPT) and Ana Ortega (AOO)"
__copyright__= "Copyright 2015, LCPT and AOO"
__license__= "GPL"
__version__= "3.0"
__email__= "l.pereztato@gmail.com"

b= 0.4
h= 0.8
A= b*h
E= 200000*9.81/1e-4 # Estimated elastic modulus for concrete.
nu= 0.3 # Poisson's ratio
G= E/(2*(1+nu)) # Shear modulus
Iy= (1/12.0*h*b**3) # Cross section moment of inertia (m4)
Iz= (1/12.0*b*h**3) # Cross section moment of inertia (m4)
J= 0.721e-8 # Cross section torsion constant (m4)
L= 5 # Element length expressed in meters.

# Problem type
feProblem= xc.FEProblem()
preprocessor=  feProblem.getPreprocessor
nodes= preprocessor.getNodeHandler
modelSpace= predefined_spaces.StructuralMechanics3D(nodes)
n1= nodes.newNodeXYZ(1.0,2.0,0.0)
n2= nodes.newNodeXYZ(1.0,2.0,L)


lin= modelSpace.newLinearCrdTransf("lin",xc.Vector([0,1,0]))

# Elements definition
elements= preprocessor.getElementHandler

# Materials
sectionProperties= xc.CrossSectionProperties3d()
sectionProperties.A= A; sectionProperties.E= E; sectionProperties.G= G
sectionProperties.Iz= Iz; sectionProperties.Iy= Iy; sectionProperties.J= J
section= typical_materials.defElasticSectionFromMechProp3d(preprocessor, "section",sectionProperties)

elements.defaultTransformation= lin.name
#  sintaxis: ElasticBeam3d[<tag>] 
elements.defaultMaterial= section.name
beam3d= elements.newElement("ElasticBeam3d",xc.ID([n1.tag,n2.tag]))



crdTransf= beam3d.getCoordTransf
P1= crdTransf.getPointGlobalCoordFromLocal(xc.Vector([0.5,0.0,0.0]))
P2= crdTransf.getPointGlobalCoordFromBasic(0.5)

ratio1= (P1-xc.Vector([1,2,0.5])).Norm()
ratio2= (P2-xc.Vector([1,2,2.5])).Norm()

''' 
print("P1: ",P1)
print("P2: ",P2)
print("ratio1= ",ratio1)
print("ratio2= ",ratio2)
  '''

import os
from misc_utils import log_messages as lmsg
fname= os.path.basename(__file__)
if (ratio1 < 1e-15) & (ratio2 < 1e-15):
    print('test '+fname+': ok.')
else:
    lmsg.error(fname+' ERROR.')
