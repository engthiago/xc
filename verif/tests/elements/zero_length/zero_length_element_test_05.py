# -*- coding: utf-8 -*-
from __future__ import print_function
''' home made test. Verifica that "ZeroLength" element gives right results
 when not aligned with global axes. '''

__author__= "Luis C. Pérez Tato (LCPT) and Ana Ortega (AOO)"
__copyright__= "Copyright 2015, LCPT and AOO"
__license__= "GPL"
__version__= "3.0"
__email__= "l.pereztato@gmail.com"

K= 1000 # Spring constant
F= 1 # Force magnitude

import xc
from model import predefined_spaces
from materials import typical_materials

# Problem type
feProblem= xc.FEProblem()
preprocessor=  feProblem.getPreprocessor
nodes= preprocessor.getNodeHandler
modelSpace= predefined_spaces.StructuralMechanics2D(nodes)

# Model definition
n1= nodes.newNodeXY(1,1)
n2= nodes.newNodeXY(1,1)

# Materials definition
k= typical_materials.defElasticMaterial(preprocessor, "k",K)


''' We define nodes at the points where loads will be applied.
    We will not compute stresses so we can use an arbitrary
    cross section of unit area.'''

# Elements definition
elements= preprocessor.getElementHandler
elements.defaultMaterial= k.name
elements.dimElem= 3 # Dimension of element space
zl= elements.newElement("ZeroLength",xc.ID([n1.tag,n2.tag]))
zl.clearMaterials()
zl.setupVectors(xc.Vector([0,1,0]),xc.Vector([-1,0,0]))
zl.setMaterial(0,"k")



# Constraints
constraints= preprocessor.getBoundaryCondHandler
spc= constraints.newSPConstraint(n1.tag,0,0.0) # Node 1
spc= constraints.newSPConstraint(n1.tag,1,0.0)
spc= constraints.newSPConstraint(n1.tag,2,0.0)
spc= constraints.newSPConstraint(n2.tag,0,0.0) # Node 2
spc= constraints.newSPConstraint(n2.tag,2,0.0) # Node 2


# Load definition.
lp0= modelSpace.newLoadPattern(name= '0')
lp0.newNodalLoad(n2.tag,xc.Vector([0,F,0]))

# We add the load case to domain.
modelSpace.addLoadCaseToDomain(lp0.name)

# Solution
result= modelSpace.analyze(calculateNodalReactions= True)

deltax= n2.getDisp[0]
deltay= n2.getDisp[1]
deltat= n2.getDisp[2] 
RX= n1.getReaction[0]
RY= n1.getReaction[1]
RT= n1.getReaction[2] 


ratio1= -RX/F
ratio2= (K*deltax)/F
ratio3= -RY/F
ratio4= (K*deltay)/F
ratio5= -RT/F
ratio6= (K*deltat)/F

''' 
print("RX= ",RX)
print("dx= ",deltax)
print("RY= ",RY)
print("dy= ",deltay)
print("ratio1= ",(ratio1))
print("ratio2= ",(ratio2))
print("ratio3= ",(ratio3))
print("ratio4= ",(ratio4))
print("ratio5= ",(ratio5))
print("ratio6= ",(ratio6))
   '''
  
import os
from misc_utils import log_messages as lmsg
fname= os.path.basename(__file__)
if (abs(ratio1)<1e-12) & (abs(ratio2)<1e-12) & (abs(ratio3-1.0)<1e-12) & (abs(ratio4-1.0)<1e-12)  & (abs(ratio5)<1e-12) & (abs(ratio6)<1e-12) :
    print('test '+fname+': ok.')
else:
    lmsg.error(fname+' ERROR.')
