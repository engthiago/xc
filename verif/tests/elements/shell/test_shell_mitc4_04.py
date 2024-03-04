# -*- coding: utf-8 -*-
from __future__ import print_function
'''Verification test taken from example 2-005 of 
   the SAP 2000 verification manual.'''

__author__= "Luis C. Pérez Tato (LCPT) and Ana Ortega (AOO)"
__copyright__= "Copyright 2015, LCPT and AOO"
__license__= "GPL"
__version__= "3.0"
__email__= "l.pereztato@gmail.com"

import geom
import xc
from solution import predefined_solutions
from model import predefined_spaces
from materials import typical_materials

# feProblem.setVerbosityLevel(0)
NumDivI= 8
NumDivJ= 8
CooMaxX= 2
CooMaxY= 2
E= 17472000 # Elastic modulus en lb/in2
nu= 0.3 # Poisson's ratio
G= 6720000
thickness= 0.0001 # Cross section depth expressed in inches.
unifLoad= 0.0001 # Uniform load in lb/in2.
ptLoad= 0.0004 # Punctual load in lb.

feProblem= xc.FEProblem()
preprocessor=  feProblem.getPreprocessor
nodes= preprocessor.getNodeHandler

# Problem type
modelSpace= predefined_spaces.StructuralMechanics3D(nodes)
# Define materials
memb1= typical_materials.defElasticMembranePlateSection(preprocessor, "memb1",E,nu,0.0,thickness)

# Define seed element.
seedElemHandler= preprocessor.getElementHandler.seedElemHandler
seedElemHandler.defaultMaterial= memb1.name
elem= seedElemHandler.newElement("ShellMITC4")

# Define problem geometry.
points= preprocessor.getMultiBlockTopology.getPoints
pt1= points.newPoint(geom.Pos3d(0.0,0.0,0.0))
pt2= points.newPoint(geom.Pos3d(CooMaxX,0.0,0.0))
pt3= points.newPoint(geom.Pos3d(CooMaxX,CooMaxY,0.0))
pt4= points.newPoint(geom.Pos3d(0.0,CooMaxY,0.0))
surfaces= preprocessor.getMultiBlockTopology.getSurfaces
s= surfaces.newQuadSurfacePts(pt1.tag,pt2.tag,pt3.tag,pt4.tag)
s.nDivI= NumDivI
s.nDivJ= NumDivJ


s.genMesh(xc.meshDir.I)

# Constraints
# Edge iterator
for l in s.getSides: # Iterate through the perimeter.
    for i in l.getEdge.getNodeTags():
        modelSpace.fixNode000_FFF(i)

# Loads definition
lp0= modelSpace.newLoadPattern(name= '0')

nNodes= s.getNumNodes

node= s.getNodeIJK(1, int(NumDivI/2+1), int(NumDivJ/2+1))
# print("Central node: ", node.tag)
# print("Central node coordinates: ", node.getCoo)
lp0.newNodalLoad(node.tag,xc.Vector([0,0,-ptLoad,0,0,0])) # Concentrated load


nElems= s.getNumElements
# We add the load case to domain.
modelSpace.addLoadCaseToDomain(lp0.name)


# Solution procedure
analysis= predefined_solutions.simple_static_linear(feProblem)
analOk= analysis.analyze(1)

node= s.getNodeIJK(1, int(NumDivI/2+1), int(NumDivJ/2+1))
# print("Central node: ", node.tag)
# print("Central node coordinates: ", node.getCoo)
# print("Central node displacements: ", node.getDisp)
UZ= node.getDisp[2]


UZTeor= -11.6
ratio1= (abs((UZ-UZTeor)/UZTeor))
ratio2= (abs((nElems-64)/64))

''' 
print("UZ= ",UZ)
print("Number of nodes: ",nNodes)
print("Number of elements: ",nElems)
print("ratio1: ",ratio1)
   '''

import os
from misc_utils import log_messages as lmsg
fname= os.path.basename(__file__)
if (abs(ratio1)<6e-3) & (abs(ratio2)<1e-9):
    print('test '+fname+': ok.')
else:
    lmsg.error(fname+' ERROR.')

# # Graphic stuff.
# from postprocess import output_handler
# oh= output_handler.OutputHandler(modelSpace)
# oh.displayFEMesh()
# oh.displayLocalAxes()
# oh.displayReactions()
# oh.displayDispRot(itemToDisp='uX', defFScale= 10.0)
# oh.displayDispRot(itemToDisp='uY', defFScale= 10.0)
