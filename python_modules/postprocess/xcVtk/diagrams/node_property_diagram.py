 # -*- coding: utf-8 -*-
''' Diagram to display a property defined at nodes over linear elements. '''

from postprocess.xcVtk.diagrams import colored_diagram as cd

class NodePropertyDiagram(cd.ColoredDiagram):
  '''Diagram to display a property defined at nodes over linear elements.'''
  def __init__(self,scaleFactor,fUnitConv,sets,attributeName):
    super(NodePropertyDiagram,self).__init__(scaleFactor,fUnitConv)
    self.lstSets= sets
    self.propertyName= attributeName

  def getValueForNode(self,node):
    if self.propertyName == 'uX':
      return node.getDispXYZ[0]
    elif self.propertyName == 'uY':
      return node.getDispXYZ[1]
    elif self.propertyName == 'uZ':
      return node.getDispXYZ[2]
    elif self.propertyName == 'rotX':
      return node.getRotXYZ[0]
    elif self.propertyName == 'rotY':
      return node.getRotXYZ[1]
    elif self.propertyName == 'rotZ':
      return node.getRotXYZ[2]
    else:
      return node.getProp(self.propertyName)

  def appendDataSetToDiagram(self, eSet,indxDiagrama,defFScale=0.0):
    ''' Append property values to diagram .
    :param eSet: Element set.
    :param defFScale: factor to apply to current displacement of nodes 
              so that the display position of each node equals to
              the initial position plus its displacement multiplied
              by this factor. (Defaults to 0.0, i.e. display of 
              initial/undeformed shape)
    ''' 
    elems= eSet.elements
    for e in elems:
      self.vDir= e.getJVector3d(True) #initialGeometry= True
      v0= self.getValueForNode(e.getNodes[0])
      v1= self.getValueForNode(e.getNodes[1])
      indxDiagrama= self.appendDataToDiagram(e,indxDiagrama,v0,v1,defFScale)

  def addDiagram(self):
    self.creaEstrucDatosDiagrama()
    self.creaLookUpTable()
    self.creaActorDiagrama()

    indxDiagrama= 0
    for s in self.lstSets:
      self.appendDataSetToDiagram(s,indxDiagrama)

    self.updateLookUpTable()
    self.updateActorDiagrama()

