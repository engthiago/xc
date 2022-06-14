# -*- coding: utf-8 -*-
''' home made test
 Reference:  Expresiones de la flecha el el Prontuario de
 Estructuras Metálicas del CEDEX. Apartado 3.3 Punctual load on cantilever.
 ISBN: 84-7790-336-0
 url={https://books.google.ch/books?id=j88yAAAACAAJ}.'''

from __future__ import print_function

'''vector2d_point_load_local verification test.'''

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
L= 20 # Bar length.
h= 0.30 # Beam cross-section depth.
b= 0.2 # Beam cross-section width.
A= b*h # Cross section area.
I= b*h**3/12 # Inertia of the beam section in inches to the fourth power.
xRelPtoAplic= 0.5 # x relative (compared to the total length) of the
                  # point on which the load is applied
P= 1e3 # Transverse load.
n= 1e6 # Axial load.

feProblem= xc.FEProblem()
preprocessor=  feProblem.getPreprocessor   
nodes= preprocessor.getNodeHandler

# Problem type
modelSpace= predefined_spaces.StructuralMechanics2D(nodes)
n1= nodes.newNodeXY(0,0)
n2= nodes.newNodeXY(L,0.0)

    
# Geometric transformation(s)
lin= modelSpace.newLinearCrdTransf("lin")
    
# Materials definition
scc= typical_materials.defElasticSection2d(preprocessor, "scc",A,E,I)


# Elements definition
elements= preprocessor.getElementHandler
elements.defaultTransformation= lin.name
elements.defaultMaterial= scc.name
beam2d= elements.newElement("ElasticBeam2d",xc.ID([n1.tag,n2.tag]))
beam2d.h= h

# end of element definition
    
# Constraints
constraints= preprocessor.getBoundaryCondHandler
modelSpace.fixNode000(n1.tag)


# Load definition.
lp0= modelSpace.newLoadPattern(name= '0')
modelSpace.setCurrentLoadPattern("0")

mesh= feProblem.getDomain.getMesh
eIter= mesh.getElementIter
elem= eIter.next()
while not(elem is None):
    elem.vector2dPointByRelDistLoadLocal(xRelPtoAplic,xc.Vector([n,-P]))
    elem= eIter.next()

loadHandler= preprocessor.getLoadHandler
# We add the load case to domain.
modelSpace.addLoadCaseToDomain(lp0.name)


# Solution
analysis= predefined_solutions.simple_static_linear(feProblem)
result= analysis.analyze(1)

delta0= n2.getDisp[0]
delta1= n2.getDisp[1]

a= xRelPtoAplic*L
delta0Teor= (n*a/E/A)
ratio0= ((delta0-delta0Teor)/delta0Teor)
delta1Teor= (-P*a**2*(3*L-a)/6/E/I)
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
if ((abs(ratio0)<1e-10) & (abs(ratio1)<1e-12)):
    print('test '+fname+': ok.')
else:
    lmsg.error(fname+' ERROR.')
