# -*- coding: utf-8 -*-
''' Base classes and function for structural steel.'''

from __future__ import division

__author__= "Ana Ortega (AO_O) and Luis C. Pérez Tato (LCPT)"
__copyright__= "Copyright 2015, AO_O and LCPT"
__license__= "GPL"
__version__= "3.0"
__email__= " ana.Ortega.Ort@gmail.com, l.pereztato@gmail.com"

from materials import typical_materials
#from misc_utils import log_messages as lmsg

class BasicSteel(typical_materials.BasicElasticMaterial):
    '''Base class for structural steels.

      :ivar fy:  yield stress.
      :ivar fu: ultimate stress.
      :ivar gammaM: material partial safety factor.
    '''
    
    alpha= 1.2e-5 # coefficient of thermal expansion (1/ºC)
    
    def __init__(self, E, nu, fy, fu, gammaM):
        ''' Constructor.

        :param E:  elastic modulus.
        :param nu:  Poisson's ratio.
        :param fy:  yield stress.
        :param fu: ultimate stress.
        :param gammaM: material partial safety factor.
        '''
        super(BasicSteel,self).__init__(E,nu,rho=7850)
        self.fy= fy
        self.fu= fu
        self.gammaM= gammaM

    def fyd(self):
        ''' Return the design value of the yield strength.'''
        return self.fy/self.gammaM

    def fmaxk(self):
        ''' Characteristic ultimate strength. This method is added for
            compatibility with ReinforcingSteel material as defined
            in concrete base.'''
        return self.fu
  
    def getDict(self):
        ''' Returns a dictionary whith the values of the internal forces.
            Makes easier export it to json.'''
        retval= super(BasicSteel,self).getDict()
        retval.update({'fy':self.fy,'fu':self.fu,'gammaM':self.gammaM})
        return retval

    def setFromDict(self,dct):
        '''Sets the internal forces from the dictionary argument.'''
        super(BasicSteel,self).setFromDict(dct)
        self.fy= dct['fy']
        self.fu= dct['fu']
        self.gammaM= dct['gammaM']

    def defJ2PlateFibre(self, preprocessor, name, alpha= .05):
        '''Constructs a J2 (Von Mises) material with a linear-strain
           hardening rule appropriate for plate analysis.

        :param  preprocessor: preprocessor of the finite element problem.
        :param  name:         name identifying the material.
        :param  alpha:        strain hardening ratio (default: (0.01), 
                              range: 0 to 1).
        '''
        return typical_materials.defJ2PlateFibre(preprocessor, name= name, E= self.E, nu= self.nu, fy= self.fy, alpha=alpha, rho= self.rho)
       
