# -*- coding: utf-8 -*-
''' Iteraction diagram computation. Home made test. Reads the interaction diagram obtained in test_interaction_diagram01. '''
from __future__ import print_function
from __future__ import division

import geom
import xc

from materials.ehe import EHE_materials

__author__= "Luis C. Pérez Tato (LCPT)"
__copyright__= "Copyright 2014, LCPT"
__license__= "GPL"
__version__= "3.0"
__email__= "l.pereztato@gmail.com"

feProblem= xc.FEProblem()
preprocessor=  feProblem.getPreprocessor
materialHandler= preprocessor.getMaterialHandler
diagInteg= materialHandler.newInteractionDiagram("diagInteg")
diagInteg.readFrom("/tmp/interaction_diagram_test_01.dat")

ratio1= diagInteg.getCapacityFactor(geom.Pos3d(352877,0,0))-1
ratio2= diagInteg.getCapacityFactor(geom.Pos3d(352877/2.0,0,0))-0.5
ratio3= diagInteg.getCapacityFactor(geom.Pos3d(-574457,41505.4,2.00089e-11))-1.0
ratio4= diagInteg.getCapacityFactor(geom.Pos3d(-978599,-10679.4,62804.3))-1.0


import os
os.system("rm -f /tmp/interaction_diagram_test_01.dat")
''' 
print("ratio1= ",(ratio1))
print("ratio2= ",(ratio2))
print("ratio3= ",(ratio3))
print("ratio4= ",(ratio4))
 '''

''' Como el método con el que se graba el interaction diagram no es exacto
   (the coordinates of the vertices are «computed») se pierde precisión por lo
   que ha habido que aumentar la toleracia en el ratio3 respecto al test 01
   (test_interaction_diagram01) '''
from misc_utils import log_messages as lmsg
fname= os.path.basename(__file__)
if((abs(ratio1)<1e-5) & (abs(ratio2)<1e-5) & (abs(ratio3)<2e-2) & (abs(ratio4)<1e-5)):
    print('test '+fname+': ok.')
else:
    lmsg.error(fname+' ERROR.')
