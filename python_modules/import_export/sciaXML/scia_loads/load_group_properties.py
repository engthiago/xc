# -*- coding: utf-8 -*-

#Based on sXML-master projet on gitHub

__author__= "Luis C. Pérez Tato (LCPT)"
__copyright__= "Copyright 2015 LCPT"
__license__= "GPL"
__version__= "3.0"
__email__= "l.pereztato@gmail.com"

#Properties for constraint nodes.

from import_export.sciaXML.xml_basics import scxml_properties_container as ctr
from import_export.sciaXML.xml_basics import scxml_property as prop
from import_export.sciaXML.xml_basics import scxml_enum_item as eI
from import_export.sciaXML.xml_basics import scxml_enum as enum
from import_export.sciaXML.xml_basics import scxml_properties_table as propTable
import xml.etree.cElementTree as ET

containerId= "{F9D4AA72-49D5-11D4-A3CF-000000000000}"
containerClsId= containerId
tbName="DataSetScia.EP_LoadGroup.1"
tbProgId= tbName
tbId="49D41DBB-F38D-4AF9-B7FD-3F91E1D06036"
tbClsId= containerClsId

idName= "{4364BC01-AAB7-11D4-B3D9-00104BC3B531}"
idLoad= "{49F9F892-83EC-11D4-B37D-000000000000}"

class LoadGroupProperties(ctr.SCXMLPropertiesContainer):
  tableProp= None
  def __init__(self):
    super(LoadGroupProperties,self).__init__(containerId,containerClsId,tbProgId)
    self.tableProp= propTable.SCXMLPropertiesTable(tbId,'',"vertical",tbClsId,tbProgId)
    propName= prop.SCXMLProperty("0","Name","string",idName)
    propLoad= prop.SCXMLProperty("1","Load","enum",idLoad,"262144")
    propLoad.value= enum.SCXMLEnum([eI.SCXMLEnumItem("0","Permanent")])

    self.tableProp.properties.append(propName) #0
    self.tableProp.properties.append(propLoad) #1

  def getXMLElement(self,parent):
    '''Returns the corresponding XML element for the load groups properties.
       Parameters:
       parent: owner of this object in the XML structure.'''
    container= ET.SubElement(parent,"def_container")
    self.populateXMLElement(container)
    self.tableProp.getXMLElement(container)
    return container

