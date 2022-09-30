# -*- coding: utf-8 -*-

#Based on sXML-master projet on gitHub

__author__= "Luis C. Pérez Tato (LCPT)"
__copyright__= "Copyright 2015 LCPT"
__license__= "GPL"
__version__= "3.0"
__email__= "l.pereztato@gmail.com"

from import_export.sciaXML.xml_basics import scxml_table_container as ctr
from import_export.sciaXML.xml_basics import scxml_table_xmlnodes as tb
from import_export.sciaXML.xml_basics import scxml_object as obj
from import_export.sciaXML.xml_basics import scxml_object_item as oI
from import_export.sciaXML.scia_loads import point_force_free_properties as pffp
from import_export.sciaXML.scia_loads import load_component_base as lcb
import uuid

idPointForceFreeContainer= "{E03984FC-B420-4C03-8D2F-72EA2FAB147D}"
tPointForceFreeContainer= pffp.tbName
idPointForceFreeContainerTb= "3599A73C-CF00-4925-A463-F72AEF07B278"
tPointForceFreeContainerTb= pffp.tbName
pointForceFreePrefix= 'PF'

class PointForceFreeComponent(lcb.LoadComponentBase):
  ''' Each of the components (X, Z or Z) of a punctual load '''
  counter= 0
  def __init__(self, loadCaseId, loadCaseName, direction, value, x, y, z):
    ''' Constructor:

        :param loadCaseId:      load case identifier.
        :param loadCaseName:    load case name.
        :param direction:       component direction (X, Y or Z).
        :param value:           value of the force.
    '''
    super(PointForceFreeComponent,self).__init__(loadCaseId, loadCaseName, direction, value)
    PointForceFreeComponent.counter+=1
    self.pointForceFreeCompId= PointForceFreeComponent.counter
    self.x= x
    self.y= y
    self.z= z 

  def getObject(self):
    '''Returns an XML object for the point force component.'''
    retval= obj.SCXMLObject() #SCIA XML basic object.
    loadCompId= str(self.pointForceFreeCompId)
    retval.setId(loadCompId)
    name= pointForceFreePrefix+loadCompId
    retval.setNm(name)
    retval.setP0(self.getLoadCaseReferenceItem()) #Reference to load case.
    retval.setP1(oI.SCXMLObjectItem(name)) #Name
    retval.setP2(oI.SCXMLObjectItem('{'+str(uuid.uuid4())+'}')) # Unique id
    retval.setP3(self.getDirectionObjectItem()) #Direction X, Y or Z
    retval.setP4(oI.SCXMLObjectItem('0','','','Force')) #Type 0 -> Force.
    retval.setP5(oI.SCXMLObjectItem('4','','','Z= 0')) #Validity
    retval.setP6(oI.SCXMLObjectItem('0','','','Auto')) #Select
    retval.setP7(self.getValueObjectItem()) #Value
    retval.setP8(oI.SCXMLObjectItem(str(self.x))) #X (position)
    retval.setP9(oI.SCXMLObjectItem(str(self.y))) #Y (position)
    retval.setP10(oI.SCXMLObjectItem(str(self.z))) #Z (position)
    retval.setP11(oI.SCXMLObjectItem(str(self.x))) #ux (position?)
    retval.setP12(oI.SCXMLObjectItem(str(self.y))) #uy (position?)
    retval.setP13(oI.SCXMLObjectItem(str(self.z))) #uz (position?)
    retval.setP14(self.getSystemItem()) #System 0 -> GCS, 1 -> LCS, 2 -> Load LCS
    return retval


def getPointForceFreeComponents(pointForceFree):
  '''get PointForceFreeComponent objects from the 3D pointForceFree argument.'''
  retval= list()
  vDir= pointForceFree.vDir
  position= pointForceFree.pos
  loadCaseId= pointForceFree.loadCaseId
  loadCaseName= pointForceFree.loadCaseName
  value= pointForceFree.value
  if(vDir[0]!=0.0):
    retval.append(PointForceFreeComponent(loadCaseId, loadCaseName, 'X',value*vDir[0],position.x,position.y,position.z)) #X component.
  if(vDir[1]!=0.0):
    retval.append(PointForceFreeComponent(loadCaseId, loadCaseName, 'Y',value*vDir[1],position.x,position.y,position.z)) #Y component.
  if(vDir[2]!=0.0):
    retval.append(PointForceFreeComponent(loadCaseId, loadCaseName, 'Z',value*vDir[2],position.x,position.y,position.z)) #Z component.
  return retval
  
def getPointForceFreeObjects(nl):
  components= getPointForceFreeComponents(nl)
  retval= list()
  for c in components:
    retval.append(c.getObject())
  return retval

class PointForceFreeContainer(ctr.SCXMLTableContainer):
  def __init__(self,pointForceFreesDict):
    super(PointForceFreeContainer,self).__init__(idPointForceFreeContainer,tPointForceFreeContainer)
    pointForceFrees= list()
    for nl in pointForceFreesDict:
      compObjects= getPointForceFreeObjects(nl)
      for c in compObjects:
        pointForceFrees.append(c)
    self.appendTable(tb.SCXMLTableXMLNodes(idPointForceFreeContainerTb,tPointForceFreeContainerTb, 'Point forces in node', None,pointForceFrees))
  
