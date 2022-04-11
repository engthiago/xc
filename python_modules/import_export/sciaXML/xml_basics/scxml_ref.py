# -*- coding: utf-8 -*-

#Based on sXML-master projet on gitHub

__author__= "Luis C. Pérez Tato (LCPT)"
__copyright__= "Copyright 2015 LCPT"
__license__= "GPL"
__version__= "3.0"
__email__= "l.pereztato@gmail.com"

from import_export.sciaXML.xml_basics import scxml_base as b
import xml.etree.cElementTree as ET

class SCXMLRef(b.SCXMLBase):
  '''SCIA XML reference.'''
  def __init__(self,id,progid):
    super(SCXMLRef,self).__init__(id,'')
    self.progid= progid
  
  def getXMLElement(self,parent):
    '''Returns the corresponding XML element for the reference..
       Parameters:
       parent: owner of this object in the XML structure.'''
    ref= ET.SubElement(parent,"def_ref")
    super(SCXMLRef,self).populateXMLElement(ref)
    if(self.progid!=''):
      ref.set("progid",self.progid)
      

  
