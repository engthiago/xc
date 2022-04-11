# -*- coding: utf-8 -*-
from __future__ import print_function


#Based on sXML-master projet on gitHub

__author__= "Luis C. Pérez Tato (LCPT)"
__copyright__= "Copyright 2015 LCPT"
__license__= "GPL"
__version__= "3.0"
__email__= "l.pereztato@gmail.com"

import os
from import_export.sciaXML.xml_basics import scxml_definition
from import_export.sciaXML import node_container as nCtr
from import_export.sciaXML import ep_plane_container as eppc
from import_export.sciaXML import node_support_container as nsc
from import_export.sciaXML.scia_loads import load_group_container as lgc
from import_export.sciaXML.scia_loads import load_case_container as lcc
from import_export.sciaXML.scia_loads import load_comb_container as lcmb
from import_export.sciaXML.scia_loads import node_load_container as nlc
from import_export.sciaXML.scia_loads import element_load_container as elc
from import_export.sciaXML import project_properties as prjDef
import xml.etree.cElementTree as ET

class SXMLBase(object):
  def __init__(self,xmlns, mesh,loadContainer):
    ''' Constructor:

        :param xmlns:  attribute that defines the XML namespace. 
        :param mesh:   finite element mesh or block topology (points,lines, surfaces,...).
        :param loadContainer:   contains loads obtained from the XC model.
    '''
    self.xmlns= xmlns
    self.uuid= ''
    self.defn= scxml_definition.SCXMLDefinition(mesh.name)
    print('XXX Write material container code.')
    self.materialContainer= None
    loads= loadContainer.loads
    self.loadGroupContainer= lgc.LoadGroupContainer(loads.loadGroups)
    self.loadCaseContainer= lcc.LoadCaseContainer(loads.loadCases)
    self.loadCombContainer= lcmb.LoadCombContainer(loads.loadCombs)
    self.fileName= mesh.name+'.xml'
  def getContainers(self):
    return [self.loadGroupContainer, self.loadCaseContainer, self.loadCombContainer]

  def getXMLElement(self,defFileName):
    '''Returns the corresponding XML element for the object.
       Parameters:
       defFileName: XML definition file name.'''
    project= ET.Element("project")
    project.set("xmlns",self.xmlns)
    df= ET.SubElement(project,"def")
    df.set("uri",defFileName)
    containers= self.getContainers()
    for c in containers:
      elem= c.getXMLElement(project)
    return project

  def getXMLTree(self,defFileName):
    '''Returns the corresponding XML tree.'''
    project= self.getXMLElement(defFileName)
    tree = ET.ElementTree(project)
    return tree

  def getFileName(self):
    return self.fileName

  def getDefFileName(self):
    return self.fileName + ".def"

  def writeXMLFile(self,outputPath= ''):
    '''Write XML files for the object.'''
    self.outputPath= outputPath 
    defFileName= self.getDefFileName()
    prj_def= prjDef.ProjectProperties(self.xmlns,defFileName)
    def_tree= prj_def.getXMLTree(defFileName)
    def_tree.write(self.outputPath+defFileName,encoding="UTF-8", xml_declaration=None, default_namespace=None, method="xml")
    tree= self.getXMLTree(defFileName)
    tree.write(self.outputPath+self.fileName,encoding="UTF-8", xml_declaration=None, default_namespace=None, method="xml")

  def indent(self):
    os.system("xmlindent -w "+self.outputPath+self.fileName)
    os.system("xmlindent -w "+self.outputPath+self.getDefFileName())
