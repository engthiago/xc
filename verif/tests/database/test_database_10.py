# -*- coding: utf-8 -*-
''' Home made test. Check of the restore function.
   In the second analysis the solution is obtained
   at the first iteration because the state
   of the model is restored from the database.
'''

from __future__ import print_function

__author__= "Luis C. Pérez Tato (LCPT)"
__copyright__= "Copyright 2014, LCPT"
__license__= "GPL"
__version__= "3.0"
__email__= "l.pereztato@gmail.com"

L= 1.0 # Bar length (m)
E= 190e9 # Elastic modulus expressed in Pa
A= 140e-6 # bar area expressed in square meters
fMax= 1860e6 # Maximum unit load of the material expressed in Pa.
fy= 1171e6 # Yield stress of the material expressed in Pa.
tInic= 0.75**2*fMax # Effective prestress (0.75*P0 y 25% prestress losses).

import xc_base
import geom
import xc
from model import predefined_spaces
from materials import typical_materials

# Problem type
feProblem= xc.FEProblem()
preprocessor=  feProblem.getPreprocessor
nodes= preprocessor.getNodeHandler

modelSpace= predefined_spaces.SolidMechanics2D(nodes)
n1= nodes.newNodeXY(0.0,0.0)
n2= nodes.newNodeXY(L,0.0)


# Materials definition
mat= typical_materials.defSteel02(preprocessor, "prestressingSteel",E,fy,0.001,tInic)
    
# Elements definition
elements= preprocessor.getElementHandler
elements.defaultMaterial= mat.name
elements.dimElem= 2 # Dimension of element space
truss= elements.newElement("Truss",xc.ID([n1.tag,n2.tag]))
truss.sectionArea= A

    
# Constraints
constraints= preprocessor.getBoundaryCondHandler
spc= constraints.newSPConstraint(n1.tag, 0,0.0)
spc= constraints.newSPConstraint(n1.tag, 1,0.0)
spc= constraints.newSPConstraint(n2.tag, 0,0.0)
spc= constraints.newSPConstraint(n2.tag, 1,0.0)

    

solu= feProblem.getSoluProc
solCtrl= solu.getSoluControl


solModels= solCtrl.getModelWrapperContainer
sm= solModels.newModelWrapper("sm")


cHandler= sm.newConstraintHandler("penalty_constraint_handler")
cHandler.alphaSP= 1.0e15
cHandler.alphaMP= 1.0e15
numberer= sm.newNumberer("default_numberer")
numberer.useAlgorithm("rcm")

solutionStrategies= solCtrl.getSolutionStrategyContainer
solutionStrategy= solutionStrategies.newSolutionStrategy("solutionStrategy","sm")
solAlgo= solutionStrategy.newSolutionAlgorithm("newton_raphson_soln_algo")
convTest= solutionStrategy.newConvergenceTest("norm_unbalance_conv_test")
convTest.tol=1.0e-9
convTest.maxNumIter= 10
convTest.printFlag= 0
integ= solutionStrategy.newIntegrator("load_control_integrator",xc.Vector([]))
soe= solutionStrategy.newSystemOfEqn("band_gen_lin_soe")
solver= soe.newSolver("band_gen_lin_lapack_solver")


analysis= solu.newAnalysis("static_analysis","solutionStrategy","")
result= analysis.analyze(1)

# Store element tags
eTag= truss.tag

import os
os.system("rm -rf /tmp/test10.db")
db= feProblem.newDatabase("BerkeleyDB","/tmp/test10.db")
db.save(100)
feProblem.clearAll()
db.restore(100)

solu= feProblem.getSoluProc
solCtrl= solu.getSoluControl


solModels= solCtrl.getModelWrapperContainer
sm= solModels.newModelWrapper("sm")


cHandler= sm.newConstraintHandler("penalty_constraint_handler")
cHandler.alphaSP= 1.0e15
cHandler.alphaMP= 1.0e15
numberer= sm.newNumberer("default_numberer")
numberer.useAlgorithm("rcm")

solutionStrategies= solCtrl.getSolutionStrategyContainer
solutionStrategy= solutionStrategies.newSolutionStrategy("solutionStrategy","sm")
solAlgo= solutionStrategy.newSolutionAlgorithm("newton_raphson_soln_algo")
convTest= solutionStrategy.newConvergenceTest("norm_unbalance_conv_test")
convTest.tol=1.0e-9
convTest.maxNumIter= 10
convTest.printFlag= 0
integ= solutionStrategy.newIntegrator("load_control_integrator",xc.Vector([]))
soe= solutionStrategy.newSystemOfEqn("band_gen_lin_soe")
solver= soe.newSolver("band_gen_lin_lapack_solver")

analysis= solu.newAnalysis("static_analysis","solutionStrategy","")
result= analysis.analyze(1)


elements= preprocessor.getElementHandler

elem1= elements.getElement(eTag)
elem1.getResistingForce()
ratio= (tInic*A-elem1.getN())/(tInic*A)

'''
print("force= ",getN())
print("prestressing force= ",(tInic*A))
print("strain= ",getStrain)
print("ratio= ",ratio)
'''
from misc_utils import log_messages as lmsg
fname= os.path.basename(__file__)
if abs(ratio)<0.02:
    print('test '+fname+': ok.')
else:
    lmsg.error(fname+' ERROR.')

os.system("rm -rf /tmp/test10.db") # Your garbage you clean it
