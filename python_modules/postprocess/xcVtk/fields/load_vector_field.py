# -*- coding: utf-8 -*-
''' Loads represented as vectors. '''

from __future__ import division
from __future__ import print_function

__author__= "Luis C. Pérez Tato (LCPT) , Ana Ortega (AO_O) "
__copyright__= "Copyright 2018, LCPT, AO_O"
__license__= "GPL"
__version__= "3.0"
__email__= "l.pereztato@ciccp.es, ana.ortega@ciccp.es "

import sys
import xc
from misc_utils import log_messages as lmsg
from postprocess.xcVtk.fields import vector_field as vf

class LoadOnPoints(vf.VectorField):
    '''Draws punctual loads.'''
    def __init__(self,loadPatternName,fUnitConv= 1e-3,scaleFactor= 1.0,showPushing= True,components= [0,1,2]):
        '''
        Parameters:
          loadPatternName: name of the load pattern to display.
          fUnitConv: unit conversion factor.
          scaleFactor: scale factor for the size of the vectors.
          showPushing: true if the loads push the loaded point (as oppssed to pull). Default: True
          components: index of the components of the load. Default: [0,1,2] 
        '''
        super(LoadOnPoints,self).__init__(loadPatternName,fUnitConv,scaleFactor,showPushing)
        self.components= components

    def getMaxLoad(self):
        ''' Calculate the maximum absolute value of the loads on the
            vector container.'''
        self.data.calculateLengths(self.fUnitConv)
        rgMaxMin=self.data.lengths.GetRange()
        return abs(max(rgMaxMin, key=abs))


class LoadVectorField(LoadOnPoints):
    '''Draws a load over a point on nodes and on elements.'''
    def __init__(self,loadPatternName,setToDisp,fUnitConv= 1e-3,scaleFactor= 1.0, showPushing= True, components= [0,1,2], multiplyByElementArea= True):
        '''
        Constructor.

        :param loadPatternName: name of the load pattern to display.
        :param setToDisp: set over which to display the loads.
        :param fUnitConv: unit conversion factor.
        :param scaleFactor: scale factor for the size of the vectors.
        :param showPushing: true if the loads push the loaded point (as oppssed to pull). Default: True
        :param components: index of the components of the load. Default: [0,1,2] 
        :param multiplyByElementArea: for loads over elements (default= True).
        '''
        super(LoadVectorField,self).__init__(loadPatternName, fUnitConv, scaleFactor, showPushing, components)
        self.multiplyByElementArea= multiplyByElementArea
        self.setToDisp=setToDisp

    def sumElementalUniformLoads(self, actLP):
        ''' Iterate over active load patterns and cumulate on elements 
            their elemental uniform loads.

        :param actLP: list of active load patterns.
        '''
        retval= dict()
        comp_i= self.components[0]; comp_j= self.components[1]; comp_k= self.components[2]
        for lp in actLP:
            lIter= lp.loads.getElementalLoadIter
            preprocessor= lp.getDomain.getPreprocessor
            elementLoad= lIter.next()
            eTagsSet=self.setToDisp.getElements.getTags()
            while(elementLoad):
                category= elementLoad.category
                if(category=='uniform' or category=='raw'):
                    if hasattr(elementLoad,'getLocalForce'):
                        tags= elementLoad.elementTags
                        for i in range(0,len(tags)):
                          eTag= tags[i]
                          if eTag in eTagsSet:
                              elem= preprocessor.getElementHandler.getElement(eTag)
                              if(elem.getDimension==2):
                                  vLoad= elem.getCoordTransf.getVectorGlobalCoordFromLocal(elementLoad.getLocalForce())
                                  if(category=='raw'): # Those loads return the total load over the element.
                                      vLoad*= (1.0/elem.getArea(True))
                                  if(self.multiplyByElementArea):
                                      vLoad*= elem.getArea(True)
                                  v= xc.Vector([vLoad[comp_i],vLoad[comp_j],vLoad[comp_k]])
                                  if eTag in retval:
                                      retval[eTag]+= v
                                  else:
                                      retval[eTag]= v
                elif(category=='punctual'):
                    # Concentrated load must be treated elsewhere
                    className= type(self).__name__
                    methodName= sys._getframe(0).f_code.co_name
                    lmsg.warning(className+'.'+methodName+'; display of concentrated loads not implemented yet.')
                else:
                    className= type(self).__name__
                    methodName= sys._getframe(0).f_code.co_name
                    lmsg.warning(className+'.'+methodName+'; category: '+str(category)+' unknown.')
                elementLoad= lIter.next()
        return retval

    def populateWithElementalLoads(self, actLP):
        ''' Populate the vector container with elemental loads 
            from the load pattern argument.

        :param actLP: list of active load patterns.
        '''
        self.elementalLoadVectors= self.sumElementalUniformLoads(actLP)
        preprocessor= actLP[0].getDomain.getPreprocessor
        for eTag in self.elementalLoadVectors.keys():
            elem= preprocessor.getElementHandler.getElement(eTag)
            if(elem.getDimension==2):
                vLoad= self.elementalLoadVectors[eTag]
                self.data.insertNextVector(vLoad[0],vLoad[1],vLoad[2])
            else:
                lmsg.warning('displaying of loads over 1D elements not yet implemented')
        return len(self.elementalLoadVectors)

    def dumpElementalPositions(self,preprocessor):
        ''' Iterate over cumulated loads dumping them into the graphic.

        '''
        for eTag in self.elementalLoadVectors.keys():
            elem= preprocessor.getElementHandler.getElement(eTag)
            if(elem.getDimension==2):
                vLoad= self.elementalLoadVectors[eTag]
                p= elem.getPosCentroid(True)
                self.data.insertNextPair(p.x,p.y,p.z,vLoad[0],vLoad[1],vLoad[2],self.fUnitConv,self.showPushing)
            else:
                lmsg.warning('displaying of loads over 1D elements not yet implemented')
        return len(self.elementalLoadVectors)

    def sumNodalLoads(self,actLP):
        ''' Iterate over loaded nodes to cumulate their loads.

        :param actLP: list of active load patterns
        '''
        retval= dict()
        dim= len(self.components)
        comp_i= None; comp_j= None; comp_k= None
        if(dim>1):
            comp_i= self.components[0];
            comp_j= self.components[1];
            comp_k= self.components[2]
        else:
            comp_k= self.components[0]
        for lp in actLP:
            lIter= lp.loads.getNodalLoadIter
            nl= lIter.next()
            nTagsSet=self.setToDisp.getNodes.getTags()
            preprocessor= lp.getDomain.getPreprocessor
            while(nl):
                nTag= nl.getNodeTag
                if nTag in nTagsSet:
                    node= preprocessor.getNodeHandler.getNode(nTag)
                    if(__debug__):
                        if(not node):
                            AssertionError('Can\'t create the node.')
                    vLoad= nl.getLoadVector
                    if(dim>1):
                        v= xc.Vector([vLoad[comp_i], vLoad[comp_j], vLoad[comp_k]])
                    else:
                        v= xc.Vector([0.0, 0.0, vLoad[comp_k]])
                    if(v.Norm()>1e-6):
                        if nTag in retval:
                            retval[nTag]+= v
                        else:
                            retval[nTag]= v
                nl= lIter.next()
        return retval

    def populateWithNodalLoads(self, actLP):
        ''' Populate the vector container with nodal loads 
            from the load pattern argument.

        :param actLP: list of active load patterns.
        '''
        self.nodalLoadVectors= self.sumNodalLoads(actLP)
        for nTag in self.nodalLoadVectors.keys():
            vLoad= self.nodalLoadVectors[nTag]
            self.data.insertNextVector(vLoad[0],vLoad[1],vLoad[2])
        return len(self.nodalLoadVectors)

    def dumpNodalPositions(self, preprocessor, defFScale):
        ''' Iterate over loaded nodes dumping its loads into the graphic.

        :param defFScale: factor to apply to current displacement of nodes 
                  so that the display position of each node equals to
                  the initial position plus its displacement multiplied
                  by this factor.    
        '''
        for nTag in self.nodalLoadVectors.keys():
            node= preprocessor.getNodeHandler.getNode(nTag)
            p= node.getCurrentPos3d(defFScale)
            vLoad= self.nodalLoadVectors[nTag]
            self.data.insertNextPair(p.x,p.y,p.z,vLoad[0],vLoad[1],vLoad[2],self.fUnitConv,self.showPushing)
        return len(self.nodalLoadVectors)

    def populateLoads(self, actLP, showElementalLoads= True, showNodalLoads= True):
        ''' Populate the vector container with loads from the active load patterns.

        :param actLP: list of active load patterns.
        :param showElementalLoads: True (default) to add element loads to vector container.
        :param showNodalLoads: True (default) to add nodal loads to vector container. 
        '''
        numberOfLoads= 0
        if(showElementalLoads):
            numberOfLoads+= self.populateWithElementalLoads(actLP)
        if(showNodalLoads):
            numberOfLoads+= self.populateWithNodalLoads(actLP)
        return numberOfLoads

    def dumpVectors(self, preprocessor, defFScale, showElementalLoads= True, showNodalLoads= True):
        ''' Iterate over loads dumping them into the graphic.

        :param preprocessor: preprocessor of the FE problem.
        :param defFScale: factor to apply to current displacement of nodes 
                  so that the display position of each node equals to
                  the initial position plus its displacement multiplied
                  by this factor.
        :param showElementalLoads: if true show loads over elements.
        :param showNodalLoads: if true show loads over nodes.
        '''
        count= 0
        activeLoadPatterns= preprocessor.getDomain.getConstraints.getLoadPatterns
        if(len(activeLoadPatterns)<1):
            lmsg.warning('No active load patterns.')
        else:
            actLP=[lp.data() for lp in activeLoadPatterns]
            numberOfLoads= self.populateLoads(actLP, showElementalLoads, showNodalLoads)
            if(numberOfLoads>0):
                maxLoad= self.getMaxLoad()
                if(maxLoad!= 0):
                    self.data.scaleFactor/= self.getMaxLoad()
                #Iterate over loaded elements.
                if showElementalLoads:
                    count+= self.dumpElementalPositions(preprocessor)
                #Iterate over loaded nodes.
                if showNodalLoads:
                    count+= self.dumpNodalPositions(preprocessor, defFScale)
                if(count==0):
                    lmsg.warning('LoadVectorField.dumpVectors: no loads defined.')
        return count

    def dumpNodalLoads(self, preprocessor, defFScale):
        ''' Iterate over nodal loads dumping them into the graphic.

        :param preprocessor: preprocessor of the FE problem.
        :param defFScale: factor to apply to current displacement of nodes 
                  so that the display position of each node equals to
                  the initial position plus its displacement multiplied
                  by this factor.
        '''
        return self.dumpVectors(preprocessor, defFScale,showElementalLoads=False,showNodalLoads=True)
    
    def dumpElementalLoads(self, preprocessor, defFScale):
            ''' Iterate over nodal loads dumping them into the graphic.

            :param preprocessor: preprocessor of the FE problem.
            :param defFScale: factor to apply to current displacement of nodes 
                      so that the display position of each node equals to
                      the initial position plus its displacement multiplied
                      by this factor.
            '''
            return self.dumpVectors(preprocessor, defFScale, showElementalLoads=True, showNodalLoads=False)
    

