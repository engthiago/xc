# -*- coding: utf-8 -*-
''' Generic train load model.'''

from __future__ import division
from __future__ import print_function

__author__= "Luis C. Pérez Tato (LCPT) Ana Ortega (AO_O)"
__copyright__= "Copyright 2023,  LCPT AO_O "
__license__= "GPL"
__version__= "3.0"
__email__= "l.pereztato@gmail.com ana.ortega.ort@gmail.com"

import sys
import geom
import xc
from misc_utils import log_messages as lmsg
from actions.railway_traffic import locomotive_load

class TrainLoadModel(object):
    ''' Model for the loads of a train.

    :ivar locomotive: locomotive model.
    :ivar uniformLoad: uniform load on the track.
    :ivar rollingStockHeight: height of the rolling stock for wind calculations
                              (see clause 8.3.1 paragraph 5 letter b) of 
                               Eurocode 1 part 1-4. Wind actions).
    '''
    def __init__(self, locomotive, uniformLoad, dynamicFactor, classificationFactor= 1.21, h= 1.8, rollingStockHeight= 4.0):
        ''' Constructor:

        :param locomotive: locomotive model.
        :param uniformLoad: uniform load on the track.

        :param dynamicFactor: dynamic factor.
        :param classificationFactor: classification factor (on lines carrying
                                     rail traffic which is heavier or lighter
                                     than normal rail traffic).
        :param h: height of the center of gravity.
        :param rollingStockHeight: height of the rolling stock for wind 
                                   calculations (see clause 8.3.1 paragraph 5 
                                   letter b) of Eurocode 1 part 1-4. Wind 
                                   actions).
        '''
        self.locomotive= locomotive
        # The dynamic factor is stored in the locomotive.
        self.locomotive.setDynamicFactor(dynamicFactor)
        self.locomotive.setClassificationFactor(classificationFactor)
        self.uniformLoad= uniformLoad
        self.h= h
        self.rollingStockHeight= rollingStockHeight

    def getDynamicFactor(self):
        ''' Return the dynamic factor.'''
        return self.locomotive.dynamicFactor

    def getClassificationFactor(self):
        ''' Return the classification factor.'''
        return self.locomotive.classificationFactor

    def getRailUniformLoad(self):
        ''' Return the raw uniform load.'''
        return self.uniformLoad/2.0

    def getClassifiedUniformLoad(self):
        ''' Return the uniform load affected by the classification factor.'''
        return self.getClassificationFactor()*self.uniformLoad
        
    def getDynamicUniformLoad(self):
        ''' Return the uniform load affected by the dynamic factor.'''
        return self.getClassificationFactor()*self.getDynamicFactor()*self.uniformLoad
    
    def getWheelLoads(self, ref, loadFactor= 1.0, directionVector= xc.Vector([0,0,-1])):
        ''' Return the loads of the wheels of the locomotive along with its
            positions.

        :param ref: reference system at the center of the locomotive.
        :param loadFactor: factor to apply to the loads.
        '''
        return self.locomotive.getWheelLoads(ref= ref, loadFactor= loadFactor, loadDirectionVector= directionVector)

    def getTotalLoad(self, trackLength):
        ''' Return the total load of the train over the given length.

        :param trackLength: length of the track that supports the train.
        '''
        locomotiveLoad= self.locomotive.getTotalLoad()
        uniformLoadedLength= trackLength-self.locomotive.getTotalLength()
        uniformLoad= self.getDynamicUniformLoad()*uniformLoadedLength
        return locomotiveLoad+uniformLoad  
