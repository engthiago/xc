# -*- coding: utf-8 -*-
''' Home made test.Design stress-strain diagram of the steel B500B.'''
from __future__ import print_function

__author__= "Luis C. Pérez Tato (LCPT) and Ana Ortega (AOO)"
__copyright__= "Copyright 2015, LCPT and AOO"
__license__= "GPL"
__version__= "3.0"
__email__= "l.pereztato@gmail.com"

import geom
import xc

from materials.sia262 import SIA262_materials
from materials import concrete_base

# Model definition
feProblem= xc.FEProblem()
feProblem.logFileName= "/tmp/erase.log" # Para no imprimir mensajes de advertencia
preprocessor=  feProblem.getPreprocessor
# Define materials
errMax= concrete_base.testReinfSteelDesignDiagram(preprocessor, SIA262_materials.B500B)


# print("errMax= ",(errMax))
import os
from misc_utils import log_messages as lmsg
fname= os.path.basename(__file__)
if errMax<1e-10:
    print('test '+fname+': ok.')
else:
    lmsg.error(fname+' ERROR.')
