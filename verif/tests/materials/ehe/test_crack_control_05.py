# -*- coding: utf-8 -*-
from __future__ import print_function
''' Reinforced concrete section. Crack control verification test.'''

import geom
import xc

from misc import scc3d_testing_bench
from solution import predefined_solutions # Solution procedure


from materials.ehe import EHE_materials
from materials.ehe import EHE_limit_state_checking
from model import predefined_spaces
import math

__author__= "Luis C. Pérez Tato (LCPT) and Ana Ortega (AOO)"
__copyright__= "Copyright 2015, LCPT and AOO"
__license__= "GPL"
__version__= "3.0"
__email__= "l.pereztato@gmail.com"

width= 1.0 # Section width expressed in meters.
depth= 1.0 # Section depth expressed in meters.
cover= 0.03 # Concrete cover expressed in meters.
areaFi10=0.79e-4
areaFi25=4.91e-4

NDato= 0 # Axial force for crack control checking.
MyDato= -67*9810*width # Bending moment force for crack control checking.
MzDato= 0 # Bending moment force for crack control checking.

rebarsSpacing= 0.15
numOfRebars= int(math.floor(width/rebarsSpacing))
rebarsOffset= ((width-(numOfRebars-1)*rebarsSpacing)/2.0)

''' 
print("rebarsSpacing= ",rebarsSpacing)
print("numOfRebars= ",numOfRebars)
print("rebarsOffset= ",rebarsOffset)
   '''

feProblem= xc.FEProblem()
preprocessor=  feProblem.getPreprocessor
# Materials definition
concrMatTag25= EHE_materials.HA25.defDiagK(preprocessor)
tagB400S= EHE_materials.B400S.defDiagK(preprocessor)

geomSecHA= preprocessor.getMaterialHandler.newSectionGeometry("geomSecHA")
regions= geomSecHA.getRegions
concrete= regions.newQuadRegion(EHE_materials.HA25.nmbDiagK)
concrete.nDivIJ= 10
concrete.nDivJK= 10
concrete.pMin= geom.Pos2d(-width/2.0,-depth/2.0)
concrete.pMax= geom.Pos2d(width/2.0,depth/2.0)
reinforcement= geomSecHA.getReinfLayers
reinforcementInf= reinforcement.newStraightReinfLayer(EHE_materials.B400S.nmbDiagK)
reinforcementInf.numReinfBars= numOfRebars
reinforcementInf.barArea= areaFi25
reinforcementInf.p1= geom.Pos2d(rebarsOffset-width/2.0,cover-depth/2.0)
reinforcementInf.p2= geom.Pos2d(width/2.0-rebarsOffset,cover-depth/2.0)
reinforcementSup= reinforcement.newStraightReinfLayer(EHE_materials.B400S.nmbDiagK)
reinforcementSup.numReinfBars= numOfRebars
reinforcementSup.barArea= areaFi10
reinforcementSup.p1= geom.Pos2d(rebarsOffset-width/2.0,depth/2.0-cover)
reinforcementSup.p2= geom.Pos2d(width/2.0-rebarsOffset,depth/2.0-cover)

materialHandler= preprocessor.getMaterialHandler
secHA= materialHandler.newMaterial("fiber_section_3d","secHA")
fiberSectionRepr= secHA.getFiberSectionRepr()
fiberSectionRepr.setGeomNamed(geomSecHA.name)
secHA.setupFibers()

zlElement, nodA, nodB= scc3d_testing_bench.sectionModel(preprocessor, "secHA")

# Constraints
modelSpace= predefined_spaces.getStructuralMechanics3DSpace(preprocessor)
modelSpace.fixNode000_000(nodA.tag)
modelSpace.fixNodeF00_0FF(nodB.tag)

# Loads definition
lp0= modelSpace.newLoadPattern(name= '0')
lp0.newNodalLoad(nodB.tag,xc.Vector([NDato,0,0,0,MyDato,MzDato]))

# We add the load case to domain.
modelSpace.addLoadCaseToDomain(lp0.name)



# Solution procedure
analysis= predefined_solutions.plain_newton_raphson(feProblem)
analOk= analysis.analyze(10)

secHAParamsFis= EHE_limit_state_checking.CrackControl('SLS_crack')



scc= zlElement.getSection()
secHAParamsFis.computeWk(scc,EHE_materials.HA25.matTagK,EHE_materials.B400S.matTagK,EHE_materials.HA25.fctm())


ratio1= ((secHAParamsFis.tensionedRebars.spacing-0.15)/0.15)
WkTeor= 0.26e-3
ratio2= ((secHAParamsFis.Wk-WkTeor)/WkTeor)


'''
secHAParamsFis.printParams()
print("ratio1= ",ratio1)
print("wk= ", secHAParamsFis.Wk)
print("ratio2= ",ratio2)
'''


import os
from misc_utils import log_messages as lmsg
fname= os.path.basename(__file__)
if (abs(ratio1)<0.05) & (abs(ratio2)<0.05):
    print('test '+fname+': ok.')
else:
    lmsg.error(fname+' ERROR.')
