# -*- coding: utf-8 -*-
from __future__ import print_function
''' Test to verify the mode combinations
taken from example A87 of Solvia Verification Manual.
This exercise is based on example E26.8 of the 
book «Dynamics of Structures» by Clough, R. W., and Penzien, J. '''
import xc

from model import predefined_spaces
from materials import typical_materials
import math
import numpy

__author__= "Luis C. Pérez Tato (LCPT)"
__copyright__= "Copyright 2014, LCPT"
__license__= "GPL"
__version__= "3.0"
__email__= "l.pereztato@gmail.com"

masaExtremo= 1e-2 # Masa en kg.
nodeMassMatrix= xc.Matrix([[masaExtremo,0,0,0,0,0],
                                         [0,masaExtremo,0,0,0,0],
                                         [0,0,masaExtremo,0,0,0],
                                         [0,0,0,0,0,0],
                                         [0,0,0,0,0,0],
                                         [0,0,0,0,0,0]])
EMat= 1 # Elastic modulus.
nuMat= 0 # Poisson's ratio.
GMat= EMat/(2.0*(1+nuMat)) # Shear modulus.

Iyy= 1 # Flexural inertia on y axis.
Izz= 1 # Flexural inertia on z axis.
Ir= 4/3.0 # Torsional inertia.
area= 1e7 # Section area.
Lx= 1
Ly= 1
Lz= 1


# Problem type
feProblem= xc.FEProblem()
preprocessor=  feProblem.getPreprocessor
nodes= preprocessor.getNodeHandler
modelSpace= predefined_spaces.StructuralMechanics3D(nodes)
nod0= nodes.newNodeIDXYZ(0,0,0,0)
nod1= nodes.newNodeXYZ(0,-Ly,0)
nod2= nodes.newNodeXYZ(0,-Ly,-Lz)
nod3= nodes.newNodeXYZ(Lx,-Ly,-Lz)
nod3.mass= nodeMassMatrix

constraints= preprocessor.getBoundaryCondHandler
nod0.fix(xc.ID([0,1,2,3,4,5]),xc.Vector([0,0,0,0,0,0]))

# Materials definition
scc= typical_materials.defElasticSection3d(preprocessor, "scc",area,EMat,GMat,Izz,Iyy,Ir)

# Geometric transformation(s)
linX= modelSpace.newLinearCrdTransf("linX",xc.Vector([1,0,0]))
linY= modelSpace.newLinearCrdTransf("linY",xc.Vector([0,1,0]))

# Elements definition
elements= preprocessor.getElementHandler
elements.defaultTransformation= linX.name
elements.defaultMaterial= scc.name
beam3d= elements.newElement("ElasticBeam3d",xc.ID([0,1]))
beam3d= elements.newElement("ElasticBeam3d",xc.ID([1,2]))
elements.defaultTransformation= linY.name
beam3d= elements.newElement("ElasticBeam3d",xc.ID([2,3]))


# Solution procedure
solu= feProblem.getSoluProc
solCtrl= solu.getSoluControl


solModels= solCtrl.getModelWrapperContainer
sm= solModels.newModelWrapper("sm")


cHandler= sm.newConstraintHandler("transformation_constraint_handler")

numberer= sm.newNumberer("default_numberer")
numberer.useAlgorithm("rcm")

solutionStrategies= solCtrl.getSolutionStrategyContainer
solutionStrategy= solutionStrategies.newSolutionStrategy("solutionStrategy","sm")
solAlgo= solutionStrategy.newSolutionAlgorithm("frequency_soln_algo")
integ= solutionStrategy.newIntegrator("eigen_integrator",xc.Vector([]))

soe= solutionStrategy.newSystemOfEqn("full_gen_eigen_soe")
solver= soe.newSolver("full_gen_eigen_solver")

analysis= solu.newAnalysis("modal_analysis","solutionStrategy","")
analOk= analysis.analyze(3)
periods= analysis.getPeriods()
angularFrequencies= analysis.getAngularFrequencies()
aceleraciones= [2.27,2.45,6.98]
crossCQCCoefficients= analysis.getCQCModalCrossCorrelationCoefficients(xc.Vector([0.05,0.05,0.05]))


from misc import matrix_utils
eigNod3= nod3.getNormalizedEigenvectors
eigenvectors= matrix_utils.matrixToNumpyArray(eigNod3)
modos= eigenvectors[0:3,0:3]
modo1= modos[:,0] # first column.
modo2= modos[:,1] # second column.
modo3= modos[:,2] # third column.

modalParticipationFactorsX= nod3.getModalParticipationFactorsForDOFs([0])
factoresDistribucion= nod3.getDistributionFactors
A1= matrix_utils.vectorToNumpyArray(nod3.getMaxModalDisplacementForDOFs(1,aceleraciones[0],[0]))
maxDispMod1= A1[0:3] #getBox(A1,1,1,3,1)
A2= matrix_utils.vectorToNumpyArray(nod3.getMaxModalDisplacementForDOFs(2,aceleraciones[1],[0]))
maxDispMod2= A2[0:3] #getBox(A2,1,1,3,1)
A3= matrix_utils.vectorToNumpyArray(nod3.getMaxModalDisplacementForDOFs(3,aceleraciones[2],[0]))
maxDispMod3= A3[0:3] #getBox(A3,1,1,3,1)


# Theorethical values taken from the exampleE26.8 of the book: Clough, R. W., and Penzien, J., Dynamics of Structures
angularFrequenciesTeor= xc.Vector([4.59,4.83,14.56])
theorPeriods= xc.Vector([2*math.pi/4.59,2*math.pi/4.83,2*math.pi/14.56])
ratio0= (angularFrequencies-angularFrequenciesTeor).Norm()
theorFrequencies= [4.59/2/math.pi,4.83/2/math.pi,14.56/2/math.pi]
ratio1= (periods-theorPeriods).Norm()
modosTeor= numpy.matrix([[-0.731,0.271,-1],
                         [0.232,1,0.242],
                         [-1,0.036,0.787]])
modo1Teor= modosTeor[:,0].reshape(3) # first column.
modo2Teor= modosTeor[:,1].reshape(3) # second column.
modo3Teor= modosTeor[:,2].reshape(3) # third column.
err= min(numpy.linalg.norm(modo1-modo1Teor), numpy.linalg.norm(modo1+modo1Teor))
err+= min(numpy.linalg.norm(modo2-modo2Teor), numpy.linalg.norm(modo2+modo2Teor))
err+= min(numpy.linalg.norm(modo3-modo3Teor), numpy.linalg.norm(modo3+modo3Teor))
ratio2= err
# This CQC coefficients are taken from the Solvia manual
crossCQCCoefficientsTeor= xc.Matrix([[1,0.79280,0.005705],[0.79280,1,0.006383],[0.005705,0.006383,1]])
ratio3= (crossCQCCoefficients-crossCQCCoefficientsTeor).Norm()

modalParticipationFactorsXTeor= xc.Vector([-.731/1.588,.271/1.075,-1/1.678])
err= 0.0
for j in range(0,3):
    v1= abs(modalParticipationFactorsX[j]-modalParticipationFactorsXTeor[j])
    v2= abs(modalParticipationFactorsX[j]+modalParticipationFactorsXTeor[j])
    err+= min(v1,v2)**2
#ratio4= (modalParticipationFactorsX-modalParticipationFactorsXTeor).Norm()
ratio4= math.sqrt(err)

''' 
maxDispMod1Teor= modo1Teor*modalParticipationFactorsXTeor[0]*aceleraciones[0]/sqr(angularFrequenciesTeor[0])
maxDispMod2Teor= modo2Teor*modalParticipationFactorsXTeor[1]*aceleraciones[1]/sqr(angularFrequenciesTeor[1])
maxDispMod3Teor= modo3Teor*modalParticipationFactorsXTeor[2]*aceleraciones[2]/sqr(angularFrequenciesTeor[2])
maxDispMod1Teor= [0.119,-0.038,0.162]*0.3048
maxDispMod2Teor= [0.039,0.143,0.005]*0.3048
maxDispMod3Teor= [0.064,-0.016,-0.055]*0.3048
   '''
# This displacements coefficients are taken from the Solvia manual.
maxDispMod1Teor= numpy.matrix([[36.202e-3],[-11.549e-3],[49.548e-3]])
maxDispMod2Teor= numpy.matrix([[7.123e-3],[26.38e-3],[0.945e-3]])
maxDispMod3Teor= numpy.matrix([[19.625e-3],[-4.746e-3],[-15.445e-3]])
ratioM1= numpy.linalg.norm(maxDispMod1-maxDispMod1Teor)
ratioM2= numpy.linalg.norm(maxDispMod2-maxDispMod2Teor)
ratioM3= numpy.linalg.norm(maxDispMod3-maxDispMod3Teor)

maxDispMod= maxDispMod1,maxDispMod2,maxDispMod3
maxDispCQC= [0.0,0.0,0.0]
maxDispCQCTeor= xc.Vector([46.53e-3,19.18e-3,52.53e-3])
nMod= 3
i= 0
j= 0
k= 0

for i in range(0,nMod):
    for j in range(0,nMod):
        maxDispModI= maxDispMod[i]
        maxDispModJ= maxDispMod[j]
        cqcCoeff= crossCQCCoefficients(i,j)
        for k in range(0,3):
            maxDispCQC[k]= maxDispCQC[k]+cqcCoeff*maxDispModI[k]*maxDispModJ[k]


for k in range(0,3):
    maxDispCQC[k]= math.sqrt(maxDispCQC[k])

maxDispCQC= xc.Vector(maxDispCQC)
ratioDispCQC= (maxDispCQC-maxDispCQCTeor).Norm()


''' 
print("angular frequencies: ",angularFrequencies)
print("angular frequencies (teor.): ",angularFrequenciesTeor)
print("ratio0= ",ratio0)
print("periods: ",periods)
print("theorPeriods: ",theorPeriods)
print("ratio1= ",ratio1)
print("modos: ",modos)
print("modosTeor: ",modosTeor)
print("ratio2= ",ratio2)
print("coefficients CQC: ",crossCQCCoefficients)
print("coefficients CQC example: ",crossCQCCoefficientsTeor)
print("ratio3= ",ratio3)
print("modalParticipationFactorsX: ",modalParticipationFactorsX)
print("modalParticipationFactorsXTeor: ",modalParticipationFactorsXTeor)
print("ratio4= ",ratio4)
print("********** maximum displacement mode 1: ",maxDispMod1*1000)
print("********** maximum displacement mode 1 (example): ",maxDispMod1Teor*1000)
print("ratioM1= ",ratioM1)
print("********** maximum displacement mode 2: ",maxDispMod2*1000)
print("********** maximum displacement mode 2 (example): ",maxDispMod2Teor*1000)
print("ratioM2= ",ratioM2)
print("********** maximum displacement mode 3: ",maxDispMod3*1000)
print("********** maximum displacement mode 3 (example): ",maxDispMod3Teor*1000)
print("ratioM3= ",ratioM3)
#print("maxDispCQC= ",maxDispCQC*1e3)
#print("maxDispCQCTeor= ",maxDispCQCTeor*1e3)
print("ratioDispCQC= ",ratioDispCQC)
   '''

import os
from misc_utils import log_messages as lmsg
fname= os.path.basename(__file__)
if( (ratio1<1e-3) & (ratio2<1e-2) & (ratio3<1e-5) & (ratio4<1e-3) & (ratioM1<1e-6) & (ratioM2<1e-6) & (ratioM3<1e-6) & (ratioDispCQC<1e-5) ): 
    print('test '+fname+': ok.')
else:
    lmsg.error(fname+' ERROR.')

