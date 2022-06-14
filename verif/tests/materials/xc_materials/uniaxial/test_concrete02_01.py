# -*- coding: utf-8 -*-
''' Home made test.'''
from __future__ import print_function

__author__= "Luis C. Pérez Tato (LCPT) and Ana Ortega (AOO)"
__copyright__= "Copyright 2015, LCPT and AOO"
__license__= "GPL"
__version__= "3.0"
__email__= "l.pereztato@gmail.com"

l= 1e-7 # Distance between nodes

import xc
from solution import predefined_solutions
from model import predefined_spaces
from materials import typical_materials
from postprocess.reports import graph_material 
from materials.ehe import EHE_materials
# import matplotlib.pyplot as plt
import numpy as np

# Model definition
feProblem= xc.FEProblem()
preprocessor=  feProblem.getPreprocessor
nodes= preprocessor.getNodeHandler
nodes.dimSpace= 1 # One coordinate for each node.
nodes.numDOFs= 1 # One degree of freedom for each node.

n1= nodes.newNodeX(1)
n2= nodes.newNodeX(1.0+l)

# Materials definition
concrAux= EHE_materials.HA25
concr=typical_materials.defConcrete02(preprocessor=preprocessor,name='concr25',epsc0=concrAux.epsilon0(),fpc=concrAux.fmaxK(),fpcu=0.85*concrAux.fmaxK(),epscu=concrAux.epsilonU(),ratioSlope=0.1,ft=concrAux.fctk(),Ets=concrAux.E0()/19.0)


# Elements definition
elements= preprocessor.getElementHandler
elements.defaultMaterial='concr25'
elements.dimElem= 1 # Dimension of element space
elem1= elements.newElement("ZeroLength",xc.ID([n1.tag, n2.tag]))

   
# Constraints
constraints= preprocessor.getBoundaryCondHandler
spc= constraints.newSPConstraint(n1.tag, 0,0.0) # Node 1


# Loads definition
loadHandler= preprocessor.getLoadHandler
lPatterns= loadHandler.getLoadPatterns
#Load modulation.
ts= lPatterns.newTimeSeries("constant_ts","ts")
lPatterns.currentTimeSeries= "ts"
#Load case definition
lp0= lPatterns.newLoadPattern("default","0")
#lPatterns.currentLoadPattern= "0"
#We add the load case to domain.
#lPatterns.addToDomain(lp0.name)

# Solution
analysis= predefined_solutions.plain_static_modified_newton(feProblem)
dom= preprocessor.getDomain

strainData= [-1.55637e-3,-1.4037e-3,-1.28474e-3,-1.18796e-3,-1.09365e-3,-1.01316e-3,-0.93755e-3,-0.86304e-3,-0.796515e-3,-0.732951e-3,-0.672066e-3,-0.611906e-3,-0.556246e-3,-0.502408e-3,-0.450266e-3,-0.399699e-3,-0.3506e-3,-0.30244e-3,-0.25618e-3,-0.211025e-3,-0.166922e-3,-0.123819e-3,-0.0816646e-3,-0.0404e-3,0e-3,0.04e-3,0.06e-3,0.068e-3,0.0721905e-3]
stressData= [-2.37699e7,-22.7776e6,-21.8025e6,-20.8787e6,-19.8658e6,-18.9134e6,-17.945e6,-16.9208e6,-15.9476e6,-14.9662e6,-13.9787e6,-12.9575e6,-11.9723e6,-10.9826e6,-9.98952e6,-8.99399e6,-7.99676e6,-6.98931e6,-5.99432e6,-4.9973e6,-3.99891e6,-2.99966e6,-1.99993e6,-0.999799e6,0e6,1e6,1.5e6,1.7e6,1.79499e6]
stressInput= [-24e6,-23e6,-22e6,-21e6,-20e6,-19e6,-18e6,-17e6,-16e6,-15e6,-14e6,-13e6,-12e6,-11e6,-10e6,-9e6,-8e6,-7e6,-6e6,-5e6,-4e6,-3e6,-2e6,-1e6,0.0,1e6,1.5e6,1.7e6,1.8e6]
calculatedStresses= []
calculatedStrains= []

for stress in stressInput:
    dom.revertToStart()
    lPatterns.removeFromDomain(lp0.name)
    lp0.clearLoads()
    nl= lp0.newNodalLoad(n2.tag, xc.Vector([stress]))
    lPatterns.addToDomain(lp0.name)
    result= analysis.analyze(1)
    calculatedStresses.append(-elem1.getResistingForce()[0])
    calculatedStrains.append(elem1.getMaterials()[0].getStrain())

residStresses= (np.array(calculatedStresses) - np.array(stressData))
residStrains= (np.array(calculatedStrains) - np.array(strainData))

ratio1= np.linalg.norm(residStresses)/-concrAux.fmaxK()
ratio2= np.linalg.norm(residStrains)/3.5e-3


''' 
print('E0= ', concr.getInitialTangent()/1e9, 2*concr.fpc/concr.epsc0/1e9)
print('epsc0= ', concr.epsc0)
print('fmaxK= ', concr.fpc/1e6)
print('epscu= ', concr.epscu)
print('ft= ', concr.ft/1e6)
print('Ets= ', concr.Ets/1e9, concr.getInitialTangent()/19e9)
print(calculatedStresses)
print('error in stress calculation: ', residStresses)
print(calculatedStrains)
print(residStrains)
print("ratio1= ",ratio1)
print("ratio2= ",ratio2)
retval= graph_material.UniaxialMaterialDiagramGraphic(epsMin=-0.0035,epsMax=0.0016,title=' concrete02 characteristic stress-strain diagram')
retval.setupGraphic(plt,concr)
plt.plot(strainData, stressData, '-')
retval.savefig(plt,'concrete02_diagram.jpeg')
   '''
  
import os
from misc_utils import log_messages as lmsg
fname= os.path.basename(__file__)
if((ratio1<1e-5) and (ratio2<1e-5)) :
    print('test '+fname+': ok.')
else:
    lmsg.error(fname+' ERROR.')
