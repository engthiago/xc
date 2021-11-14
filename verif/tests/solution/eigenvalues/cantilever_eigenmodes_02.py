# -*- coding: utf-8 -*-
from __future__ import print_function
# Tomado del example A47 del SOLVIA Verification Manual
import xc_base
import geom
import xc

from model import predefined_spaces
from solution import predefined_solutions
from materials import typical_materials
import math

__author__= "Luis C. Pérez Tato (LCPT)"
__copyright__= "Copyright 2014, LCPT"
__license__= "GPL"
__version__= "3.0"
__email__= "l.pereztato@gmail.com"

L= 1 # Cantilever length in meters
b= 0.05 # Cross section width in meters
h= 0.10 # Cross section depth in meters
A= b*h # Cross section area en m2
I= 1/12.0*b*h**3 # Moment of inertia in m4
J= 10*I # Cross section torsion constant
theta= math.radians(30)
E= 2.0E11 # Elastic modulus N/m2
nu= 0.3 # Poisson's ratio
G= E/(2*(1+nu)) # Shear modulus
dens= 7800 # Steel density kg/m3
m= A*dens

NumDiv= 10
# Problem type
feProblem= xc.FEProblem()
preprocessor=  feProblem.getPreprocessor
nodes= preprocessor.getNodeHandler

modelSpace= predefined_spaces.StructuralMechanics3D(nodes)
# Define materials
scc= typical_materials.defElasticSection3d(preprocessor, "scc",A,E,G,I,I,J)




# Geometric transformation(s)
lin= modelSpace.newLinearCrdTransf("lin",xc.Vector([0,0,1]))
# Seed element definition
seedElemHandler= preprocessor.getElementHandler.seedElemHandler
seedElemHandler.defaultMaterial= scc.name
seedElemHandler.defaultTransformation= lin.name
seedElemHandler.defaultTag= 1 #Tag for the next element.
beam3d= seedElemHandler.newElement("ElasticBeam3d",xc.ID([0,0]))
beam3d.rho= m


points= preprocessor.getMultiBlockTopology.getPoints
pt1= modelSpace.newKPoint(0.0,0.0,0.0)
pt2= modelSpace.newKPoint(L*math.cos(theta),L*math.sin(theta),0.0)
l= modelSpace.newLine(pt1,pt2)
l.nDiv= NumDiv


setTotal= preprocessor.getSets.getSet("total")
setTotal.genMesh(xc.meshDir.I)

tagN1= pt1.getNode().tag
tagN2= pt2.getNode().tag

# Constraints
constraints= preprocessor.getBoundaryCondHandler

spc= constraints.newSPConstraint(tagN1,0,0.0) # gdl 0
spc= constraints.newSPConstraint(tagN1,1,0.0) # gdl 1
spc= constraints.newSPConstraint(tagN1,2,0.0) # gdl 2
spc= constraints.newSPConstraint(tagN1,3,0.0) # gdl 0
spc= constraints.newSPConstraint(tagN1,4,0.0) # gdl 1
spc= constraints.newSPConstraint(tagN1,5,0.0) # gdl 2


# Solution procedure
analysis= predefined_solutions.frequency_analysis(feProblem)
analOk= analysis.analyze(1)

eig1= analysis.getEigenvalue(1)
omega= eig1**0.5
T= 2*math.pi/omega
fcalc= 1/T



lambdA= 1.87510407
fteor= lambdA**2/(2*math.pi*L**2)*math.sqrt(E*I/m)
ratio1= abs(fcalc-fteor)/fteor

''' 
print("omega= ",(omega))
print("T= ",(T))
print("fcalc= ",(fcalc))
print("fteor= ",(fteor))
print("ratio1= ",(ratio1))
   '''

import os
from misc_utils import log_messages as lmsg
fname= os.path.basename(__file__)
if (abs(ratio1)<5e-3):
    print('test '+fname+': ok.')
else:
    lmsg.error(fname+' ERROR.')
