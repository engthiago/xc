# -*- coding: utf-8 -*-
''' Test the newLineSequence method. Creates a line sequence from a list of lines.'''

from __future__ import print_function
from __future__ import division

__author__= "Luis C. Pérez Tato (LCPT)"
__copyright__= "Copyright 2014, LCPT"
__license__= "GPL"
__version__= "3.0"
__email__= "l.pereztato@gmail.com"

import geom
import xc
import math
import os
from model import predefined_spaces
from materials import typical_materials

NumDiv= 6
CooMax= 10

# Problem type
feProblem= xc.FEProblem()
preprocessor=  feProblem.getPreprocessor
nodes= preprocessor.getNodeHandler
modelSpace= predefined_spaces.SolidMechanics3D(nodes)
# Materials definition
elast= typical_materials.defElasticMaterial(preprocessor, "elast",3000)


seedElemHandler= preprocessor.getElementHandler.seedElemHandler
seedElemHandler.dimElem= 3 #Bars defined in a three dimensional space.
seedElemHandler.defaultMaterial= elast.name
truss= seedElemHandler.newElement("Truss")
truss.sectionArea= 10

points= preprocessor.getMultiBlockTopology.getPoints
pt1= points.newPoint(geom.Pos3d(0.0,0.0,0.0))
pt2= points.newPoint(geom.Pos3d(CooMax/3,CooMax/3,CooMax/3))
pt3= points.newPoint(geom.Pos3d(2*CooMax/3,2*CooMax/3,2*CooMax/3))
pt4= points.newPoint(geom.Pos3d(CooMax,CooMax,CooMax))

lines= preprocessor.getMultiBlockTopology.getLines
l3= lines.newLine(pt1.tag,pt2.tag)        

# print("name: ",name," p1:",p1.name," p2:",p2.name)
l2= lines.newLine(pt4.tag,pt3.tag)
# print("name: ",name," p1:",p1.name," p2:",p2.name)
l1= lines.newLine(pt2.tag,pt3.tag)

# print("name: ",name," p1:",p1.name," p2:",p2.name)
l4= lines.newLineSequence()
l4.addLines(xc.ID([l1.tag,l2.tag,l3.tag]))
l4.nDiv= NumDiv

''' 
for_each_side

    print("name: ",edge.name," directo:",edge.directo)
  edge{print(" p1:",p1.name," p2:",p2.name )

'''


setTotal= preprocessor.getSets.getSet("total")
setTotal.genMesh(xc.meshDir.I)

nlinPline= l4.getNumEdges
nnodPline= l4.getNumNodes
nelemPline= l4.getNumElements



''' 
print("number of nodes: ",nnod)
nodes= preprocessor.getNodeHandler

for_each
    print("  node: ",tag," x= ",coord[0],", y= ",coord[1],", z= ",coord[2])
               

print("number of elements: ",nelem)
'''
elements= setTotal.getElements
ratio1= 0.0
vteor2= (CooMax/NumDiv)**2
lteor= math.sqrt(3*vteor2)
for e in elements:
  # print("  elem: ",tag," nod. I: ",nod[0].tag," nod. J: ",nod[1].tag," L= ",length)
# print("lteor: ",(lteor))
  ratio1= (e.getLength(True)-lteor)/lteor

ratio2= (nnodPline-(NumDiv+1))
ratio3= (nelemPline-NumDiv)
ratio4= (nlinPline-3)

'''
print("ratio1: ", ratio1)
print("ratio2= ", ratio2)
print("ratio3= ", ratio3)
print("nlinPline= ", nlinPline)
print("ratio4= ", ratio4)
'''

import os
from misc_utils import log_messages as lmsg
fname= os.path.basename(__file__)
if (ratio1<1e-10) & (ratio4<1e-10) & (ratio2<=1e-10) & (ratio3<=1e-10):
    print('test '+fname+': ok.')
else:
    lmsg.error(fname+' ERROR.')
