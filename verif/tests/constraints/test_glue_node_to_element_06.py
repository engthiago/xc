# -*- coding: utf-8 -*-
'''Attaching two nodes to the same element. Home made test.'''

from __future__ import print_function

__author__= "Luis C. Pérez Tato (LCPT) and Ana Ortega (AOO)"
__copyright__= "Copyright 2015, LCPT and AOO"
__license__= "GPL"
__version__= "3.0"
__email__= "l.pereztato@gmail.com"

E= 2.1e6 # Young modulus of the steel en kg/cm2.
nu= 0.3 # Poisson's ratio.
h= 0.1 # Thickness.
dens= 1.33 # Density kg/m2.

import geom
import xc
from model import predefined_spaces
from materials import typical_materials
from postprocess.quick_inquiry import nodal_reactions
from solution import predefined_solutions
from misc_utils import log_messages as lmsg

feProblem= xc.FEProblem()
preprocessor=  feProblem.getPreprocessor
nodes= preprocessor.getNodeHandler


p1= geom.Pos3d(0,0,0)
p2= geom.Pos3d(1,0,0)
p3= geom.Pos3d(1,1,0)
p4= geom.Pos3d(0,1,0)

# Problem type
modelSpace= predefined_spaces.StructuralMechanics3D(nodes)
n1= nodes.newNodeXYZ(p1.x,p1.y,p1.z)
n2= nodes.newNodeXYZ(p2.x,p2.y,p2.z)
n3= nodes.newNodeXYZ(p3.x,p3.y,p3.z)
n4= nodes.newNodeXYZ(p4.x,p4.y,p4.z)

n10= nodes.newNodeXYZ(p1.x,p1.y,p1.z)
n20= nodes.newNodeXYZ(p2.x,p2.y,p2.z)
nodesToGlue= [n10,n20]#[n10,n20]

# Materials definition
memb1= typical_materials.defElasticMembranePlateSection(preprocessor, "memb1",E,nu,dens,h)
elements= preprocessor.getElementHandler
elements.defaultMaterial= memb1.name
elem= elements.newElement("ShellMITC4",xc.ID([n1.tag,n2.tag,n3.tag,n4.tag]))

# Constraints
constraints= preprocessor.getBoundaryCondHandler
modelSpace.fixNode000_FFF(n1.tag)
modelSpace.fixNode000_FFF(n2.tag)
modelSpace.fixNode000_FFF(n3.tag)
modelSpace.fixNode000_FFF(n4.tag)
# modelSpace.fixNode000_000(n1.tag)
# modelSpace.fixNode000_000(n2.tag)
# modelSpace.fixNode000_000(n3.tag)
# modelSpace.fixNode000_000(n4.tag)

# Glued nodes.
gluedDOFs= [0,3,4,5]
loadOnDOFs= [0,0,0,0,0,0]
for n in nodesToGlue:
  for i in range(0,6):
    if i not in gluedDOFs:
      modelSpace.constraints.newSPConstraint(n.tag,i,0.0)
    else:
      loadOnDOFs[i]= -1000.0
  glue= modelSpace.constraints.newGlueNodeToElement(n,elem,xc.ID(gluedDOFs))

# Loads definition
lp0= modelSpace.newLoadPattern(name= '0')
lp0.newNodalLoad(n10.tag,xc.Vector(loadOnDOFs))
# We add the load case to domain.
modelSpace.addLoadCaseToDomain(lp0.name)

# Solution
solProc= predefined_solutions.SimpleLagrangeStaticLinear(feProblem)
analOk= solProc.solve(True, reactionCheckTolerance= 1e-7)
if(analOk!=0):
    lmsg.error('Failed to solve for: ', lp0.name)
    quit()

reactionNode10= n10.getReaction
ratio1= reactionNode10.Norm()
svdReactionNodes= nodal_reactions.getReactionFromNodes(nodes,"UVWRxRyRz",elem.getNodes.getExternalNodes)
actionNode10= xc.Vector(loadOnDOFs)
actionNode10Norm= actionNode10.Norm()
svdAction= nodal_reactions.getSlidingVectorsSystemfromSlidingVector("UVWRxRyRz",n10.get3dCoo,actionNode10)
svdResid= svdReactionNodes+svdAction
ratio2= svdResid.getResultant().getModulus()/actionNode10Norm
ratio3= svdResid.getMoment().getModulus()/actionNode10Norm

reactionNode20= n20.getReaction
ratio4= reactionNode20.Norm()
actionNode20= xc.Vector([0,0,0,0,0,0])
actionNode20Norm= actionNode20.Norm()

# print("svdAction= ", svdAction)
# print("svdReactionNodes= ", svdReactionNodes)
# print("svdResid= ", svdResid)
# print("ratio1= ", ratio1)
# print("ratio2= ", ratio2)
# print("ratio3= ", ratio3)
# print("ratio4= ", ratio4)


import os
fname= os.path.basename(__file__)
condNode10= (abs(ratio1)<1e-10) & (abs(ratio2)<1e-9) & (abs(ratio3)<1e-9)
condNode20= (abs(ratio4)<1e-10)
if (analOk==0) & condNode10 & condNode20:
    print('test '+fname+': ok.')
else:
    lmsg.error(fname+' ERROR.')
