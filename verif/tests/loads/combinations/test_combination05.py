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

import geom
import xc
from model import predefined_spaces
from materials import typical_materials
from solution import database_helper as dbHelper
from solution import predefined_solutions

# Problem type
feProblem= xc.FEProblem()
preprocessor=  feProblem.getPreprocessor
nodes= preprocessor.getNodeHandler
modelSpace= predefined_spaces.StructuralMechanics3D(nodes)
# Create nodes.
n1= nodes.newNodeXYZ(0,0,0)
n2= nodes.newNodeXYZ(1,0,0)
n3= nodes.newNodeXYZ(2,0,0)
n4= nodes.newNodeXYZ(3,0,0)
n5= nodes.newNodeXYZ(0,1,0)
n6= nodes.newNodeXYZ(1,1,0)
n7= nodes.newNodeXYZ(2,1,0)
n8= nodes.newNodeXYZ(3,1,0)
n9= nodes.newNodeXYZ(0,2,0)
n10= nodes.newNodeXYZ(1,2,0)
n11= nodes.newNodeXYZ(2,2,0)
n12= nodes.newNodeXYZ(3,2,0)


# Materials definition
hLosa= typical_materials.defElasticMembranePlateSection(preprocessor, "hLosa",Ec,nuC,densLosa,hLosa)

prestressingSteel= typical_materials.defSteel02(preprocessor, "prestressingSteel",Ep,fy,0.001,tInic)

elements= preprocessor.getElementHandler
# Reinforced concrete deck
elements.defaultMaterial= hLosa.name
elemA= elements.newElement("ShellMITC4",xc.ID([n1.tag, n2.tag, n6.tag, n5.tag]))
elemB= elements.newElement("ShellMITC4",xc.ID([n2.tag, n3.tag, n7.tag, n6.tag]))
elemC= elements.newElement("ShellMITC4",xc.ID([n3.tag, n4.tag, n8.tag, n7.tag]))
elemD= elements.newElement("ShellMITC4",xc.ID([n5.tag, n6.tag, n10.tag, n9.tag]))
elemE= elements.newElement("ShellMITC4",xc.ID([n6.tag, n7.tag, n11.tag, n10.tag]))
elemF= elements.newElement("ShellMITC4",xc.ID([n7.tag, n8.tag, n12.tag, n11.tag]))

# active reinforcement
elements.defaultMaterial= prestressingSteel.name
elements.dimElem= 3 # Dimension of element space
trussA= elements.newElement("Truss",xc.ID([n1.tag, n2.tag]))
trussA.sectionArea= Ap
trussB= elements.newElement("Truss",xc.ID([n2.tag, n3.tag]))
trussB.sectionArea= Ap
trussC= elements.newElement("Truss",xc.ID([n3.tag, n4.tag]))
trussC.sectionArea= Ap
trussD= elements.newElement("Truss",xc.ID([n5.tag, n6.tag]))
trussD.sectionArea= Ap
trussE= elements.newElement("Truss",xc.ID([n6.tag, n7.tag]))
trussE.sectionArea= Ap
trussF= elements.newElement("Truss",xc.ID([n7.tag, n8.tag]))
trussF.sectionArea= Ap
trussG= elements.newElement("Truss",xc.ID([n9.tag, n10.tag]))
trussG.sectionArea= Ap
trussH= elements.newElement("Truss",xc.ID([n10.tag, n11.tag]))
trussH.sectionArea= Ap
trussI= elements.newElement("Truss",xc.ID([n11.tag, n12.tag]))
trussI.sectionAreay= Ap

# Constraints
modelSpace.fixNode000_000(n1.tag)
modelSpace.fixNode000_000(n5.tag)
modelSpace.fixNode000_000(n9.tag)

# Loads definition
loadHandler= preprocessor.getLoadHandler
lPatterns= loadHandler.getLoadPatterns

#Load modulation.
ts= lPatterns.newTimeSeries("constant_ts","ts")
lPatterns.currentTimeSeries= "ts"

lpG= lPatterns.newLoadPattern("default","G")
lpSC= lPatterns.newLoadPattern("default","SC")
lpVT= lPatterns.newLoadPattern("default","VT")
lpNV= lPatterns.newLoadPattern("default","NV")
#lPatterns.currentLoadPattern= "G"
n4Load= lpG.newNodalLoad(n4.tag, xc.Vector([F,0.0,0.0,0.0,0.0,0.0]))
n8Load= lpG.newNodalLoad(n8.tag, xc.Vector([F,0.0,0.0,0.0,0.0,0.0]))
n12Load= lpG.newNodalLoad(n12.tag, xc.Vector([F,0.0,0.0,0.0,0.0,0.0]))

#lPatterns.currentLoadPattern= "SC"
n4Load= lpSC.newNodalLoad(n4.tag, xc.Vector([F,0.0,0.0,0.0,0.0,0.0]))
n8Load= lpSC.newNodalLoad(n8.tag, xc.Vector([F,0.0,0.0,0.0,0.0,0.0]))
n12Load= lpSC.newNodalLoad(n12.tag, xc.Vector([F,0.0,0.0,0.0,0.0,0.0]))

#lPatterns.currentLoadPattern= "VT"
n4Load= lpVT.newNodalLoad(n4.tag, xc.Vector([F,0.0,0.0,0.0,0.0,0.0]))
n8Load= lpVT.newNodalLoad(n8.tag, xc.Vector([F,0.0,0.0,0.0,0.0,0.0]))
n12Load= lpVT.newNodalLoad(n12.tag, xc.Vector([F,0.0,0.0,0.0,0.0,0.0]))

#lPatterns.currentLoadPattern= "NV"
n4Load= lpNV.newNodalLoad(n4.tag, xc.Vector([F,0.0,0.0,0.0,0.0,0.0]))
n8Load= lpNV.newNodalLoad(n8.tag, xc.Vector([F,0.0,0.0,0.0,0.0,0.0]))
n12Load= lpNV.newNodalLoad(n12.tag, xc.Vector([F,0.0,0.0,0.0,0.0,0.0]))

# Combinaciones
combs= loadHandler.getLoadCombinations
comb001= combs.newLoadCombination("ELU001","1.00*G")
comb002= combs.newLoadCombination("ELU002","1.35*G")
comb003= combs.newLoadCombination("ELU003","1.00*G + 1.50*SC")
comb004= combs.newLoadCombination("ELU004","1.00*G + 1.50*SC + 0.90*NV")
comb005= combs.newLoadCombination("ELU005","1.00*G + 1.50*SC + 0.90*VT")
comb006= combs.newLoadCombination("ELU006","1.00*G + 1.50*SC + 0.90*VT + 0.90*NV")
comb007= combs.newLoadCombination("ELU007","1.00*G + 1.50*VT")
comb008= combs.newLoadCombination("ELU008","1.00*G + 1.50*VT + 0.90*NV")
comb009= combs.newLoadCombination("ELU009","1.00*G + 1.05*SC + 1.50*VT")
comb010= combs.newLoadCombination("ELU010","1.00*G + 1.05*SC + 1.50*VT + 0.90*NV")
comb011= combs.newLoadCombination("ELU011","1.00*G + 1.50*NV")
comb012= combs.newLoadCombination("ELU012","1.00*G + 0.90*VT + 1.50*NV")
comb013= combs.newLoadCombination("ELU013","1.00*G + 1.05*SC + 1.50*NV")
comb014= combs.newLoadCombination("ELU014","1.00*G + 1.05*SC + 0.90*VT + 1.50*NV")
comb015= combs.newLoadCombination("ELU015","1.35*G + 1.50*SC")
comb016= combs.newLoadCombination("ELU016","1.35*G + 1.50*SC + 0.90*NV")
comb017= combs.newLoadCombination("ELU017","1.35*G + 1.50*SC + 0.90*VT")
comb018= combs.newLoadCombination("ELU018","1.35*G + 1.50*SC + 0.90*VT + 0.90*NV")
comb019= combs.newLoadCombination("ELU019","1.35*G + 1.50*VT")
comb020= combs.newLoadCombination("ELU020","1.35*G + 1.50*VT + 0.90*NV")
comb021= combs.newLoadCombination("ELU021","1.35*G + 1.05*SC + 1.50*VT")
comb022= combs.newLoadCombination("ELU022","1.35*G + 1.05*SC + 1.50*VT + 0.90*NV")
comb023= combs.newLoadCombination("ELU023","1.35*G + 1.50*NV")
comb024= combs.newLoadCombination("ELU024","1.35*G + 0.90*VT + 1.50*NV")
comb025= combs.newLoadCombination("ELU025","1.35*G + 1.05*SC + 1.50*NV")
comb026= combs.newLoadCombination("ELU026","1.35*G + 1.05*SC + 0.90*VT + 1.50*NV")

printFlag= 0

solProc= predefined_solutions.PlainNewtonRaphson(feProblem, maxNumIter= 10,  convergenceTestTol= 1e-3)

def solveStaticLinearComb(comb,db,dbHelp):
    preprocessor.resetLoadCase()
    dbHelp.helpSolve(comb,db)
    exec(open("solution/database_helper_solve.xci").read())
    ''' 
      print("previousName= ",previousName)
      print("tag= ",comb.tag)
      print("tagPrevia= ",tagPrevia)
      print("descomp previa= ",getComponentsCombPrevia)
      print("resto sobre previa= ",getComponentsRestoSobrePrevia)
    '''
    comb.addToDomain()
    analOk= solProc.solve(1)
    db.save(comb.tag*100)
    comb.removeFromDomain()


dXMin=1e9
dXMax=-1e9

def procesResultVerif(comb):
  nodes= preprocessor.getNodeHandler

  deltaX= n8.getDisp[0] # x displacement of node 8
  global dXMin
  dXMin=min(dXMin,deltaX)
  global dXMax
  dXMax=max(dXMax,deltaX)
  ''' 
    print("tagComb= ",comb.tagComb)
    print("nmbComb= ",nmbComb)
    print("dXMin= ",(dXMin*1e3)," mm\n")
    print("dXMax= ",(dXMax*1e3)," mm\n")
   '''

import os
os.system("rm -r -f /tmp/test_combinacion_05.db")
db= feProblem.newDatabase("BerkeleyDB","/tmp/test_combinacion_05.db")

helper= dbHelper.DatabaseHelperSolve(db)

loadHandler= preprocessor.getLoadHandler
previousName="" 
tagPrevia= 0 
tagSave= 0
for key in combs.getKeys():
  comb= combs[key]
  helper.solveComb(comb,solProc)
  procesResultVerif(comb)

## Small differences after changing
## setSectionArea
# 2019.12.22 Values changed when initial displacements were accounted
# Maybe there is some error here.
ratio1= abs(dXMax-0.143641332276e-3)/0.143641332276e-3
ratio2= abs(dXMin+0.162147274048e-3)/0.162147274048e-3

'''
print("dXMax= ",(dXMax*1e3)," mm\n")
print("dXMin= ",(dXMin*1e3)," mm\n")
print("ratio1= ",ratio1)
print("ratio2= ",ratio2)
''' 


from misc_utils import log_messages as lmsg
fname= os.path.basename(__file__)
if (ratio1<1e-2) & (ratio2<1e-2) :
    print('test '+fname+': ok.')
else:
    lmsg.error(fname+' ERROR.')

os.system("rm -r -f /tmp/test_combinacion_05.db") # Your garbage you clean it
