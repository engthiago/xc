# -*- coding: utf-8 -*-
''' Trivial check of 3D block.'''

from __future__ import division
from __future__ import print_function

import math
import geom
import xc
from model import predefined_spaces
from materials import typical_materials

__author__= "Luis C. Pérez Tato (LCPT)"
__copyright__= "Copyright 2024, LCPT"
__license__= "GPL"
__version__= "3.0"
__email__= "l.pereztato@gmail.com"

feProblem= xc.FEProblem()
preprocessor=  feProblem.getPreprocessor
nodes= preprocessor.getNodeHandler
# Problem type
modelSpace= predefined_spaces.SolidMechanics3D(nodes)

# Material definition
elast3d= typical_materials.defElasticIsotropic3d(preprocessor, "elast3d", 1e6,0.25, 0.0)

#
#     5 +---------+ 8     0: Bottom face; vertices 1,4,3,2. (0,3,2,1)
#      /|        /|       1: Left-side face; vertices 1,2,6,5. (0,1,5,4)
#     / |       / |       2: Front face; vertices 2,3,7,6. (1,2,6,5)
#  6 +---------+7 |       3: Right-side face; vertices 3,4,8,7. (2,3,7,6)
#    |  |      |  |       4: Back face; vertices 1,5,8,4. (0,4,7,3)
#    |1 +------|--+ 4     5: Top face; vertices 5,6,7,8. (4,5,6,7)
#    | /       | /
#    |/        |/
#  2 +---------+ 3
#

# Define k-points.
pt1= modelSpace.newKPoint(0,0,0)
pt2= modelSpace.newKPoint(1,0,0)
pt3= modelSpace.newKPoint(1,1,0)
pt4= modelSpace.newKPoint(0,1,0)

pt5= modelSpace.newKPoint(0,0,1)
pt6= modelSpace.newKPoint(1,0,1)
pt7= modelSpace.newKPoint(1,1,1)
pt8= modelSpace.newKPoint(0,1,1)

pt9= modelSpace.newKPoint(0,0,2)
pt10= modelSpace.newKPoint(1,0,2)
pt11= modelSpace.newKPoint(1,1,2)
pt12= modelSpace.newKPoint(0,1,2)

# Define block.
bodies= preprocessor.getMultiBlockTopology.getBodies
b1= bodies.newBlockPts(pt1.tag, pt2.tag, pt3.tag, pt4.tag, pt5.tag, pt6.tag, pt7.tag, pt8.tag)
b1.nDivI= 1
b1.nDivJ= 1
b1.nDivK= 1
b2= bodies.newBlockPts(pt5.tag, pt6.tag, pt7.tag, pt8.tag, pt9.tag, pt10.tag, pt11.tag, pt12.tag)
vl= b2.getVerbosityLevel() # Get current verbosity level.
b2.setVerbosityLevel(0) # Don't bother with warning messages
                        # that will be solved.
b2.nDivI= 1
b2.nDivJ= 1
b2.nDivK= 1
b2.setVerbosityLevel(vl) # Restore previous verbosity level.

diffVector= geom.Vector3d(0,0,1)
posErr= 0.0 
positions1= b1.getPositions()
positions2= b2.getPositions()

for l1, l2 in zip(positions1, positions2):
    for m1, m2 in zip(l1, l2):
        for p1, p2 in zip(m1, m2):
            posErr+= (p2-p1-diffVector).getModulus2()
posErr= math.sqrt(posErr)


# Get the vertices with the block internal order.
vertices1= b1.getVertices()
internalOrderOfVertices1= [pt1, pt4, pt3, pt2, pt5, pt6, pt7, pt8]
vDiff= 0
for v1, v2 in zip(vertices1, internalOrderOfVertices1):
    vDiff+= abs(v1.tag-v2.tag)
vertices2= b2.getVertices()
internalOrderOfVertices2= [pt5, pt8, pt7, pt6, pt9, pt10, pt11, pt12]
for v1, v2 in zip(vertices2, internalOrderOfVertices2):
    vDiff+= abs(v1.tag-v2.tag)


# Compute block volume
volume1= b1.getVolume()
volume2= b2.getVolume()
ratio1= math.sqrt((volume1-1)**2+(volume2-1)**2)

# Mesh definition.
## Seed element.
seedElemHandler= preprocessor.getElementHandler.seedElemHandler
seedElemHandler.defaultMaterial= elast3d.name
brick= seedElemHandler.newElement("Brick")

xcTotalSet= modelSpace.getTotalSet()
xcTotalSet.genMesh(xc.meshDir.I)

refCentroids= [geom.Pos3d(0.5, 0.5, 0.5), geom.Pos3d(0.5, 0.5, 1.5)]
err= 0.0
for e, refCen in zip(b1.elements, refCentroids):
    centroid= e.getPosCentroid(True)
    err+= centroid.dist2(refCen)

err= math.sqrt(err)

import os
from misc_utils import log_messages as lmsg
fname= os.path.basename(__file__)
if((err<=1e-8) and (ratio1<=1e-8) and (posErr<1e-8)):
    print("test "+fname+": ok.")
else:
    lmsg.error(fname+' ERROR.')

# #Graphic stuff.
# from postprocess import output_handler
# oh= output_handler.OutputHandler(modelSpace)

# # oh.displayBlocks()
# oh.displayFEMesh()
# oh.displayLocalAxes()
# # oh.displayLoads()
# # oh.displayReactions()
# # oh.displayDispRot(itemToDisp='uX')
# # oh.displayDispRot(itemToDisp='uY')
# # oh.displayDispRot(itemToDisp='uZ')
