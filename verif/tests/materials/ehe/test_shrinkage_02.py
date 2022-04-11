# -*- coding: utf-8 -*-
''' Home made test.'''

from __future__ import print_function
#    Shrinking load pattern test.

import geom
import xc
from materials.ehe import EHE_materials

__author__= "Luis C. Pérez Tato (LCPT)"
__copyright__= "Copyright 2014, LCPT"
__license__= "GPL"
__version__= "3.0"
__email__= "l.pereztato@gmail.com"

concrHA30= EHE_materials.HA30
concrHA30.cemType='N'
fckHA30= 30e6 # Concrete characteristic compressive strength HA-30.
Hrel= 0.8 # Relative humidity of the air.
Ec= 2e5*9.81/1e-4 # Concrete Young modulus (Pa).
nuC= 0.2 # Concrete Poisson's ratio EHE-08.
deckThk= 0.2 # Thickness.
densLosa= 2500*deckThk # Deck density kg/m2.
t0= 28 # Concrete age at loading
tFin= 10000 # Service life.
arido= "cuarcita"


# Load
F= 5.5e4 # Load magnitude en N

# Concrete shrinkage
tS= 7 # Inicio del secado.

# active reinforcement
Ep= 190e9 # Elastic modulus expressed in MPa
Ap= 140e-6 # bar area expressed in square meters
fMax= 1860e6 # Maximum unit load of the material expressed in MPa.
fy= 1171e6 # Yield stress of the material expressed in Pa.
tInic= 0.75**2*fMax # Effective prestress (0.75*P0 y 25% prestress losses).

import geom
import xc
from solution import predefined_solutions
from model import predefined_spaces
from materials import typical_materials

# Problem type
feProblem= xc.FEProblem()
preprocessor=  feProblem.getPreprocessor
nodes= preprocessor.getNodeHandler
modelSpace= predefined_spaces.StructuralMechanics3D(nodes)
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

deckMat= typical_materials.defElasticMembranePlateSection(preprocessor, "deckMat",Ec,nuC,densLosa,deckThk)

prestressingSteel= typical_materials.defSteel02(preprocessor, "prestressingSteel",Ep,fy,0.001,tInic)

elements= preprocessor.getElementHandler
# Reinforced concrete deck
elements.defaultMaterial= deckMat.name
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
trussI.sectionArea= Ap

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

lpSHRINKAGE= lPatterns.newLoadPattern("default","SHRINKAGE")
lpFLU= lPatterns.newLoadPattern("default","FLU")

lpG= lPatterns.newLoadPattern("default","G")
lpSC= lPatterns.newLoadPattern("default","SC")
lpVT= lPatterns.newLoadPattern("default","VT")
lpNV= lPatterns.newLoadPattern("default","NV")

nodes= preprocessor.getNodeHandler
lpG.newNodalLoad(n4.tag,xc.Vector([F,0.0,0.0,0.0,0.0,0.0]))
lpG.newNodalLoad(n8.tag,xc.Vector([F,0.0,0.0,0.0,0.0,0.0]))
lpG.newNodalLoad(n12.tag,xc.Vector([F,0.0,0.0,0.0,0.0,0.0]))

lpSC.newNodalLoad(n4.tag,xc.Vector([F,0.0,0.0,0.0,0.0,0.0]))
lpSC.newNodalLoad(n8.tag,xc.Vector([F,0.0,0.0,0.0,0.0,0.0]))
lpSC.newNodalLoad(n12.tag,xc.Vector([F,0.0,0.0,0.0,0.0,0.0]))

lpVT.newNodalLoad(n4.tag,xc.Vector([F,0.0,0.0,0.0,0.0,0.0]))
lpVT.newNodalLoad(n8.tag,xc.Vector([F,0.0,0.0,0.0,0.0,0.0]))
lpVT.newNodalLoad(n12.tag,xc.Vector([F,0.0,0.0,0.0,0.0,0.0]))

lpNV.newNodalLoad(n4.tag,xc.Vector([F,0.0,0.0,0.0,0.0,0.0]))
lpNV.newNodalLoad(n8.tag,xc.Vector([F,0.0,0.0,0.0,0.0,0.0]))
lpNV.newNodalLoad(n12.tag,xc.Vector([F,0.0,0.0,0.0,0.0,0.0]))


# Combinaciones
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



#printFlag= 0
#ctest.printFlag= printFlag
analysis= predefined_solutions.penalty_newton_raphson(feProblem)

from solution import database_helper

def solveStaticLinearComb(tagComb,comb,tagSaveFase0):
    preprocessor.resetLoadCase()
    db.restore(tagSaveFase0)

    #exec(open("solution/database_helper_solve.xci").read())

    ''' 
    print("previousName= ",previousName)
    print("tag= ",tagComb)
    print("tagPrevia= ",tagPrevia)
    print("descomp previa= ",getComponentsCombPrevia)
    print("resto sobre previa= ",getComponentsRestoSobrePrevia)
    '''

    comb.addToDomain()
    result= analysis.analyze(1)
    db.save(tagComb*100)
    comb.removeFromDomain()


dXMin=1e9
dXMax=-1e9

def procesResultVerif(tagComb, nmbComb):
    nodes= preprocessor.getNodeHandler
    deltaX= n8.getDisp[0] # x displacement of node 8
    global dXMin; dXMin= min(dXMin,deltaX)
    global dXMax; dXMax= max(dXMax,deltaX)
    ''' 
    print("nmbComb= ",nmbComb)
    print("tagComb= ",tagComb)
    print("descomp= ",getComponents("%3.1f"))
    print("dXMin= ",(dXMin*1e3)," mm\n")
    print("dXMax= ",(dXMax*1e3)," mm\n")
   '''
import os
os.system("rm -r -f /tmp/test_shrinkage_02.db")
db= feProblem.newDatabase("BerkeleyDB","/tmp/test_shrinkage_02.db")

# Fase 0: pretensado, shrinking
# Fase 1: pretensado, shrinking and creep

# Shrinkage strains.
sets= preprocessor.getSets

shells= preprocessor.getSets.defSet("shells")

elements= preprocessor.getSets.getSet("total").getElements
for e in elements:
    dim= e.getDimension
    if(dim==2):
        shells.getElements.append(e)

shellElems= shells.getElements
for e in shellElems:
    averageSideLength= e.getPerimeter(True)/4.0
    material= e.getPhysicalProperties.getVectorMaterials[0]
    grueso= material.h
    Ac= averageSideLength*grueso
    u= 2*averageSideLength+grueso
    espMedio= 2.0*Ac/u
    RH=Hrel*100
    h0mm=espMedio*1e3
    epsShrinkage= concrHA30.getShrEpscs(tFin,tS,RH,h0mm)


#loadHandler.currentLoadPattern= "SHRINKAGE"

for e in shellElems:
    eleLoad= lpSHRINKAGE.newElementalLoad("shell_strain_load")
    eleLoad.elementTags= xc.ID([e.tag]) 
    eleLoad.setStrainComp(0,0,epsShrinkage) #(id of Gauss point, id of component, value)
    eleLoad.setStrainComp(1,0,epsShrinkage)
    eleLoad.setStrainComp(2,0,epsShrinkage)
    eleLoad.setStrainComp(3,0,epsShrinkage)
    eleLoad.setStrainComp(0,1,epsShrinkage) #(id of Gauss point, id of component, value)
    eleLoad.setStrainComp(1,1,epsShrinkage)
    eleLoad.setStrainComp(2,1,epsShrinkage)
    eleLoad.setStrainComp(3,1,epsShrinkage)


preprocessor.resetLoadCase()

comb= combs.newLoadCombination("FASE0","1.00*SHRINKAGE")
tagSaveFase0= comb.tag*100
comb.addToDomain()
result= analysis.analyze(1)
combs.remove("FASE0")  # Avoid adding shrinking on each computation.
db.save(tagSaveFase0)




db.restore(tagSaveFase0)

previousName="" 
tagPrevia= 0 
tagSave= 0
for key in combs.getKeys():
    comb= combs[key]
    solveStaticLinearComb(comb.tag,comb,tagSaveFase0)
    procesResultVerif(comb.tag,comb.getName)

dXMaxTeor= -0.752509e-3
ratio1= abs(((dXMax-dXMaxTeor)/dXMaxTeor))
dXMinTeor= -0.955324e-3
ratio2= abs(((dXMin-dXMinTeor)/dXMinTeor))

''' 
print("dXMax= ",(dXMax*1e3)," mm")
print("dXMaxTeor= ",(dXMaxTeor*1e3)," mm")
print("ratio1= ",ratio1)
print("dXMin= ",(dXMin*1e3)," mm")
print("dXMinTeor= ",(dXMinTeor*1e3)," mm")
print("ratio2= ",ratio2)
   '''

import os
from misc_utils import log_messages as lmsg
fname= os.path.basename(__file__)
if (ratio1<1e-5) & (ratio2<1e-5) :
    print('test '+fname+': ok.')
else:
    lmsg.error(fname+' ERROR.')

os.system("rm -r -f /tmp/test_shrinkage_02.db") # Your garbage you clean it
