# -*- coding: utf-8 -*-
# Home made test.

from __future__ import division
from __future__ import print_function

__author__= "Luis C. Pérez Tato (LCPT) and Ana Ortega (AOO)"
__copyright__= "Copyright 2015, LCPT and AOO"
__license__= "GPL"
__version__= "3.0"
__email__= "l.pereztato@gmail.com"

from actions.railway_traffic import SIA_rail_load_models

LM1= SIA_rail_load_models.LM1 # Load model.
qDerailmentModel2_25= LM1.trainDerailmentModel2(25.0)
qDerailmentModel2_8_4= LM1.trainDerailmentModel2(8.4)
qDerailmentModel2_6_4= LM1.trainDerailmentModel2(6.4)
ratio1= abs(qDerailmentModel2_25-145e3)/145e3
ratio2= abs(qDerailmentModel2_8_4-208.970588235e3)/208.970588235e3
ratio3= abs(qDerailmentModel2_6_4-220e3)/220e3

'''
print("ratio1= ", ratio1)
print("ratio2= ", ratio2)
print("ratio3= ", ratio3)
'''

import os
from misc_utils import log_messages as lmsg
fname= os.path.basename(__file__)
if ((ratio1<1e-15) and (ratio2<1e-11) and (ratio3<1e-15)):
    print('test '+fname+': ok.')
else:
    lmsg.error(fname+' ERROR.')
