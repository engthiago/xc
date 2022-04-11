# -*- coding: utf-8 -*-
from __future__ import print_function
'''Silly test about sets.'''
import geom
import xc

__author__= "Luis C. Pérez Tato (LCPT)"
__copyright__= "Copyright 2014, LCPT"
__license__= "GPL"
__version__= "3.0"
__email__= "l.pereztato@gmail.com"

test= xc.FEProblem()
testSetA= test.getPreprocessor.getSets.exists('pp')
testSetB= test.getPreprocessor.getSets.exists('total')

'''
print(testSetA)
print(testSetB)
'''

import os
from misc_utils import log_messages as lmsg
fname= os.path.basename(__file__)

if(not(testSetA) and testSetB):
    print('test '+fname+': ok.')
else:
    lmsg.error(fname+' ERROR.')


