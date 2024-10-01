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
from import_export.sciaXML.xml_basics import scxml_ref as rf
from import_export.sciaXML.xml_basics import scxml_enum_item as eI
from import_export.sciaXML.xml_basics import scxml_enum as enum
from import_export.sciaXML.xml_basics import scxml_properties_table as propTable
from import_export.sciaXML.xml_basics import scxml_properties_sub_table as propSubTable
import xml.etree.cElementTree as ET
from import_export.sciaXML.scia_loads import load_case_properties as lcp

containerId= "{F8371A21-F459-11D4-94D3-000000000000}"
containerClsId= containerId
tbName= "DataAddLoad.EP_PointForcePoint.1"
tbProgId= tbName
tbId="18E5DA6A-A48A-4522-A210-E961AF296D93"
tbClsId= containerClsId

idName= "{4364BC01-AAB7-11D4-B3D9-00104BC3B531}"
idLoadCase= "{AACF96D6-D986-421C-9FF2-D0B696A6B1F6}"
idLoadCaseRef= "{0908D21F-481F-11D4-AB84-00C06C452330}"
idRefTable= "{8F96D00B-2125-411D-9218-EF0038D0056F}"
idMemberType= "{3334884C-E566-404D-A135-4093CC88257D}"
idMemberTypeName= "{FF14758E-BC03-4650-88B5-5EA8F1016952}"
idMemberName= "{66D67076-ED1C-44EC-BA07-93E993F493CB}"
idDirection= "{49F8017D-A4E5-11D4-A43A-000000000000}"
idType= "{49F8017C-A4E5-11D4-A43A-000000000000}"
idValueF= "{49F80183-A4E5-11D4-A43A-000000000000}"
idSystem= "{ACC6FEC0-B7FC-11D4-ADD9-000000000000}"

class NodeLoadProperties(ctr.SCXMLPropertiesContainer):
  tableProp= None
  def __init__(self):
    super(NodeLoadProperties,self).__init__(containerId,containerClsId,tbProgId)
    self.tableProp= propTable.SCXMLPropertiesTable(tbId,r"XML\default","vertical",tbClsId,tbProgId)
    propName= prop.SCXMLProperty("0","Name","string",idName)
    propRefLoadCase= prop.SCXMLProperty("1","Load case","ref",idLoadCase)
    propRefLoadCase.value= rf.SCXMLRef(idLoadCaseRef,lcp.tbName)
    propRefTable= prop.SCXMLProperty("2","Reference Table","table",idRefTable)
    propRefTable.value= propSubTable.SCXMLPropertiesSubTable('','',"vertical",'','')
    propRefTable.value.properties.append(prop.SCXMLProperty("0","Member Type","string",idMemberType))
    propRefTable.value.properties.append(prop.SCXMLProperty("1","Member Type Name","string",idMemberTypeName))
    propRefTable.value.properties.append(prop.SCXMLProperty("2","Member Name","string",idMemberName))

    propDirection= prop.SCXMLProperty("3","Direction","enum",idDirection,"262144")
    propDirection.value= enum.SCXMLEnum([eI.SCXMLEnumItem("0","X"),eI.SCXMLEnumItem("1","Y"),eI.SCXMLEnumItem("2","Z")])
    propType= prop.SCXMLProperty("4","Type","enum",idType,"262144")
    propType.value= enum.SCXMLEnum([eI.SCXMLEnumItem("0","Force")])
    propValueF= prop.SCXMLProperty("5","Value - F","param",idValueF,"33619968")
    propSystem= prop.SCXMLProperty("6","System","enum",idSystem)
    propSystem.value= enum.SCXMLEnum([eI.SCXMLEnumItem("0","GCS"),eI.SCXMLEnumItem("1","LCS")])

    self.tableProp.properties.append(propName) #0
    self.tableProp.properties.append(propRefLoadCase) #1
    self.tableProp.properties.append(propRefTable) #2
    self.tableProp.properties.append(propDirection) #3
    self.tableProp.properties.append(propType) #4
    self.tableProp.properties.append(propValueF) #5
    self.tableProp.properties.append(propSystem) #6

  def getXMLElement(self,parent):
    '''Returns the corresponding XML element for the node loads properties.
       Parameters:
       parent: owner of this object in the XML structure.'''
    container= ET.SubElement(parent,"def_container")
    self.populateXMLElement(container)
    self.tableProp.getXMLElement(container)
    return container

