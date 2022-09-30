# -*- coding: utf-8 -*-

#Based on sXML-master projet on gitHub

__author__= "Luis C. Pérez Tato (LCPT)"
__copyright__= "Copyright 2016 LCPT"
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
import xml.etree.cElementTree as ET
from import_export.sciaXML.scia_loads import load_case_properties as lcp
from import_export.sciaXML.scia_loads import node_load_properties as nlp

containerId= "{E03984FC-B420-4C03-8D2F-72EA2FAB147D}"
containerClsId= containerId
tbName= "DataAddLoad.EP_PointForceFree.1"
tbProgId= tbName
tbId= "3599A73C-CF00-4925-A463-F72AEF07B278"
tbClsId= containerClsId

idLoadCase= nlp.idLoadCase
idLoadCaseRef= nlp.idLoadCaseRef
idName= "{4364BC01-AAB7-11D4-B3D9-00104BC3B531}"
idUniqueId= "{EB2C0F85-7275-4F94-9EA7-E8C3DBFB0FA6}"
idDirection= "{49F8017D-A4E5-11D4-A43A-000000000000}"
idType= "{49F8017C-A4E5-11D4-A43A-000000000000}"
idValidity= "{0FB44074-7F6B-41A0-9CE3-EB7779861490}"
idSelect= "{CDAA15F3-0D30-41EE-8010-534623E39891}"
idValueF= "{49F80183-A4E5-11D4-A43A-000000000000}"
idCoordX= "{C1DD759A-4291-481B-B819-92E3AA9E04B7}"
idCoordY= "{A84A15AC-FEC2-4C27-84B4-CFA676615934}"
idCoordZ= "{07B9CEDF-76BA-49F1-BEA4-66595982516A}"
idCoordUX= "{691F7087-E286-480A-ADBD-478710E6341C}"
idCoordUY= "{DBE9CDE0-61D0-4379-B822-085F36333AD6}"
idCoordUZ= "{E12A9C50-CF65-4B97-8C59-46DAF880AE8A}"
idSystem= "{A98C4369-F195-4992-ACBE-9119E1E6E0A3}"

class PointForceFreeProperties(ctr.SCXMLPropertiesContainer):
  '''Free point force properties in SCIA XML file.''' 
  def __init__(self):
    '''Constructor.'''
    super(PointForceFreeProperties,self).__init__(containerId,containerClsId,tbProgId)
    self.tableProp= propTable.SCXMLPropertiesTable(tbId,"XML\default","vertical",tbClsId,tbProgId)
    propRefLoadCase= prop.SCXMLProperty("0","Load case","ref",idLoadCase)
    propRefLoadCase.value= rf.SCXMLRef(idLoadCaseRef,lcp.tbName)
    propName= prop.SCXMLProperty("1","Name","string",idName)
    propUniqueID= prop.SCXMLProperty('2','UniqueID','string',idUniqueId)
    propDirection= prop.SCXMLProperty("3","Direction","enum",idDirection,"262144")
    propDirection.value= enum.SCXMLEnum([eI.SCXMLEnumItem("0","X"),eI.SCXMLEnumItem("1","Y"),eI.SCXMLEnumItem("2","Z")])
    propType= prop.SCXMLProperty("4","Type","enum",idType)
    propType.value= enum.SCXMLEnum([eI.SCXMLEnumItem("0","Force")])
    propValidity= prop.SCXMLProperty("5","Validity","enum",idValidity,"262144")
    propValidity.value= enum.SCXMLEnum([eI.SCXMLEnumItem("0","Tout"), eI.SCXMLEnumItem("1", "-Z"), eI.SCXMLEnumItem("2", "+Z"), eI.SCXMLEnumItem("3", "Intervalle"), eI.SCXMLEnumItem("4", "Z=0"), eI.SCXMLEnumItem("5", "-Z (yc. 0)"), eI.SCXMLEnumItem("6", "+Z (yc. 0)")])
    propSelect= prop.SCXMLProperty("6","Select","enum",idSelect)
    propSelect.value= enum.SCXMLEnum([eI.SCXMLEnumItem("0","Auto"),eI.SCXMLEnumItem("1","Select")])
    propValueF= prop.SCXMLProperty("7","Value - F","param",idValueF,"33619968")
    propCoordX= prop.SCXMLProperty("8","Coord X","param",idCoordX,"33619968")
    propCoordY= prop.SCXMLProperty("9","Coord Y","param",idCoordY,"33619968")
    propCoordZ= prop.SCXMLProperty("10","Coord Z","param",idCoordZ,"33619968")
    propCoordUX= prop.SCXMLProperty("11","Coord ux","real",idCoordUX,"33619968")
    propCoordUY= prop.SCXMLProperty("12","Coord uy","real",idCoordUY,"33619968")
    propCoordUZ= prop.SCXMLProperty("13","Coord uz","real",idCoordUZ,"33619968")
    propSystem= prop.SCXMLProperty("14","System","enum",idSystem)
    propSystem.value= enum.SCXMLEnum([eI.SCXMLEnumItem("0","GCS"),eI.SCXMLEnumItem("1","Member LCS"),eI.SCXMLEnumItem("2","Load LCS")])

    self.tableProp.properties.append(propRefLoadCase) #0
    self.tableProp.properties.append(propName) #1
    self.tableProp.properties.append(propUniqueID) #2
    self.tableProp.properties.append(propDirection) #3
    self.tableProp.properties.append(propType) #4
    self.tableProp.properties.append(propValidity) #5
    self.tableProp.properties.append(propSelect) #6
    self.tableProp.properties.append(propValueF) #7
    self.tableProp.properties.append(propCoordX) #8
    self.tableProp.properties.append(propCoordY) #9
    self.tableProp.properties.append(propCoordZ) #10
    self.tableProp.properties.append(propCoordUX) #11
    self.tableProp.properties.append(propCoordUY) #12
    self.tableProp.properties.append(propCoordUZ) #13
    self.tableProp.properties.append(propSystem) #14

  def getXMLElement(self,parent):
    '''Returns the corresponding XML element..
       Parameters:
       parent: owner of this object in the XML structure.'''
    container= ET.SubElement(parent,"def_container")
    self.populateXMLElement(container)
    self.tableProp.getXMLElement(container)
    return container

