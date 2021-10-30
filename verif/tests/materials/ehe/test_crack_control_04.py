# -*- coding: utf-8 -*-
from __future__ import print_function
''' Crack control checking of a reinforced concrete section. according 
    to EHE-08. The results are compared with those of the problem 9 
    of the book "El hormigón armado en problemas según la EHE." 
    isbn: 9788438001592 
    Alejandro Castillo Linares and Angel Vallecillo Capilla.
'''

import xc_base
import geom
import xc

from misc import scc3d_testing_bench
from solution import predefined_solutions # Solution procedure


from materials.ehe import EHE_materials
from materials.ehe import EHE_limit_state_checking
from model import predefined_spaces

__author__= "Luis C. Pérez Tato (LCPT) and Ana Ortega (A_OO)"
__copyright__= "Copyright 2015, LCPT and AO_O"
__license__= "GPL"
__version__= "3.0"
__email__= "l.pereztato@gmail.com ana.ortega.ort@gmal.com"

width= 1.70 # Section width expressed in meters.
depth= 1.10 # Section depth expressed in meters.
cover= 0.05 # Concrete cover expressed in meters.
areaFi20= 3.14e-4
areaFi25= 4.91e-4
areaFi32= 8.04e-4

NDato= 0 # Axial force for crack control checking.
MyDato= -195.3*9810 # Bending moment force for crack control checking.
MzDato= 0 # Bending moment force for crack control checking.

numOfRebars= 16
rebarsSpacing= ((width-2*cover)/(numOfRebars-1))
rebarsOffset= ((width-(numOfRebars-1)*rebarsSpacing)/2)

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
reinforcementA= reinforcement.newStraightReinfLayer(EHE_materials.B400S.nmbDiagK)
reinforcementA.numReinfBars= int(numOfRebars/2)
reinforcementA.barArea= areaFi20
reinforcementA.p1= geom.Pos2d(cover-width/2.0,cover-depth/2.0)
reinforcementA.p2= geom.Pos2d(width/2.0-cover-rebarsSpacing,cover-depth/2.0)
reinforcementB= reinforcement.newStraightReinfLayer(EHE_materials.B400S.nmbDiagK)
reinforcementB.numReinfBars= int(numOfRebars/2)
reinforcementB.barArea= areaFi32
reinforcementB.p1= geom.Pos2d(cover+rebarsSpacing-width/2.0,cover-depth/2.0)
reinforcementB.p2= geom.Pos2d(width/2.0-cover,cover-depth/2.0)
reinforcementC= reinforcement.newStraightReinfLayer(EHE_materials.B400S.nmbDiagK)
reinforcementC.numReinfBars= 13
reinforcementC.barArea= areaFi25
reinforcementC.p1= geom.Pos2d(cover-width/2.0,depth/2.0-cover) # top layer.
reinforcementC.p2= geom.Pos2d(width/2.0-cover,depth/2.0-cover)

materialHandler= preprocessor.getMaterialHandler
secHA= materialHandler.newMaterial("fiber_section_3d","secHA")
fiberSectionRepr= secHA.getFiberSectionRepr()
fiberSectionRepr.setGeomNamed(geomSecHA.name)
secHA.setupFibers()

scc3d_testing_bench.sectionModel(preprocessor, "secHA")

# Constraints
modelSpace= predefined_spaces.getStructuralMechanics3DSpace(preprocessor)
modelSpace.fixNode000_000(1)
modelSpace.fixNodeF00_0FF(2)

# Loads definition
lp0= modelSpace.newLoadPattern(name= '0')
lp0.newNodalLoad(2,xc.Vector([NDato,0,0,0,MyDato,MzDato]))

# We add the load case to domain.
modelSpace.addLoadCaseToDomain(lp0.name)



# Solution procedure
analysis= predefined_solutions.plain_newton_raphson(feProblem)
analOk= analysis.analyze(10)


secHAParamsFis= EHE_limit_state_checking.CrackControl('SLS_crack')



elements= preprocessor.getElementHandler
ele1= elements.getElement(1)
scc= ele1.getSection()
secHAParamsFis.computeWk(scc,EHE_materials.HA25.matTagK,EHE_materials.B400S.matTagK,EHE_materials.HA25.fctm())

ratio1= ((secHAParamsFis.tensionedRebars.spacing-0.105)/0.105)
ratio2= ((secHAParamsFis.Wk-0.3e-3)/0.3e-3)


'''
secHAParamsFis.printParams()
print("ratio1= ",ratio1)
print("ratio2= ",ratio2)
'''


import os
from misc_utils import log_messages as lmsg
fname= os.path.basename(__file__)
if (abs(ratio1)<0.05) & (abs(ratio2)<0.1):
    print('test '+fname+': ok.')
else:
    lmsg.error(fname+' ERROR.')
