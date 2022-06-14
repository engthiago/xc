# -*- coding: utf-8 -*-
'''Save and restore methods verification.'''

from __future__ import division
from __future__ import print_function


import xc
from model import predefined_spaces
from solution import predefined_solutions
from materials import typical_materials

__author__= "Luis C. Pérez Tato (LCPT)"
__copyright__= "Copyright 2014, LCPT"
__license__= "GPL"
__version__= "3.0"
__email__= "l.pereztato@gmail.com"

# Material properties
E= 2.1e6*9.81/1e-4 # Elastic modulus (Pa)
nu= 0.3 # Poisson's ratio
G= E/(2*(1+nu)) # Shear modulus

# Cross section properties (IPE-80)
A= 7.64e-4 # Cross section area (m2)
Iy= 80.1e-8 # Cross section moment of inertia (m4)
Iz= 8.49e-8 # Cross section moment of inertia (m4)
J= 0.721e-8 # Cross section torsion constant (m4)

# Geometry
L= 1.5 # Bar length (m)

# Load
F= 1.5e3 # Load magnitude (kN)

feProblem= xc.FEProblem()
preprocessor=  feProblem.getPreprocessor   
nodes= preprocessor.getNodeHandler

# Problem type
modelSpace= predefined_spaces.StructuralMechanics3D(nodes)
# Create nodes.
n1= nodes.newNodeXYZ(0,0.0,0.0)
n2= nodes.newNodeXYZ(L,0.0,0.0)

# Materials definition.
scc= typical_materials.defElasticSection3d(preprocessor, "scc",A,E,G,Iz,Iy,J)

# Element definition.
lin= modelSpace.newLinearCrdTransf("lin",xc.Vector([0,1,0]))
elements= preprocessor.getElementHandler
elements.defaultTransformation= lin.name
elements.defaultMaterial= scc.name
#  sintaxis: ElasticBeam3d[<tag>] 
beam3d= elements.newElement("ElasticBeam3d",xc.ID([n1.tag,n2.tag]))

# Constraints.
modelSpace.fixNode000_000(n1.tag)

# Load definition.
lp0= modelSpace.newLoadPattern(name= '0')
lp0.newNodalLoad(n2.tag, xc.Vector([F,0,0,0,0,0]))
# We add the load case to domain.
modelSpace.addLoadCaseToDomain(lp0.name)

# Solution
analysis= predefined_solutions.simple_static_linear(feProblem)
result= analysis.analyze(1)

# Store node and element tags
n1Tag= n1.tag
n2Tag= n2.tag
eTag= beam3d.tag

import os
os.system("rm -r -f /tmp/test08.db")
db= feProblem.newDatabase("BerkeleyDB","/tmp/test08.db")
db.save(100)
feProblem.clearAll()
feProblem.setVerbosityLevel(0) # Dont print(warning messages)
                            # about pointers to material.
db.restore(100)
feProblem.setVerbosityLevel(1) # print(warnings again )


nodes= preprocessor.getNodeHandler
 
n2= nodes.getNode(n2Tag)

delta= n2.getDisp[0]  # x displacement of node 2

elements= preprocessor.getElementHandler

elem1= elements.getElement(eTag)
elem1.getResistingForce()
N1= elem1.getN1 # axial internal force.



deltateor= (F*L/(E*A))
ratio1= abs(delta-deltateor)/deltateor
ratio2= abs(N1-F)/F

''' 
print("delta= ",delta)
print("deltateor= ",deltateor)
print("ratio1= ",ratio1)
print("N1= ",N1)
print("ratio2= ",ratio2)
'''

from misc_utils import log_messages as lmsg
fname= os.path.basename(__file__)
if ((ratio1<1e-6) and (ratio2<1e-5)):
    print('test '+fname+': ok.')
else:
    lmsg.error(fname+' ERROR.')

os.system("rm -rf /tmp/test08.db") # Your garbage you clean it
