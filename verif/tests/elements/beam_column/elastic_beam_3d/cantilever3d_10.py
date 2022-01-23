# -*- coding: utf-8 -*-
''' Home made test. Cantilever under punctual horizontal load in local x direction.'''

from __future__ import print_function

__author__= "Luis C. Pérez Tato (LCPT) and Ana Ortega (AOO)"
__copyright__= "Copyright 2015, LCPT and AOO"
__license__= "GPL"
__version__= "3.0"
__email__= "l.pereztato@gmail.com"


import xc_base
import geom
import xc
from solution import predefined_solutions
from model import predefined_spaces
from materials import typical_materials

# Material properties
E= 2.1e6*9.81/1e-4 # Elastic modulus (Pa)
nu= 0.3 # Poisson's ratio
G= E/(2.0*(1+nu)) # Shear modulus

# Cross section properties (IPE-80)
A= 7.64e-4 # Cross section area (m2)
Iy= 8.49e-8 # Cross section moment of inertia (m4)
Iz= 80.1e-8 # Cross section moment of inertia (m4)
J= 0.721e-8 # Cross section torsion constant (m4)

# Geometry
L= 1.5 # Bar length (m)

# Load
F= 1.5e3 # Load magnitude (kN)
Q1= 1.2e2 # Load magnitude (kN)
Q2= 0.9e2 # Load magnitude (kN)

# Problem type
feProblem= xc.FEProblem()
preprocessor=  feProblem.getPreprocessor   
nodes= preprocessor.getNodeHandler
modelSpace= predefined_spaces.StructuralMechanics3D(nodes)
nodes.defaultTag= 1 # First node number.
nod= nodes.newNodeXYZ(0,0.0,0.0)
nodes.defaultTag= 2 # Next node number.
nod= nodes.newNodeXYZ(L,0.0,0.0)


# Geometric transformation(s)
lin= modelSpace.newLinearCrdTransf("lin",xc.Vector([0,-1,0]))
# Materials
sectionProperties= xc.CrossSectionProperties3d()
sectionProperties.A= A; sectionProperties.E= E; sectionProperties.G= G
sectionProperties.Iz= Iz; sectionProperties.Iy= Iy; sectionProperties.J= J
section= typical_materials.defElasticSectionFromMechProp3d(preprocessor, "section",sectionProperties)

# Elements definition
elements= preprocessor.getElementHandler
elements.defaultTransformation= lin.name
elements.defaultMaterial= section.name
elements.defaultTag= 1 # Tag for the next element.
beam3d= elements.newElement("ElasticBeam3d",xc.ID([1,2]))

# Constraints
modelSpace.fixNode000_000(1)


# Load case definition.
lp0= modelSpace.newLoadPattern(name= '0')
lp0.alpha= 2/3.0
lp0.a= L*lp0.alpha
pl= lp0.newElementalLoad("beam3d_point_load")
pl.elementTags= xc.ID([1])
pl.axialComponent= F
pl.transYComponent= Q1
pl.transZComponent= Q2
pl.x= lp0.alpha

# We add the load case to domain.
modelSpace.addLoadCaseToDomain(lp0.name)

# Solution
analysis= predefined_solutions.simple_static_linear(feProblem)
result= analysis.analyze(1)


nodes= preprocessor.getNodeHandler 
nod2= nodes.getNode(2)
deltax= nod2.getDisp[0] 
deltay= nod2.getDisp[2]
deltaz= -(nod2.getDisp[1])

elements= preprocessor.getElementHandler
elem1= elements.getElement(1)

elem1.getResistingForce()
N1= elem1.getN1 # Axial force at the back end.
N2= elem1.getN2 # Axial force at the front end.



deltaxteor= (F*lp0.alpha*L/(E*A))
ratiodx= abs(deltax-deltaxteor)/deltaxteor
deltayteor= (Q1*lp0.a*lp0.a**2*(3*L-lp0.a)/(6*E*Iz))
ratiody= abs(deltay-deltayteor)/deltayteor
deltazteor= (Q2*lp0.a*lp0.a**2*(3*L-lp0.a)/(6*E*Iy))
ratiodz= abs(deltaz-deltazteor)/deltazteor
N1teor= (F)
ratioN1= abs(N1-N1teor)/N1teor
ratioN2= (abs(N2)<1e-3)

''' 
print("deltax= ",deltax)
print("deltaxteor= ",deltaxteor)
print("ratiodx= ",ratiodx)
print("deltay= ",deltay)
print("deltayteor= ",deltayteor)
print("ratiody= ",ratiody)
print("deltaz= ",deltaz)
print("deltazteor= ",deltazteor)
print("ratiodz= ",ratiodz)
print("N1= ",N1)
print("N1teor= ",N1teor)
print("ratioN1= ",ratioN1)
print("N2= ",N2)
print("N2teor= ",0)
print("ratioN2= ",ratioN2)
 '''
import os
from misc_utils import log_messages as lmsg
fname= os.path.basename(__file__)
if (ratiodx<1e-5) & (ratiody<1e-5) & (ratiodz<1e-5) & (ratioN1<1e-5) & ratioN2:
    print('test '+fname+': ok.')
else:
    lmsg.error(fname+' ERROR.')
