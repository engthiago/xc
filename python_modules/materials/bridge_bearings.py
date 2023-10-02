# -*- coding: utf-8 -*-
'''Classes for modelling bridge bearings.

This module provides some classes that allow modelling elastomeric and pot type
bridge bearings. For now they are modelized as linear joints that connect
two nodes with springs that introduce translational and/or rotational stiffness
that are approximately equal to those of the real bearing.


Examples\:
    Examples of the use of these classes are given in the following verification tests\:
        * ./verif/tests/elements/test_elastomeric_bearing_01.py
        * ./verif/tests/elements/test_pot_bearing_01.py
        * ./verif/tests/elements/test_pot_bearing_02.py
        * ./verif/tests/elements/test_pot_bearing_03.py
        * ./verif/tests/materials/test_elastomeric_bearing_stiffness.py

Args:
    * x2 (list): Abcissae of the points that define the values of V2 as a function of b/a.
    * y2 (list): Ordinates of the points that define the values of V2 as a function of b/a.
    * x3 (list): Abcissae of the points that define the values of V3 as a function of b/a.
    * y3 (list): Ordinates of the points that define the values of V3 as a function of b/a.
    * x4 (list): Abcissae of the points that define the values of V4 as a function of b/a.
    * y4 (list): Ordinates of the points that define the values of V4 as a function of b/a.
    * xBeta (list): Abcissae of the points that define the values of beta (alpha in the book) as a function of b/a.
    * yBeta (list): Ordinates of the points that define the values of beta (alpha in the book) as a function of h/b.

Todo:
    * Extend the module to cover other bearing types.

'''

from __future__ import division

__author__= "Ana Ortega (AO_O) and Luis C. Pérez Tato (LCPT)"
__copyright__= "Copyright 2015, AO_O and LCPT"
__license__= "GPL"
__version__= "3.0"
__email__= " ana.Ortega.Ort@gmail.com, l.pereztato@gmail.com"

import sys
import math
import scipy.interpolate
from itertools import count
from materials import typical_materials
import xc


class Bearing(object):
    '''Bearings base class.

    :ivar materials: material list (one material for each degree of freedom).
    :ivar materialHandler: XC material handler.
    :ivar id: object identifier (auto).
    '''
    _ids = count(0) #Object counter.
    def __init__(self):
        ''' Constructor.

        :param materials: material list (one material for each degree 
                         of freedom).
        :param materialHandler: XC material handler.
        :param id: object identifier (auto).
        '''
        self.materials= list()
        self.materialHandler= None
        self.id= next(self._ids) #Object identifier.
    def getMaterial(self,i):
        ''' Returns the i-th uniaxial material that modelizes the response in the i-th direction.'''
        return self.materialHandler.getMaterial(self.materials[i])
    def getMaterialNames(self):
        '''Return material names for each DOF in a list.'''
        return self.materials
    def setBearingBetweenNodes(self,predefinedSpace, iNodA,iNodB, orientation= None):
      '''Modelize a bearing between the nodes and return newly created zero 
       length element that represents the bearing.

       :param predefinedSpace: model space (object).
       :param iNodA: (int) first node identifier (tag).
       :param iNodB: (int) second node identifier (tag).
      '''
      return predefinedSpace.setBearingBetweenNodes(iNodA,iNodB,self.materials, orientation)
    def setBearing(self,predefinedSpace, iNodA, orientation= None):
      '''Modelize a bearing on X, XY or XYZ directions.

         :param predefinedSpace: model space (object).
         :param iNod: (int) node identifier (tag).
         :param bearingMaterials (list): material names for the zero length element.
         :return rtype: (int, int) new node tag, new element tag.
      '''
      return predefinedSpace.setBearing(iNodA,self.materials, orientation)
    
class ElastomericBearing(Bearing):
    '''Rectangular elastomeric bearing.

    Reference:
        :"Puentes": book from Javier Manterola Armisén pagea 591 and 592.

    Class-wide members:

        v2table: interpolation function for the V2 shape factor.
        v3table: interpolation function for the V3 shape factor.
        v4table: interpolation function for the V4 shape factor.
        betaTable: interpolation function for the beta shape factor.

    Attributes:

    :ivar G: (float) Elastomer shear modulus.
    :ivar a: (float) Width of the bearing (parallel to bridge longitudinal axis).
    :ivar b: (float) Length of the bearing (parallel to the bridge transverse axis)
    :ivar e: (float) Net thickness of the bearing (without steel plates).
    '''
    # ENHANCE (FOR ALL THE INTERPOLATIONS): it will be great
    # if we can store the interpolation in a file to avoid repeating
    # computations every time the module is loaded.
    
    # Points that define the values of V2 as a function of b/a
    #   see "Puentes" book from Javier Manterola Armisén page 591
    x2= [1,1.5,2,3,4,6,8,10,10000]
    y2= [0.208,0.231,0.246,0.267,0.282,0.299,0.307,0.313,1/3.0]
    v2table= scipy.interpolate.interp1d(x2,y2)
    
    # Points that define the values of V3 as a function of b/a
    #   see "Puentes" book from Javier Manterola Armisén page 591
    x3= [1,1.5,2,3,4,6,8,10,10000]
    y3= [0.14,0.196,0.229,0.263,0.281,0.299,0.307,0.313,1/3.0]
    v3table=  scipy.interpolate.interp1d(x3,y3)

    # Points that define the values of V4 as a function of b/a
    #   see "Puentes" book from Javier Manterola Armisén page 592
    x4= [1,2,4,8,10000]
    y4= [85.7,71.4,64.5,61.2,60]
    v4table= scipy.interpolate.interp1d(x4,y4)

    # Points that define the values of beta as a function of h/b
    #   see ALPHA in "Hormigón" book from Jiménez Montoya et al.
    #   14th. edition page 405
    xBeta= [1.0,1.25,1.5,2.0,3.0,4.0,6.0,10.0,10000]
    yBeta= [0.14,0.171,0.196,0.229,0.263,0.281,0.299,0.313,1/3.0]
    betaTable= scipy.interpolate.interp1d(xBeta,yBeta)

    def __init__(self,G: float,a: float,b: float ,e: float):
        '''Class constructor.

        :param G: elastomer shear modulus.
        :param a: width of the bearing (parallel to bridge longitudinal axis).
        :param b: length of the bearing (parallel to the bridge transverse axis)
        :param e: net thickness of the bearing (without steel plates).
        '''
        super(ElastomericBearing,self).__init__()
        self.G= G
        self.a= a
        self.b= b
        self.e= e

    def getKhoriz(self):
        '''Return horizontal stiffness of the bearing.'''
        return self.G*self.a*self.b/self.e
    def getV2(self):
        '''Return the V2 shape factor of the bearing. '''
        return self.v2table(self.b/self.a)
    def getV3(self):
        '''Return the V3 shape factor of the bearing. '''
        return self.v3table(self.b/self.a)
    def getEbearing(self):
        ''' Return the elastic modulus with respect to the vertical
        displacement.
        '''
#        return 3*self.G*(self.a/self.e)**2*self.getV3() #29/04/2018
#       this formula was extracted from Manterola book but is wrong
#       We adopt a more rational value of E_vertical=600e6 Pa until we
#       find a better approximation
        return 600e6
    
    def getKvert(self):
        ''' Return the vertical stiffness.'''
        return self.getEbearing()*self.a*self.b/self.e
    
    def getV4(self):
        ''' Return the v4 shape factor of the bearing.'''
        return self.v4table(self.b/self.a)
    
    def getKrotationTransvBridgeAxis(self):
        ''' Stiffness with respect to the rotation around an axis
            parallel to the transverse bridge direction by the center of the bearing.
        '''
        return self.G*self.a*pow(self.b,5.0)/(self.getV4()*pow(self.e,3.0))
    
    def getKrotationLongBridgeAxis(self):
        ''' Stiffness with respect to the rotation around the longitudinal bridge axis
            by the center of the bearing.
        '''
        return self.G*self.b*pow(self.a,5.0)/(self.getV4()*pow(self.e,3.0))
    
    def getBeta(self):
        ''' Return the value of the beta factor.'''
        if(self.a<self.b):
            retval= self.betaTable(self.b/self.a)
        else:
            retval= self.betaTable(self.a/self.b)
        return retval
    
    def getKrotationVerticalAxis(self):
        ''' Stiffness  with respect to the rotation around a vertical axis.'''
        return self.getBeta()*self.G*self.a*pow(self.b,3.0)/self.e
    
    def defineMaterials(self,preprocessor):
        '''Define the following six materials to modelize the elastomeric bearing:
        KX: translation along the element X axis, that must match the 
               bridge longitudinal axis direction
        KY: translation along the element Y axis, that must match the 
               bridge transverse axis direction
        KZ: translation along vertical direction
        THX: rotation about the element X axis, that must match the 
               bridge longitudinal axis direction
        THY: rotation about the element Y axis, that must match the 
               bridge transverse axis direction
        THZ: rotation about vertical direction

       :param preprocessor: (:obj:'Preprocessor') preprocessor to use.
        '''
        self.materialHandler= preprocessor.getMaterialHandler
        # Material names.
        nameRoot= 'neop'+str(self.id)
        self.matXName= nameRoot+'X'
        self.matYName= nameRoot+'Y'
        self.matZName= nameRoot+'Z'
        self.matTHXName= nameRoot+'THX'
        self.matTHYName= nameRoot+'THY'
        self.matTHZName= nameRoot+'THZ'
        self.materials.extend([self.matXName, self.matYName, self.matZName])
        self.materials.extend([self.matTHXName, self.matTHYName, self.matTHZName])
        # Material objects.
        self.matKX= typical_materials.defElasticMaterial(preprocessor, self.matXName, self.getKhoriz())
        self.matKY= typical_materials.defElasticMaterial(preprocessor, self.matYName, self.getKhoriz())
        self.matKZ= typical_materials.defElasticMaterial(preprocessor, self.matZName, self.getKvert())
        self.matKTHX= typical_materials.defElasticMaterial(preprocessor, self.matTHXName, self.getKrotationLongBridgeAxis())
        self.matKTHY= typical_materials.defElasticMaterial(preprocessor, self.matTHYName, self.getKrotationTransvBridgeAxis()) 
        self.matKTHZ= typical_materials.defElasticMaterial(preprocessor, self.matTHZName, self.getKrotationVerticalAxis())

    def putBetweenNodes(self,modelSpace,iNodA:int, iNodB:int, orientation= None):
        ''' Puts the bearing between the nodes. The element must be oriented so that its local x axis is in the direction of the longitudinal axis of the bridge and its local y axis parallel to the transverse axis of the transverse axis of the bridge.


            :param modelSpace (:obj:'PredefinedSpace'): space dimension and number of DOFs.
            :param iNodA (int): first node identifier (tag).
            :param iNodB (int): second node identifier (tag).
            :param orientation: (list) of two vectors [x,yp] used to orient 
        the zero length element, where: 
        x: are the vector components in global coordinates defining 
        local x-axis, that must be parallel to the bridge longitudinal axis (optional)
        yp: vector components in global coordinates defining a  vector
        that lies in the local x-y plane of the element (optional).
        If the optional orientation vector are not specified, the local
        element axes coincide with the global axes
        '''
        return modelSpace.setBearingBetweenNodes(iNodA,iNodB,self.materials, orientation)

    def putAsSupport(self,modelSpace, iNod:int , orientation= None):
        ''' Puts the bearing between the nodes.

            :param modelSpace (:obj:'PredefinedSpace'): space dimension and number of DOFs.
            :param iNod (int): node to support.

        '''
        nodeHandler= modelSpace.getNodeHandler()
        newNode= nodeHandler.duplicateNode(iNod) # new node.
        
        newElement= modelSpace.setBearingBetweenNodes(newNode.tag,iNod,self.materials, orientation)
        # Boundary conditions
        numDOFs= nodeHandler.numDOFs
        for i in range(0,numDOFs):
            spc= modelSpace.newSPConstraint(newNode.tag,i,0.0)
            if(__debug__):
                if(not spc):
                    AssertionError('Can\'t create the constraint.')
        return newNode, newElement

# Points that define the Teflon coefficient of friction of
# from the mean compressive stress
# See the book «Aparatos de apoyo en puentes» from AIPCR page 46

# Table 59.8.2 of clase 59.8.2 of EAE (page 256)
xT= [5e6,10e6,20e6,30e6,45e6,1000e6]
yT= [0.08,0.06,0.04,0.03,0.025,0.024]


class PTFEPotBearing(Bearing):
    '''PTFE slide bearing.

        Attibutes:
    :ivar d: (float) Pot diameter.
    '''
    teflonMuTable= scipy.interpolate.interp1d(xT,yT)

    def __init__(self,d):
        '''Class constructor.

        :param d: pot diameter.
        '''
        super(PTFEPotBearing,self).__init__()
        self.d= d
        
    def getHorizontalStiffness(self):
        '''Returns the fictitious stiffness with respect to the horizontal displacement of a PTFE slide bearing.

        Stiffness is calculated so that when the displacement reach 20 mm
        the spring reaction equals the force of friction.

        sg_media= 35 MPa mean compressive stress.
        mov= 20e-3 Displacement when the friction force is reached.

        '''
        return self.teflonMuTable(35e6)*math.pi*(self.d/2.0)**2*35e6/20e-3

    def defineMaterials(self, preprocessor):
        '''Define the materials to modelize the pot (Teflon).

            :param preprocessor: (:obj:'Preprocessor') preprocessor to use.
        '''
        self.materialHandler= preprocessor.getMaterialHandler
        # Material names.
        nameRoot= 'pot'+str(self.id)
        self.matXName= nameRoot+'X'
        self.matYName= nameRoot+'Y'
        self.materials.extend([self.matXName, self.matYName])
        # Material objects.
        self.matKX= typical_materials.defElasticMaterial(preprocessor, self.matXName, self.getHorizontalStiffness())
        self.matKY= typical_materials.defElasticMaterial(preprocessor, self.matYName, self.getHorizontalStiffness())

    def putBetweenNodes(self, modelSpace, iNodA, iNodB):
        ''' Puts the bearing between the nodes. Set the springs between the 
            nodes in X and Y directions and imposes equal displacement in Z
            direction.

        :param modelSpace: (:obj:'PredefinedSpace') space dimension and 
                           number of DOFs.
        :param iNodA: (int) first node identifier (tag).
        :param iNodB: (int) second node identifier (tag).
        '''
        # Set the springs in the X and Y directions.
        self.potElement= modelSpace.setBearingBetweenNodes(iNodA, iNodB, self.materials)
        # Impose equal displacement in the Z directions.
        eDofs= modelSpace.constraints.newEqualDOF(iNodA, iNodB, xc.ID([2])) # 2 DOF -> Uz
        if(__debug__):
            if(not eDofs):
                AssertionError('Can\'t create constraints.')
        return self.potElement

    def putOnXBetweenNodes(self,modelSpace,iNodA, iNodB):
        ''' Puts the bearing between the nodes only in direction X.

        :param modelSpace: (:obj:'PredefinedSpace') space dimension and number of DOFs.
        :param iNodA: (int) first node identifier (tag).
        :param iNodB: (int) second node identifier (tag).
        '''
        # Set the spring in the X direction.
        self.potElement= modelSpace.setBearingBetweenNodes(iNodA,iNodB,[self.matXName])
        # Impose equal displacement in the Y and Z directions.
        eDofs= modelSpace.constraints.newEqualDOF(iNodA,iNodB,xc.ID([1,2])) # 1 and 2 DOFs -> Uy and Uz
        if(__debug__):
            if(not eDofs):
                AssertionError('Can\'t create constraints.')
        return self.potElement

    def putOnYBetweenNodes(self,modelSpace,iNodA, iNodB):
        ''' Puts the bearing between the nodes only in direction Y.

        :param modelSpace: (:obj:'PredefinedSpace') space dimension and number of DOFs.
        :param iNodA: (int) first node identifier (tag).
        :param iNodB: (int) second node identifier (tag).
        '''
        # Set the spring in the Y direction.
        self.potElement= modelSpace.setBearingBetweenNodes(iNodA,iNodB,[None,self.matYName])
        # Impose equal displacement in the X and Z directions.
        eDofs= modelSpace.constraints.newEqualDOF(iNodA,iNodB,xc.ID([0,2])) # 0 and 2 DOFs -> Ux and Uz
        if(__debug__):
            if(not eDofs):
                AssertionError('Can\'t create constraints.')
        return self.potElement

    def getReaction(self):
        ''' Return the reaction in the fixed node.'''
        return self.potElement.nodes[0].getReaction
    

def get_reaction_on_pot(preprocessor,iElem,inclInertia= False):
    ''' Return the element reaction.

    :param preprocessor: (:obj:'Preprocessor') preprocessor to use.
    :param iElem: (int) new zero length elem identifier (tag).
    :param inclInertia: (bool) true if the reaction must include inertia forces.
    '''
    methodName= sys._getframe(0).f_code.co_name
    lmsg.warning(methodName+"; don't use this function, is very inefficient: TO DEPRECATE.")
    nodes= preprocessor.getNodeHandler
    nodes.calculateNodalReactions(inclInertia, 1e-7) # It has NO sense to compute the reactions
                                                     # each time you call this function.
  
    elem= preprocessor.getElementHandler.getElement(iElem)
    reac0= elem.getNodes[0].getReaction
    return xc.Vector([reac0[0],reac0[1],reac0[2]])
