# -*- coding: utf-8 -*-

__author__= "Luis C. Pérez Tato (LCPT)"
__copyright__= "Copyright 2016, LCPT"
__license__= "GPL"
__version__= "3.0"
__email__= "l.pereztato@gmail.com"

from postprocess.reports import common_formats as fmt
from misc_utils import log_messages as lmsg
from materials.sections import rebar_family


class RCSection(object):
    '''Reinforced concrete section.

    :ivar b: section width.
    :ivar h: section depth.
    :ivar concrete: concrete material.
    :ivar tensionRebars: main reinforcement.
    '''
    def __init__(self, tensionRebars, concrete, b, h):
        ''' Constructor.

        :param b: section width.
        :param h: section depth.
        :param concrete: concrete material.
        :param tensionRebars: main reinforcement.
        '''
        self.tensionRebars= tensionRebars
        self.concrete= concrete
        self.b= b
        self.h= h
        self.stressLimitUnderPermanentLoads= 230e6

    def d(self):
        ''' Return the effective depth of the reinforcement.

        :param thickness: height of the rectangular section.
        '''
        return self.tensionRebars.d(self.h)

    def getAc(self):
        ''' Return the concrete area.'''
        return self.b*self.h-self.tensionRebars.getAs()

    def getDimensionlessMomentRatio(self, MEd):
        ''' Return the ratio: MEd/(fcd*b*d^2).'''
        return -MEd/self.concrete.fcd()/self.b/self.d()**2

    def getDimensionlessReinforcementRatio(self):
        ''' Return the ratio: As*fyd/(Ac*fcd).'''
        fcd= -self.concrete.fcd()
        return self.tensionRebars.getT()/self.getAc()/fcd

    def setReinforcement(self,tensionRebars):
        self.tensionRebars= tensionRebars
        
    def getMinReinfAreaUnderFlexion(self):
        return self.tensionRebars.getMinReinfAreaUnderFlexion(concrete= self.concrete, thickness= self.h)
    
    def getMinReinfAreaUnderTension(self):
        return self.tensionRebars.getMinReinfAreaUnderTension(concrete= self.concrete,thickness= self.h)
    
    def getMR(self, z= None):
        ''' Return the resisting moment of the section.

        :param z: inner lever arm.
        '''
        return self.tensionRebars.getMR(self.concrete,self.b,self.h)
    
    def getVR(self,Nd,Md):
        return self.tensionRebars.getVR(self.concrete,Nd,Md,self.b,self.h)
    
    def writeResultFlexion(self,outputFile,Nd,Md,Vd):
        AsMin= self.getMinReinfAreaUnderFlexion()
        outputFile.write("  RC section dimensions; b= "+ fmt.Length.format(self.b)+ " m, h= "+ fmt.Length.format(self.h)+ " m\\\\\n")
        self.tensionRebars.writeRebars(outputFile, self.concrete, AsMin)
        if(abs(Md)>0):
            MR= self.getMR()
            outputFile.write("  Bending check: Md= "+ fmt.Esf.format(Md/1e3)+ " kN m, MR= "+ fmt.Esf.format(MR/1e3)+ "kN m")
            rebar_family.writeF(outputFile,"  F(M)", MR/Md)
        if(abs(Vd)>0):
            VR= self.getVR(Nd,Md)
            outputFile.write("  Shear check: Vd= "+ fmt.Esf.format(Vd/1e3)+ " kN,  VR= "+ fmt.Esf.format(VR/1e3)+ " kN")
            rebar_family.writeF(outputFile,"  F(V)",VR/Vd)
          
    def writeResultTraction(self,outputFile,Nd):
        AsMin= self.getMinReinfAreaUnderTension()/2.0
        self.tensionRebars.writeRebars(outputFile,self.concrete,AsMin)
        if(abs(Nd)>0):
          lmsg.error("ERROR; tension not implemented.")
          
    def writeResultCompression(self,outputFile,Nd,AsTrsv):
        ''' Results for compressed rebars.

        :param AsTrsv: Rebar area in transverse direction.
         '''
        AsMin= 0.2*AsTrsv # 20% of the transversal area.
        self.tensionRebars.writeRebars(outputFile,self.concrete,AsMin)
        if(abs(Nd)!=0.0):
          lmsg.error("ERROR; not implemented.")

    def writeResultStress(self,outputFile,M):
      '''Cheking of stresses under permanent loads (SIA 262 fig. 31)'''
      if(abs(M)>0):
          stress= M/(0.9*self.h*self.tensionRebars.getAs())
          outputFile.write("  Stress check: M= "+ fmt.Esf.format(M/1e3)+ " kN m, $\sigma_s$= "+ fmt.Esf.format(stress/1e6)+ " MPa\\\\\n")
          outputFile.write("    $\sigma_{lim}$= "+ fmt.Esf.format(self.stressLimitUnderPermanentLoads/1e6)+ " MPa")
          rebar_family.writeF(outputFile,"  F($\sigma_s$)", self.stressLimitUnderPermanentLoads/stress)
