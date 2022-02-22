# -*- coding: utf-8 -*-
from __future__ import print_function

import xc_base
import geom
import xc
import math
import os
from model import predefined_spaces
from materials import typical_materials

__author__= "Luis C. Pérez Tato (LCPT)"
__copyright__= "Copyright 2014, LCPT"
__license__= "GPL"
__version__= "3.0"
__email__= "l.pereztato@gmail.com"

NumDiv= 10
CooMax= 10
root3= math.sqrt(3)

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
truss= seedElemHandler.newElement("Truss",xc.ID([0,0]))
truss.sectionArea= 10

points= preprocessor.getMultiBlockTopology.getPoints
pt1= points.newPoint(geom.Pos3d(0.0,0.0,0.0))
pt2= points.newPoint(geom.Pos3d(CooMax,CooMax,CooMax))

lines= preprocessor.getMultiBlockTopology.getLines
l= lines.newDividedLine(pt1.tag,pt2.tag)
l.setLengths(xc.Vector([root3,root3,root3,root3,root3,root3,root3,root3,root3,root3]))

setTotal= preprocessor.getSets.getSet("total")
setTotal.genMesh(xc.meshDir.I)

''' 
print("number of nodes: ",nnod)
nodes= preprocessor.getNodeHandler
for_each
    print("  node: ",tag," x= ",coord[0],", y= ",coord[1],", z= ",coord[2])

print("number of elements: ",nelem)
'''
elements= setTotal.getElements
cumple= 1
vteor2= (CooMax/NumDiv)**2
lteor= math.sqrt(3*vteor2)
for e in elements:
  # print("  elem: ",tag," nod. I: ",nod[0].tag," nod. J: ",nod[1].tag," L= ",length)
  # print("lteor: ",(lteor))
  ratio1= (lteor/e.getLength(True))
  cumple= (abs(ratio1-1.0)<1e-5) & (cumple) 
  # print("cumple: ",(cumple))

import os
from misc_utils import log_messages as lmsg
fname= os.path.basename(__file__)
if cumple:
    print('test '+fname+': ok.')
else:
    lmsg.error(fname+' ERROR.')
