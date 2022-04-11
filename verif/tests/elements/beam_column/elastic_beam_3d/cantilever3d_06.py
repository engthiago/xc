# -*- coding: utf-8 -*-
''' Home made test. Ménsula sometida a carga horizontal en su extremo.'''

from __future__ import print_function

__author__= "Luis C. Pérez Tato (LCPT) and Ana Ortega (AOO)"
__copyright__= "Copyright 2015, LCPT and AOO"
__license__= "GPL"
__version__= "3.0"
__email__= "l.pereztato@gmail.com"

# Material properties
E= 2.1e6*9.81/1e-4 # Elastic modulus (Pa)
nu= 0.3 # Poisson's ratio
G= E/(2*(1+nu)) # Shear modulus

import geom
import xc
from solution import predefined_solutions
from model import predefined_spaces
from materials import typical_materials

# Cross section properties (IPE-80)
A= 7.64e-4 # Cross section area (m2)
Iy= 80.1e-8 # Cross section moment of inertia (m4)
Iz= 8.49e-8 # Cross section moment of inertia (m4)
J= 0.721e-8 # Cross section torsion constant (m4)

# Geometry
L= 1.5 # Bar length (m)

# Load
M= 1.5e3 # Moment magnitude (kN)

# Problem type
feProblem= xc.FEProblem()
preprocessor=  feProblem.getPreprocessor   
nodes= preprocessor.getNodeHandler
modelSpace= predefined_spaces.StructuralMechanics3D(nodes)
n1= nodes.newNodeXYZ(0,0.0,0.0)
n2= nodes.newNodeXYZ(L,0.0,0.0)


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
beam3d= elements.newElement("ElasticBeam3d",xc.ID([n1.tag, n2.tag]))


# Constraints
modelSpace.fixNode000_000(n1.tag)

# Load definition.
lp0= modelSpace.newLoadPattern(name= '0')
lp0.newNodalLoad(n2.tag, xc.Vector([0,0,0,0,0,M]))
# We add the load case to domain.
modelSpace.addLoadCaseToDomain(lp0.name)

# Solution
analysis= predefined_solutions.simple_static_linear(feProblem)
result= analysis.analyze(1)


delta= n2.getDisp[1]  # z displacement of node 2
theta= n2.getDisp[5]  # y rotation of the node

elements= preprocessor.getElementHandler

beam3d.getResistingForce()
M1= beam3d.getMy1
V= beam3d.getVz()



deltateor= (M*L**2/(2*E*Iy))
thetateor= (M*L/(E*Iy))
ratio1= (delta/deltateor)
ratio2= abs((M1+M)/M)
ratio3= (theta/thetateor)

''' 
print("delta prog.= ", delta)
print("delta teor.= ", deltateor)
print(ratio1)
print("M= ",M)
print("M1= ",M1)
print(ratio2)
print("theta prog.= ", theta)
print("theta teor.= ", thetateor)
print(ratio3)
 '''
cumple= (abs(ratio1-1.0)<1e-5) & (ratio2<1e-5) & (abs(ratio3-1.0)<1e-5)
import os
from misc_utils import log_messages as lmsg
fname= os.path.basename(__file__)
if cumple:
    print('test '+fname+': ok.')
else:
    lmsg.error(fname+' ERROR.')
