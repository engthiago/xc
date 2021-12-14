# -*- coding: utf-8 -*-
''' Test from Ansys manual
 Reference:  Strength of Material, Part I, Elementary Theory and Problems, pg. 26, problem 10.'''

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
import math

E= 30e6 # Young modulus (psi)
l= 10 # Cable length in inches
sigmaPret= 1500 # Prestressing force (pounds)
area= 2
fPret= sigmaPret*area # Prestressing force (pounds)
F= 100 # Prestressing force (pounds)

# Model definition
feProblem= xc.FEProblem()
preprocessor=  feProblem.getPreprocessor
nodes= preprocessor.getNodeHandler

# Problem type
modelSpace= predefined_spaces.SolidMechanics2D(nodes)


nodes.defaultTag= 1 # First node number.
nod= nodes.newNodeXY(0,0)
nod= nodes.newNodeXY(l/2,0.0)
nod= nodes.newNodeXY(l,0.0)

# Materials definition
cable= typical_materials.defCableMaterial(preprocessor, "cable",E,sigmaPret,0.0)
    
''' We define nodes at the points where loads will be applied.
    We will not compute stresses so we can use an arbitrary
    cross section of unit area.'''
    
# Elements definition
elements= preprocessor.getElementHandler
elements.defaultMaterial= cable.name
elements.dimElem= 2 # Dimension of element space
elements.defaultTag= 1 # First node number.
truss1= elements.newElement("CorotTruss",xc.ID([1,2]))
truss1.sectionArea= area
truss2= elements.newElement("CorotTruss",xc.ID([2,3]))
truss2.sectionArea= area
    
# Constraints
constraints= preprocessor.getBoundaryCondHandler
#
spc= constraints.newSPConstraint(1,0,0.0) # Node 1
spc= constraints.newSPConstraint(1,1,0.0)
spc= constraints.newSPConstraint(3,0,0.0) # Node 3
spc= constraints.newSPConstraint(3,1,0.0)


# Load case definition.
lPattern= '0'
lp0= modelSpace.newLoadPattern(name= lPattern)

lp0.newNodalLoad(2,xc.Vector([0,-F]))
modelSpace.addLoadCaseToDomain(lPattern)

# Solution procedure
analysis= predefined_solutions.plain_newton_raphson(feProblem)
result= analysis.analyze(10)


nodes.calculateNodalReactions(True,1e-7)

nod3= nodes.getNode(3)
R1X= nod3.getReaction[0]
R1Y= nod3.getReaction[1] 
nod1= nodes.getNode(1)
R2X= nod1.getReaction[0]
R2Y= nod1.getReaction[1] 
nod2= nodes.getNode(2)
deltaX= nod2.getDisp[0]
deltaY= nod2.getDisp[1]  

ele1= elements.getElement(1)
tension= ele1.getN()


alpha= -math.atan2(deltaY,l/2)
tensTeor= F/(2*math.sin(alpha))
ratio1= ((R1X+R2X)/fPret)
ratio2= ((tension-tensTeor)/tensTeor)
ratio3= ((R1Y-F/2.0)/(F/2))
ratio4= ((R2Y-F/2.0)/(F/2))
    
''' 
print("alpha= ",rad2deg((alpha)))
print("R1X= ",R1X)
print("R1Y= ",R1Y)
print("R2X= ",R2X)
print("R2Y= ",R2Y)
print("deltaX= ",deltaX)
print("deltaY= ",deltaY)
print("tensTeor= ",(tensTeor))
print("tension= ",(tension))
print("ratio1= ",(ratio1))
print("ratio2= ",(ratio2))
print("ratio3= ",(ratio3))
print("ratio4= ",(ratio4))
   '''
    
import os
from misc_utils import log_messages as lmsg
fname= os.path.basename(__file__)
if (abs(ratio1)<1e-11) & (abs(ratio2)<1e-11) & (abs(ratio3)<1e-11) & (abs(ratio4)<1e-11):
    print('test '+fname+': ok.')
else:
    lmsg.error(fname+' ERROR.')
