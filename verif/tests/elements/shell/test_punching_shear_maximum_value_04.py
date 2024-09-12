# -*- coding: utf-8 -*-
''' Punching shear. Computation of the maximum value of the existing shear force in the control perimeter to be used for the punching shear design.'''

from __future__ import print_function

__author__= "Luis C. Pérez Tato (LCPT) , Ana Ortega (AO_O)"
__copyright__= "Copyright 2024, LCPT  AO_O"
__license__= "GPL"
__version__= "3.0"
__email__= "l.pereztato@ciccp.es ana.Ortega@ciccp.es"


import math
import geom
import xc
from model import predefined_spaces
from materials.ehe import EHE_materials
from materials.sections import section_properties as sp
from solution import predefined_solutions
from postprocess.quick_inquiry import element_resisting_forces as erf

feProblem= xc.FEProblem()
preprocessor=  feProblem.getPreprocessor
nodes= preprocessor.getNodeHandler

# Problem type
modelSpace= predefined_spaces.StructuralMechanics3D(nodes)

halfSide= 2.5
## Problem geometry.
#
#                      p9
# p8 +----------------+
#    |                |
#    |                |
#    |                |
#    |   slabs[0]     |
#    |                |
#    |                |
#    |                |
#    +----------------+
#   p1                p5
#
### column bottom.
p0= modelSpace.newKPoint(0,0,0)
### corners.
p1= modelSpace.newKPoint(-halfSide, -halfSide, 5)
### mid-points
p5= modelSpace.newKPoint(0, -halfSide, 5)
p8= modelSpace.newKPoint(-halfSide, 0, 5)
### center
p9= modelSpace.newKPoint(0, 0, 5)

### column.
lColumn= modelSpace.newLine(p0, p9)
### slab
slabs= list()
slabs.append(modelSpace.newQuadSurface(p1, p5, p9, p8))
for s in slabs:
    s.setElemSizeIJ(.2, .2)
slabArea= len(slabs)*halfSide**2

## Define sets.
slabSet= modelSpace.defSet(setName= 'slabSet', surfaces= slabs)
columnSet= modelSpace.defSet(setName= 'columnSet', lines= [lColumn])

## Define materials.
concrete= EHE_materials.HA30
slabDepth= 0.35 # thickness.
slabEffectiveDepth= 0.9*slabDepth 
slabMaterial= concrete.defElasticMembranePlateSection(preprocessor= preprocessor, name= "slabMaterial", thickness= slabDepth)
columnWidth= 0.4
columnDepth= 0.6
columnMaterial= concrete.defElasticShearSection3d(preprocessor= preprocessor, sectionProperties= sp.RectangularSection(name= None, b= columnWidth, h= columnDepth))

## Generate mesh.
### Slab mesh.
modelSpace.setDefaultMaterial(slabMaterial)
modelSpace.newSeedElement('ShellMITC4')
slabSet.genMesh(xc.meshDir.I)
### Column mesh.
lin= modelSpace.newLinearCrdTransf("lin",xc.Vector([0,1,0])) # Coord. transf.
modelSpace.setDefaultCoordTransf(lin)
modelSpace.setDefaultMaterial(columnMaterial)
modelSpace.newSeedElement('ElasticBeam3d')
lColumn.genMesh(xc.meshDir.I)
columnSet.fillDownwards()

### Constraints.
modelSpace.fixNode(DOFpattern= '000_000', nodeTag= p0.getNode().tag)

### Loads.
lp0= modelSpace.newLoadPattern(name= '0', setCurrent= True)
uniformLoad= -20e3
loadVector= xc.Vector([0, 0, uniformLoad])
for e in slabSet.elements:
    e.vector3dUniformLoadGlobal(loadVector)
modelSpace.addLoadCaseToDomain(lp0.name)

# Solve
analysis= predefined_solutions.simple_static_linear(feProblem)
analOk= analysis.analyze(1)

# Compute critical perimeter.
## Get strong and weak axes.
columnElement= lColumn.elements[0]
columnStrongAxis= geom.Vector3d(columnElement.getVDirStrongAxisGlobalCoord(True).getList()) # initialGeometry= True
columnWeakAxis= geom.Vector3d(columnElement.getVDirWeakAxisGlobalCoord(True).getList()) # initialGeometry= True
## Compute column contour
pA= p9.getPos-columnWidth*columnStrongAxis-columnDepth*columnWeakAxis
pB= p9.getPos+columnWidth*columnStrongAxis-columnDepth*columnWeakAxis
pC= p9.getPos+columnWidth*columnStrongAxis+columnDepth*columnWeakAxis
pD= p9.getPos-columnWidth*columnStrongAxis+columnDepth*columnWeakAxis
columnContour= [pA, pB, pC, pD]
## Compute mid-plane of the slab
slabMidPlane= slabSet.nodes.getRegressionPlane(1.0)
## Project the column contour on the mid-plane of the slab.
projVertices= slabMidPlane.getProjection(columnContour)
projColumnContour= geom.Polygon3d(projVertices)

criticalContour= projColumnContour.getBuffer(2.0*slabEffectiveDepth)
criticalContourArea= criticalContour.getArea()/4.0 # Only a quarter of the contour lies inside the slab.

# Compute contour control positions
controlPositions= erf.ContourControlPositions(contour= criticalContour, elemSet= slabSet, tol= 0.005)

criticalPerimeterForces= controlPositions.computeContourForces()

# Compute vertical force
Fz= 0.0
for nTag in criticalPerimeterForces:
    Fz+= criticalPerimeterForces[nTag]['sliding_vector_system'].getResultant().z

loadInsideCriticalContour= uniformLoad*criticalContourArea
totalLoad= uniformLoad*slabArea
FzRef= totalLoad-loadInsideCriticalContour
error= FzRef-Fz
relError= error/FzRef
testOK= abs(relError)<0.06

columnElement.getResistingForce()
FzRef2= columnElement.getN1-loadInsideCriticalContour
errorB= abs(FzRef-FzRef2)/FzRef
testOK= testOK and (errorB<1e-6)

'''
print('Fz= ', Fz/1e3)
print('FzRef= ', FzRef/1e3)
print('error= ', error/1e3)
print('relError= ', relError*100,'%')
print('FzRef2= ', FzRef2/1e3)
print('errorB= ', errorB)
'''

import os
from misc_utils import log_messages as lmsg
fname= os.path.basename(__file__)
if testOK:
    print('test '+fname+': ok.')
else:
    lmsg.error(fname+' ERROR.')

# # Graphic stuff.
# elementsOnCriticalContour= controlPositions.elementsOnContourBorder
# from postprocess import output_handler
# oh= output_handler.OutputHandler(modelSpace)
# # oh.displayBlocks()
# oh.displayFEMesh(setsToDisplay= [slabSet, elementsOnCriticalContour])
# # oh.displayLocalAxes()
# # oh.displayStrongWeakAxis(setToDisplay= columnSet)
# # oh.displayLoads()
# # oh.displayReactions()
# # oh.displayDispRot(itemToDisp='uX', defFScale= 10.0)
# # oh.displayDispRot(itemToDisp='uY', defFScale= 10.0)

# import matplotlib.pyplot as plt
# from mpl_toolkits.mplot3d import Axes3D
# X= list()
# Y= list()
# Z= list()
# Fx= list()
# Fy= list()
# Fz= list()

# xmin= 6.023e23
# xmax= -xmin
# ymin= xmin
# ymax= xmax
# zmin= xmin
# zmax= xmax
# for nTag in criticalPerimeterForces:
#     values= criticalPerimeterForces[nTag]
#     svs= values['sliding_vector_system']
#     F= svs.getResultant()
#     P= svs.getOrg()
#     X.append(P.x)
#     Y.append(P.y)
#     Z.append(P.z)
#     tributaryLength= values['tributary_length']
#     fx= F.x/1e3/tributaryLength
#     fy= F.y/1e3/tributaryLength
#     fz= F.z/1e3/tributaryLength
#     Fx.append(fx)
#     Fy.append(fy)
#     Fz.append(fz)
#     xmax= max(xmax, P.x)
#     xmin= min(xmin, P.x)
#     xmax= max(xmax, P.x+fx)
#     xmin= min(xmin, P.x+fx)
#     ymax= max(ymax, P.y)
#     ymin= min(ymin, P.y)
#     ymax= max(ymax, P.y+fy)
#     ymin= min(ymin, P.y+fy)
#     zmax= max(zmax, P.z)
#     zmin= min(zmin, P.z)
#     zmax= max(zmax, P.z+fz)
#     zmin= min(zmin, P.z+fz)

# fig = plt.figure()
# ax = fig.add_subplot(111, projection='3d')
# ax.quiver(X, Y, Z, Fx, Fy, Fz, arrow_length_ratio = 0.001)
# ax.set_xlim([xmin, xmax])
# ax.set_ylim([ymin, ymax])
# ax.set_zlim([zmin, zmax])
# plt.show()
