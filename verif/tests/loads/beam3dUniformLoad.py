# -*- coding: utf-8 -*-
from __future__ import print_function
# home made test
# Reference:  Expresiones de la flecha el el Prontuario de
# Estructuras Metálicas del CEDEX. Apartado 3.3 Punctual load on cantilever.
# ISBN: 84-7790-336-0
# url={https://books.google.ch/books?id=j88yAAAACAAJ},

import xc_base
import geom
import xc
from solution import predefined_solutions
from model import predefined_spaces
from materials import typical_materials

__author__= "Luis C. Pérez Tato (LCPT) and Ana Ortega (AOO)"
__copyright__= "Copyright 2015, LCPT and AOO"
__license__= "GPL"
__version__= "3.0"
__email__= "l.pereztato@gmail.com"

E= 2e6 # Elastic modulus
nu= 0.3 # Poisson's ratio
G= E/(2*(1+nu)) # Shear modulus
L= 20 # Bar length.
h= 0.30 # Cross section depth
b= 0.2 # Cross section width.
A= b*h # Cross section area.
Iz= b*h**3/12 # Cross section moment of inertia
Iy= h*b**3/12 # Cross section moment of inertia
J= 1e-8 # Cross section torsion constant
x= 0.5 # Relative abscissae where the punctual load is applied.
P= 1e3 # punctual load.
n= 1e6 # Axial uniform load.

feProblem= xc.FEProblem()
preprocessor=  feProblem.getPreprocessor   
nodes= preprocessor.getNodeHandler

# Problem type
modelSpace= predefined_spaces.StructuralMechanics3D(nodes)
nodes.defaultTag= 1 # First node number.
nodes.newNodeXYZ(0,0,0)
nodes.newNodeXYZ(L,0,0)
    
# Geometric transformation(s)
lin= modelSpace.newLinearCrdTransf("lin",xc.Vector([0,0,1]))    
# Materials definition
scc= typical_materials.defElasticSection3d(preprocessor, "scc",A,E,G,Iz,Iy,J)
    
# Elements definition
elements= preprocessor.getElementHandler
elements.defaultTransformation= lin.name
elements.defaultMaterial= scc.name
elements.defaultTag= 1 # Tag for next element.
beam3d= elements.newElement("ElasticBeam3d",xc.ID([1,2]))

# Constraints
modelSpace.fixNode000_000(1)

# Load definition.
lp0= modelSpace.newLoadPattern(name= '0')
# set_current_load_pattern{"0"}
eleLoad= lp0.newElementalLoad("beam3d_uniform_load")
eleLoad.elementTags= xc.ID([1])
eleLoad.axialComponent= n
eleLoad.transComponent= -P
# We add the load case to domain.
modelSpace.addLoadCaseToDomain(lp0.name)

# Solution
analysis= predefined_solutions.simple_static_linear(feProblem)
result= analysis.analyze(1)

delta0= nodes.getNode(2).getDisp[0] # xAxis displacement of node 2.
delta1= nodes.getNode(2).getDisp[1] # yAxis displacement of node 2.

delta0Teor= (n*L**2/2/E/A)
ratio0= ((delta0-delta0Teor)/delta0Teor)
Q= P*L
delta1Teor= (-Q*L**3/8/E/Iz)
ratio1= ((delta1-delta1Teor)/delta1Teor)

# print("delta0= ",delta0)
# print("delta0Teor= ",delta0Teor)
# print("ratio0= ",ratio0)
# print("delta1= ",delta1)
# print("delta1Teor= ",delta1Teor)
# print("ratio1= ",ratio1)

import os
from misc_utils import log_messages as lmsg
fname= os.path.basename(__file__)
if (abs(ratio0)<1e-10) & (abs(ratio1)<1e-12):
    print('test '+fname+': ok.')
else:
    lmsg.error(fname+' ERROR.')
