# -*- coding: utf-8 -*-
'''Gusset plate mesh generation using paving algorithm. Home cooked test.'''

from __future__ import division
from __future__ import print_function

__author__= "Luis C. Pérez Tato (LCPT) and Ana Ortega (AOO)"
__copyright__= "Copyright 2020, LCPT and AOO"
__license__= "GPL"
__version__= "3.0"
__email__= "l.pereztato@gmail.com"

import math
import geom
import xc
from materials.astm_aisc import ASTM_materials
from model import predefined_spaces
from materials import typical_materials
# import matplotlib.pyplot as plt
# from postprocess import output_handler

def round_up_to_even(f):
    return math.ceil(f / 2.) * 2

def getIntermediatePoints(corners, elemSide):
    ''' Return a series of intermediate points uniformly distributed
        on the sides of the polygon argument.

        corners: list of point that define the original contour.
        elemSide: length of the element side.
    ''' 
    pts= list()
    p0= corners[0]
    for p in corners[1:]:
        sg= geom.Segment3d(p0,p)
        nDiv= int(round_up_to_even(sg.getLength()/elemSide))
        pts.extend(sg.Divide(nDiv)[:-1])
        p0= p
    sg= geom.Segment3d(corners[-1],corners[0])
    nDiv= int(round_up_to_even(sg.getLength()/elemSide))
    pts.extend(sg.Divide(nDiv)[:-1])
    return geom.Polygon3d(pts)

def getHoleAsPolygonalSurface(hole, surfaces):
    ''' Create the polygonal surface that represents the hole.

    :param hole: hole to create the surface for.
    :param surfaces: XC surface handler.
    '''
    plg= b.getHoleAsPolygon()
    vertices= plg.getVertexList()
    newPnts= list()
    for v in vertices:
        newPnts.append(points.newPoint(v))
    pntTags= list()
    for p in newPnts:
        pntTags.append(p.tag)
    pFace= surfaces.newPolygonalFacePts(pntTags)
    return pFace

def drawPolygons(plgList):
    ''' Draws the polygons on the argument list using matplotlib.

    plgList: list containing the polygons to draw.
    '''
    x= list()
    y= list()
    for plg in plgList:
        vtx= plg.getVertexList()
        xi= list()
        yi= list()
        for v in vtx:
            xi.append(v.x)
            yi.append(v.y)
        x.append(xi)
        y.append(yi)
    plt.figure()
    for xi, yi in zip(x,y):
        plt.plot(xi,yi) 
        plt.scatter(xi, yi)
    plt.axis('equal')
    plt.show()
    
def drawMesh(nodes, quads):
    ''' Draw the quad mesh using matplotlib.

    :param nodes: list of the positions of the nodes.
    :param quads: index of the element nodes in the
                  positions list.
    '''
    # Draw elements
    plg= list()
    for q in quads:
        qx= list()
        qy= list()
        q.append(q[0])
        for i in q:
            pos= nodPos[i]
            qx.append(pos.x)
            qy.append(pos.y)
        plg.append((qx,qy))

    plt.figure()
    for p in plg:
        xs= p[0]
        ys= p[1]
        #plt.scatter(xs, ys)
        plt.fill(xs,ys) 
    plt.axis('equal')
    plt.show()

p1= geom.Pos3d(-1.595,-1.4046,0)
p2= geom.Pos3d(-1.4376,-0.7453,0)
p3= geom.Pos3d(-1.529,-0.7047,0)
p4= geom.Pos3d(-1.595,-0.7527,0)
corners= [p1, p2, p3, p4]
#corners= [geom.Pos3d(-1.5950,-1.333,0), geom.Pos3d(-1.5779,-1.333,0), geom.Pos3d(-1.4376,-0.7453,0), geom.Pos3d(-1.529,-0.7047,0), geom.Pos3d(-1.595,-0.7527,0)]

bolts= list()

boltDiameter= 0.016
bolts.append(ASTM_materials.BoltFastener(diameter= boltDiameter, pos3d= geom.Pos3d(-1.4909,-0.8037,0.0)))
bolts.append(ASTM_materials.BoltFastener(diameter= boltDiameter, pos3d= geom.Pos3d(-1.4706,-0.7580,0.0)))
bolts.append(ASTM_materials.BoltFastener(diameter= boltDiameter, pos3d= geom.Pos3d(-1.5163,-0.7377,0.0)))
bolts.append(ASTM_materials.BoltFastener(diameter= boltDiameter, pos3d= geom.Pos3d(-1.5366,-0.7834,0.0)))

plgExt= getIntermediatePoints(corners, elemSide= 0.75*boltDiameter)
contours= [plgExt]
holes= list()
for b in bolts:
    plg= b.getHoleAsPolygon()
    holes.append(plg)
    contours.append(plg)

# drawPolygons(contours)

# Meshing directly with paving algorithm
paver= xc.Paver()
paver.mesh(plgExt, holes)
nodPos= paver.getNodePositions()
quads= paver.getQuads()

# drawMesh(nodPos, quads)

nNodPos= len(nodPos)
ratio1= (nNodPos-615)
nquads= len(quads)
ratio2= (nquads-449)

# Test paving routine inside XC modeler.

## Problem type
feProblem= xc.FEProblem()
preprocessor= feProblem.getPreprocessor
nodes= preprocessor.getNodeHandler

modelSpace= predefined_spaces.StructuralMechanics3D(nodes)

### Define k-points.
points= modelSpace.getPointHandler()

#### Exterior contour
pt1= points.newPoint(p1)
pt2= points.newPoint(p2)
pt3= points.newPoint(p3)
pt4= points.newPoint(p4)

print('A')
### Define polygonal surfaces
surfaces= modelSpace.getSurfaceHandler()
polyFace= surfaces.newPolygonalFacePts([pt1.tag, pt2.tag, pt3.tag, pt4.tag])
pFaceHoles= list()
for b in bolts:
    pFace= getHoleAsPolygonalSurface(b,surfaces)
    holes.append(pFace)
    pFaceHoles.append(pFace)

print('B')

### Define material
mat= typical_materials.defElasticMembranePlateSection(preprocessor, "mat",E=2.1e9,nu=0.3,rho= 7850,h= 0.015)

### Define template element
seedElemHandler= preprocessor.getElementHandler.seedElemHandler
seedElemHandler.defaultMaterial= mat.name
elem= seedElemHandler.newElement("ShellMITC4",xc.ID([0,0,0,0]))

### Generate mesh.
polyFace.setElemSize(0.75*boltDiameter, True)
for h in pFaceHoles:
    print('x')
    h.setNDiv(1)
    polyFace.addHole(h)
print('C')
polyFace.genMesh(xc.meshDir.I, True)
print('D')

xcTotalSet= modelSpace.getTotalSet()
nNodes= len(xcTotalSet.nodes)
nElements= len(xcTotalSet.elements)

ratio3= (nNodes-nNodPos)
ratio4= (nElements-nquads)

'''
print(nNodPos)
print(nquads)
'''

import os
from misc_utils import log_messages as lmsg
fname= os.path.basename(__file__)
if (ratio1==0) & (ratio2==0) & (ratio3==0) & (ratio4==0) :
    print("test "+fname+": ok.")
else:
    lmsg.error(fname+' ERROR.')

# Graphic stuff.
#oh= output_handler.OutputHandler(modelSpace)

#oh.displayBlocks()#setToDisplay= )
#oh.displayFEMesh()#setsToDisplay=[])
#oh.displayLocalAxes()
