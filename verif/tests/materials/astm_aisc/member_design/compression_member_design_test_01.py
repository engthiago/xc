# -*- coding: utf-8 -*-
# COMPANION TO THE AISC STEEL CONSTRUCTION MANUAL
# Volume 1: Design Examples
# EXAMPLE E.1A W-SHAPE COLUMN DESIGN WITH PINNED ENDS

from __future__ import division
from __future__ import print_function

__author__= "Ana Ortega (AO_O) and Luis C. Pérez Tato (LCPT)"
__copyright__= "Copyright 2020, AO_O and LCPT"
__license__= "GPL"
__version__= "3.0"
__email__= " ana.Ortega.Ort@gmail.com, l.pereztato@gmail.com"

from materials.astm_aisc import ASTM_materials
from materials.astm_aisc import AISC_limit_state_checking as aisc

inch2meter= 0.0254
MPa2ksi= 0.145038
kN2kips= 0.2248
N2kips= 0.2248e-3
kip2kN= 1.0/kN2kips
foot2meter= 0.3048
m2Toin2= 1.0/inch2meter**2

#Materials
## Steel material
steel= ASTM_materials.A992
steel.gammaM= 1.00
## Shape geometry
shape= ASTM_materials.WShape(steel,'W14X132')
## Steel column
column= aisc.Member('column', shape,30.0*foot2meter)

## Load.
Pd= 840e3*kip2kN

## Compressive strength
Pu= column.getDesignCompressiveStrength()
PuRef= 4098332.2632 # 893*kip2kN*1e3
ratio= abs(Pu-PuRef)/PuRef

'''
print('Pd= ',Pd/1e3,' kN / (',Pd*N2kips,'kips)')
print('Pu= ',Pu/1e3,' kN / (',Pu*N2kips,'kips)')
print('PuRef= ',PuRef/1e3,' kN / (',PuRef*N2kips,'kips)')
print('ratio= ',ratio)
'''

import os
from misc_utils import log_messages as lmsg
fname= os.path.basename(__file__)
if(ratio<1e-5):
    print('test '+fname+': ok.')
else:
    lmsg.error(fname+' ERROR.')
