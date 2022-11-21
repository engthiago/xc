# -*- coding: utf-8 -*-
'''Rigid beam between two nodes. Home made test'''

from __future__ import print_function

__author__= "Luis C. Pérez Tato (LCPT) and Ana Ortega (AO_O)"
__copyright__= "Copyright 2015, LCPT and AO_O"
__license__= "GPL"
__version__= "3.0"
__email__= "l.pereztato@ciccp.es ana.ortega@ciccp.es"

import xc
from solution import predefined_solutions
from model import predefined_spaces
import math


# Geometry
L= 15 # Bar length (m)

# Load
F= 1.5e3 # Load magnitude (kN)

# Problem type
feProblem= xc.FEProblem()
preprocessor=  feProblem.getPreprocessor   
nodes= preprocessor.getNodeHandler
modelSpace= predefined_spaces.StructuralMechanics3D(nodes)
nod1= nodes.newNodeXYZ(0,0.0,0.0)
nod2= nodes.newNodeXYZ(L,0.0,0.0)


# Constraints

modelSpace.fixNode000_000(nod1.tag)
rr= preprocessor.getBoundaryCondHandler.newRigidBeam(nod1.tag,nod2.tag)


# Loads definition
lp0= modelSpace.newLoadPattern(name= '0')
lp0.newNodalLoad(nod2.tag,xc.Vector([-F,F,-F,0,0,0]))
# We add the load case to domain.
modelSpace.addLoadCaseToDomain(lp0.name)

# Solution
analysis= predefined_solutions.simple_static_linear(feProblem)
result= analysis.analyze(1)


deltaX= nod2.getDisp[0]
deltaY= nod2.getDisp[1]
deltaZ= nod2.getDisp[2]  # Node 2 displacement


ratio1= math.sqrt(deltaX**2+deltaY**2+deltaZ**2)

''' 
print(deltaX)
print(deltaY)
print(deltaZ)
print(ratio1)
  '''

import os
from misc_utils import log_messages as lmsg
fname= os.path.basename(__file__)
if ratio1<1e-9:
    print('test '+fname+': ok.')
else:
    lmsg.error(fname+' ERROR.')
