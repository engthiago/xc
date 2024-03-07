# -*- coding: utf-8 -*-
from __future__ import print_function
from __future__ import division

__author__= "Luis C. Pérez Tato (LCPT) and Ana Ortega (AOO)"
__copyright__= "Copyright 2015, LCPT and AOO"
__license__= "GPL"
__version__= "3.0"
__email__= "l.pereztato@gmail.com"

'''Analysis of PDelta effect in a frame.'''
# Taken from example «PDelta-3» of the Ansys verification manual.

# This example is very similar to the example in "test_pdelta_01.xc" but
# here we use two consecutive linear analysis (so a second order analysis)
# instead a non-linear analysis.


B= 100 # Frame width expressed in inches
H= 100 # Frame height expressed in inches
# Section mass properties (IPE-80)
Es= 29e6 # Young modulus (psi)
area= 1 # Section area expressed in in2
Iz= 1/12 # Cross-section moment of inertia expressed in in4
P= 1000 # Load (force-pounds).
nDivLines= 8 # Load (force-pounds).
offset= 2*B # Load (force-pounds).

import geom
import xc
from solution import predefined_solutions
from model import predefined_spaces
from materials import typical_materials

feProblem= xc.FEProblem()
preprocessor=  feProblem.getPreprocessor
nodes= preprocessor.getNodeHandler
modelSpace= predefined_spaces.StructuralMechanics2D(nodes)


pd= modelSpace.newPDeltaCrdTransf("pd")

# Materials definition
scc= typical_materials.defElasticSection2d(preprocessor, "scc",area,Es,Iz)

seedElemHandler= preprocessor.getElementHandler.seedElemHandler
seedElemHandler.defaultMaterial= scc.name
seedElemHandler.defaultTransformation= pd.name
beam2d= seedElemHandler.newElement("ElasticBeam2d")

points= preprocessor.getMultiBlockTopology.getPoints
pt1= points.newPoint(geom.Pos3d(0.0,0.0,0))
pt2= points.newPoint(geom.Pos3d(0.0,H,0))
pt3= points.newPoint(geom.Pos3d(B,0.0,0))
pt4= points.newPoint(geom.Pos3d(B,H,0))
pt5= points.newPoint(geom.Pos3d(offset,0.0,0))
pt6= points.newPoint(geom.Pos3d(offset,H,0))
pt7= points.newPoint(geom.Pos3d(offset+B,0.0,0))
pt8= points.newPoint(geom.Pos3d(offset+B,H,0))

lines= preprocessor.getMultiBlockTopology.getLines
l1= lines.newLine(pt1.tag,pt2.tag)
l1.nDiv= nDivLines
l2= lines.newLine(pt2.tag,pt4.tag)
l2.nDiv= nDivLines*2
l3= lines.newLine(pt4.tag,pt3.tag)
l3.nDiv= nDivLines
l4= lines.newLine(pt5.tag,pt6.tag)
l4.nDiv= nDivLines
l5= lines.newLine(pt6.tag,pt8.tag)
l5.nDiv= nDivLines*2
l6= lines.newLine(pt8.tag,pt7.tag)
l6.nDiv= nDivLines

setTotal= preprocessor.getSets.getSet("total")
setTotal.genMesh(xc.meshDir.I)

pt1.getNode().fix(xc.ID([0,1,2]),xc.Vector([0.0,0.0,0.0]))
pt3.getNode().fix(xc.ID([0,1,2]),xc.Vector([0.0,0.0,0.0]))
pt5.getNode().fix(xc.ID([0,1,2]),xc.Vector([0.0,0.0,0.0]))
pt7.getNode().fix(xc.ID([0,1,2]),xc.Vector([0.0,0.0,0.0]))

nodeTag2= 0.0
nodeTag6= 0.0
nodeTag8= 0.0
nodeTagCentralLoad= 0.0
nodeTagLateralLoad= 0.0
tagElem1= 0.0
tagElem2= 0.0
tagElem4= 0.0
tagElem5D= 0.0
tagElem5F= 0.0
tagElem6= 0.0


nodeTag2= pt2.getTagNode
nodeTag6= pt6.getTagNode
nodeTag8= pt8.getTagNode

mesh= feProblem.getDomain.getMesh
nodeTagCentralLoad= mesh.getNearestNode(geom.Pos3d(B/2,H,0.0)).tag
nodeTagLateralLoad= mesh.getNearestNode(geom.Pos3d(offset+B/4,H,0.0)).tag
tagElem1= mesh.getNearestElement(geom.Pos3d(0,H/(10*nDivLines),0.0)).tag
tagElem2= mesh.getNearestElement(geom.Pos3d(B/(10*nDivLines),H,0.0)).tag
tagElem4= mesh.getNearestElement(geom.Pos3d(offset,H/(10*nDivLines),0.0)).tag
tagElem5D= mesh.getNearestElement(geom.Pos3d(offset+B/(10*nDivLines),H,0.0)).tag
tagElem5F= mesh.getNearestElement(geom.Pos3d(offset+B-B/(10*nDivLines),H,0.0)).tag
tagElem6= mesh.getNearestElement(geom.Pos3d(offset+B,H/(10*nDivLines),0.0)).tag


# Load definition.
lp0= modelSpace.newLoadPattern(name= '0')

lp0.newNodalLoad(nodeTagCentralLoad,xc.Vector([0,-P,0]))
lp0.newNodalLoad(nodeTagLateralLoad,xc.Vector([0,-P,0]))
# We add the load case to domain.
modelSpace.addLoadCaseToDomain(lp0.name)

# Solution procedure

analysis= predefined_solutions.simple_static_linear(feProblem)
result= analysis.analyze(1)
result= analysis.analyze(1)

theta2= 0.0 # Rotation of the node 2
theta2Teor= -0.09192 # Theorethical value of the rotation of the node 2
ratioTheta2= 0.0
M12= 0.0 # Bending moment acting on element 1 front end.
M12Teor= 4606.6 # Theoretical value of bending moment
                # acting on element 1 front end.
ratioM12= 0.0
M21= 0.0 # bending moment acting on back end of the element 2
M21Teor= -8254.0 # Theoretical value of bending moment acting on back end of the element 2
ratioM21= 0.0
Q= 0.0 # Shear on element 1
QTeor= 128.6 # Theoretical value of shear on element 1
ratioQ= 0.0
delta6= 0.0 # Desplazamiento horizontal of the node 6
delta6Teor= 1.893 # Theoretical value of desplazamiento horizontal of the node 6
ratioDelta6= 0.0
theta6= 0.0 # Rotation of the node 6
theta6Teor= -0.1013 # Theorethical value of the rotation of the node 6
ratioTheta6= 0.0
theta8= 0.0 # Rotation of the node 6
theta8Teor= 0.0367 # Theorethical value of the rotation of the node 6
ratioTheta8= 0.0
M61= 0.0 # Bending moment acting on back end of element 4
M61Teor= 2544.9 # Theoretical value of bending moment acting
                # on back end of element 4.
ratioM61= 0.0
Q4= 0.0 # Shear on element 1
Q4Teor= 101.4 # Theoretical value of shear on element 1
ratioQ4= 0.0
M51= 0.0 # bending moment acting on back end of element 5
M51Teor= -6088.6 # Theoretical value of bending moment
                 # acting on back end of the element 5
ratioM51= 0.0
M71= 0.0 # Bending moment acting on back end of element 6
M71Teor= 4456.9 # Theoretical value of bending moment
                # acting on back end of element 6
ratioM71= 0.0
M52= 0.0 # bending moment acting on front end of the element 5
M52Teor= -6153.0 # Theoretical value of bending moment
                 # acting on front end of the element 5.
ratioM52= 0.0


nodes= preprocessor.getNodeHandler
nodT2= nodes.getNode(nodeTag2)
theta2= nodT2.getDisp[2]
nodT6= nodes.getNode(nodeTag6)
delta6= nodT6.getDisp[0]
theta6= nodT6.getDisp[2]
nodT8= nodes.getNode(nodeTag8)
theta8= nodT8.getDisp[2]

elements= preprocessor.getElementHandler
eletagElem1= elements.getElement(tagElem1)
eletagElem1.getResistingForce()
Q= eletagElem1.getV1
M12= eletagElem1.getM1

eletagElem2= elements.getElement(tagElem2)
eletagElem2.getResistingForce()
M21= eletagElem2.getM1

eletagElem4= elements.getElement(tagElem4)
eletagElem4.getResistingForce()
Q4= eletagElem4.getV1
M61= eletagElem4.getM1

eletagElem5D= elements.getElement(tagElem5D)
eletagElem5D.getResistingForce()
M51= eletagElem5D.getM1

eletagElem5F= elements.getElement(tagElem5F)
eletagElem5F.getResistingForce()
M52= eletagElem5F.getM2

eletagElem6= elements.getElement(tagElem6)
eletagElem6.getResistingForce()
M71= eletagElem6.getM2

ratioTheta2= abs((theta2-theta2Teor)/theta2Teor)
ratioDelta6= abs((delta6-delta6Teor)/delta6Teor)
ratioM12= abs((M12-M12Teor)/M12Teor)
ratioM21= abs((M21-M21Teor)/M21Teor)
ratioQ= abs((Q-QTeor)/QTeor)
ratioTheta6= abs((theta6-theta6Teor)/theta6Teor)
ratioTheta8= abs((theta8-theta8Teor)/theta8Teor)
ratioM61= abs((M61-M61Teor)/M61Teor)
ratioQ4= abs((Q4-Q4Teor)/Q4Teor)
ratioM51= abs((M51-M51Teor)/M51Teor)
ratioM71= abs((M71-M71Teor)/M71Teor)
ratioM52= abs((M52-M52Teor)/M52Teor)

'''
print("theta2= ",theta2)
print("theta2Teor= ",theta2Teor)
print("ratioTheta2= ",ratioTheta2)
print("M12= ",M12)
print("M12Teor= ",M12Teor)
print("ratioM12= ",ratioM12)
print("M21= ",M21)
print("M21Teor= ",M21Teor)
print("ratioM21= ",ratioM21)
print("Q= ",Q)
print("QTeor= ",QTeor)
print("ratioQ= ",ratioQ)
print("delta6= ",delta6)
print("delta6Teor= ",delta6Teor)
print("ratioDelta6= ",ratioDelta6)
print("theta6= ",theta6)
print("theta6Teor= ",theta6Teor)
print("ratioTheta6= ",ratioTheta6)
print("theta8= ",theta8)
print("theta8Teor= ",theta8Teor)
print("ratioTheta8= ",ratioTheta8)
print("M61= ",M61)
print("M61Teor= ",M61Teor)
print("ratioM61= ",ratioM61)
print("Q4= ",Q4)
print("Q4Teor= ",Q4Teor)
print("ratioQ4= ",ratioQ4)
print("M51= ",M51)
print("M51Teor= ",M51Teor)
print("ratioM51= ",ratioM51)
print("M71= ",M71)
print("M71Teor= ",M71Teor)
print("ratioM71= ",ratioM71)
print("M52= ",M52)
print("M52Teor= ",M52Teor)
print("ratioM52= ",ratioM52)
 '''

import os
from misc_utils import log_messages as lmsg
fname= os.path.basename(__file__)
if (ratioTheta2<1e-2) & (ratioDelta6<1e-2) & (ratioM12<0.05) & (ratioM21<2e-2) & (ratioQ<0.06) & (ratioTheta6<1e-2) & (ratioTheta8<1e-2) & (ratioM61<0.05) & (ratioQ4<0.06) & (ratioM51<0.05) & (ratioM71<0.05) & (ratioM52<0.05):
    print('test '+fname+': ok.')
else:
    lmsg.error(fname+' ERROR.')
