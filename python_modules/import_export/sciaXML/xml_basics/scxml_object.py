# -*- coding: utf-8 -*-

#Based on sXML-master projet on gitHub

__author__= "Luis C. Pérez Tato (LCPT)"
__copyright__= "Copyright 2015 LCPT"
__license__= "GPL"
__version__= "3.0"
__email__= "l.pereztato@gmail.com"

from import_export.sciaXML.xml_basics import scxml_object_item as oi
from import_export.sciaXML.xml_basics import scxml_obj_base as ob
import xml.etree.cElementTree as ET

class SCXMLObject(ob.SCXMLObjBase):
    '''XML SCIA object with 21 items (ObjectItem) p0 to p10.'''
    def __init__(self,p0= oi.SCXMLObjectItem(), p1= oi.SCXMLObjectItem(), p2= oi.SCXMLObjectItem(), p3= oi.SCXMLObjectItem(), p4= oi.SCXMLObjectItem(), p5= oi.SCXMLObjectItem(), p6= oi.SCXMLObjectItem(), p7= oi.SCXMLObjectItem(), p8= oi.SCXMLObjectItem(), p9= oi.SCXMLObjectItem(), p10= oi.SCXMLObjectItem(), p11= oi.SCXMLObjectItem(), p12= oi.SCXMLObjectItem(), p13= oi.SCXMLObjectItem(), p14= oi.SCXMLObjectItem(), p15= oi.SCXMLObjectItem(), p16= oi.SCXMLObjectItem(), p17= oi.SCXMLObjectItem(), p18= oi.SCXMLObjectItem(), p19= oi.SCXMLObjectItem(), p20= oi.SCXMLObjectItem() ):
      self.nm= ''
      self.p0 = p0
      super(SCXMLObject,self).__init__('',p1,p2)
      self.p3 = p3
      self.p4 = p4
      self.p5 = p5
      self.p6 = p6
      self.p7 = p7
      self.p8 = p8
      self.p9 = p9
      self.p10 = p10
      self.p11 = p11
      self.p12 = p12
      self.p13 = p13
      self.p14 = p14
      self.p15 = p15
      self.p16 = p16
      self.p17 = p17
      self.p18 = p18
      self.p19 = p19
      self.p20 = p20

    def getP0(self):
      return self.p0
    def setP0(self, p0):
      self.p0 = p0

    def getP3(self):
      return self.p3
    def setP3(self, p3):
      self.p3 = p3

    def getP4(self):
      return self.p4
    def setP4(self, p4):
      self.p4 = p4

    def getP5(self):
      return self.p5
    def setP5(self, p5):
      self.p5 = p5

    def getP6(self):
      return self.p6
    def setP6(self, p6):
      self.p6 = p6

    def getP7(self):
      return self.p7
    def setP7(self, p7):
      self.p7 = p7

    def getP8(self):
      return self.p8
    def setP8(self, p8):
      self.p8 = p8

    def getP9(self):
      return self.p9
    def setP9(self, p9):
      self.p9 = p9

    def getP10(self):
      return self.p10
    def setP10(self, p10):
      self.p10 = p10

    def getP11(self):
      return self.p11
    def setP11(self, p11):
      self.p11 = p11

    def getP12(self):
      return self.p12
    def setP12(self, p12):
      self.p12 = p12

    def getP13(self):
      return self.p13
    def setP13(self, p13):
      self.p13 = p13

    def getP14(self):
      return self.p14
    def setP14(self, p14):
      self.p14 = p14

    def getP15(self):
      return self.p15
    def setP15(self, p15):
      self.p15 = p15

    def getP16(self):
      return self.p16
    def setP16(self, p16):
      self.p16 = p16

    def getP17(self):
      return self.p17
    def setP17(self, p17):
      self.p17 = p17

    def getP18(self):
      return self.p18
    def setP18(self, p18):
      self.p18 = p18

    def getP19(self):
      return self.p19
    def setP19(self, p19):
      self.p19 = p19

    def getP20(self):
      return self.p20
    def setP20(self, p20):
      self.p20 = p20

    def getNm(self):
      return self.nm
    def setNm(self, nm):
      self.nm = nm

    def getXMLElement(self,parent):
        '''Returns the corresponding XML element for the object.
           Parameters:
           parent: owner of this object in the XML structure.'''
        oo= ET.SubElement(parent,"obj")
        oo.set("nm",self.nm)
        oo.set("id",self.id)
        cont= 0
        items=[self.p0, self.p1, self.p2, self.p3, self.p4, self.p5, self.p6, self.p7, self.p8, self.p9, self.p10, self.p11, self.p12, self.p13, self.p14, self.p15, self.p16, self.p17, self.p18, self.p19, self.p20]
        for i in items:
            if i is not None:
                if((i.t != '') or (i.v != '') or (i.i != '') or (i.hasRows()) ):
                    ii= i.getXMLElement(oo,cont)
                    if __debug__:
                        if(not ii):
                            AssertionError('Couldn\'t get XML element.')
            cont+= 1
        return oo
