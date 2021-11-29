# -*- coding: utf-8 -*-
''' Response spectrum modal analysis test
taken from the publication from Andrés Sáez Pérez: «Estructuras III»
 E.T.S. de Arquitectura de Sevilla (España). Eigenvalue problem is solved
by means of the Arpack library. '''
from __future__ import print_function
from __future__ import division
import xc_base
import geom
import xc

from model import predefined_spaces
from solution import predefined_solutions
from materials import typical_materials
import math

__author__= "Luis C. Pérez Tato (LCPT)"
__copyright__= "Copyright 2014, LCPT"
__license__= "GPL"
__version__= "3.0"
__email__= "l.pereztato@gmail.com"


storeyMass= 134.4e3
nodeMassMatrix= xc.Matrix([[storeyMass,0,0],[0,storeyMass,0],[0,0,0]])
Ehorm= 200000*1e5 # Concrete elastic modulus.

Bbaja= 0.45 # Columns size.
Ibaja= 1/12.0*Bbaja**4 # Cross section moment of inertia.
Hbaja= 4 # First floor height.
B1a= 0.40 # Columns size.
I1a= 1/12.0*B1a**4 # Cross section moment of inertia.
H= 3 # Height of the rest of the floors.
B3a= 0.35 # Columns size.
I3a= 1/12.0*B3a**4 # Cross section moment of inertia.


kPlBaja= 20*12*Ehorm*Ibaja/(Hbaja**3)
kPl1a= 20*12*Ehorm*I1a/(H**3)
kPl2a= kPl1a
kPl3a= 20*12*Ehorm*I3a/(H**3)
kPl4a= kPl3a

feProblem= xc.FEProblem()
preprocessor=  feProblem.getPreprocessor
nodes= preprocessor.getNodeHandler

# Problem type
modelSpace= predefined_spaces.StructuralMechanics2D(nodes)
nodes.defaultTag= 0
nod= nodes.newNodeXY(0,0)
nod.mass= nodeMassMatrix
nod.setProp("gdlsCoartados",xc.ID([0,1,2]))
nod= nodes.newNodeXY(0,4)
nod.mass= nodeMassMatrix
nod.setProp("gdlsCoartados",xc.ID([1,2]))
nod= nodes.newNodeXY(0,4+3)
nod.mass= nodeMassMatrix
nod.setProp("gdlsCoartados",xc.ID([1,2]))
nod= nodes.newNodeXY(0,4+3+3)
nod.mass= nodeMassMatrix
nod.setProp("gdlsCoartados",xc.ID([1,2]))
nod= nodes.newNodeXY(0,4+3+3+3)
nod.mass= nodeMassMatrix
nod.setProp("gdlsCoartados",xc.ID([1,2]))
nod= nodes.newNodeXY(0,4+3+3+3+3)
nod.mass= nodeMassMatrix
nod.setProp("gdlsCoartados",xc.ID([1,2]))
setTotal= preprocessor.getSets.getSet("total")
nodes= setTotal.getNodes
for n in nodes:
    n.fix(n.getProp("gdlsCoartados"),xc.Vector([0,0,0]))

# Materials definition
materialHandler= preprocessor.getMaterialHandler
#scc= typical_materials.defElasticSection2d(preprocessor, "scc",A,E,I)

sccPlBaja= typical_materials.defElasticSection2d(preprocessor, "sccPlBaja",20*Bbaja*Bbaja,Ehorm,20*Ibaja)
sccPl1a= typical_materials.defElasticSection2d(preprocessor, "sccPl1a",20*B1a*B1a,Ehorm,20*I1a) 
sccPl2a= typical_materials.defElasticSection2d(preprocessor, "sccPl2a",20*B1a*B1a,Ehorm,20*I1a) 
sccPl3a= typical_materials.defElasticSection2d(preprocessor, "sccPl3a",20*B3a*B3a,Ehorm,20*I3a) 
sccPl4a= typical_materials.defElasticSection2d(preprocessor, "sccPl4a",20*B3a*B3a,Ehorm,20*I3a)


# Geometric transformation(s)
lin= modelSpace.newLinearCrdTransf("lin")

# Elements definition
elements= preprocessor.getElementHandler
elements.defaultTransformation= lin.name
elements.defaultMaterial= sccPlBaja.name
elements.defaultTag= 1 #Tag for next element.
beam2d= elements.newElement("ElasticBeam2d",xc.ID([0,1]))
beam2d.h= Bbaja
elements.defaultMaterial= sccPl1a.name 
beam2d= elements.newElement("ElasticBeam2d",xc.ID([1,2]))
beam2d.h= B1a
elements.defaultMaterial= sccPl2a.name 
beam2d= elements.newElement("ElasticBeam2d",xc.ID([2,3]))
beam2d.h= B1a
elements.defaultMaterial= sccPl3a.name 
beam2d= elements.newElement("ElasticBeam2d",xc.ID([3,4]))
beam2d.h= B3a
elements.defaultMaterial= sccPl4a.name 
beam2d= elements.newElement("ElasticBeam2d",xc.ID([4,5]))
beam2d.h= B3a

targetTotalMass= 5*storeyMass

# Solution procedure
analysis= predefined_solutions.frequency_analysis(feProblem, systemPrefix= 'band_arpackpp', shift= 0.0)
analOk= analysis.analyze(3)

periods= analysis.getPeriods()
modes= analysis.getNormalizedEigenvectors()
modalParticipationFactors= analysis.getModalParticipationFactors()
effectiveModalMasses= analysis.getEffectiveModalMasses()
totalMass= analysis.getTotalMass()
distributionFactors= analysis.getDistributionFactors()



targetPeriods= xc.Vector([0.468,0.177,0.105])
ratio1= (periods-targetPeriods).Norm()
# Modes are obtained with different signs.
exempleModes= xc.Matrix([[0.323,-0.764,-0.946],
                         [0.521,-0.941,-0.378],
                         [0.685,-0.700,0.672],
                         [0.891,0.241,1.000],
                         [1.000,1.000,-0.849]])
diff_modes= (modes-exempleModes)
ratio2= (diff_modes).rowNorm()


ratio3= abs(totalMass-targetTotalMass)/targetTotalMass
exampleDistribFactors= xc.Matrix([[0.419,0.295,0.148],
                                   [0.676,0.363,0.059],
                                   [0.889,0.27,-0.105],
                                   [1.157,-0.093,-0.156],
                                   [1.298,-0.386,0.133]])
diff_fdib= distributionFactors-exampleDistribFactors
ratio4= (diff_fdib).rowNorm()

'''
print("kPlBaja= ",kPlBaja)
print("kPl1a= ",kPl1a)
print("kPl3a= ",kPl3a)
print("periods: ",periods)
print("ratio1= ",ratio1)
# Los modos se obtienen con distinto signo
print("modes: ",modes)
print("subtract_modes: ",diff_modes)
print("ratio2= ",ratio2)

print("modalParticipationFactors: ",modalParticipationFactors)
print("effectiveModalMasses: ",effectiveModalMasses)
print("totalMass: ",totalMass)
print("targetTotalMass: ",targetTotalMass)
print("ratio3= ",ratio3)
print("distributionFactors: ",distributionFactors)
print("ratio4= ",ratio4)
'''

import os
from misc_utils import log_messages as lmsg
fname= os.path.basename(__file__)
if((ratio1<1e-3) & (ratio3<1e-12) & (ratio4<5e-2)):
    print('test '+fname+': ok.')
else:
    lmsg.error(fname+' ERROR.')
