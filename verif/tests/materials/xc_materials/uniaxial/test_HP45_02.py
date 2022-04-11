# -*- coding: utf-8 -*-
''' Home made test.'''
from __future__ import print_function

__author__= "Luis C. Pérez Tato (LCPT) and Ana Ortega (AOO)"
__copyright__= "Copyright 2015, LCPT and AOO"
__license__= "GPL"
__version__= "3.0"
__email__= "l.pereztato@gmail.com"

import math
import geom
import xc
from materials import typical_materials
from materials import concrete_base
from materials.ehe import EHE_materials

# Model definition
feProblem= xc.FEProblem()
feProblem.logFileName= "/tmp/erase.log" # Ignore warning messages
preprocessor=  feProblem.getPreprocessor
HP45= EHE_materials.HA45
errMax= concrete_base.concreteDesignTangentTest(preprocessor, HP45)

#print("errMax= ",errMax)
import os
from misc_utils import log_messages as lmsg
fname= os.path.basename(__file__)
if(errMax<1e-15):
    print('test '+fname+': ok.')
else:
    lmsg.error(fname+' ERROR.')

