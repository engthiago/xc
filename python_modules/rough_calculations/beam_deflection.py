# -*- coding: utf-8 -*-

from __future__ import division

__author__= "Ana Ortega (A_OO) "
__copyright__= "Copyright 2016, A_OO"
__license__= "GPL"
__version__= "3.0"
__email__= "ana.Ortega.Ort@gmail.com "

#See notations in XC users manual

def getFlechaVigaBiapQUnif(l,EI,q):
    '''
    Beam deflection at mid-span for a simple supported beam under
    uniform load.

    :param l: span.
    :param EI: bending stiffness.
    :param q: load per unit length (uniform).
    '''  
    return -5*q*l**3/384/EI
  
def deflCantBeamPconcentr(l,EI,P,a):
    '''
    Maximum deflection in a cantilever beam with a concentrated load at any point
    '''
    return P*a**2/6/EI*(3*l-a)

def deflCantBeamQunif(l,EI,q):
    '''
    Maximum deflection in a cantilever beam with a uniformly distributed load
    '''
    return q*l**4/8/EI

def deflCantBeamMend(l,EI,M):
    '''
    Maximum deflection in a cantilever beam with a couple moment at the free end
    '''
    return M*l**2/2/EI

def deflSimplSupBeamPconcentr(l,EI,P,b):
    '''
    Maximum deflection in a beam simply supported at ends with a concentrated load at any point
    '''
    return P*b*(l**2-b**2)**(3/2)/9/3**(1/2)/l/EI

def deflSimplSupBeamQunif(l,EI,q):
    '''
    Maximum deflection in a beam simply supported at ends with a uniformly distributed load
    '''
    return 5*q*l**4/384/EI

def deflSimplSupBeamMend(l,EI,M):
    '''
    Maximum deflection in a beam simply supported at ends with a couple moment at the right end
    '''
    return M*l**2/9/3**(1/2)/EI


