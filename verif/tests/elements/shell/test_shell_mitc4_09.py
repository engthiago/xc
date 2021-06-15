# -*- coding: utf-8 -*-
from __future__ import print_function
'''Verification test taken from example 2-005 of 
   the SAP 2000 verification manual.'''


__author__= "Luis C. Pérez Tato (LCPT) and Ana Ortega (AOO)"
__copyright__= "Copyright 2015, LCPT and AOO"
__license__= "GPL"
__version__= "3.0"
__email__= "l.pereztato@gmail.com"

# feProblem.setVerbosityLevel(0)
NumDivI= 32
NumDivJ= 32
CooMaxX= 10
CooMaxY= 2
E= 17472000 # Elastic modulus en lb/in2
nu= 0.3 # Poisson's ratio
G= 6720000
thickness= 0.0001 # Cross section depth expressed in inches.
unifLoad= 0.0001 # Uniform load in lb/in2.
ptLoad= 0.0004 # Punctual load in lb.

import xc_base
import geom
import xc
from solution import predefined_solutions
from model import predefined_spaces
from materials import typical_materials

# Problem type
feProblem= xc.FEProblem()
preprocessor=  feProblem.getPreprocessor
nodes= preprocessor.getNodeHandler

modelSpace= predefined_spaces.StructuralMechanics3D(nodes)
# Define materials
elast= typical_materials.defElasticMaterial(preprocessor, "elast",E)



# Define materials
memb1= typical_materials.defElasticMembranePlateSection(preprocessor, "memb1",E,nu,0.0,thickness)



seedElemHandler= preprocessor.getElementHandler.seedElemHandler
seedElemHandler.defaultMaterial= memb1.name
seedElemHandler.defaultTag= 1
elem= seedElemHandler.newElement("ShellMITC4",xc.ID([0,0,0,0]))



points= preprocessor.getMultiBlockTopology.getPoints
pt= points.newPntIDPos3d(1,geom.Pos3d(0.0,0.0,0.0))
pt= points.newPntIDPos3d(2,geom.Pos3d(CooMaxX,0.0,0.0))
pt= points.newPntIDPos3d(3,geom.Pos3d(CooMaxX,CooMaxY,0.0))
pt= points.newPntIDPos3d(4,geom.Pos3d(0.0,CooMaxY,0.0))
surfaces= preprocessor.getMultiBlockTopology.getSurfaces
surfaces.defaultTag= 1
s= surfaces.newQuadSurfacePts(1,2,3,4)
s.nDivI= NumDivI
s.nDivJ= NumDivJ



f1= preprocessor.getSets.getSet("f1")
f1.genMesh(xc.meshDir.I)
sides= s.getSides
# Edge iterator
for l in sides:
  for i in l.getEdge.getNodeTags():
    modelSpace.fixNode000_FFF(i)

# Load definition.
lp0= modelSpace.newLoadPattern(name= '0')



f1= preprocessor.getSets.getSet("f1")
nNodes= f1.getNumNodes
 
node= f1.getNodeIJK(1, int(NumDivI/2+1), int(NumDivJ/2+1))
# print("Central node: ", node.tag)
# print("Central node coordinates: ", node.getCoo)
lp0.newNodalLoad(node.tag,xc.Vector([0,0,-ptLoad,0,0,0])) # Concentrated load


nElems= f1.getNumElements
# We add the load case to domain.
modelSpace.addLoadCaseToDomain(lp0.name)


# Solution procedure
analysis= predefined_solutions.simple_static_linear(feProblem)
analOk= analysis.analyze(1)

f1= preprocessor.getSets.getSet("f1")

nodes= preprocessor.getNodeHandler

node= f1.getNodeIJK(1, int(NumDivI/2+1), int(NumDivJ/2+1))
# print("Central node: ", node.tag)
# print("Central node coordinates: ", node.getCoo)
# print("Central node displacements: ", node.getDisp)
UZ= node.getDisp[2]


UZTeor= -16.95
ratio1= (abs((UZ-UZTeor)/UZTeor))
ratio2= (abs((nElems-1024)/1024))

''' 
print("UZ= ",UZ)
print("Number of nodes: ",nNodes)
print("Number of elements: ",nElems)
print("ratio1: ",ratio1)
print("ratio2: ",ratio2)
   '''

import os
from misc_utils import log_messages as lmsg
fname= os.path.basename(__file__)
if (abs(ratio1)<7e-3) & (abs(ratio2)<1e-9):
    print('test '+fname+': ok.')
else:
    lmsg.error(fname+' ERROR.')
