# -*- coding: utf-8 -*-
''' Wind according to the last version of ASCE 7 (ASCE 7-22)
(*) means unchanged from ASCE 7-16 
'''

from __future__ import division
from __future__ import print_function

__author__= "Luis C. Pérez Tato (LCPT) and Ana Ortega (AO_O)"
__copyright__= "Copyright 2021, AO_O and LCPT"
__license__= "GPL"
__version__= "1.0"
__email__= "l.pereztato@gmail.com  ana.Ortega.Ort@gmail.com"

import math
import scipy.interpolate
from actions.wind import base_wind
from misc_utils import log_messages as lmsg

# Topographic multipliers (tables from figure 26.8-1)
xk1= [0.2, 0.25, 0.3, 0.35, 0.4, 0.45, 0.5] # H/Lh
k12DRidge= [0.29, 0.36, 0.43, 0.51, 0.58, 0.65, 0.72] # k1 multiplier 2D ridge
k12DRidgeInterp= scipy.interpolate.interp1d(xk1,k12DRidge)
k12DEscarpment= [0.17, 0.21, 0.26, 0.3, 0.34, 0.38, 0.43] # k1 multiplier 2D escarpment
k12DEscarpmentInterp= scipy.interpolate.interp1d(xk1,k12DEscarpment)
k13DAxisymmetricalHill= [0.21, 0.26, 0.32, 0.37, 0.42, 0.47, 0.53] # k1 multiplier axisymmetrical hill
k13DAxisymmetricalHillInterp= scipy.interpolate.interp1d(xk1,k13DAxisymmetricalHill)

def K1K2K3(siteCondition: str, H: float, Lh: float, x: float, upWindOfCrest: bool, z: float):
    ''' Return K1, K2 and K3 factors according to figure 26.8.1 of ASCE 7-22 (*).

    :param siteCondition: site condition ('2DR':2D ridge, '2DE': 2D escarpment,
                          '3DA': 3D axisymmetrical hill).
    :param H: Height of hill or escarpment relative to the upwind terrain.
    :param Lh: Distance upwind of crest to where the difference in ground 
               elevation is half the height of hill or escarpment.
    :param x: Distance (upwind or downwind) from the crest to the site of 
              the building or other structure.
    :param upWindOfCrest: true if the building is upwind from the crest,
                          false otherwise.
    :param z: Height above ground surface at the site of the building or 
              other structure.
    '''
    H_Lh= min(H/Lh, 0.5)
    gamma= 3.0 # height attenuation factor for 2D ridge
    mu= 1.5 # horizontal attenuation factor
    k1= 0.72 # Factor to account for shape of topographic feature and maximum 
             # speed-up effect according to figure 26.8.1 of ASCE 7-22 (*).
    if(siteCondition=='2DR'):
        k1= float(k12DRidgeInterp(H_Lh))
    elif(siteCondition=='2DE'):
        k1= float(k12DEscarpmentInterp(H_Lh))
        gamma= 2.5
        if(not upWindOfCrest):
            mu= 4.0 
    elif(siteCondition=='3DA'):
        k1= float(k13DAxisymmetricalHillInterp(H_Lh))
        gamma= 4.0
    k2= max(1.0-abs(x)/(mu*Lh),0) # factor to account for reduction in speed-up with
                         # distance upwind or downwind of crest.
    k3= max(math.exp(-gamma*z/Lh),0) # factor to account for reduction in speed-up
                              # with height above local terrain
    return k1*k2*k3

def Kzt(siteCondition: str, H: float, Lh: float, x: float, upWindOfCrest: bool, z: float):
    ''' Return Topographic factor Kzt according to figure 26.8-1 of ASCE 7-22 (*).

    :param siteCondition: site condition ('2DR':2D ridge, '2DE': 2D escarpment,
                          '3DA': 3D axisymmetrical hill).
    :param H: Height of hill or escarpment relative to the upwind terrain.
    :param Lh: Distance upwind of crest to where the difference in ground 
               elevation is half the height of hill or escarpment.
    :param x: Distance (upwind or downwind) from the crest to the site of 
              the building or other structure.
    :param upWindOfCrest: true if the building is upwind from the crest,
                          false otherwise.
    :param z: Height above ground surface at the site of the building or 
              other structure.
    '''
    k1k2k3= K1K2K3(siteCondition, H, Lh, x, upWindOfCrest, z)
    return (1+k1k2k3)**2

def Ke(zg):
    ''' Ground elevation factor Ke according to expression note 2 of 
        table 26.9-1 of ASCE 7-22 (*). The ground elevation factor, Ke, was
        introduced in ASCE 7-16 to consider the variation in the air 
        density based on ground elevation above mean sea level. 

    :param zg: ground elevation above sea level (m).
    '''
    return math.exp(-0.000119*zg)

def alphaConstant(exposure: str):
    ''' Return the terrain exposure constant alpha
        according to table 26.11-1 of ASCE 7-22.

    :param exposure: 'B', 'C' or 'D'
    '''
    if(exposure=='B'):
        return 7.0
    elif(exposure=='C'):
        return 9.8
    elif(exposure=='D'):
        return 11.5
    else:
        lmsg.error('exposure value: \''+exposure+'\' unknown.')

def zgConstant(exposure: str):
    ''' Return the terrain exposure constant zg
        according to table 26.11-1 of ASCE 7-22.

    :param exposure: 'B', 'C' or 'D'
    '''
    if(exposure=='B'):
        return 1000.00
    elif(exposure=='C'):
        return 750.00
    elif(exposure=='D'):
        return 590.00
    else:
        lmsg.error('exposure value: \''+exposure+'\' unknown.')

def Kz(exposure: str, z: float):
    ''' Return velocity pressure coefficient Kz and Kh according
        to note 1 of table 26.10-1 of ASCE 7-22. This parameter depends 
        on the height above ground level of the point where the wind 
        pressure is considered, and the exposure category. 

    :param exposure: 'B', 'C' or 'D'
    :param z: Height above ground surface at the site of the building or 
              other structure.
    '''
    zgC= zgConstant(exposure)
    alphaC= alphaConstant(exposure)
    zv= max(z,4.6)
    return 2.41*math.pow(zv/zgC,2.0/alphaC)


def qz(z, Kz, Kzt, Kd, zg, V):
    ''' Velocity pressure according to expression 26.10-1.si
        of ASCE 7-22 (*).

    :param Kz: velocity pressure exposure coefﬁcient, according to 
               section 26.10.1 of ASCE 7-22.
    :param Kzt: topographic factor, according to 
               section 26.8.2 of ASCE 7-22.
    :param Kd: wind directionality factor, according to 
               table 26.6-1 of ASCE 7-22.
    :param zg: ground elevation above sea level.
    :param V: basic wind speed, , according to 
               section 26.5 of ASCE 7-22.
    '''
    return 0.613*Kz*Kzt*Kd*Ke(zg)*V**2 


# Wall pressure coefficients Cp
leewardWall_x= [0.0,1.0,2.0,4.0,1.0e6]  # L/B values
leewardWall_y= [-0.5,-0.5,-0.3,-0.2,-0.2] # Cp values (to be used with qh)
leewardWallPressureCoefs= scipy.interpolate.interp1d(leewardWall_x,leewardWall_y)
def wallExternalPressureCoefficient(orientation: base_wind.windSurfaceOrientation, L: float, B: float):
    ''' Return the external pressure coefficient for a wall according to 
        figure 27.3-1 of ASCE 7-22 (*). Windward values must be 
        used with qz (evaluated at height z above the ground) and 
        leeward and side values must be used with qh (evaluated at mean roof height). 
        Plus and minus signs signify pressures 
        acting toward(+) and away(-) from the surfaces, respectively.

    :param orientation: orientation of the wall surface with respect to the 
                        wind: windward, side or leeward.
    :param L: Horizontal dimension of building, measured parallel to wind direction.
    :param B: Horizontal dimension of building, measured normal to wind direction.
    '''
    retval= 0.8 # Cp windward wall for all values of L/B  (used with qz)
    if(orientation == base_wind.windSurfaceOrientation.side):
        retval= -0.7 # Cp sidewall for all values of L/B (used with qh)
    elif(orientation == base_wind.windSurfaceOrientation.leeward):
        retval= leewardWallPressureCoefs(L/B)
    return retval

# Windward roof pressure coefficients Cp for use with qh
# with h/L<=0.25
roofAngles_x=              [ 10.0,  15.0,  20.0, 25.0, 30.0, 35.0, 45.0, 60.0, 90.0]
roofWindwardThGEQ10025A_y= [ -0.7,  -0.5,  -0.3, -0.2, -0.2,  0.0,  0.0,  0.0,  0.0]
roofWindwardThGEQ10025A= scipy.interpolate.interp1d(roofAngles_x,roofWindwardThGEQ10025A_y)

def roofWindwardThGEQ10025B(theta: float):
    ''' Return the value corresponding to the windward roof pressure coefficients 
       with h/L<=0.25.
       From table in figure 27.3-1 of ASCE 7-22 (*)
        
    :param theta: angle of plane of roof from horizontal, in degrees.
    '''
    y= [-0.18,   0.0,   0.2,  0.3,  0.3,  0.4,  0.4,  0.01*theta, 0.01*theta]
    return float(scipy.interpolate.interp1d(roofAngles_x,y)(theta))

# with 0.25<h/L<1.0
roofWindwardThGEQ1005A_y=  [ -0.9,  -0.7,  -0.4, -0.3, -0.2, -0.2,  0.0,  0.0,  0.0] 
roofWindwardThGEQ1005A= scipy.interpolate.interp1d(roofAngles_x,roofWindwardThGEQ1005A_y)

def roofWindwardThGEQ1005B(theta: float):
    ''' Return the value corresponding to the windward roof pressure coefficients 
       with 0.25<h/L<1.0
       From table in figure 27.3-1 of ASCE 7-22 (*).

    :param theta: angle of plane of roof from horizontal, in degrees.
    '''
    y=  [-0.18, -0.18,   0.0,  0.2,  0.2,  0.3,  0.4,  0.01*theta, 0.01*theta]
    return float(scipy.interpolate.interp1d(roofAngles_x,y)(theta))

roofWindwardThGEQ1010A_y=  [ -1.3,  -1.0,  -0.7, -0.5, -0.3, -0.2,  0.0,  0.0,  0.0] 
roofWindwardThGEQ1010A= scipy.interpolate.interp1d(roofAngles_x,roofWindwardThGEQ1010A_y)

#with h/L>=1.0
def roofWindwardThGEQ1010B(theta: float):
    ''' Return the value corresponding to the windward roof pressure coefficients 
       with h/L>=1.0.
       From table in figure 27.3-1 of ASCE 7-22 (*).

    :param theta: angle of plane of roof from horizontal, in degrees.
    '''
    y=  [-0.18, -0.18, -0.18,  0.0,  0.2,  0.2,  0.3,  0.01*theta, 0.01*theta] 
    return float(scipy.interpolate.interp1d(roofAngles_x,y)(theta))

# Leeward roof pressure coefficients Cp for use with qh
roofLeewardAngles_x=     [ 10.0,  15.0,  20.0, 90.0]
# with h/L<=0.25
roofLeewardThGEQ10025_y= [ -0.3,  -0.5,  -0.6, -0.6]
roofLeewardThGEQ10025= scipy.interpolate.interp1d(roofLeewardAngles_x, roofLeewardThGEQ10025_y)

# with 0.25<h/L<1.0
roofLeewardThGEQ1005_y=  [ -0.5,  -0.5,  -0.6, -0.6] 
roofLeewardThGEQ1005= scipy.interpolate.interp1d(roofLeewardAngles_x,roofLeewardThGEQ1005_y)

# with h/L>=1.0
roofLeewardThGEQ1010_y=  [ -0.7,  -0.6,  -0.6, -0.6] 
roofLeewardThGEQ1010= scipy.interpolate.interp1d(roofLeewardAngles_x,roofLeewardThGEQ1010_y)

def roofExternalPressureCoefficient(orientation: base_wind.windSurfaceOrientation, L: float, h: float, theta: float, hDistFromWindwardEdge= 0.0):
    ''' Return the external pressure coefficient for a roof according to 
        figure 27.3-1 of ASCE 7-22 (*). Windward values must be used with
        qz (eave height) and leeward and side values must be used with
        qh (mean roof height). Plus and minus signs signify pressures 
        acting toward(+) and away(-) from the surfaces, respectively.

    :param orientation: orientation of the wall surface with respect to the 
                        wind: windward, side or leeward.
    :param L: length of the building (parallel to the wind direction).
    :param h: mean height of the roof.
    :param theta: angle of plane of roof from horizontal, in degrees.
    :param hDistFromWindwardEdge: for roof surfaces, horizontal distance 
                                  from windward edge (defaults to 0).
    '''
    retval= [1.3]
    h_L= h/L
    windParallelToRidge= (orientation == base_wind.windSurfaceOrientation.side)
    if(not windParallelToRidge and (theta>=10.0)):
        h_L_x= [0.0, 0.25, 0.5, 1.0, 1e6] # (h/L) abscissae
        if(orientation == base_wind.windSurfaceOrientation.windward):
            value025A= roofWindwardThGEQ10025A(theta)
            value025B= roofWindwardThGEQ10025B(theta)
            value05A= roofWindwardThGEQ1005A(theta)
            value05B= roofWindwardThGEQ1005B(theta)
            value10A= roofWindwardThGEQ1010A(theta)
            value10B= roofWindwardThGEQ1010B(theta)
            CpA_y= [value025A, value025A, value05A, value10A, value10A]
            CpB_y= [value025B, value025B, value05B, value10B, value10B]
            CpA= float(scipy.interpolate.interp1d(h_L_x, CpA_y)(h_L))
            CpB= float(scipy.interpolate.interp1d(h_L_x, CpB_y)(h_L))
            retval= [CpA, CpB]
        elif(orientation == base_wind.windSurfaceOrientation.leeward):
            value025= roofLeewardThGEQ10025(theta)
            value05= roofLeewardThGEQ1005(theta)
            value10= roofLeewardThGEQ1010(theta)
            Cp_y= [value025, value025, value05, value10, value10]
            Cp= float(scipy.interpolate.interp1d(h_L_x, Cp_y)(h_L))
            retval= [Cp]
    else: # (theta < 10.0 or wind parallel to ridge)
        dists05=   [  0.0,     h, 1.01*h, 2.0*h, 2.01*h,   1e6]
        values05A= [ -0.9,  -0.9,   -0.5,  -0.5,   -0.3,  -0.3]
        Cp05A= float(scipy.interpolate.interp1d(dists05, values05A)(hDistFromWindwardEdge))
        dists10=   [  0.0, 0.5*h, 0.51*h,  1e6]
        values10A=  [ -1.3,  -1.3,   -0.7, -0.7]
        Cp10A= float(scipy.interpolate.interp1d(dists10, values10A)(hDistFromWindwardEdge))
        h_L_x=    [  0.0,   0.5,   1.0,  1.01,   1e6] # (h/L) abscissae
        valuesCp= [Cp05A, Cp05A, Cp10A, Cp10A, Cp10A]
        CpA= float(scipy.interpolate.interp1d(h_L_x, valuesCp)(h_L))
        CpB= -0.18
        retval= [CpA, CpB]
    return retval
