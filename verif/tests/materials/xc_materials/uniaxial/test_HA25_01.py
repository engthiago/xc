# -*- coding: utf-8 -*-
from __future__ import print_function

#*    Nov. 2015   Ana Ortega    *

# Home made test

import math
import xc_base
import geom
import xc
from materials import concrete_base
from materials.ehe import EHE_materials

__author__= "Luis C. Pérez Tato (LCPT) and Ana Ortega (AOO)"
__copyright__= "Copyright 2015, LCPT and AOO"
__license__= "GPL"
__version__= "3.0"
__email__= "l.pereztato@gmail.com"

# Model definition
feProblem= xc.FEProblem()
feProblem.logFileName= "/tmp/erase.log" # Para no imprimir mensajes de advertencia
mdlr= feProblem.getPreprocessor
concr= EHE_materials.HA25
concr.alfacc=0.85 #f_maxd= 0.85*fcd
errMax= concrete_base.concreteDesignDiagramTest(mdlr,concr)


#print("errMax= ",(errMax))

import os
from misc_utils import log_messages as lmsg
fname= os.path.basename(__file__)
if errMax<1e-8:
    print('test '+fname+': ok.')
else:
    lmsg.error(fname+' ERROR.')
