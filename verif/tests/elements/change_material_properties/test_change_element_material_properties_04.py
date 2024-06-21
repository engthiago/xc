# -*- coding: utf-8 -*-
'''Change the properties of the materials of a ZeroLength element.'''
from __future__ import print_function

__author__= "Luis C. Pérez Tato (LCPT) and Ana Ortega (AOO)"
__copyright__= "Copyright 2015, LCPT and AOO"
__license__= "GPL"
__version__= "3.0"
__email__= "l.pereztato@gmail.com"

KX= 1000 # Spring constant
KY= 2000 # Spring constant
l= 1e-7 # Distance between nodes
FX= 1 # Force magnitude
FY= 2 # Force magnitude

import xc
from model import predefined_spaces
from materials import typical_materials

# Problem type
feProblem= xc.FEProblem()
preprocessor=  feProblem.getPreprocessor
nodes= preprocessor.getNodeHandler
modelSpace= predefined_spaces.SolidMechanics2D(nodes)

# Model definition

n1= nodes.newNodeXY(1,1)
n2= nodes.newNodeXY(1,1)

# Materials definition
kx= typical_materials.defElasticMaterial(preprocessor, "kx",KX)
ky= typical_materials.defElasticMaterial(preprocessor, "ky",KY)


''' We define nodes at the points where loads will be applied.
    We will not compute stresses so we can use an arbitrary
    cross section of unit area.'''
    
# Elements definition
elements= preprocessor.getElementHandler
elements.defaultMaterial= kx.name
elements.dimElem= 2 # Dimension of element space
elem= elements.newElement("ZeroLength",xc.ID([n1.tag,n2.tag]))
elem.clearMaterials()
elem.setMaterial(0,"kx")
elem.setMaterial(1,"ky")

# Constraints
constraints= preprocessor.getBoundaryCondHandler
#
spc= constraints.newSPConstraint(n1.tag,0,0.0) # Node 1
spc= constraints.newSPConstraint(n1.tag,1,0.0)


# Load definition.
lp0= modelSpace.newLoadPattern(name= '0')
lp0.newNodalLoad(n2.tag,xc.Vector([FX,FY]))
# We add the load case to domain.
modelSpace.addLoadCaseToDomain(lp0.name)

# Change material properties.
stiffnessFactor= 3.0
materials= elem.getMaterials()
for i, m in enumerate(materials):
    m.E*= stiffnessFactor
        
# Solution
result= modelSpace.analyze(calculateNodalReactions= True)

# Check results.
deltax= n2.getDisp[0]
deltaxRef= FX/(stiffnessFactor*KX)
ratio1= abs(deltax-deltaxRef)/deltaxRef
deltay= n2.getDisp[1] 
deltayRef= FY/(stiffnessFactor*KY)
ratio2= abs(deltay-deltayRef)/deltayRef

'''
print("dx= ",deltax)
print("ratio1= ", ratio1)
print("dy= ",deltay)
print("ratio2= ", ratio2)
'''

import os
from misc_utils import log_messages as lmsg
fname= os.path.basename(__file__)
if (abs(ratio1)<1e-6) and (abs(ratio2)<1e-6):
    print('test '+fname+': ok.')
else:
    lmsg.error(fname+' ERROR.')
