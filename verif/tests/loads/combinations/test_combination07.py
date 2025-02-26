# -*- coding: utf-8 -*-
from __future__ import print_function
'''Using the database as combination results storage to accelerate computation. Home made test.'''

__author__= "Luis C. Pérez Tato (LCPT)"
__copyright__= "Copyright 2014, LCPT"
__license__= "GPL"
__version__= "3.0"
__email__= "l.pereztato@gmail.com"

Ec= 2e5*9.81/1e-4 # Concrete Young modulus (Pa).
nuC= 0.2 # Concrete Poisson's ratio EHE-08.
hLosa= 0.2 # Thickness.
densLosa= 2500*hLosa # Deck density kg/m2.
# Load
F= 5.5e4 # Load magnitude en N

# active reinforcement
Ep= 190e9 # Elastic modulus expressed in MPa
Ap= 140e-6 # bar area expressed in square meters
fMax= 1860e6 # Maximum unit load of the material expressed in MPa.
fy= 1171e6 # Yield stress of the material expressed in Pa.
tInic= 0.75**2*fMax # Effective prestress (0.75*P0 y 25% prestress losses).

import xc
from model import predefined_spaces
from materials import typical_materials
from solution import database_helper as dbHelper


# Problem type
feProblem= xc.FEProblem()
preprocessor=  feProblem.getPreprocessor
nodes= preprocessor.getNodeHandler
modelSpace= predefined_spaces.StructuralMechanics3D(nodes)
nod1= nodes.newNodeXYZ(0,0,0)
nod2= nodes.newNodeXYZ(1,0,0)
nod3= nodes.newNodeXYZ(2,0,0)
nod4= nodes.newNodeXYZ(3,0,0)
nod5= nodes.newNodeXYZ(0,1,0)
nod6= nodes.newNodeXYZ(1,1,0)
nod7= nodes.newNodeXYZ(2,1,0)
nod8= nodes.newNodeXYZ(3,1,0)
nod9= nodes.newNodeXYZ(0,2,0)
nod10= nodes.newNodeXYZ(1,2,0)
nod11= nodes.newNodeXYZ(2,2,0)
nod12= nodes.newNodeXYZ(3,2,0)


# Materials definition

hLosa= typical_materials.defElasticMembranePlateSection(preprocessor, "hLosa",Ec,nuC,densLosa,hLosa)

prestressingSteel= typical_materials.defSteel02(preprocessor, "prestressingSteel",Ep,fy,0.001,tInic)

elements= preprocessor.getElementHandler
# Reinforced concrete deck
elements.defaultMaterial= hLosa.name
shell1= elements.newElement("ShellMITC4",xc.ID([nod1.tag,nod2.tag,nod6.tag,nod5.tag]))
shell2= elements.newElement("ShellMITC4",xc.ID([nod2.tag,nod3.tag,nod7.tag,nod6.tag]))
shell3= elements.newElement("ShellMITC4",xc.ID([nod3.tag,nod4.tag,nod8.tag,nod7.tag]))
shell4= elements.newElement("ShellMITC4",xc.ID([nod5.tag,nod6.tag,nod10.tag,nod9.tag]))
shell5= elements.newElement("ShellMITC4",xc.ID([nod6.tag,nod7.tag,nod11.tag,nod10.tag]))
shell6= elements.newElement("ShellMITC4",xc.ID([nod7.tag,nod8.tag,nod12.tag,nod11.tag]))

# active reinforcement
elements.defaultMaterial= prestressingSteel.name
elements.dimElem= 3 # Dimension of element space
truss1= elements.newElement("Truss",xc.ID([nod1.tag,nod2.tag]))
truss1.sectionArea= Ap
truss2= elements.newElement("Truss",xc.ID([nod2.tag,nod3.tag]))
truss2.sectionArea= Ap
truss3= elements.newElement("Truss",xc.ID([nod3.tag,nod4.tag]))
truss3.sectionArea= Ap
truss4= elements.newElement("Truss",xc.ID([nod5.tag,nod6.tag]))
truss4.sectionArea= Ap
truss5= elements.newElement("Truss",xc.ID([nod6.tag,nod7.tag]))
truss5.sectionArea= Ap
truss6= elements.newElement("Truss",xc.ID([nod7.tag,nod8.tag]))
truss6.sectionArea= Ap
truss7= elements.newElement("Truss",xc.ID([nod9.tag,nod10.tag]))
truss7.sectionArea= Ap
truss8= elements.newElement("Truss",xc.ID([nod10.tag,nod11.tag]))
truss8.sectionArea= Ap
truss9= elements.newElement("Truss",xc.ID([nod11.tag,nod12.tag]))
truss9.sectionArea= Ap

# Constraints

modelSpace.fixNode000_000(nod1.tag)
modelSpace.fixNode000_000(nod5.tag)
modelSpace.fixNode000_000(nod9.tag)

# Loads definition
loadHandler= preprocessor.getLoadHandler

lPatterns= loadHandler.getLoadPatterns

#Load modulation.
ts= lPatterns.newTimeSeries("constant_ts","ts")
lPatterns.currentTimeSeries= "ts"

lpG0= lPatterns.newLoadPattern("default","G0")
lpG= lPatterns.newLoadPattern("default","G")
lpSC= lPatterns.newLoadPattern("default","SC")
lpVT= lPatterns.newLoadPattern("default","VT")
lpNV= lPatterns.newLoadPattern("default","NV")

#lPatterns.currentLoadPattern= "G"
lpG.newNodalLoad(nod4.tag,xc.Vector([F,0.0,0.0,0.0,0.0,0.0]))
lpG.newNodalLoad(nod8.tag,xc.Vector([F,0.0,0.0,0.0,0.0,0.0]))
lpG.newNodalLoad(nod12.tag,xc.Vector([F,0.0,0.0,0.0,0.0,0.0]))

#lPatterns.currentLoadPattern= "SC"
lpSC.newNodalLoad(nod4.tag,xc.Vector([F,0.0,0.0,0.0,0.0,0.0]))
lpSC.newNodalLoad(nod8.tag,xc.Vector([F,0.0,0.0,0.0,0.0,0.0]))
lpSC.newNodalLoad(nod12.tag,xc.Vector([F,0.0,0.0,0.0,0.0,0.0]))

#lPatterns.currentLoadPattern= "VT"
lpVT.newNodalLoad(nod4.tag,xc.Vector([F,0.0,0.0,0.0,0.0,0.0]))
lpVT.newNodalLoad(nod8.tag,xc.Vector([F,0.0,0.0,0.0,0.0,0.0]))
lpVT.newNodalLoad(nod12.tag,xc.Vector([F,0.0,0.0,0.0,0.0,0.0]))

#lPatterns.currentLoadPattern= "NV"
lpNV.newNodalLoad(nod4.tag,xc.Vector([F,0.0,0.0,0.0,0.0,0.0]))
lpNV.newNodalLoad(nod8.tag,xc.Vector([F,0.0,0.0,0.0,0.0,0.0]))
lpNV.newNodalLoad(nod12.tag,xc.Vector([F,0.0,0.0,0.0,0.0,0.0]))


# Load combinations
combs= loadHandler.getLoadCombinations
comb= combs.newLoadCombination("ELU001","1.00*G")
comb= combs.newLoadCombination("ELU002","1.35*G")
comb= combs.newLoadCombination("ELU003","1.00*G + 1.50*SC")
comb= combs.newLoadCombination("ELU004","1.00*G + 1.50*SC + 0.90*NV")
comb= combs.newLoadCombination("ELU005","1.00*G + 1.50*SC + 0.90*VT")
comb= combs.newLoadCombination("ELU006","1.00*G + 1.50*SC + 0.90*VT + 0.90*NV")
comb= combs.newLoadCombination("ELU007","1.00*G + 1.50*VT")
comb= combs.newLoadCombination("ELU008","1.00*G + 1.50*VT + 0.90*NV")
comb= combs.newLoadCombination("ELU009","1.00*G + 1.05*SC + 1.50*VT")
comb= combs.newLoadCombination("ELU010","1.00*G + 1.05*SC + 1.50*VT + 0.90*NV")
comb= combs.newLoadCombination("ELU011","1.00*G + 1.50*NV")
comb= combs.newLoadCombination("ELU012","1.00*G + 0.90*VT + 1.50*NV")
comb= combs.newLoadCombination("ELU013","1.00*G + 1.05*SC + 1.50*NV")
comb= combs.newLoadCombination("ELU014","1.00*G + 1.05*SC + 0.90*VT + 1.50*NV")
comb= combs.newLoadCombination("ELU015","1.35*G + 1.50*SC")
comb= combs.newLoadCombination("ELU016","1.35*G + 1.50*SC + 0.90*NV")
comb= combs.newLoadCombination("ELU017","1.35*G + 1.50*SC + 0.90*VT")
comb= combs.newLoadCombination("ELU018","1.35*G + 1.50*SC + 0.90*VT + 0.90*NV")
comb= combs.newLoadCombination("ELU019","1.35*G + 1.50*VT")
comb= combs.newLoadCombination("ELU020","1.35*G + 1.50*VT + 0.90*NV")
comb= combs.newLoadCombination("ELU021","1.35*G + 1.05*SC + 1.50*VT")
comb= combs.newLoadCombination("ELU022","1.35*G + 1.05*SC + 1.50*VT + 0.90*NV")
comb= combs.newLoadCombination("ELU023","1.35*G + 1.50*NV")
comb= combs.newLoadCombination("ELU024","1.35*G + 0.90*VT + 1.50*NV")
comb= combs.newLoadCombination("ELU025","1.35*G + 1.05*SC + 1.50*NV")
comb= combs.newLoadCombination("ELU026","1.35*G + 1.05*SC + 0.90*VT + 1.50*NV")
 

printFlag= 0

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
ctest= solutionStrategy.newConvergenceTest("norm_unbalance_conv_test")
ctest.tol= 1e-3
ctest.maxNumIter= 10
#ctest.printFlag= printFlag
integ= solutionStrategy.newIntegrator("load_control_integrator",xc.Vector([]))
soe= solutionStrategy.newSystemOfEqn("band_gen_lin_soe")
solver= soe.newSolver("band_gen_lin_lapack_solver")
analysis= solu.newAnalysis("static_analysis","solutionStrategy","")


def solve_static_linear_comb(comb,tagSaveFase0,hlp):
    preprocessor.resetLoadCase()
    db.restore(tagSaveFase0)
    hlp.helpSolve(comb)
    comb.addToDomain()
    analOk= analysis.analyze(1)
    comb.removeFromDomain()
    hlp.db.save(comb.tag*100)

dXMin=1e9
dXMax=-1e9

def process_results_verif(comb):
    deltaX= nod8.getDisp[0] # x displacement of node 8
    global dXMin
    dXMin=min(dXMin,deltaX)
    global dXMax
    dXMax=max(dXMax,deltaX)
    ''' 
      print("tagComb= ",comb.tag)
      print("nmbComb= ",comb.getName)
      print("dXMin= ",(dXMin*1e3)," mm\n")
      print("dXMax= ",(dXMax*1e3)," mm\n")
     '''

import os
os.system("rm -r -f /tmp/test_combinacion_07.db")
db= feProblem.newDatabase("BerkeleyDB","/tmp/test_combinacion_07.db")
helper= dbHelper.DatabaseHelperSolve(db)

# Fase 0: pretensado
preprocessor.resetLoadCase()
loadHandler= preprocessor.getLoadHandler

combs= loadHandler.getLoadCombinations
comb= combs.newLoadCombination("FASE0","1.00*G0")
tagSaveFase0= comb.tag*100
comb.addToDomain()
analOk= analysis.analyze(1)
comb.removeFromDomain()

combs.remove("FASE0") # We withdraw the combination (is no longer useful).
db.save(tagSaveFase0)



previousName="" 
tagPrevia= 0
tagSave= 0
for key in combs.getKeys():
  c= combs[key]
  solve_static_linear_comb(c,tagSaveFase0,helper)
  process_results_verif(comb)

# 2019.12.22 Values changed when initial displacements were accounted
# dXMax changed from 0.115734e-3 to 0.0252486379903e-3
# dXMin changed from -0.0872328e-3 to -0.293653533883e-3
# 2023.08.19 Values changed again to the old values when initial displacements
# were replaced by initial strains (persistentInitialDeformation).
# dXMax changed from 0.0252486379903e-3 to 0.11573400959124706e-3
# dXMin changed from -0.293653533883e-3 to -0.08723276034009012 
# Maybe there was some error here.
ratio1= abs(dXMax-0.11573400959124706e-3)/0.11573400959124706e-3
ratio2= abs(dXMin+0.08723276034009012e-3)/0.08723276034009012e-3

''' 
print("dXMin= ",(dXMin*1e3)," mm\n")
print("dXMax= ",(dXMax*1e3)," mm\n")
print("ratio1= ",ratio1)
print("ratio2= ",ratio2)
   '''

from misc_utils import log_messages as lmsg
fname= os.path.basename(__file__)
if (ratio1<1e-5) & (ratio2<1e-5) :
    print('test '+fname+': ok.')
else:
    lmsg.error(fname+' ERROR.')

os.system("rm -r -f /tmp/test_combinacion_07.db") # Your garbage you clean it
