# -*- coding: utf-8 -*-
''' Plate washer model.'''

from __future__ import division
from __future__ import print_function

__author__= "Luis C. Pérez Tato (LCPT) , Ana Ortega (AO_O) "
__copyright__= "Copyright 2020, LCPT, AO_O"
__license__= "GPL"
__version__= "3.0"
__email__= "l.pereztato@ciccp.es, ana.ortega@ciccp.es "

# from misc_utils import log_messages as lmsg
from connections.steel_connections import bolts

class SquarePlateWasher(object):
    ''' Base class for plate washers. This class must be code agnostic
        i.e. no AISC, EC3, EAE clauses here.

    :ivar bolt: bolt.
    :ivar side: plate side.
    :ivar thickness: plate thickness.
    :ivar steelType: steel type.
    '''
    def __init__(self, bolt, side= None, thickness= None, steelType= None):
        ''' Constructor.

        :param bolt: bolt.
        :param side: plate side (if None compute automatically).
        :param thickness: plate thickness (defaults to nut height).
        :param steelType: steel type.
        '''
        self.bolt= bolt
        if(thickness):
            self.thickness= thickness
        else:
            self.thickness= bolt.getNut().getMaxHeight()
        if(side):
            self.side= side
        else:
            self.side= self.getMinSide()
        self.steelType= steelType            

    def getNut(self):
        ''' Return a nut object that corresponds to the bolt.'''
        return self.bolt.getNut()
        
    def getMinSide(self):
        ''' Return the minimum side of the plate to take full
            advantage of its thickness.'''
        nut= self.getNut()
        e= nut.getWidthAcrossCorners()
        return e+2.0*self.thickness

    def getBearingArea(self):
        ''' Return the bearing area of the plate washer.'''
        s= min(self.side, self.getMinSide())
        retval= s*s-self.bolt.getArea()
        return retval
    
    def getDict(self):
        ''' Put member values in a dictionary.'''
        retval= {'bolt':self.bolt.getDict()}
        retval['thickness']= self.thickness
        retval['side']= self.side
        steelTypeClassName= str(self.steelType.__class__)[8:-2]
        retval.update({'steelTypeClassName':steelTypeClassName, 'steelType':self.steelType.getDict()})
        return retval

    def setFromDict(self,dct):
        ''' Read member values from a dictionary.'''
        self.bolt= bolts.BoltBase(diameter= None)
        self.bolt.setFromDict(dct['bolt'])
        self.thickness= dct['thickness']
        self.side= dct['side']
        steelTypeClassName= dct['steelTypeClassName']+'()'
        self.steelType= eval(steelTypeClassName)
        self.steelType.setFromDict(dct['steelType'])
