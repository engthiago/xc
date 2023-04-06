# -*- coding: utf-8 -*-
''' Home made test. ZeroLengthSection elements sign criteria.

    Axial and shear forces have the same direction and sense that
    the local axes.
    Mx torque and My bending moment have the same direction and sense
    that the local axes.
    Bending moment Mz has the same direction and its sense is the OPPOSITE to local Z axis.
    Section's y axis is element z axis.
'''

from __future__ import print_function
from __future__ import division

__author__= "Luis C. Pérez Tato (LCPT) and Ana Ortega (AOO)"
__copyright__= "Copyright 2015, LCPT and AOO"
__license__= "GPL"
__version__= "3.0"
__email__= "l.pereztato@gmail.com"

import sys
import xc
from solution import predefined_solutions
from model import predefined_spaces
from postprocess import prop_statistics

# Problem type
feProblem= xc.FEProblem()
preprocessor=  feProblem.getPreprocessor   


# Positive Mz (section)
epsilon= 3.5e-3
epsilon1= epsilon
epsilon2= -epsilon
epsilon3= -epsilon
epsilon4= epsilon

# Read section definition from file.
import os
pth= os.path.dirname(__file__)
# print("pth= ", pth)
if(not pth):
  pth= "."
auxModulePath= pth+"/../../aux"
sys.path.append(auxModulePath)
import four_fiber_section
fourFibersSection= four_fiber_section.buildFourFiberSection(preprocessor, epsilon1, epsilon2, epsilon3, epsilon4)

sigma= four_fiber_section.E*epsilon
F= sigma*four_fiber_section.fiberArea
N= 0.0
My= 0.0
Mz= -4*F*four_fiber_section.depthOverY/2.0 # Mz positive is in the opposite direction with respecto to the positive y-axis.

fourFibersSection.revertToStart()
nodes= preprocessor.getNodeHandler
modelSpace= predefined_spaces.StructuralMechanics3D(nodes)
n1= nodes.newNodeXYZ(0.0,0.0,0.0)
n2= nodes.newNodeXYZ(0.0,0.0,0.0)

# Elements definition
elements= preprocessor.getElementHandler
elements.defaultMaterial= fourFibersSection.name
elements.dimElem= 1 # Dimension of element space
zl= elements.newElement("ZeroLengthSection",xc.ID([n1.tag,n2.tag]))

# Constraints
modelSpace.fixNode000_000(n1.tag)
spc= modelSpace.constraints.newSPConstraint(n2.tag,1,0.0)
spc= modelSpace.constraints.newSPConstraint(n2.tag,2,0.0)
spc= modelSpace.constraints.newSPConstraint(n2.tag,3,0.0)


# Load definition.
lp0= modelSpace.newLoadPattern(name= '0')
lp0.newNodalLoad(n2.tag,xc.Vector([N,0,0,0,My,Mz])) # Section's y axis is element z axis.

# Add the load pattern to the domain.
modelSpace.addLoadCaseToDomain(lp0.name)

# Solution
analysis= predefined_solutions.simple_static_linear(feProblem)
result= analysis.analyze(1)

zl.getResistingForce()
section= zl.getSection()

N1= section.getFibers().getResultant()
Mz1= section.getFibers().getMz(0.0)
My1= section.getFibers().getMy(0.0)

section.setupFibers()
RR= section.getStressResultant()
R1= xc.Vector([RR[0],RR[2],RR[1]])

vTeor= xc.Vector([N,My,Mz])
v0= xc.Vector([four_fiber_section.N0,four_fiber_section.My0,four_fiber_section.Mz0])
v1= xc.Vector([N1,My1,Mz1])

import math
error= math.sqrt((vTeor-v0).Norm2()+(vTeor-v1).Norm2()+(vTeor-four_fiber_section.R0).Norm2()+(vTeor-R1).Norm2())

'''
print("I= ", zl.getIVector)
print("K= ", zl.getKVector)
print("EA= ",E*A)
print("EIz= ",E*Iz)
print("EIy= ",E*Iy)
print(fourFibersSection.getInitialTangentStiffness())

print("v0= ",v0)
print("v1= ",v1)
print("R0= ",R0)
print("R1= ",R1)
print("vTeor= ", vTeor)
print("error= ", error)
'''

from misc_utils import log_messages as lmsg
fname= os.path.basename(__file__)
if (error < 1e-3):
    print('test '+fname+': ok.')
else:
    lmsg.error(fname+' ERROR.')
