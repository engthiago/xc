# -*- coding: utf-8 -*-
''' Home made test. Horizontal cantilever under vertical load at his front end.'''
from __future__ import print_function

__author__= "Luis C. Pérez Tato (LCPT) and Ana Ortega (AOO)"
__copyright__= "Copyright 2015, LCPT and AOO"
__license__= "GPL"
__version__= "3.0"
__email__= "l.pereztato@gmail.com"

import math
import xc
from solution import predefined_solutions
from model import predefined_spaces
from materials import typical_materials

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

modelSpace.fixNode000_000(n1.tag)

# Load case definition.
lp0= modelSpace.newLoadPattern(name= '0')
lp0.newNodalLoad(n2.tag, xc.Vector([0,-F,0,0,0,0]))
# We add the load case to domain.
modelSpace.addLoadCaseToDomain(lp0.name)

# Solution
analysis= predefined_solutions.simple_static_linear(feProblem)
result= analysis.analyze(1)

delta= n2.getDisp[1] # ydisplacement of node 2.
beam3d.getResistingForce()
M= beam3d.getMy1
MTeor= F*L
V= beam3d.getVz()

deltateor= (-F*L**3/(3*E*Iy))
ratio1= (delta/deltateor)
ratio2= abs((M-MTeor)/MTeor)
ratio3= (V/F)

# Check getMy1 and getMy2 (LP 28/04/2024).
My1= beam3d.getMy1
My1Ref= F*L
My2= beam3d.getMy2
ratio4= math.sqrt((My1-My1Ref)**2+(My2)**2)

# Check getVz1 and getVz2 (LP 28/04/2024).
Vz1= beam3d.getVz1
VzRef= -F
Vz2= beam3d.getVz2
ratio5= math.sqrt((Vz1-VzRef)**2+(Vz2-VzRef)**2)

'''
print('delta= ', delta)
print('refDelta= ', deltateor)
print('ratio1= ', ratio1)
print("M= ",M)
print(ratio2)
print("V1= ",V)
print(ratio3)
print('My1Ref= ', My1Ref/1e3, 'My1= ', My1/1e3, ' My2= ', My2/1e3, ' ratio4= ', ratio4)
print('Vz1Ref= ', VzRef/1e3, 'Vz1= ', Vz1/1e3, ' Vz2= ', Vz2/1e3, ' ratio5= ', ratio5)
'''

import os
from misc_utils import log_messages as lmsg
fname= os.path.basename(__file__)
if (abs(ratio1-1.0)<1e-5) & (ratio2<1e-5) & (abs(ratio3-1.0)<1e-5) & (abs(ratio4)<1e-10) & (abs(ratio5)<1e-10):
    print('test '+fname+': ok.')
else:
    lmsg.error(fname+' ERROR.')
    
# # Graphic stuff.
# from postprocess import output_handler
# oh= output_handler.OutputHandler(modelSpace)
# # oh.displayFEMesh()#setsToDisplay= [columnSet, pileSet])
# # oh.displayDispRot(itemToDisp='uX', defFScale= 100.0)
# oh.displayLocalAxes()
# oh.displayLoads()
# # oh.displayIntForcDiag('N')
# # oh.displayIntForcDiag('Mz')
# oh.displayIntForcDiag('My')
# oh.displayIntForcDiag('Vz')
# # oh.displayIntForcDiag('Vy')
# #oh.displayLocalAxes()
