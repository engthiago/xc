# -*- coding: utf-8 -*-
''' Wood construction materials according to
    National Design Specification for Wood Construction
    2018 Edition. 
VERIFICATION TESTS REQUIRED!!!.'''

from __future__ import division
from __future__ import print_function

import math
import scipy.interpolate
from misc_utils import log_messages as lmsg
from materials import wood_base
from materials.sections import section_properties as sp
from materials import typical_materials
from materials import steel_member_base
import pickle
from misc_utils import pickle_utils
import os

__author__= "Luis C. Pérez Tato (LCPT) , Ana Ortega (AO_O) "
__copyright__= "Copyright 2016, LCPT, AO_O"
__license__= "GPL"
__version__= "3.0"
__email__= "l.pereztato@ciccp.es, ana.ortega@ciccp.es "

# ENHANCE: it will be great if we can store
# the interpolation in a file to avoid repeating
# computations every time the module is loaded.
tenMinutes= 10/(365.25*24*60)
sevenDays= 7/365.25
twoMonths= 2/12
x= [0,tenMinutes,sevenDays,twoMonths,10,1000]
y= [2.0,1.6,1.25,1.15,1.0,0.9]

table232= scipy.interpolate.interp1d(x,y)

in2meter= 0.0254 # inch to meter conversion factor.
psi2Pa= 6894.76 # pounds per square inch to Pascal conversion factor.
pound2N= 4.44822 # pound to Newton conversion factor.
foot2meter= 0.3048 # foot to meter conversion factor.
pound2kg= 0.453592 # pound to kilogram conversion factor.

def getLoadDurationFactor(duration):
    '''Return load duration factor according to table 2.3.2
       of AWC-NDS2018.

       :param duration: duration of the load (years)
    '''
    return table232(duration)

def convertToFahrenheit(celsius):
    '''Convert to Fahrenheit degrees.'''
    return celsius * 9 / 5 + 32

def convertToCelsius(fahrenheit):
    '''Convert to Celsius degrees.'''
    return (fahrenheit - 32) * 5 / 9

def getWetServiceFactor(refValue, moistureContent, sawnLumber= False):
    '''Return the wet service factor according to AWC-NDS2018.

       :param refValue: reference design values (Ft,E,Emin,Fb,Fv,Fc and FcT)
       :param moistureContent: moisture content (%)
       :param sawnLumber: True or False
    '''
    retval= 1.0
    if(sawnLumber):
        if(moistureContent>=19):
            if(refValue in ['Fb','Ft','Fv','E','Emin']):
                retval= 1.00
            elif(refValue=='FcT'):
                retval= 0.67
            elif(refValue=='Fc'):
                retval= 0.91
            else:
                retval= 1.0
    else:
        if(moistureContent>=16):
            if(refValue in ['Fb','Ft']):
                retval= 0.8
            elif(refValue=='Fv'):
                retval= 0.875
            elif(refValue=='FcT'):
                retval= 0.53
            elif(refValue=='Fc'):
                retval= 0.73
            elif(refValue in ['E','Emin']):
                retval= 0.833
            else:
                retval= 1.0
    return retval

def getTemperatureFactor(refValue, moisture, fahrenheit):
    '''Return the temperature factor according to table 2.3.3
       of AWC-NDS2018.

       :param refValue: reference design values (Ft,E,Emin,Fb,Fv,Fc and FcT)
       :param moisture: moisture (dry or wet)
       :param fahrenheit: temperature (fahrenheit degrees)
    '''
    retval= 1.0
    dry= (moisture=='dry')
    line1= False
    if(refValue in ['Ft','E','Emin']):
        line1= True
    if(fahrenheit>100.0):
        if(fahrenheit<=125.0):
            if(line1):
                retval= 0.9
            else:
                if(dry):
                    retval= 0.8
                else:
                    retval= 0.7
        elif(fahrenheit<=150.0):
            if(line1):
                retval= 0.9
            else:
                if(dry):
                    retval= 0.7
                else:
                    retval= 0.5
        else:
            lmsg.error('Temperature too high: '+fahrenheit)
            retval= 0.01
    return retval
       
def getFormatConversionFactor(refValue, connection= False):
    '''Return the format conversion factor (LRFD) according to table 2.3.5
       of AWC-NDS2018.

      :param refValue: reference design values (Ft,Emin,Fb,Fv,
                       Frt,Fs,Fc and FcT)
      :param connection: connection design (True or False)
    '''
    retval= 3.32
    if(not connection):
        if(refValue=='Fb'):
            retval= 2.54
        elif(refValue=='Ft'):
            retval= 2.70
        elif(refValue in ['Ft','Frt','Fs']):
            retval= 2.88
        elif(refValue=='Fc'):
            retval= 2.40
        elif(refValue=='FcT'):
            retval= 1.67
        elif(refValue=='Emin'):
            retval= 1.76
    return retval

def getResistanceFactor(refValue, connection= False):
    '''Return the format conversion factor (LRFD) according to table 2.3.6
       of AWC-NDS2018.

      :param refValue: reference design values (Ft,Emin,Fb,Fv,
                       Frt,Fs,Fc and FcT)
      :param connection: connection design (True or False)
    '''
    retval= 0.65
    if(not connection):
        if(refValue=='Fb'):
            retval= 0.85
        elif(refValue=='Ft'):
            retval= 0.80
        elif(refValue in ['Ft','Frt','Fs']):
            retval= 0.75
        elif(refValue in ['Fc','FcT']):
            retval= 0.90
        elif(refValue=='Emin'):
            retval= 0.85
    return retval

def getVolumeFactor(L,b,h,southernPine= False):
    '''Return the volume factor according to clause 5.3.6
       of AWC-NDS2018.

      :param L: length of bending member between points
                of zero moment (m).
      :param b: width of bending member (m).
      :param d: depth of bending member (m).
    '''
    x= 1/10.0
    if(southernPine):
        x= 1/20.0
    bmax= min(b,10.75*in2meter)
    retval= math.pow(21*0.3048/L,x)*math.pow(12*in2meter/h,x)*math.pow(5.125*in2meter/bmax,x)
    retval= min(retval,1.0)
    return retval

def getFireDesignAdjustementFactor(refValue):
    '''Return the format conversion factor (LRFD) according to table 16.2.2
       of AWC-NDS2018.

      :param refValue: reference design values (Fb, FbE, Ft,Fc,
                       and FcE)
    '''
    retval= 2.03
    if(refValue=='Fb'):
        retval= 2.85
    elif(refValue=='FbE'):
        retval= 2.03
    elif(refValue=='Ft'):
        retval= 2.85
    elif(refValue=='Fv'):
        retval= 2.75
    elif(refValue=='Fc'):
        retval= 2.58
    elif(refValue=='FcE'):
        retval= 2.03
    else:
        retval= 2.03
    return retval

class Wood(wood_base.Wood):
    '''Base class for wood materials according
       to AWC-NDS2018.    
    '''
    def __init__(self, name, specificGravity= None):
        '''Constructor.

        :param name: wood name.
        :param specificGravity: specific gravity of the wood.   
        '''
        super(Wood,self).__init__(name, specificGravity)
        
    def getDowelBearingStrength(self, diameter, theta, endGrain= False):
        ''' Return the dowel bearing strength for Dowel-Type
            Fasteners in Wood Members according to Eq.
            12.3-11 and table 12.3.3 of National Design Specification of
            the American Wood Council

        :param diameter: diameter of the dowel-type fastener. 
        :param theta: angle between the direction of load and the
                      direction of grain (longitudinal axis of member).
        :param endGrain: true for dowel-type fasteners with D > 1/4" that are 
                         inserted into the end grain of the main member, 
                         with the fastener axis parallel to the wood fibers.
                         See clause 12.3.3.4 of NDS.
        '''
        retval= 0.0
        if(diameter<0.25*in2meter):
            retval= 16600.0*pow(self.specificGravity,1.84)
        else:
            Fe_perp= 6100.0*pow(self.specificGravity,1.84)/math.sqrt(diameter)
            if(endGrain):
                retval= Fe_perp
            else:
                Fe_parallel= 11200*self.specificGravity
                retval= Fe_parallel*Fe_perp/(Fe_parallel*math.sin(theta)**2+Fe_perp*math.cos(theta)**2)
        retval*= psi2Pa
        return retval


class LSL(Wood):
    ''' Base class for laminated strand lumber (LSL)'''
    def __init__(self, name= 'LSL'):
        ''' Constructor.

        :param name: material name.
        '''
        super(LSL,self).__init__(name)
        
    def getFb(self,depth):
        ''' Return the allowable bending stress given
            the member depth.

        :param depth: member depth.
        '''
        if(depth<3.5*in2meter):
            return 1.159*self.Fb_12
        else:
            return math.pow((12.0/depth),0.12)*self.Fb_12

class LSL_135E(LSL):
    ''' LSL 1.35E.'''
    E= 1.35e6*psi2Pa # Elastic modulus (Pa)
    Emin= E
    Fb_12= 1730*psi2Pa # Bending stress for 12" depth.
    Fv= 410*psi2Pa # Shear stress.
    Fc_pll= 1650*psi2Pa # Compression stress (parallel to grain)
    Fc_perp= 750*psi2Pa # Compression stress (perpendicular to grain)
    xc_material_name= 'LSL_135E'
    def __init__(self, name= 'LSL_135E'):
        ''' Constructor.

        :param name: material name.
        '''
        super(LSL_135E,self).__init__(name)
        
    def getFb(self,depth):
        ''' Return the allowable bending stress given
            the member depth.

        :param depth: member depth.
        '''
        if(depth<3.5*in2meter):
            return 1.159*Fb_12
        else:
            return math.pow((12.0/depth),0.12)*Fb_12

class LSL_155E(LSL):
    ''' LSL 1.35E.'''
    E= 1.55e6*psi2Pa # Elastic modulus (Pa)
    Emin= E
    Fb_12= 2360*psi2Pa # Bending stress for 12" depth.
    Fv= 410*psi2Pa # Shear stress.
    Fc_pll= 2175*psi2Pa # Compression stress (parallel to grain)
    Fc_perp= 875*psi2Pa # Compression stress (perpendicular to grain)
    xc_material_name= 'LSL_155E'
    def __init__(self, name= 'LSL_135E'):
        ''' Constructor.

        :param name: material name.
        '''
        super(LSL_155E,self).__init__(name)

class LVL_2900Fb2E(LSL):
    ''' LVL 2900Fb 2.0E structural.'''
    E= 2.0e6*psi2Pa # Elastic modulus (Pa)
    Emin= E
    Fb_12= 2900*psi2Pa # Bending stress for 12" depth.
    Fv= 285*psi2Pa # Shear stress.
    Fc_pll= 3200*psi2Pa # Compression stress (parallel to grain)
    Fc_perp= 750*psi2Pa # Compression stress (perpendicular to grain)
    xc_material_name= 'LVL_2900Fb2E'
    def __init__(self, name= 'LVL_2900Fb2E'):
        ''' Constructor.

        :param name: material name.
        '''
        super(LVL_2900Fb2E,self).__init__(name)


class MemberBase(object):
    ''' Base class for beam and column members according to chapter 
        3 of NDS-2018.
    '''
    def __init__(self, unbracedLength, section, connection= steel_member_base.MemberConnection()):
        ''' Constructor. '''
        self.unbracedLength= unbracedLength
        self.section= section
        self.connection= connection
        
class BeamMember(MemberBase):
    ''' Beam member according to chapter 3.3 of NDS-2018.'''
    def __init__(self, unbracedLength, section, connection= steel_member_base.MemberConnection()):
        ''' Constructor. '''
        super(BeamMember,self).__init__(unbracedLength, section, connection)
    def getEffectiveLength(self,numberOfConcentratedLoads= 0, lateralSupport= False, cantilever= False):
        ''' Return the effective length of the beam according to table
            3.3.3 of NDS-2018.

           :param numberOfConcentratedLoads: number of concentrated loads equally
                                             spaced along the beam (0: uniform load).
           :param lateralSupport: if true beam has a lateral support on each load.
           :param cantilever: if true cantilever beam otherwise single span beam.
        '''
        retval= self.unbracedLength
        if(cantilever):
            if(numberOfConcentratedLoads==0):
                retval*= 1.33 # Uniform load
            else:
                retval*= 1.87 # Concentrated load at unsupported end.
        else:
            ratio= self.unbracedLength/self.section.h
            if(numberOfConcentratedLoads==0):
                if(ratio<7.0):
                    retval*= 2.06 # Uniform load
                else:
                    retval= 1.63*self.unbracedLength+3.0*self.section.h
            elif((numberOfConcentratedLoads==1) and (not lateralSupport)):
                if(ratio<7.0):
                    retval*= 1.80 # Uniform load
                else:
                    retval= 1.37*self.unbracedLength+3.0*self.section.h
            elif(numberOfConcentratedLoads==1):
                if(lateralSupport):
                    retval*=1.11
                else:
                    lmsg.error('Load case not implemented.')
                    retval*=10.0
            elif(numberOfConcentratedLoads==2):
                if(lateralSupport):
                    retval*=1.68
                else:
                    lmsg.error('Load case not implemented.')
                    retval*=10.0
            elif(numberOfConcentratedLoads==3):
                if(lateralSupport):
                    retval*=1.54
                else:
                    lmsg.error('Load case not implemented.')
                    retval*=10.0
            elif(numberOfConcentratedLoads==4):
                if(lateralSupport):
                    retval*=1.68
                else:
                    lmsg.error('Load case not implemented.')
                    retval*=10.0
            elif(numberOfConcentratedLoads==5):
                if(lateralSupport):
                    retval*=1.73
                else:
                    lmsg.error('Load case not implemented.')
                    retval*=10.0
            elif(numberOfConcentratedLoads==6):
                if(lateralSupport):
                    retval*=1.78
                else:
                    lmsg.error('Load case not implemented.')
                    retval*=10.0
            elif(numberOfConcentratedLoads==7):
                if(lateralSupport):
                    retval*=1.84
                else:
                    lmsg.error('Load case not implemented.')
                    retval*=10.0
        return retval
    def getBendingSlendernessRatio(self,numberOfConcentratedLoads= 0, lateralSupport= False, cantilever= False):
        ''' Return the slenderness ratio according to equation
            3.3-5 of NDS-2018.

           :param numberOfConcentratedLoads: number of concentrated loads equally
                                             spaced along the beam (0: uniform load).
           :param lateralSupport: if true beam has a lateral support on each load.
           :param cantilever: if true cantilever beam otherwise single span beam.
        '''
        le= self.getEffectiveLength(numberOfConcentratedLoads, lateralSupport, cantilever)
        return math.sqrt(le*self.section.h/self.section.b**2)
    def getFbECriticalBucklingDesignValue(self,numberOfConcentratedLoads= 0, lateralSupport= False, cantilever= False):
        ''' Return the critical bucking design value for bending according to 
            section 3.3.3.8 of NDS-2018.

           :param numberOfConcentratedLoads: number of concentrated loads equally
                                             spaced along the beam (0: uniform load).
           :param lateralSupport: if true beam has a lateral support on each load.
           :param cantilever: if true cantilever beam otherwise single span beam.
        '''
        RB= self.getBendingSlendernessRatio(numberOfConcentratedLoads, lateralSupport, cantilever)
        return 1.2*self.section.wood.Emin/RB**2
    def getBeamStabilityFactor(self,numberOfConcentratedLoads= 0, lateralSupport= False, cantilever= False):
        ''' Return the beam stability factor according to equation 
            3.3.6 of NDS-2018.

           :param numberOfConcentratedLoads: number of concentrated loads equally
                                             spaced along the beam (0: uniform load).
           :param lateralSupport: if true beam has a lateral support on each load.
           :param cantilever: if true cantilever beam otherwise single span beam.
        '''
        FbE= self.getFbECriticalBucklingDesignValue(numberOfConcentratedLoads, lateralSupport, cantilever)
        ratio= FbE/self.section.wood.getFb(self.section.h)
        A= (1+ratio)/1.9
        B= A**2
        C= ratio/0.95
        return A-math.sqrt(B-C)

class ColumnMember(MemberBase):
    ''' Column member according to chapter 3.7 and 3.9 of NDS-2018.'''
    def __init__(self, unbracedLengthB, unbracedLengthH, section, connection= steel_member_base.MemberConnection()):
        ''' Constructor. '''
        super(ColumnMember,self).__init__(unbracedLengthB, section, connection)
        self.unbracedLengthH= unbracedLengthH

    def getUnbracedLengthB(self):
        ''' Return the B unbraced length.'''
        return self.unbracedLength

    def getEffectiveBucklingLengthCoefficientRecommended(self):
        '''Return the column effective buckling length coefficients
           according to NDS 2018 appendix G'''
        return self.connection.getEffectiveBucklingLengthCoefficientRecommended()
    def getBSlendernessRatio(self):
        ''' Return the slenderness ratio for the B dimension.'''
        Ke= self.getEffectiveBucklingLengthCoefficientRecommended()
        return Ke*self.getUnbracedLengthB()/self.section.b
    
    def getHSlendernessRatio(self):
        ''' Return the slenderness ratio for the H dimension.'''
        Ke= self.getEffectiveBucklingLengthCoefficientRecommended()
        return Ke*self.unbracedLengthH/self.section.h
        
    def getSlendernessRatio(self):
        ''' Return the slenderness ratio.'''
        Ke= self.getEffectiveBucklingLengthCoefficientRecommended()
        srB= Ke*self.getUnbracedLengthB()/self.section.b
        srH= Ke*self.unbracedLengthH/self.section.h
        return max(srB,srH)

    def getColumnStabilityFactor(self, c, E_adj, Fc_adj):
        ''' Return the column stability factor according
            to expression 3.7-1 of NDS-2.018. 

        :param E_adj: adjusted modulus of elasticity for beam 
                      stability and column stability calculations.
        :param Fc_adj: adjusted compression stress design value parallel 
                       to grain.
        :param c: 0.8 for sawn lumber, 0.85 for round timber poles 
                  and piles and 0.9 for structural glued laminated
                  timber structural composite lumber, and 
                  cross-laminated timber.
        '''
        sr= self.getSlendernessRatio()
        FcE= 0.822*E_adj/((sr)**2)
        ratio= FcE/Fc_adj
        tmp= (1+ratio)/2.0/c
        return tmp-math.sqrt(tmp**2-ratio/c)
    def getFcE1(self, E_adj):
        ''' Return the value of F_{cE1} as defined in section
            3.9.2 of NDS-2.018.

        :param E_adj: adjusted modulus of elasticity for beam 
                      stability and column stability calculations.
        '''
        if(self.section.h>self.section.b): # Wide side: H
            return 0.822*E_adj/(self.getHSlendernessRatio())**2
        else: # Wide side B
            return 0.822*E_adj/(self.getBSlendernessRatio())**2
    def getFcE2(self, E_adj):
        ''' Return the value of F_{cE2} as defined in section
            3.9.2 of NDS-2.018.

        :param E_adj: adjusted modulus of elasticity for beam 
                      stability and column stability calculations.
        '''
        if(self.section.h<self.section.b): # Narrow side: H
            return 0.822*E_adj/(self.getHSlendernessRatio())**2
        else: # Narrow side B
            return 0.822*E_adj/(self.getBSlendernessRatio())**2
    def getBendingSlendernessRatioH(self):
        ''' Return the slenderness ratio for bending in
            the h plane.'''
        Ke= self.getEffectiveBucklingLengthCoefficientRecommended()
        le= Ke*self.unbracedLengthH
        return math.sqrt(le*self.section.h/self.section.b**2)
    def getBendingSlendernessRatioB(self):
        ''' Return the slenderness ratio for bending in the
            B plane.'''
        Ke= self.getEffectiveBucklingLengthCoefficientRecommended()
        le= Ke*self.getUnbracedLengthB()
        return math.sqrt(le*self.section.b/self.section.h**2)
    def getFbE(self, E_adj):
        ''' Return the value of F_{bE} as defined in section
            3.9.2 of NDS-2.018.

        :param E_adj: adjusted modulus of elasticity for beam 
                      stability and column stability calculations.
        '''
        sr= 1.0
        if(self.section.h<self.section.b): # Narrow side: H
            sr= self.getBendingSlendernessRatioB()
        else: # Narrow side B
            sr= self.getBendingSlendernessRatioH()
        return 1.2*E_adj/sr**2
    def getCapacityFactor(self, E_adj, Fc_adj, Fb1_adj, Fb2_adj, fc,fb1, fb2):
        ''' Return the capacity factor for members subjected to a 
            combination of bending about one or both principal axes 
            and axial compression according to section 3.9.2 of
            NDS-2.018.

        :param E_adj: adjusted modulus of elasticity for beam 
                      stability and column stability calculations.
        :param Fc_adj: adjusted value of reference compression stress.
        :param Fb1_adj: adjusted value of reference bending stress (for
                        bending load applied to narrow face of member).
        :param Fb2_adj: adjusted value of reference bending stress (for
                        bending load applied to wide face of member).
        :param fc: compression stress.
        :param fb1: bending stress (bending load applied to narrow face
                    of member).
        :param fb2: bending stress (bending load applied to wide face
                    of member).
        '''
        val393= (fc/Fc_adj)**2 #Equation 3-9-3
        FcE1= self.getFcE1(E_adj) #Critical buckling design values.
        FcE2= self.getFcE2(E_adj)
        FbE= self.getFbE(E_adj)
        almostOne= 1-1e-15
        val393+= fb1/(Fb1_adj*(1-min(fc/FcE1,almostOne)))
        val393+= fb2/(Fb2_adj*(1-min(fc/FcE2,almostOne)-min(fb1/FbE,almostOne)**2))
        val394= fc/FcE2+(fb1/FbE)**2 #Equation 3-9-4
        return max(val393,val394)

class WoodPanelSection(sp.RectangularSection):
    ''' Wood structural panel.'''
    def __init__(self, name, b, h, shear_constant):
        ''' Constructor.

        :param name: section name.
        :param b: section width.
        :param h: section height.
        :param shear_constant: shear constant.
        '''
        super(WoodPanelSection,self).__init__(name, b, h)
        self.shearConstant= shear_constant
    def getArealDensity(self):
        ''' Return the mass per unit area (thickness= h).'''
        return self.rho*self.h
    def getSpanRating(self):
        ''' Return the span rating from the panel thickness according
            to the table B of the documents:
            "Design Capacities for Oriented Strand Board V1.0 – 01/2008"
            and "Design Capacities for Structural Plywood V1.0 – 01/2008"
            both from PFS TECO • 1507 Matt Pass, Cottage Grove, WI 53527, USA
        '''
        spanRating= 'unknown'
        if(self.name=='3/8'):
            spanRating= '24/0'
        elif(self.name=='7/16'):
            spanRating= '24/16'
        elif(self.name=='15/32'):
            spanRating= '32/16'
        elif(self.name=='1/2'):
            spanRating= '32/16'
        elif(self.name=='19/32'):
            spanRating= '40/20'
        elif(self.name=='5/8'):
            spanRating= '40/20'
        elif(self.name=='23/32'):
            spanRating= '48/24'
        elif(self.name=='3/4'):
            spanRating= '48/24'
        elif(self.name=='7/8'):
            spanRating= '48/24'
        elif(self.name=='1'):
            spanRating= '48/24'
        elif(self.name=='1-1/8'):
            spanRating= '48/24'
        else:
            lmsg.error('name: ', self.name, 'unknown')
        return spanRating
    def getAPARatedSturdIFloor(self):
        ''' Return the APA rated Sturd-I-Floor from the panel thickness 
            according to the table B of the documents:
            "Design Capacities for Oriented Strand Board V1.0 – 01/2008"
            and "Design Capacities for Structural Plywood V1.0 – 01/2008"
            both from PFS TECO • 1507 Matt Pass, Cottage Grove, WI 53527, USA
        '''
        spanRating= 'unknown'
        if(self.name=='3/8'):
            spanRating= '0oc'
        elif(self.name=='7/16'):
            spanRating= '0oc'
        elif(self.name=='15/32'):
            spanRating= '0oc'
        elif(self.name=='1/2'):
            spanRating= '0oc'
        elif(self.name=='19/32'):
            spanRating= '20oc'
        elif(self.name=='5/8'):
            spanRating= '20oc'
        elif(self.name=='23/32'):
            spanRating= '24oc'
        elif(self.name=='3/4'):
            spanRating= '24oc'
        elif(self.name=='7/8'):
            spanRating= '32oc'
        elif(self.name=='1'):
            spanRating= '32oc'
        elif(self.name=='1-1/8'):
            spanRating= '48oc'
        else:
            lmsg.error('name: ', self.name, 'unknown')
        return spanRating


# Properties of Plywood structural panels taken from:
# http://www.pfsteco.com/techtips/pdf/tt_plywooddesigncapacities
# table C.

class PlywoodPanelSection(WoodPanelSection):
    ''' Plywood structural panel.'''
    rho= 577.941243312 # density kg/m3
    def __init__(self, name, b, h, shear_constant):
        ''' Constructor.

        :param name: section name.
        :param b: section width.
        :param h: section height.
        :param shear_constant: shear constant.
        '''
        super(PlywoodPanelSection,self).__init__(name, b, h, shear_constant)

# Oriented strand board panels according to document:
# "Panel design specification" Form No. D510C/Revised May 2012/0300

class OSBPanelSection(WoodPanelSection):
    ''' Oriented strand board panel.'''
    rho= 632.62 # average density kg/m3 Table 12
    def __init__(self, name, b, h, shear_constant):
        ''' Constructor.

        :param name: section name.
        :param b: section width.
        :param h: section height.
        :param shear_constant: shear constant.
        '''
        super(OSBPanelSection,self).__init__(name, b, h, shear_constant)
    def getFb(self, angle= math.pi/2.0):
        ''' Return the bending stress Fb or the panel according
            to the table A of the document:
            "Design Capacities for Oriented Strand Board V1.0 – 01/2008"
            from PFS TECO • 1507 Matt Pass, Cottage Grove, WI 53527, USA

            angle: angle of the stress with the strength axis.
        '''
        spanRating= self.getAPARatedSturdIFloor()
        FbS= 0.0
        if((angle!=0.0) and (angle!=math.pi/2.0)):
            lmsg.error('angle must be 0 or PI/2')
        if(spanRating!='unknown'):
            if(spanRating=='16oc'):
                if(angle==0.0):
                    FbS= 500.0
                else:
                    FbS= 180.0
            elif(spanRating=='20oc'):
                if(angle==0.0):
                    FbS= 575.0
                else:
                    FbS= 250.0
            elif(spanRating=='24oc'):
                if(angle==0.0):
                    FbS= 770.0
                else:
                    FbS= 385.0                
            elif(spanRating=='32oc'):
                if(angle==0.0):
                    FbS= 1050.0
                else:
                    FbS= 685.0                
            elif(spanRating=='48oc'):
                if(angle==0.0):
                    FbS= 1900.0
                else:
                    FbS= 1200.0                
        else:
            spanRating= self.getSpanRating()
            if(spanRating=='24/0'):
                if(angle==0.0):
                    FbS= 300.0
                else:
                    FbS= 97.0
            elif(spanRating=='24/16'):
                if(angle==0.0):
                    FbS= 385.0
                else:
                    FbS= 115.0
            elif(spanRating=='32/16'):
                if(angle==0.0):
                    FbS= 445.0
                else:
                    FbS= 165.0                
            elif(spanRating=='40/20'):
                if(angle==0.0):
                    FbS= 750.0
                else:
                    FbS= 270.0                
            elif(spanRating=='48/24'):
                if(angle==0.0):
                    FbS= 1000.0
                else:
                    FbS= 405.0
        Fb= FbS/self.Wzel()*4.44822*0.0254/0.3048
        return Fb
    def getFv(self):
        ''' Return the bending stress Fb or the panel according
            to the table A of the document:
            "Design Capacities for Oriented Strand Board V1.0 – 01/2008"
            from PFS TECO • 1507 Matt Pass, Cottage Grove, WI 53527, USA
        '''
        spanRating= self.getAPARatedSturdIFloor()
        Fvtv= 0.0
        if(spanRating!='unknown'):
            if(spanRating=='16oc'):
                Fvtv= 170.0
            elif(spanRating=='20oc'):
                Fvtv= 195.0
            elif(spanRating=='24oc'):
                Fvtv= 215.0
            elif(spanRating=='32oc'):
                Fvtv= 230.0
            elif(spanRating=='48oc'):
                Fvtv= 305.0
        else:
            spanRating= self.getSpanRating()
            if(spanRating=='24/0'):
                Fvtv= 155.0
            elif(spanRating=='24/16'):
                Fvtv= 165.0
            elif(spanRating=='32/16'):
                Fvtv= 180.0
            elif(spanRating=='40/20'):
                Fvtv= 195.0
            elif(spanRating=='48/24'):
                Fvtv= 220.0
        Fv= Fvtv/self.h*4.44822/0.3048
        return Fv
    def getE(self, angle= math.pi/2.0):
        ''' Return the bending stress Fb or the panel according
            to the table A of the document:
            "Design Capacities for Oriented Strand Board V1.0 – 01/2008"
            from PFS TECO • 1507 Matt Pass, Cottage Grove, WI 53527, USA

        :param angle: angle of the stress with the strength axis.
        '''
        spanRating= self.getAPARatedSturdIFloor()
        EI= 0.0
        if((angle!=0.0) and (angle!=math.pi/2.0)):
            lmsg.error('angle must be 0 or PI/2')
        if(spanRating!='unknown'):
            if(spanRating=='16oc'):
                if(angle==0.0):
                    EI= 150e3
                else:
                    EI= 34e3
            elif(spanRating=='20oc'):
                if(angle==0.0):
                    EI= 210e3
                else:
                    EI= 40.5e3
            elif(spanRating=='24oc'):
                if(angle==0.0):
                    EI= 300e3
                else:
                    EI= 80.5e3
            elif(spanRating=='32oc'):
                if(angle==0.0):
                    EI= 650e3
                else:
                    EI= 235e3
            elif(spanRating=='48oc'):
                if(angle==0.0):
                    EI= 1150e3
                else:
                    EI= 495e3
        else:
            spanRating= self.getSpanRating()
            if(spanRating=='24/0'):
                if(angle==0.0):
                    EI= 60e3
                else:
                    EI= 11e3
            elif(spanRating=='24/16'):
                if(angle==0.0):
                    EI= 78e3
                else:
                    EI= 16e3
            elif(spanRating=='32/16'):
                if(angle==0.0):
                    EI= 115e3
                else:
                    EI= 25e3
            elif(spanRating=='40/20'):
                if(angle==0.0):
                    EI= 225e3
                else:
                    EI= 56e3
            elif(spanRating=='48/24'):
                if(angle==0.0):
                    EI= 400e3
                else:
                    EI= 91.5e3
        E= EI/self.Iz()*4.44822*0.0254**2/0.3048
        return E
    
    def defElasticShearSection2d(self,preprocessor, angle= math.pi/2.0, overrideRho= None):
        ''' Defines a elastic shear section for two-dimensional
            problems.

        :param preprocessor: pre-processor for the finite element problem.
        :param angle: angle of the stress with the strength axis.
        :param overrideRho: if defined (not None), override the value of 
                            the material density.
        '''
        matName= self.name+'_'+'osbMaterial'
        rho= self.rho
        if(overrideRho!=None):
            rho= overrideRho
        osbMaterial= typical_materials.MaterialData(name= matName,E= self.getE(angle), nu=0.2, rho= rho)
        retval= super(OSBPanelSection,self).defElasticShearSection2d(preprocessor,osbMaterial, overrideRho= overrideRho)
        return retval



class HeaderSection(sp.RectangularSection):
    ''' Structural beam/header.'''
    nu= 0.2
    def __init__(self, name, b, h, Ms, Vs, linearDensity, wood):
        ''' Constructor.

        :param name: section name.
        :param b: section width.
        :param h: section height.
        :param Ms: allowable moment.
        :param Vs: allowable shear.
        :param linearDensity: mass per unit length.
        :param wood: timber material.
        '''
        super(HeaderSection,self).__init__(name, b, h)
        self.Ms= Ms # Allowable moment.
        self.Vs= Vs # Allowable shear.
        self.rho= linearDensity/b/h
        self.wood= wood
        self.xc_wood_material= None
        self.xc_section= None
    def getLinearDensity(self):
        ''' Return the mass per unit length.'''
        return self.rho*self.b*self.h
    def getFb(self):
        return self.getVolumeFactor()*self.wood.Fb_12
    def defXCMaterial(self, overrideRho= None):
        '''Defines the material in XC.

        :param overrideRho: if defined (not None), override the value of 
                            the material density.
        '''
        rho= self.rho
        if(overrideRho!=None):
            rho= overrideRho
        if(not self.xc_wood_material):
            self.xc_wood_material= typical_materials.MaterialData(name= self.wood.xc_material_name,E=self.wood.E,nu=self.nu,rho= rho)
        return self.xc_wood_material
    
    def defElasticShearSection2d(self, preprocessor, overrideRho= None):
        ''' Defines a elastic shear section for two-dimensional
            problems.

        :param preprocessor: pre-processor for the finite element problem.
        :param overrideRho: if defined (not None), override the value of 
                            the material density.
        '''
        mat= self.defXCMaterial(overrideRho= overrideRho)
        self.xc_section= super(HeaderSection,self).defElasticShearSection2d(preprocessor,mat, overrideRho)
        return self.xc_section
    
    def defElasticShearSection3d(self, preprocessor, overrideRho= None):
        ''' Defines a elastic shear section for three-dimensional
            problems.

        :param preprocessor: pre-processor for the finite element problem.
        :param overrideRho: if defined (not None), override the value of 
                            the material density.
        '''
        mat= self.defXCMaterial(overrideRho= overrideRho)
        self.xc_section= super(HeaderSection,self).defElasticShearSection3d(preprocessor,mat, overrideRho)
        return self.xc_section

    
# Properties of LSL beams and headers taken from:
# LP SolidStart LSL Beam & Header Technical Guide

class LSLHeaderSection(HeaderSection):
    ''' LSL structural beam/header.'''
    def __init__(self, name, b, h, Ms, Vs, linearDensity, wood):
        ''' Constructor.

        :param name: section name.
        :param b: section width.
        :param h: section height.
        :param Ms: allowable moment.
        :param Vs: allowable shear.
        :param linearDensity: mass per unit length.
        :param wood: timber material.
        '''
        super(LSLHeaderSection,self).__init__(name, b, h, Ms, Vs, linearDensity, wood)
    def getVolumeFactor(self):
        '''Return volumen factor.'''
        retval= 1.0
        in12= 12.0*in2meter
        in3_5= 3.5*in2meter
        if(self.h<in3_5):
            retval*= 1.159
        else:
            retval= pow(in12/self.h,0.12)
        return retval

class LSL_135E_HeaderSection(LSLHeaderSection):
    ''' LSL 1.35E structural beam/header.'''
    def __init__(self, name, b, h, Ms, Vs, linearDensity):
        ''' Constructor.

        :param name: section name.
        :param b: section width.
        :param h: section height.
        :param Ms: allowable moment.
        :param Vs: allowable shear.
        :param linearDensity: mass per unit length.
        '''
        super(LSL_135E_HeaderSection,self).__init__(name, b, h, Ms, Vs, linearDensity, wood= LSL_135E())

class LSL_155E_HeaderSection(LSLHeaderSection):
    ''' LSL 1.55E structural beam/header.'''
    def __init__(self, name, b, h, Ms, Vs, linearDensity):
        ''' Constructor.

        :param name: section name.
        :param b: section width.
        :param h: section height.
        :param Ms: allowable moment.
        :param Vs: allowable shear.
        :param linearDensity: mass per unit length.
        '''
        super(LSL_155E_HeaderSection,self).__init__(name, b, h, Ms, Vs, linearDensity, wood= LSL_155E())

# Properties of LVL beams and headers taken from:
# LP SolidStart LVL Beam & Header Technical Guide

class LVLHeaderSection(HeaderSection):
    ''' LVL structural beam/header.'''
    def __init__(self, name, b, h, Ms, Vs, linearDensity, wood):
        ''' Constructor.

        :param name: section name.
        :param b: section width.
        :param h: section height.
        :param Ms: allowable moment.
        :param Vs: allowable shear.
        :param linearDensity: mass per unit length.
        :param wood: timber material.
        '''
        super(LVLHeaderSection,self).__init__(name, b, h, Ms, Vs, linearDensity, wood)
    def getVolumeFactor(self):
        '''Return volumen factor.'''
        retval= 1.0
        in12= 12.0*0.0254
        in3_5= 3.5*0.0254
        if(self.h>in12):
            retval*= pow((in12/self.h),0.143)
        else:
            if(self.h<in3_5):
                retval*= 1.147
            else:
                retval*= pow((in12/self.h),0.111)
        return retval

class LVL_2900Fb2E_HeaderSection(LVLHeaderSection):
    ''' LVL 2900Fb 2.0E structural beam/header.'''
    def __init__(self, name, b, h, Ms, Vs, linearDensity):
        ''' Constructor.

        :param name: section name.
        :param b: section width.
        :param h: section height.
        :param Ms: allowable moment.
        :param Vs: allowable shear.
        :param linearDensity: mass per unit length.
        '''
        super(LVL_2900Fb2E_HeaderSection,self).__init__(name, b, h, Ms, Vs, linearDensity, wood= LVL_2900Fb2E())

class CustomLumberSection(sp.RectangularSection):
    ''' Section of a lumber member with custom dimensions.'''
    def __init__(self, name, b, h, woodMaterial):
        ''' Constructor.

        :param name: section name.
        :param b: section width.
        :param h: section height.
        :param woodMaterial: timber material.
        '''
        super(CustomLumberSection,self).__init__(name, b, h)
        self.wood= woodMaterial
        self.xc_section= None
        
    def getFb(self):
        return self.wood.getFb(self.h)
    
    def defElasticShearSection2d(self, preprocessor, overrideRho= None):
        ''' Defines a elastic shear section for two-dimensional
            problems.

        :param preprocessor: pre-processor for the finite element problem.
        :param overrideRho: if defined (not None), override the value of 
                            the material density.
        '''
        mat= self.wood.defXCMaterial(overrideRho= overrideRho)
        self.xc_section= super(CustomLumberSection,self).defElasticShearSection2d(preprocessor,mat, overrideRho= overrideRho)
        return self.xc_section
    
    def defElasticShearSection3d(self, preprocessor, overrideRho= None):
        ''' Defines a elastic shear section for three-dimensional
            problems.

        :param preprocessor: pre-processor for the finite element problem.
        :param overrideRho: if defined (not None), override the value of 
                            the material density.
        '''
        mat= self.wood.defXCMaterial(overrideRho= overrideRho)
        self.xc_section= super(CustomLumberSection,self).defElasticShearSection3d(preprocessor, mat, overrideRho= overrideRho)
        return self.xc_section
    
    def getBendingFlatUseFactor(self):
        ''' Return the flat use factor for the bending design
            value Fb.'''
        return self.wood.getBendingFlatUseFactor(self.b,self.h)
    def getBendingSizeFactor(self):
        ''' Return the size factor for the bending design
            value Fb.'''
        return self.wood.getBendingSizeFactor(self.b,self.h)
    def getTensionSizeFactor(self):
        ''' Return the size factor for the tension design
            value Ft.'''
        return self.wood.getTensionSizeFactor(self.b,self.h)
    def getCompressionSizeFactor(self):
        ''' Return the size factor for the compression design
            value Fc.'''
        return self.wood.getCompressionSizeFactor(self.b,self.h)
    def getFbAdj(self, Cr= 1.0):
        ''' Return the adjusted value of Fb.

        :param Cr: repetitive member factor
        '''
        return self.wood.getFbAdj(self.b,self.h, Cr)
    def getFtAdj(self):
        ''' Return the adjusted value of Ft.'''
        return self.wood.getFtAdj(self.b,self.h)
    def getFvAdj(self):
        ''' Return the adjusted value of Fv.'''
        return self.wood.getFvAdj()
    def getFc_perpAdj(self, Cb= 1.0):
        ''' Return the adjusted value of Fc_perp.

        :param Cb: bearing area factor
        '''
        return self.wood.getFc_perpAdj(Cb)
    def getFcAdj(self):
        ''' Return the adjusted value of Fc.'''
        return self.wood.getFcAdj(self.b,self.h)
    def getEAdj(self):
        ''' Return the adjusted value of E.'''
        return self.wood.getEAdj()
    def getEminAdj(self):
        ''' Return the adjusted value of Emin.'''
        return self.wood.getEminAdj()

dimensionLumberSizes= dict()
dimensionLumberSizes['1x2']= (19e-3, 38e-3)
dimensionLumberSizes['1x3']= (19e-3, 64e-3)
dimensionLumberSizes['1x4']= (19e-3, 89e-3)
dimensionLumberSizes['1x5']= (19e-3, 114e-3)
dimensionLumberSizes['1x6']= (19e-3, 140e-3)
dimensionLumberSizes['1x8']= (19e-3, 184e-3)
dimensionLumberSizes['1x10']= (19e-3, 235e-3)
dimensionLumberSizes['1x12']= (19e-3, 286e-3)
dimensionLumberSizes['2x2']= (38e-3, 38e-3)
dimensionLumberSizes['2x3']= (38e-3, 64e-3)
dimensionLumberSizes['2x4']= (38e-3, 89e-3)
dimensionLumberSizes['2x6']= (38e-3, 140e-3)
dimensionLumberSizes['2x8']= (38e-3, 184e-3)
dimensionLumberSizes['2x10']= (38e-3, 235e-3)
dimensionLumberSizes['2x12']= (38e-3, 286e-3)
dimensionLumberSizes['4x4']= (89e-3, 89e-3)
dimensionLumberSizes['4x6']= (89e-3, 140e-3)
dimensionLumberSizes['6x6']= (140e-3, 140e-3)
dimensionLumberSizes['2x1']= (38e-3, 19e-3)
dimensionLumberSizes['3x1']= (64e-3, 19e-3)
dimensionLumberSizes['4x1']= (89e-3, 19e-3)
dimensionLumberSizes['5x1']= (114e-3, 19e-3)
dimensionLumberSizes['6x1']= (140e-3, 19e-3)
dimensionLumberSizes['8x1']= (184e-3, 19e-3)
dimensionLumberSizes['10x1']= (235e-3, 19e-3)
dimensionLumberSizes['12x1']= (286e-3, 19e-3)
dimensionLumberSizes['3x2']= (64e-3, 38e-3)
dimensionLumberSizes['4x2']= (89e-3, 38e-3)
dimensionLumberSizes['6x2']= (140e-3, 38e-3)
dimensionLumberSizes['8x2']= (184e-3, 38e-3)
dimensionLumberSizes['10x2']= (235e-3, 38e-3)
dimensionLumberSizes['12x2']= (286e-3, 38e-3)
dimensionLumberSizes['6x4']= (140e-3, 89e-3)

class DimensionLumberSection(CustomLumberSection):
    ''' Section of a dimension lumber member.'''
    def __init__(self, name, woodMaterial):
        ''' Constructor.

        :param name: section name.
        :param woodMaterial: timber material.
        '''
        b= 0.0
        h= 0.0
        if(name in dimensionLumberSizes.keys()):
            size= dimensionLumberSizes[name]
            b= size[0]
            h= size[1]
        else:
            lmsg.error('section: \''+name+'\' doesn\'t exists.')
        super(DimensionLumberSection,self).__init__(name, b, h, woodMaterial)
