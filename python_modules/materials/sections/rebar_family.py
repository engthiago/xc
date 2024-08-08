# -*- coding: utf-8 -*-
'''Definition of typical reinforcement schemes.'''
from __future__ import division
from __future__ import print_function

__author__= "Luis C. Pérez Tato (LCPT)"
__copyright__= "Copyright 2018, LCPT"
__license__= "GPL"
__version__= "3.0"
__email__= "l.pereztato@gmail.com"

import sys
import math
from rough_calculations import ng_simple_bending_reinforcement
from postprocess.reports import common_formats as fmt
from misc_utils import log_messages as lmsg

class Rebar(object):
    ''' Reinforcement bar.

      :ivar diam: diameter of the bars.
      :ivar steel: steel material.
    '''
    def __init__(self, diam:float, steel= None):
        ''' Constructor.

        :param diam: diameter of the bar.
        :param steel: steel material.
        '''
        self.diam= diam
        self.steel= steel
      
    def getDiam(self):
        ''' Return the diameter of the bars.'''
        return self.diam
    
    def getArea(self):
        ''' Return the area of each bar.'''
        return math.pi*(self.diam/2.0)**2

    def getDesignStrength(self):
        ''' Return the design value of the bar strength.'''
        retval= 0.0
        if(self.steel is not None):
            retval= self.getArea()*self.steel.fyd()
        else:
            lmsg.warning('Reinforcing steel undefined.')
        return retval
    
class RebarRow(Rebar):
    ''' Row of reinforcement bars.

      :ivar diam: diameter of the bars.
      :ivar spacing: spacing of the bars.
      :ivar concreteCover: concrete cover of the bars.
    '''
    def __init__(self,diam:float ,spacing:float ,concreteCover:float, steel= None):
        ''' Constructor.

        :param diam: diameter of the bars.
        :param spacing: spacing of the bars.
        :param concreteCover: concrete cover of the bars.
        '''
        super(RebarRow, self).__init__(diam= diam, steel= steel)
        self.spacing= spacing
        self.concreteCover= concreteCover
      
    def getBarArea(self):
        ''' Return the area of each bar.'''
        return super(RebarRow, self).getArea()
    
    def getNumBarsPerMeter(self, roundUp= False):
        ''' Return the number of bars per unit length.

        :param roundUp: if true return an integer number of bars greater or
                        equal to (width/spacing).
        '''
        retval= 1.0/self.spacing
        if(roundUp):
            retval= math.ceil(retval)
        return retval
        
    def getNumBars(self, width, roundUp= False):
        ''' Return the number of bars in the length argument.

        :param width: width of the reinforcement.
        :param roundUp: if true return an integer number of bars greater or
                        equal to (width/spacing).
        '''
        retval= width/self.spacing
        if(roundUp):
            retval= math.ceil(retval)
        return retval
    
    def getAs(self, width= 1.0, roundUp= False):
        ''' Return the total area of the bars.

        :param width: width of the reinforcement.
        '''
        return self.getNumBars(width, roundUp)*self.getBarArea()
    
    def getEffectiveCover(self):
        ''' returns the effective cover of the rebar family.

        Returns the distance between the surface of the concrete and the 
        centroid of the rebars family.
        '''
        return self.concreteCover+self.diam/2.0
    
    def getT(self, width= 1.0, roundUp= False):
        ''' Return the design value of the ultimate tension force in 
            the reinforcement.

        :param width: width of the reinforcement.
        :param roundUp: if true return an integer number of bars greater or
                        equal to (width/spacing).
        '''
        return self.getAs(width, roundUp)*self.steel.fyd()

    def getClearDist(self):
        '''Return the clear distance between parallel bars
        '''
        retval=self.spacing-self.diam
        return retval

    def getClearDistLap(self):
        '''Return the clear distance between lapped parallel bars
        Lapped bars are supposed to touch one another within the lap length
        '''
        retval=self.spacing-2*self.diam
        return retval
    
class RebarFamily(RebarRow):
    ''' Family of reinforcement bars.'''
    minDiams= 50
    
    def __init__(self, steel, diam, spacing, concreteCover):
        ''' Constructor.

        :param steel: reinforcing steel material.
        :param diam: diameter of the bars.
        :param spacing: spacing of the bars.
        :param concreteCover: concrete cover of the bars.
        '''
        super(RebarFamily,self).__init__(diam= diam, spacing= spacing, concreteCover= concreteCover, steel= steel)
      
    def __repr__(self):
        return self.steel.materialName + ", diam: " + str(int(self.diam*1e3)) + " mm, e= " + str(int(self.spacing*1e3))+' mm'
    
    def getCopy(self):
        ''' Virtual constructor.'''
        return RebarFamily(steel= self.steel, diam= self.diam, spacing= self.spacing, concreteCover= self.concreteCover)
            
    def getMR(self,concrete,b,thickness, z= None):
        '''Return the bending resistance of the (b x thickness) rectangular section.

        :param concrete: concrete material.
        :param b: width of the rectangular section.
        :param thickness: height of the rectangular section.
        :param z: inner lever arm (if None z= 0.9*d).
        '''
        return ng_simple_bending_reinforcement.Mu(As= self.getAs(width= b), fcd= concrete.fcd(), fsd= self.steel.fyd(), b= b, d= self.d(thickness), z= z, c_depth= concrete.getCDepth())
      
    def d(self, thickness):
        ''' Return the effective depth of the reinforcement.

        :param thickness: height of the rectangular section.
        '''
        return thickness-self.getEffectiveCover()
      
    def getDefStr(self):
        ''' Return definition strings for drawSchema.'''
        return ("  $\\phi$ "+ fmt.Diam.format(self.getDiam()*1000) + " mm, e= "+ fmt.Diam.format(self.spacing*1e2)+ " cm")
    
    def getDefStrings(self):
        ''' Return definition strings for drawSchema.'''
        retval= []
        retval.append(self.getDefStr())
        retval.append(" - ")
        return retval
    
    def writeDef(self,outputFile,concrete):
        outputFile.write("  diam: "+ fmt.Diam.format(self.getDiam()*1000) + " mm, spacing: "+ fmt.Diam.format(self.spacing*1e3)+ " mm")
        reinfDevelopment= self.getBasicAnchorageLength(concrete)
        outputFile.write("  reinf. development L="+ fmt.Length.format(reinfDevelopment) + " m ("+ fmt.Diam.format(reinfDevelopment/self.getDiam())+ " diameters).\\\\\n")
    
    def writeRebars(self, outputFile, concrete, AsMin):
        '''Write rebar family data.
 
        :param outputFile: output file.
        :param concrete: concrete object.
        :param AsMin: minimum required reinforcement area.
        '''
        self.writeDef(outputFile,concrete)
        outputFile.write("  area: As= "+ fmt.Area.format(self.getAs()*1e4) + " cm2/m areaMin: " + fmt.Area.format(AsMin*1e4) + " cm2/m")
        writeF(outputFile,"  F(As)", self.getAs()/AsMin)
        
class FamNBars(RebarFamily):
    ''' Family of "n" rebars.

    :ivar n: number of rebars.
    '''
    n= 2 #Number of bars.
    def __init__(self, steel, n, diam, spacing, concreteCover):
        ''' Constructor.

        :param steel: reinforcing steel material.
        :param n: number of bars.
        :param diam: diameter of the bars.
        :param spacing: spacing of the bars.
        :param concreteCover: concrete cover of the bars.
        '''
        super(FamNBars, self).__init__(steel= steel, diam= diam, spacing= spacing, concreteCover= concreteCover)
        self.n= int(n) # number of bars
        
    def __repr__(self):
        retval= super(FamNBars, self).__repr__()
        return str(self.n) + " x " + retval
  
    def getNumBars(self):
        ''' Return the number of bars in the length argument.

        :param width: width of the reinforcement.
        :param roundUp: if true return an integer number of bars greater or
                        equal to (width/spacing).
        '''
        return self.n

    def getWidth(self):
        ''' Return the width occupied by the rebar family.'''
        return (self.n-1)*self.spacing()
    
    def getAs(self):
        ''' Return the total area of the bars.
        '''
        return self.getNumBars()*self.getBarArea()
    
    def getT(self):
        ''' Return the design value of the ultimate tension force in 
            the reinforcement.
        '''
        return self.getAs()*self.steel.fyd()
    
    def writeDef(self,outputFile,concrete):
      outputFile.write("  n= "+str(self.n)+" diam: "+ fmt.Diam.format(self.getDiam()*1000) + " mm, spacing: "+ fmt.Diam.format(self.spacing*1e3)+ " mm")
      reinfDevelopment= self.getBasicAnchorageLength(concrete)
      outputFile.write("  reinf. development L="+ fmt.Length.format(reinfDevelopment) + " m ("+ fmt.Diam.format(reinfDevelopment/self.getDiam())+ " diameters).\\\\\n")

class RebarArrangement(object):
    ''' rebar arrangement (number of rebars, spacing and width).

    :ivar numberOfRebars: number of rebars.
    :ivar spacing: distance between bars.
    :ivar width: total length occupied by the bar set.
    '''
    def __init__(self, numberOfRebars= None, spacing= None, width= None):
        ''' Construct the rebar arrangement (number of rebars, spacing 
            and width) from any two of the three parameters.

        :param numberOfRebars: number of rebars.
        :param spacing: distance between bars.
        :param width: total length occupied by the bar set.
        '''
        if(numberOfRebars is None):
            if((spacing is None) or (width is None)):
                methodName= sys._getframe(0).f_code.co_name
                lmsg.error(methodName+'; spacing and width values needed.')
            else:
                self.spacing= spacing
                self.width= width
                self.numberOfRebars= int(self.width/self.spacing)+1
        elif(spacing is None):
            if((numberOfRebars is None) or (width is None)):
                methodName= sys._getframe(0).f_code.co_name
                lmsg.error(methodName+'; number of rebars and width values needed.')
            else:
                self.numberOfRebars= numberOfRebars
                self.width= width
                self.spacing= self.width/(self.numberOfRebars-1)
        elif(width is None):
            self.numberOfRebars= numberOfRebars
            self.spacing= spacing
            self.width= (self.numberOfRebars-1)*self.spacing
        else:
            methodName= sys._getframe(0).f_code.co_name
            lmsg.error(methodName+'; one of the values (number of rebars, spacing or width) is supposed to be unknown (None).')

    def getNBarsFamily(self, steel, diam, concreteCover):
        ''' Return a family of "n" rebars.

        :param steel: reinforcing steel material.
        :param diam: diameter of the bars.
        :param concreteCover: concrete cover of the bars.
        '''
        return FamNBars(steel= steel, n= self.numberOfRebars, diam= diam, spacing= self.spacing, concreteCover= concreteCover)

class DoubleRebarFamily(object):
    ''' Two reinforcement bars families.

    :ivar f1: first rebar family.
    :ivar f2: second rebar family.
    '''
    def __init__(self,f1,f2):
        ''' Constructor.

        :param f1: first rebar family.
        :param f2: second rebar family.
        '''
        self.f1= f1
        self.f2= f2
    def __repr__(self):
        return self.f1.__repr__() + " + " + self.f2.__repr__()
    def getCopy(self):
        return DoubleRebarFamily(self.f1.getCopy(),self.f2.getCopy())
    def getAs(self, width= 1.0):
      ''' Return the total area of the bars.

      :param width: width of the reinforcement.
      '''
      return self.f1.getAs(width= width)+self.f2.getAs(width= width)
  
    def getSpacing(self, width= 1.0):
        ''' Return the average spacing of the bars.

        :param width: width of the reinforcement.
        '''
        n1= self.f1.getAs(width= width)/self.f1.getBarArea()
        n2= self.f2.getAs(width= width)/self.f2.getBarArea()
        return 1/(n1+n2)
    def getEffectiveCover(self):
        ''' returns the effective cover of the rebar family.

        Returns the distance between the surface of the concrete and the 
        centroid of the rebars family.
        '''
        T1= self.f1.getT()
        T2= self.f2.getT()
        T= T1+T2
        return (self.f1.getEffectiveCover()*T1+self.f2.getEffectiveCover()*T2)/T
    def getBasicAnchorageLength(self,concrete):
        ''' Return the basic anchorage length of the bars.

        :param concrete: concrete material.
        '''
        l1= self.f1.getBasicAnchorageLength(concrete)
        l2= self.f2.getBasicAnchorageLength(concrete)
        return max(l1,l2)
    
    def getMinReinfAreaInBending(self, concrete, thickness, memberType):
        '''Return the minimun amount of bonded reinforcement to control cracking
           for reinforced concrete sections under bending.

        :param concrete: concrete material.
        :param thickness: thickness of the bended member.
        :param memberType: type of member (slab, wall,...).
        '''
        retval= self.f1.getMinReinfAreaInBending(concrete= concrete,thickness= thickness, memberType= memberType)
        return retval
    
    def getMinReinfAreaInTension(self, concrete, thickness, memberType):
        '''Return the minimun amount of bonded reinforcement to control cracking
           for reinforced concrete sections under tension.

        :param concrete: concrete material.
        :param thickness: thickness of the tensioned member.
        :param memberType: type of member (slab, wall,...).
        '''
        return self.f1.getMinReinfAreaInTension(concrete= concrete, thickness= thickness, memberType= memberType)
    
    def getMR(self, concrete, b, thickness, z= None):
        '''Return the bending resistance of the (b x thickness) rectangular section.

        :param concrete: concrete material.
        :param b: width of the rectangular section.
        :param thickness: height of the rectangular section.
        :param z: inner lever arm (if None z= 0.9*d).
        '''
        className= type(self).__name__
        methodName= sys._getframe(0).f_code.co_name
        lmsg.error(className+'.'+methodName+'; not implemented yet.\n')
        MR1= self.f1.getMR(concrete= concrete, b= b, thickness= thickness, z= z)
        MR2= self.f2.getMR(concrete= concrete, b= b, thickness= thickness, z= z)
        return max(MR1,MR2)
    
    def d(self,thickness):
        return thickness-self.getEffectiveCover()

    def getDefStrings(self):
        ''' Return definition strings for drawSchema.'''
        retval= []
        retval.append(self.f1.getDefStr())
        retval.append(self.f2.getDefStr())
        return retval

    def writeDef(self,outputFile,concrete):
        self.f1.writeDef(outputFile,concrete)
        self.f2.writeDef(outputFile,concrete)
        
    def writeRebars(self, outputFile, concrete, AsMin):
        '''Write rebar family data.

        :param outputFile: output file.
        :param concrete: concrete material.
        :param AsMin: minimum amount of reinforcement.
        '''
        self.writeDef(outputFile,concrete)
        outputFile.write("  area: As= "+ fmt.Area.format(self.getAs()*1e4) + " cm2/m areaMin: " + fmt.Area.format(AsMin*1e4) + " cm2/m")
        writeF(outputFile,"  F(As)", self.getAs()/AsMin)

def writeF(outputFile,text,F):
    fmt= "{:4.2f}"
    if(F>1):
        outputFile.write(text+ "= "+ fmt.format(F)+ " OK!\\\\\n")
    elif(F>=0.95):
        outputFile.write(text+ "= "+ fmt.format(F)+ " $\\sim$ OK!\\\\\n")
    else:
        outputFile.write(text+ "= "+ fmt.format(F)+ " Error!\\\\\n")
