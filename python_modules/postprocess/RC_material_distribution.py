# -*- coding: utf-8 -*-

from __future__ import print_function
from __future__ import division

import sys
from postprocess import element_section_map
import pickle
from misc_utils import log_messages as lmsg
import xc
# Macros
from solution import predefined_solutions
from postprocess import phantom_model as phm
from materials.sections import RC_sections_container as sc
from model.sets import sets_mng as sUtils
from postprocess import limit_state_data as lsd

__author__= "Luis C. Pérez Tato (LCPT) and Ana Ortega (AO_O)"
__copyright__= "Copyright 2016, LCPT and AO_O"
__license__= "GPL"
__version__= "3.0"
__email__= "l.pereztato@ciccp.es" "ana.Ortega@ciccp.es"

class RCMaterialDistribution(object):
    '''Spatial distribution of reinforced concrete material (RC sections 
       distribution over the elements).
    It refers to the reinforced concrete sections associated with the 
    element (i.e. for shell elements we typically define two RC sections, 
    one for each main direction; in the case of beam elements the most 
    common way is to define RC sections in the front and back ends 
    of the elements)

    :ivar sectionDefinition: Container with the section definitions
                             (see RC_sections_container module).
    :ivar sectionDistribution: dictionary that stores a section name(s)
                               for each element number. This way it defines
                               a spatial distribution of the sections over
                               the elements.

    :ivar elementSetNames: list of element sets with an assigned section.
    '''
    mapSectionsFileName= './mapSectionsReinforcement.pkl'

    def __init__(self):
        '''Constructor.

        '''
        self.sectionDefinition= sc.SectionContainer()
        self.sectionDistribution= element_section_map.ElementSectionMap()
        self.elementSetNames= list() #Elements sets with an assigned section.

    def assign(self,elemSet,setRCSects):
        '''Assigns the sections names to the elements of the set.

           :param elemSet: set of elements that receive the section name property.
           :param setRCSects: RC section definition, name, concrete type,
                              rebar positions,...
        '''
        self.sectionDistribution.assign(elemSet,setRCSects)
        self.elementSetNames.append(elemSet.owner.name)

    def assignFromElementProperties(self, elemSet):
        '''Creates the section materials from the element properties
           and assigns them to the elements of the argument set .

           :param elemSet: set of elements that receive the section names 
                           property.
        '''
        self.sectionDefinition= self.sectionDistribution.assignFromElementProperties(elemSet= elemSet, sectionWrapperName= elemSet.owner.name)
        self.elementSetNames.append(elemSet.owner.name)
        
    def getElementSet(self,preprocessor):
        '''Returns an XC set that contains all the elements with an
           assigned section.'''
        retvalName= ''
        for name in self.elementSetNames:
            retvalName+= '|'+name
        retval= preprocessor.getSets.defSet(retvalName)
        sets= list()
        for name in self.elementSetNames:
            sets.append(preprocessor.getSets.getSet(name))
        sUtils.append_sets(retval,sets)
        return retval

    def getSectionNamesForElement(self,tagElem):
        '''Returns the section names for the element which tag is being passed
           as a parameter.'''
        if tagElem in self.sectionDistribution.keys():
            return self.sectionDistribution[tagElem]
        else:
            className= type(self).__name__
            methodName= sys._getframe(0).f_code.co_name
            lmsg.warning(className+'.'+methodName+'; element with tag: '+str(tagElem)+' not found.')
            return None

    def getSectionDefinition(self,sectionName):
        '''Returns the section definition which has the name being passed
           as a parameter.''' 
        return self.sectionDefinition[sectionName]

    def getSectionDefinitionsForElement(self,tagElem):
        '''Returns the section names for the element which tag is being passed
           as a parameter.'''
        retval= []
        sectionNames= self.getSectionNamesForElement(tagElem)
        if(sectionNames):
            for s in sectionNames:
                retval.append(self.sectionDefinition.mapSections[s])
        else:
            lmsg.error("section names for element with tag: "+str(tagElem)+" not found.")
        return retval

    def dump(self):
        '''Writes this object in a pickle file.'''
        with open(self.mapSectionsFileName, 'wb') as f:
            pickle.dump(self, f, pickle.HIGHEST_PROTOCOL)

    def load(self):
        '''Reads this object from a pickle file.'''
        with open(self.mapSectionsFileName, 'rb') as f:
            self.sectionDistribution= pickle.load(f)
            self.sectionDefinition= pickle.load(f)
        f.close()

    def runChecking(self, limitStateData, matDiagType, threeDim= True, outputCfg= lsd.VerifOutVars()):
        '''Creates the phantom model and runs the verification on it.

        :param limitStateData: object that contains the name of the file
                               containing the internal forces 
                               obtained for each element 
                               for the combinations analyzed and the
                               controller to use for the checking.
        :param matDiagType: type of the material diagram (d: design, 
               k: characteristic).
        :param threeDim: true if it's 3D (Fx,Fy,Fz,Mx,My,Mz) 
               false if it's 2D (Fx,Fy,Mz).
        :param outputCfg: instance of class 'VerifOutVars' which defines the 
                   variables that control the output of the checking (set of 
                   elements to be analyzed, append or not the results to a file,
                   generation or not of lists, ...)
        '''
        feProblem= xc.FEProblem()
        preprocessor= feProblem.getPreprocessor
        if 'straight' in str(outputCfg.controller).lower():
             for s in self.sectionDefinition.sections:
                 s.fiberSectionParameters.concrType.initTensStiff='Y'
        self.sectionDefinition.createRCsections(preprocessor,matDiagType) #creates
                          #for each element in the container the fiber sections
                          #(RCsimpleSections) associated with it.
        if(threeDim):
            self.sectionDefinition.calcInteractionDiagrams(preprocessor,matDiagType)
        else:
            self.sectionDefinition.calcInteractionDiagrams(preprocessor,matDiagType,'NMy')
        outputCfg.controller.solutionProcedure= outputCfg.controller.solutionProcedureType(feProblem)
        phantomModel= phm.PhantomModel(preprocessor,self)
        result= phantomModel.runChecking(limitStateData,outputCfg)
        return (feProblem, result)

    def internalForcesVerification3D(self, limitStateData, matDiagType, outputCfg):
        '''Limit state verification based on internal force (Fx,Fy,Fz,Mx,My,Mz) values.

        :param limitStateData: object that contains the name of the file
                               containing the internal forces 
                               obtained for each element 
                               for the combinations analyzed and the
                               controller to use for the checking.
        :param matDiagType: type of the material diagram (d: design, k: characteristic).
        :param outputCfg: instance of class 'VerifOutVars' which defines the 
                   variables that control the output of the checking (set of 
                   elements to be analyzed, append or not the results to a file,
                   generation or not of lists, ...)
        '''
        (tmp, retval)= self.runChecking(limitStateData= limitStateData, matDiagType= matDiagType, threeDim= True, outputCfg= outputCfg)
        tmp.clearAll() #Free memory.
        return retval

    def internalForcesVerification2D(self, limitStateData, matDiagType, outputCfg):
        '''Limit state verification based on internal force (Fx,Fy,Mz) values.

        :param limitStateData: object that contains the name of the file
                               containing the internal forces 
                               obtained for each element 
                               for the combinations analyzed and the
                               controller to use for the checking.
        :param matDiagType: type of the material diagram (d: design, 
               k: characteristic).
        :param outputCfg: instance of class 'VerifOutVars' which defines the 
                   variables that control the output of the checking (set of 
                   elements to be analyzed, append or not the results to a file,
                   generation or not of lists, ...)
        '''
        (tmp, retval)= self.runChecking(limitStateData= limitStateData, matDiagType= matDiagType, threeDim= False, outputCfg= outputCfg)
        tmp.clearAll() #Free memory.
        return retval


def loadRCMaterialDistribution():
    '''Load the reinforced concrete sections on each element from file.'''
    with open(RCMaterialDistribution.mapSectionsFileName, 'rb') as f:
        return pickle.load(f)

