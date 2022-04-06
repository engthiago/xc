# -*- coding: utf-8 -*-
from __future__ import print_function
''' Reinforced concrete section verification test.
   results are compared with those of the prontuario.
   informático del hormigón estructural (Cátedra de hormigón de la ETSICCP-IECA
   UPM). '''

# feProblem.logFileName= "/tmp/erase.log"  # Ignore warning messages

import xc_base
import geom
import xc
from misc import scc3d_testing_bench
from solution import predefined_solutions


from materials.ehe import EHE_materials
from materials.sections.fiber_section import fiber_sets
from model import predefined_spaces

__author__= "Luis C. Pérez Tato (LCPT)"
__copyright__= "Copyright 2014, LCPT"
__license__= "GPL"
__version__= "3.0"
__email__= "l.pereztato@gmail.com"

MzDato= 55.949e3 # The "prontuario informático" reaches only ~54.4 kN m because it uses a simplified diagram for the steel.
NDato= 0.0 
feProblem= xc.FEProblem()
preprocessor=  feProblem.getPreprocessor
# Materials definition
tag= EHE_materials.B500S.defDiagD(preprocessor)
concr= EHE_materials.HA25
concr.alfacc=0.85    # f_maxd= 0.85*fcd concrete long term compressive strength factor (normally alfacc=1)
tag= concr.defDiagD(preprocessor)
import os
pth= os.path.dirname(__file__)
if(not pth):
  pth= "."
# print("pth= ", pth)
exec(open(pth+"/concrete_section_01.py").read())
materialHandler= preprocessor.getMaterialHandler
secHA= materialHandler.newMaterial("fiber_section_3d","secHA")
fiberSectionRepr= secHA.getFiberSectionRepr()
fiberSectionRepr.setGeomNamed(concreteSectionGeom01.name)
secHA.setupFibers()

scc3d_testing_bench.sectionModel(preprocessor, "secHA")

# Constraints
modelSpace= predefined_spaces.getStructuralMechanics3DSpace(preprocessor)
modelSpace.fixNode000_000(1)
modelSpace.fixNodeF00_00F(2)

# Loads definition
lp0= modelSpace.newLoadPattern(name= '0')
lp0.newNodalLoad(2,xc.Vector([NDato,0,0,0,0,MzDato]))

# We add the load case to domain.
modelSpace.addLoadCaseToDomain(lp0.name)


# Solution procedure
analysis= predefined_solutions.plain_newton_raphson(feProblem)
analOk= analysis.analyze(10)

nodes= preprocessor.getNodeHandler
nodes.calculateNodalReactions(True,1e-7)

RN= nodes.getNode(1).getReaction[0] 
RM= nodes.getNode(1).getReaction[5] 
RN2= nodes.getNode(2).getReaction[0] 

elements= preprocessor.getElementHandler
ele1= elements.getElement(1)
scc= ele1.getSection()
esfN= scc.getStressResultantComponent("N")
esfMy= scc.getStressResultantComponent("My")
esfMz= scc.getStressResultantComponent("Mz")
defMz= scc.getSectionDeformationByName("defMz")
defN= scc.getSectionDeformationByName("defN")
concrFibers= fiber_sets.FiberSet(scc,'concrete',EHE_materials.HA25.matTagD)
fibraCEpsMin= concrFibers.getFiberWithMinStrain()
epsCMin= fibraCEpsMin.getMaterial().getStrain() # Minimum concrete strain.
fibraCEpsMax= concrFibers.getFiberWithMaxStrain()
epsCMax= fibraCEpsMax.getMaterial().getStrain() # Maximum concrete strain.
reinfFibers= fiber_sets.FiberSet(scc,"reinforcement",EHE_materials.B500S.matTagD)
fibraSEpsMax= reinfFibers.getFiberWithMaxStrain()
epsSMax= fibraSEpsMax.getMaterial().getStrain() # Maximum steel strain

from materials.sections import section_properties
from materials.ehe import EHE_limit_state_checking
solicitationType= section_properties.solicitationType(epsCMin,epsSMax)
solicitationTypeString= section_properties.solicitationTypeString(solicitationType)
cumpleFT= EHE_materials.ReinforcedConcreteLimitStrainsEHE08.bendingOK(epsCMin,epsSMax)
aprovSecc= EHE_materials.ReinforcedConcreteLimitStrainsEHE08.getBendingEfficiency(epsCMin,epsSMax)

ratio1= (RM+MzDato)/MzDato
ratio2= (esfMz-MzDato)/MzDato
ratio3= (esfN-NDato)
ratio4= cumpleFT
ratio5= (RN+NDato)
ratio6= (epsSMax-10e-3)/10e-3

''' 
print("ratio1= ",(ratio1))
print("ratio2= ",(ratio2))
print("ratio3= ",(ratio3))
print("ratio4= ",(ratio4))
print("ratio5= ",(ratio5))
print("ratio6= ",(ratio6))

print("Minimum concrete strain: ",(epsCMin))
print("Maximum concrete strain: ",(epsCMax))
print("Maximum rebar strain: ",(epsSMax))
print("Solicitation type: ",solicitationTypeString," (",(solicitationType),") \n")
print("Cumple a ",solicitationTypeString,": ",(cumpleFT))
print("Aprovechamiento a ",solicitationTypeString,": ",(aprovSecc))
print("RN= ",(RN/1e3)," kN\n")
print("RN2= ",(RN2/1e3)," kN\n")
print("N= ",(esfN/1e3)," kN\n")
print("esfMy= ",(esfMy/1e3),"kN m\n")
print("esfMz= ",(esfMz/1e3),"kN m\n")
print("defMz= ",(defMz))
print("defN= ",(defN))
print("analOk= ",(analOk))
 '''


import os
from misc_utils import log_messages as lmsg
fname= os.path.basename(__file__)
if (abs(ratio1)<1e-10) & (abs(ratio2)<1e-10) & (abs(ratio3)<5e-10) & ratio4 & (abs(ratio5)<5e-10) & (abs(ratio6)<0.001) & (abs(RN2)<5e-10) & (abs(esfMy)<1e-10) & (solicitationType == 2)  & (analOk == 0.0) :
    print('test '+fname+': ok.')
else:
    lmsg.error(fname+' ERROR.')
