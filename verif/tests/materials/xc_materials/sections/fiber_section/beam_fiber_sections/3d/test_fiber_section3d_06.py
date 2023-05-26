# -*- coding: utf-8 -*-
''' Reinforced concrete section verification test.
   results are compared with those of the prontuario.
   informático del hormigón estructural (Cátedra de hormigón de la ETSICCP-IECA
   UPM). '''

from __future__ import print_function

__author__= "Luis C. Pérez Tato (LCPT) and Ana Ortega (AOO)"
__copyright__= "Copyright 2015, LCPT and AOO"
__license__= "GPL"
__version__= "3.0"
__email__= "l.pereztato@gmail.com"


import os
import sys
import xc

from materials.ehe import EHE_materials

CurvZ1= 11.474e-3 # Curvature test 1.
x1= 0.0997 # Neutral axis depth 1.

CurvZ2= 34.787e-3 # Curvature test 2.
x2= 0.0649 # Neutral axis depth 2.
feProblem= xc.FEProblem()
feProblem.logFileName= "/tmp/erase.log" # Ignore warning messages
preprocessor=  feProblem.getPreprocessor

concr= EHE_materials.HA25
concr.alfacc=0.85    #f_maxd= 0.85*fcd concrete long term compressive strength factor (normally alfacc=1)

tag= concr.defDiagD(preprocessor)
tag= EHE_materials.B500S.defDiagD(preprocessor)
# Define materials
# Create RC section model.
pth= os.path.dirname(__file__)
if(not pth):
  pth= "."
sys.path.append(pth+"/../../../../../../aux/")
import concrete_section_01
defSec= concrete_section_01.defSec
concreteSectionGeom01= concrete_section_01.gmSecHA01(preprocessor, nmbGeomSecc="concreteSectionGeom01",defSec= defSec,concrDiagName= EHE_materials.HA25.nmbDiagD,reinfSteelDiagramName= EHE_materials.B500S.nmbDiagD)

secHA= preprocessor.getMaterialHandler.newMaterial("fiber_section_3d","secHA")
fiberSectionRepr= secHA.getFiberSectionRepr()
fiberSectionRepr.setGeomNamed(concreteSectionGeom01.name)
secHA.setupFibers()

epsG1= CurvZ1*(defSec['depth']/2-x1)
secHA.revertToStart()
secHA.setTrialSectionDeformation(xc.Vector([epsG1,CurvZ1,0]))
secHA.commitState()
N1= secHA.getStressResultantComponent("N")
Mz1= secHA.getStressResultantComponent("Mz")
Mz1Dato= 54.4e3
ratio1= (Mz1-Mz1Dato)/Mz1Dato

epsG2= CurvZ2*(defSec['depth']/2-x2)
secHA.revertToStart()
secHA.setTrialSectionDeformation(xc.Vector([epsG2,CurvZ2,0]))
secHA.commitState()
N2= secHA.getStressResultantComponent("N")
Mz2= secHA.getStressResultantComponent("Mz")
Mz2Dato= 55.7e3
ratio2= (Mz2-Mz2Dato)/Mz2Dato




''' 
print("computed Axil; N1= ",(N1/1e3)," kN \n")
print("computed moment about z axis; Mz1= ",(Mz1/1e3)," kN m\n")
print("moment about z axis dato; Mz1Dato= ",(Mz1Dato/1e3)," kN \n")
print("ratio1= ",(ratio1))

print("computed Axil; N2= ",(N2/1e3)," kN \n")
print("computed moment about z axis; Mz2= ",(Mz2/1e3)," kN m\n")
print("moment about z axis dato; Mz2Dato= ",(Mz2Dato/1e3)," kN \n")
print("ratio2= ",(ratio2))
 '''

from misc_utils import log_messages as lmsg
fname= os.path.basename(__file__)
if (abs(ratio1)<1e-4) & (abs(ratio2)<1e-4) :
    print('test '+fname+': ok.')
else:
    lmsg.error(fname+' ERROR.')

