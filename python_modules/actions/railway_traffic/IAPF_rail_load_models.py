# -*- coding: utf-8 -*-
''' Functions to model railway loads according to spanish code IAPF.'''

from __future__ import division
from __future__ import print_function

import math
__author__= "Ana Ortega (AO_O) Luis C. Pérez Tato"
__copyright__= "Copyright 2017, AO_O LCPT"
__license__= "GPL"
__version__= "3.0"
__email__= "ana.Ortega@ciccp.es l.pereztato@ciccp.es"

'''
Axle loads of high speed trains according to IAPF clause
C.2 of annex C.
'''

cargasTrenICE2= [[0.00,195e3],[3.00,195e3],[11.46,195e3],[14.46,195e3],[19.31,112e3],[21.81,112e3],[38.31,112e3],[40.81,112e3],[45.71,112e3],[48.21,112e3],[64.71,112e3],[67.21,112e3],[72.11,112e3],[74.61,112e3],[91.11,112e3],[93.61,112e3],[98.51,112e3],[101.01,112e3],[117.51,112e3],[120.01,112e3],[124.91,112e3],[127.41,112e3],[143.91,112e3],[146.41,112e3],[151.31,112e3],[153.81,112e3],[170.31,112e3],[172.81,112e3],[177.71,112e3],[180.21,112e3],[196.71,112e3],[199.21,112e3],[204.11,112e3],[206.61,112e3],[223.11,112e3],[225.61,112e3],[230.51,112e3],[233.01,112e3],[249.51,112e3],[252.01,112e3],[256.91,112e3],[259.41,112e3],[275.91,112e3],[278.41,112e3],[283.31,112e3],[285.81,112e3],[302.31,112e3],[304.81,112e3],[309.71,112e3],[312.21,112e3],[328.71,112e3],[331.21,112e3],[336.06,195e3],[339.06,195e3],[347.52,195e3],[350.52,195e3]]

cargasTrenTalgo350= [[0.00,170e3],[2.65,170e3],[11.00,170e3],[13.65,170e3],[19.13,170e3],[28.10,170e3],[41.24,170e3],[54.38,170e3],[67.52,170e3],[80.66,170e3],[93.80,170e3],[106.94,170e3],[120.08,170e3],[133.22,170e3],[146.36,170e3],[155.33,170e3],[160.80,170e3],[163.45,170e3],[171.80,170e3],[174.45,170e3],[181.60,170e3],[184.25,170e3],[192.60,170e3],[195.25,170e3],[200.73,170e3],[209.70,170e3],[222.84,170e3],[235.98,170e3],[249.12,170e3],[262.26,170e3],[275.40,170e3],[288.54,170e3],[301.68,170e3],[314.82,170e3],[327.96,170e3],[336.93,170e3],[342.40,170e3],[345.05,170e3],[353.40,170e3],[356.05,170e3]]

def CoefReductorCentrifugaIAPF(v, Lf):
    '''Returns the reduction factor of the centrifugal force
     (according to expr. 2.3. IAPF)

    :param v: speed (m/s).
    :param Lf: 
    '''
    vkmhIAPF= v*3.6
    if(vkmhIAPF<120):
        coefFIAPF= 1
    elif(vkmhIAPF<300):
        coefFIAPF= 1-(vkmhIAPF-120)/1000*(814/vkmhIAPF+1.75)*(1-math.sqrt(2.88/Lf)) 
    else:
       coefFIAPF= 0.197+0.803*(math.sqrt(2.88/Lf))
    coefFIAPF= max(coefFIAPF,0.35)
    return coefFIAPF
