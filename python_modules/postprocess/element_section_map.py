# -*- coding: utf-8 -*-

from __future__ import division
from __future__ import print_function

import string
import copy
import sys
from misc_utils import log_messages as lmsg
from materials.sections.fiber_section import def_simple_RC_section
from materials.sections import RC_sections_container as sc
from materials import typical_materials
import matplotlib.pyplot as plt

__author__= "Luis C. Pérez Tato (LCPT) and Ana Ortega (AO_O)"
__copyright__= "Copyright 2016, LCPT and AO_O"
__license__= "GPL"
__version__= "3.0"
__email__= "l.pereztato@ciccp.es" "ana.Ortega@ciccp.es"

class ElementSections(object):
    '''This class defines the list of reinforced concrete sections that are 
       going to be associated to a set of elements in order to carry out the 
       verifications of the limit states.

    :ivar name:       name given to the list of reinforced concrete sections
    :ivar directions: list of the directions to consider for each integration
                      point.
    :ivar gaussPoints: list of the integration points to consider for each
                       element.
    :ivar lstRCSects: list of reinforced concrete fiber-sections that will be 
                      associated to a set of elements in order to carry out 
                      their LS verifications. The items of the list are 
                      instances of the objects derived from *RCSectionBase*

                      The sections are ordered by integration point and then 
                      by direction. For example for an element with three integration
                      point and two directions the order is as follows:
                       
                      lstRCSects[0]= integration point 1, direction 1
                      lstRCSects[1]= integration point 1, direction 2
                      lstRCSects[2]= integration point 2, direction 1
                      lstRCSects[3]= integration point 2, direction 2
                      lstRCSects[4]= integration point 3, direction 1
                      lstRCSects[5]= integration point 3, direction 2                      
    ''' 
    def __init__(self,name,directions= [1, 2], gaussPoints= [1]):
        '''Constructor.

        :param name: name given to the list of reinforced concrete sections
        :param directions: list of the directions to consider for each integration
                          point.
        :param gaussPoints: list of the integration points to consider for each
                           element.
        '''
        self.name=name
        self.directions= directions
        self.gaussPoints= gaussPoints
        self.lstRCSects= list()

    def append_section(self, RCSect):
        ''' Append the section argument to the container.

        :param RCSect: reinforced concrete section to append.
        '''
        self.lstRCSects.append(RCSect)
        return

    def find_section(self, sectionName):
        ''' Return the section whose name is the value passed as parameter or
            None if the section is not in the list.

        :param sectionName: section name.
        '''
        return next((sct for sct in self.lstRCSects if(sct.name==sectionName)), None)

    def createSections(self, templateSections):
        '''create the fiber sections that represent the material to be used 
        for the checking on each integration point and/or each direction. These
        sections are also added to the attribute 'lstRCSects' that contains 
        the list of sections.
        '''
        if(len(self.lstRCSects)>0):
            lmsg.warning('Sections already created.')
        ngp= len(self.gaussPoints)
        ndir= len(self.directions)
        if(ngp>1 and ndir>1):
            i= 0
            for gp in self.gaussPoints:
                for d in self.directions:
                    self.creaSingleSection(templateSections[i], direction= d, gaussPnt= gp)
                    i+= 1
        elif(ngp==1 and ndir>0): # ngp==1
            for i, d in enumerate(self.directions):
                self.creaSingleSection(templateSections[i], direction= d, gaussPnt= None)
        elif(ndir==1 and ngp>0): # ndir==1
            for i, gp in enumerate(self.gaussPoints):
                self.creaSingleSection(templateSections[i], direction= None, gaussPnt= gp)
            
    def creaSingleSection(self, templateSection, direction, gaussPnt):
        '''create a copy of the section argument for the gauss points and
           the direction arguments.
        '''
        name= templateSection.name
        description= templateSection.sectionDescr
        directionText= ''
        if(direction):
            directionText= str(direction)
            name+= directionText 
            description+= ". "+ directionText + " direction."
        ipText= ''
        if(gaussPnt):
            ipText= str(gaussPnt)
            if(direction):
                name+= '_'
            name+= ipText
            description+= ' ' + ipText + " integration point."
        if(self.find_section(name) is None):
            sect= copy.copy(templateSection)
            sect.name= name
            sect.sectionDescr= description
            self.append_section(sect)

    def plot(self, preprocessor, matDiagType= 'k'):
        ''' Get a drawing of the sections using matplotlib.'''
        numSections= len(self.lstRCSects)
        if(numSections>0):
            numColumns= 2
            numRows= int(numSections/numColumns)
            fig, axs = plt.subplots(nrows= numRows, ncols= numColumns)
            # fig.tight_layout()
            if(numRows==1):
                for c in range(0,numColumns):
                    section= self.lstRCSects[c]
                    section.subplot(axs[c], preprocessor, matDiagType)
            else:
                for r in range(0,numRows):
                    for c in range(0,numColumns):
                        idx= r*numColumns+c
                        section= self.lstRCSects[idx]
                        section.subplot(axs[r,c], preprocessor, matDiagType)
            plt.show()
            
class RawShellSections(ElementSections):
    '''This class is an specialization of ElemenSections for rectangulars
       sections in two-dimensional members (slabs, walls). The items of the list 
       are instances of the object *RCRectangularSection*
    '''
    alreadyDefinedSections= list() # Names of the already defined sections.
    
    def __init__(self,name, templateSections, directions= [1,2], gaussPoints=[1]):
        '''Constructor.


        :param name: name given to the list of reinforced concrete sections
        :param templateSections: RCSectionBase derived objects that define the
                                 reinforced concrete sections of the element. 
                                 The sections must be ordered by integration 
                                 point and then by direction. For example for 
                                 an element with three integration point and 
                                 two directions the order is as follows:             
                                 templateSections[0]= Gauss point 1, direction 1
                                 templateSections[1]= Gauss point 1, direction 2
                                 templateSections[2]= Gauss point 2, direction 1
                                 templateSections[3]= Gauss point 2, direction 2
                                 templateSections[4]= Gauss point 3, direction 1
                                 templateSections[5]= Gauss point 3, direction 2
        :param directions: list of the directions to consider for each integration
                           point (defaults to [1]).
        :param gaussPoints: list of the integration points to consider for each
                           element (defaults to [1,2]).
        '''
        super(RawShellSections,self).__init__(name, directions, gaussPoints)
        self.templateSections= templateSections
        
    def createSections(self):
        '''create the fiber sections that represent the reinforced concrete fiber 
        section to be used for the checking on each integration point and/or each 
        direction. These sections are also added to the attribute 'lstRCSects' 
        that contains the list of sections.
        '''
        for templateSection in self.templateSections:
            name= templateSection.name
            if(not name in self.alreadyDefinedSections): # new section.
                self.append_section(templateSection)
                self.alreadyDefinedSections.append(name)

    def report(self, os= sys.stdout, indentation= ''):
        ''' Get a report of the object contents.

        :param os: output stream.
        :param indentation: indentation to apply to the subsequent lines.
        '''
        for templateSection in self.templateSections:
            templateSection.report(os, indentation)
    
class setRCSections2SetElVerif(ElementSections):
    '''This class is an specialization of ElemenSections for rectangular
       sections. The items of the list are instances of the 
       object *RCRectangularSection*
    ''' 
    def __init__(self,name, directions, gaussPoints):
        '''Constructor.


        :param name: name given to the list of reinforced concrete sections
        :param directions: list of the directions to consider for each integration
                          point.
        :param gaussPoints: list of the integration points to consider for each
                           element.
        '''
        super(setRCSections2SetElVerif,self).__init__(name,directions, gaussPoints)

    def setShearReinf(self,sectNmb,nShReinfBranches,areaShReinfBranch,spacing):
        '''sets parameters of the shear reinforcement of the simple section 
        identified by the sectNmb

        :param sectNmb: integer number identifying the section 
                         (1 correponds to the section stored in  lstRCSects[0] ...)
        :param nShReinfBranches: number of shear reinforcing branches
        :param areaShReinfBranch: area of the cross-section of each stirrup
        :param spacing:        spacing of the stirrups
        '''
        simplSect=self.lstRCSects[sectNmb-1]
        simplSect.shReinfZ.nShReinfBranches= nShReinfBranches 
        simplSect.shReinfZ.areaShReinfBranch= areaShReinfBranch 
        simplSect.shReinfZ.shReinfSpacing= spacing
        return

    def getAsneg(self,sectNmb):
        '''Steel area in local negative face of the simple section identified by the sectNmb

         :param sectNmb: integer number identifying the section 
                         (1 correponds to the section stored in  lstRCSects[0] ...)
        '''
        return self.lstRCSects[sectNmb-1].getAsNeg()

    def getAspos(self,sectNmb):
        '''Steel area in local positive face of the simple section identified by the sectNmb

         :param sectNmb: integer number identifying the section 
                         (1 correponds to the section stored in  lstRCSects[0] ...)
        '''
        return self.lstRCSects[sectNmb-1].getAsPos()

    def getSpos(self,sectNmb):
        '''list of distances between bars of rows the in local positive face of the simple section identified by the sectNmb

          :param sectNmb: integer number identifying the section 
                         (1 correponds to the section stored in  lstRCSects[0] ...)
        '''
        return self.lstRCSects[sectNmb-1].getSPos()

    def getSneg(self,sectNmb):
        '''list of distances between bars of rows  in the local negative face of the simple section identified by the sectNmb

         :param sectNmb: integer number identifying the section 
                         (1 correponds to the section stored in  lstRCSects[0] ...)
        '''
        return self.lstRCSects[sectNmb-1].getSNeg()

    def getDiamneg(self,sectNmb):
        '''list of bar diameter in rows of the local negative face  of the simple section identified by the sectNmb

         :param sectNmb: integer number identifying the section 
                         (1 correponds to the section stored in  lstRCSects[0] ...)
        '''
        return self.lstRCSects[sectNmb-1].getDiamNeg()

    def getDiampos(self,sectNmb):
        '''list of bar diameter in rows of the local positive face of the simple section identified by the sectNmb

         :param sectNmb: integer number identifying the section 
                         (1 correponds to the section stored in  lstRCSects[0] ...)
        '''
        return self.lstRCSects[sectNmb-1].getDiamPos()

    def getNBarpos(self,sectNmb):
        '''list of number of bars in rows of the local positive face of the simple section identified by the sectNmb

         :param sectNmb: integer number identifying the section 
                         (1 correponds to the section stored in  lstRCSects[0] ...)
        '''
        return self.lstRCSects[sectNmb-1].getNBarPos()

    def getNBarneg(self,sectNmb):
        '''list of number of bars in rows of the local negative face of the simple section identified by the sectNmb

         :param sectNmb: integer number identifying the section 
                         (1 correponds to the section stored in  lstRCSects[0] ...)
        '''
        return self.lstRCSects[sectNmb-1].getNBarNeg()

class RCSlabBeamSection(setRCSections2SetElVerif):
    '''This class is used to define the variables that make up the two 
    reinforced concrete sections that define the two reinforcement directions
    of a slab or the front and back ending sections of a beam element

    :ivar sectionDescr:    section description
    :ivar concrType:       type of concrete (e.g. EHE_materials.HA25)     
    :ivar reinfSteelType:  type of reinforcement steel
    :ivar depth:           cross-section depth
    :ivar width:           cross-section width (defaults to 1.0)
    :ivar dir1PositvRebarRows: layers of main rebars in direction 1 in the local 
                          positive face of the section (list of ReinfRow)
    :ivar dir1NegatvRebarRows: layers of main rebars in direction 1 in the local 
                          negative face of the section (list of ReinfRow)
    :ivar dir2PositvRebarRows: layers of main rebars in direction 2 in the local 
                          positive face of the section (list of ReinfRow)
    :ivar dir2NegatvRebarRows: layers of main rebars in direction 2 in the local 
                          negative face of the section (list of ReinfRow)
    :ivar dir1ShReinfY: instance of class ShearReinforcement that represents  
                        the Y shear reinforcement in section 1
    :ivar dir1ShReinfZ: instance of class ShearReinforcement that represents                      
                        the Z shear reinforcement in section 1
    :ivar dir2ShReinfY: instance of class ShearReinforcement that represents
                        the Y shear reinforcement in section 2
    :ivar dir2ShReinfZ: instance of class ShearReinforcement that represents
                        the Z shear reinforcement in section 2
    '''
    def __init__(self,name,sectionDescr,concrType,reinfSteelType,depth,width=1.0):
        '''Constructor.


        :param name: name given to the list of reinforced concrete sections.
        :param sectionDescr:    section description.
        :param concrType:       type of concrete (e.g. EHE_materials.HA25)     
        :param reinfSteelType:  type of reinforcement steel.
        :param depth:           cross-section depth.
        :param width:           cross-section width (defaults to 1.0).
        '''
        super(RCSlabBeamSection,self).__init__(name,directions= [1,2], gaussPoints= [1])
        self.sectionDescr= sectionDescr
        self.concrType= concrType
        self.reinfSteelType= reinfSteelType
        self.depth= depth
        self.width= width
        self.dir1PositvRebarRows= []
        self.dir1NegatvRebarRows= []
        self.dir2PositvRebarRows= []
        self.dir2NegatvRebarRows= []
        self.dir1ShReinfY= def_simple_RC_section.ShearReinforcement()
        self.dir1ShReinfZ= def_simple_RC_section.ShearReinforcement()
        self.dir2ShReinfY= def_simple_RC_section.ShearReinforcement()
        self.dir2ShReinfZ= def_simple_RC_section.ShearReinforcement()

    def createSections(self):
        '''create the fiber sections of type 'RCRectangularSection' that 
        represent the reinforced concrete fiber section to be used for the
        checking on each integration point and/or each direction. These 
        sections are also added to the attribute 'lstRCSects' that contains 
        the list of sections.
        '''
        templateSection1= self.getTemplateSection(posReb=self.dir1PositvRebarRows,negReb=self.dir1NegatvRebarRows,YShReinf=self.dir1ShReinfY,ZShReinf=self.dir1ShReinfZ)
        templateSection2= self.getTemplateSection(posReb=self.dir2PositvRebarRows,negReb=self.dir2NegatvRebarRows,YShReinf=self.dir2ShReinfY,ZShReinf=self.dir2ShReinfZ)
        super(RCSlabBeamSection,self).createSections([templateSection1,templateSection2])

    def getTemplateSection(self, posReb,negReb,YShReinf,ZShReinf):
        ''' Return the template section to use with createSingleSection
            method.'''
        sect= def_simple_RC_section.RCRectangularSection()
        sect.name= self.name
        sect.sectionDescr= self.sectionDescr
        sect.fiberSectionParameters.concrType= self.concrType
        sect.h= self.depth
        sect.b= self.width
        sect.fiberSectionParameters.reinfSteelType= self.reinfSteelType
        sect.positvRebarRows= posReb
        sect.negatvRebarRows= negReb
        sect.shReinfY= YShReinf
        sect.shReinfZ= ZShReinf
        return sect

    def setShearReinfD2(self,nShReinfBranches,areaShReinfBranch,spacing):
        self.lstRCSects[1].shReinfZ.nShReinfBranches= nShReinfBranches
        self.lstRCSects[1].shReinfZ.areaShReinfBranch= areaShReinfBranch
        self.lstRCSects[1].shReinfZ.shReinfSpacing= spacing

    def setShearReinfD1(self,nShReinfBranches,areaShReinfBranch,spacing):
        self.lstRCSects[0].shReinfZ.nShReinfBranches= nShReinfBranches
        self.lstRCSects[0].shReinfZ.areaShReinfBranch= areaShReinfBranch
        self.lstRCSects[0].shReinfZ.shReinfSpacing= spacing

    def getAs1neg(self):
        '''Steel area in local negative face direction 1.'''
        if(len(self.lstRCSects)>0):
            return self.lstRCSects[0].getAsNeg()
        else:
            return 0.0
    
    def getAs1pos(self):
        '''Steel area in local positive face direction 1.'''
        if(len(self.lstRCSects)>0):
            return self.lstRCSects[0].getAsPos()
        else:
            return 0.0
  
    def getAs2neg(self):
        '''Steel area in local negative face direction 2.'''
        if(len(self.lstRCSects)>1):
            return self.lstRCSects[1].getAsNeg()
        else:
            return 0.0
    
    def getAs2pos(self):
        '''Steel area in local positive face direction 2.'''
        if(len(self.lstRCSects)>1):
            return self.lstRCSects[1].getAsPos()
        else:
            return 0.0
  
    def getReinfArea(self,code):
        '''get steel area.
        code='As1+' for direction 1, positive face
        code='As1-' for direction 1, negative face
        code='As2+' for direction 2, positive face
        code='As2-' for direction 2, negative face
        '''
        if(code=='As1-'):
            return self.getAs1neg()
        elif(code=='As1+'):
            return self.getAs1pos()
        elif(code=='As2-'):
            return self.getAs2neg()
        elif(code=='As2+'):
            return self.getAs2pos()
        else:
            sys.stderr.write("code: "+ code + " unknown.\n")
            return None

    def getAs(self):
        ''' Return the total reinforcement area.'''
        return self.getAs1neg()+self.getAs1pos()+self.getAs2neg()+self.getAs2pos()
    def getS1pos(self):
        '''list of distances between bars of rows the in local positive face direction 1.'''
        return self.lstRCSects[0].getSPos()
  
    def getS1neg(self):
        '''list of distances between bars of rows  in the local negative face direction 1.'''
        return self.lstRCSects[0].getSNeg()
  
    def getS2pos(self):
        '''list of distances between bars of rows  in the local positive face direction 2.'''
        return self.lstRCSects[1].getSPos()
  
    def getS2neg(self):
        '''list of distances between bars of rows  in the local negative face direction 2.'''
        return self.lstRCSects[1].getSNeg()
  
    def getS(self,code):
        '''list of distances between bars
        code='s1+' for direction 1, positive face
        code='s1-' for direction 1, negative face
        code='s2+' for direction 2, positive face
        code='s2-' for direction 2, negative face
        '''
        if(code=='s1-'):
            return self.getS1neg()
        elif(code=='s1+'):
            return self.getS1pos()
        elif(code=='s2-'):
            return self.getS2neg()
        elif(code=='s2+'):
            return self.getS2pos()
        else:
            sys.stderr.write("code: "+ code + " unknown.\n")
            return None

    def getDiam1neg(self):
        '''list of bar diameter in rows of the local negative face direction 1.'''
        return self.lstRCSects[0].getDiamNeg()
  
    def getDiam1pos(self):
        '''list of bar diameter in rows of the local positive face direction 1.'''
        return self.lstRCSects[0].getDiamPos()
  
    def getDiam2neg(self):
        '''list of bar diameter in rows of the local negative face direction 2.'''
        return self.lstRCSects[1].getDiamNeg()
  
    def getDiam2pos(self):
        '''list of bar diameter in rows of the local positive face direction 2.'''
        return self.lstRCSects[1].getDiamPos()
  
    def getDiam(self,code):
        '''list of bar diameter.'''
        if(code=='d1-'):
            return self.getDiam1neg()
        elif(code=='d1+'):
            return self.getDiam1pos()
        elif(code=='d2-'):
            return self.getDiam2neg()
        elif(code=='d2+'):
            return self.getDiam2pos()
        else:
            sys.stderr.write("code: "+ code + " unknown.\n")
            return None

    def getNBar1pos(self):
        '''list of number of bars in rows of the local positive face direction 1.'''
        return self.lstRCSects[0].getNBarPos()
  
    def getNBar1neg(self):
        '''list of number of bars in rows of the local negative face direction 1.'''
        return self.lstRCSects[0].getNBarNeg()
  
    def getNBar2pos(self):
        '''list of number of bars in rows of the local positive face direction 2.'''
        return self.lstRCSects[1].getNBarPos()
  
    def getNBar2neg(self):
        '''list of number of bars in rows of the local negative face direction 2.'''
        return self.lstRCSects[1].getNBarNeg()

    def getMainReinfProperty(self,code):
        if('As' in code):
            return self.getReinfArea(code)
        elif('nBar' in code):
            return self.getNBar(code)
        elif('s' in code):
            return self.getS(code)
        elif('d' in code):
            return self.getDiam(code)

    def getElasticMembranePlateSection(self, preprocessor, reductionFactor= 1.0):
        ''' Return an elastic membrane plate section material.

        :param preprocessor: proprocessor for the finite element problem.
        :param reductionFactor: factor that divides the concrete elastic
                                modulus to simulate the effect of cracking,
                                normally between 1.0 and 7.0.
        '''
        shellMatName= self.name+'_membrane_plate'
        Ec= self.concrType.getEcm()/reductionFactor
        nu= 0.3
        thickness= self.depth
        rho= self.concrType.density()
        return typical_materials.defElasticMembranePlateSection(preprocessor,shellMatName,Ec,nu,rho,thickness)
    
class LegacyRCSlabBeamSection(RCSlabBeamSection):
    '''This class is used for compatibility with older code. The main
    difference is that the slab reinforcement is linked with an
    element set.

    :ivar dir2ShReinfZ: instance of class ShearReinforcement that represents
                        the Z shear reinforcement in section 2

    '''
    def __init__(self,name,sectionDescr,concrType,reinfSteelType,depth,width=1.0, elemSet= None):
        '''Constructor.

        :param elemSet:set with the elements to which to assign the section.
                         (defaults to None)
        '''
        super(LegacyRCSlabBeamSection,self).__init__(name= name, sectionDescr= sectionDescr, concrType= concrType, reinfSteelType= reinfSteelType, depth= depth, width= width)
        self.elemSet= elemSet


class RCMemberSection(ElementSections):
    '''This class is an specialization of ElemenSections for rectangular
       sections. The items of the list are instances of the object 
       *RCRectangularSection*
    ''' 
    def __init__(self,name, templateSections, directions= [1], gaussPoints=[1,2]):
        '''Constructor.


        :param name: name given to the list of reinforced concrete sections
        :param templateSections: RCSectionBase derived objects that define the
                                 reinforced concrete sections of the element. 
                                 The sections must be ordered by integration 
                                 point and then by direction. For example for 
                                 an element with three integration point and 
                                 two directions the order is as follows:             
                                 templateSections[0]= Gauss point 1, direction 1
                                 templateSections[1]= Gauss point 1, direction 2
                                 templateSections[2]= Gauss point 2, direction 1
                                 templateSections[3]= Gauss point 2, direction 2
                                 templateSections[4]= Gauss point 3, direction 1
                                 templateSections[5]= Gauss point 3, direction 2
        :param directions: list of the directions to consider for each integration
                           point (defaults to [1]).
        :param gaussPoints: list of the integration points to consider for each
                           element (defaults to [1,2]).
        '''
        super(RCMemberSection,self).__init__(name, directions, gaussPoints)
        self.templateSections= templateSections
        
    def createSections(self):
        '''create the fiber sections that represent the reinforced concrete fiber 
        section to be used for the checking on each integration point and/or each 
        direction. These sections are also added to the attribute 'lstRCSects' 
        that contains the list of sections.
        '''
        super(RCMemberSection,self).createSections(self.templateSections)
        
def loadMainRefPropertyIntoElements(elemSet, sectionContainer, code):
    '''add to each element of the set the
       desired property (As1+,As1-,...,d1+,d1-,...).''' 
    for e in elemSet:
        if(e.hasProp('name')):
            name= e.getProp('name')
            s= sectionContainer.search(name)
            e.setProp(code,s.getMainReinfProperty(code))
        else:
            sys.stderr.write("element: "+ str(e.tag) + " section undefined.\n")
            e.setProp(code,0.0)

class ElementSectionMap(dict):
    '''dictionary that stores a section name(s) for each element number. 
       This way it defines  a spatial distribution of the sections over
       the structure.

    :ivar elementDimension: dictionary to store the dimension (1, 2 or 3) 
                            of each element.
    '''
    propName= 'name' # Name of the property that stores the section names.

    def __init__(self):
        ''' Constructor.'''
        super(ElementSectionMap, self).__init__()
        self.elementDimension= dict() # Store dimension (1, 2 or 3) of each element.

    def getElementDimension(self, elemTag):
        ''' Return the dimension of the element whose tag is being passed
           as parameter.

        :param tagElem: master element identifier.
        '''
        if elemTag in self.elementDimension.keys():
            return self.elementDimension[elemTag]
        else:
            className= type(self).__name__
            methodName= sys._getframe(0).f_code.co_name
            lmsg.error(className+'.'+methodName+"; element: "+ str(elemTag) + " not found.")
            
        
    def assign(self, elemSet, setRCSects):
        '''Assigns the sections names to the elements of the set.

           :param elemSet: set of elements that receive the section names 
                           property.
           :param setRCSects: RC section definition, name, concrete type,
                              rebar positions,...
        '''
        if len(elemSet)== 0:
            className= type(self).__name__
            methodName= sys._getframe(0).f_code.co_name
            lmsg.warning(className+'.'+methodName+'; element set is empty.')

        for e in elemSet:
            if(not e.hasProp(self.propName)):
                self[e.tag]= list()
                self.elementDimension[e.tag]= e.getDimension
                for s in setRCSects.lstRCSects:
                    self[e.tag].append(s.name)
                e.setProp(self.propName,setRCSects.name)
            else:
                className= type(self).__name__
                methodName= sys._getframe(0).f_code.co_name
                lmsg.error(className+'.'+methodName+"; element: "+ str(e.tag) + " has already a section ("+e.getProp(self.propName)+")\n")

    def assignFromElementProperties(self, elemSet, sectionWrapperName):
        '''Creates the section materials from the element properties
           and assigns them to the elements of the argument set .

        :param elemSet: set of elements that receive the section names 
                        property.
        :param sectionWrapperName: name to use as prefix for the sections
                                   that will be created for the elements
                                   of the set.
        '''
        def n2a(n,b=string.ascii_uppercase):
            d, m = divmod(n,len(b))
            return n2a(d-1,b)+b[m] if d else b[m]
        
        if len(elemSet)== 0:
            className= type(self).__name__
            methodName= sys._getframe(0).f_code.co_name
            lmsg.warning(className+'.'+methodName+'; element set is empty.')
            
        # Compute the different sections from the element properties.
        # propName: name of the element properties that stores the section name
        rcSections= def_simple_RC_section.get_element_rc_sections(elemSet, propName= self.propName)
        # Compute section pairs.
        sectionPairs= list()
        for el in elemSet:
            elementSections= el.getProp(self.propName)
            self[el.tag]= elementSections
            self.elementDimension[el.tag]= el.getDimension
            if(elementSections not in sectionPairs):
                sectionPairs.append(elementSections)
        # Rename sections and create RCMemberSection objects.
        retval= sc.SectionContainer()
        for i, names in enumerate(sectionPairs):
            letter= n2a(i)
            sect0= next((rcs for rcs in rcSections if(rcs.name==names[0])), None)
            sect1= next((rcs for rcs in rcSections if(rcs.name==names[1])), None)
            newName= sectionWrapperName+letter
            rcMemberSection= RawShellSections(name= newName, templateSections= [sect0, sect1], directions= [1,2], gaussPoints=[1])
            retval.append(rcMemberSection)
        return retval
        

