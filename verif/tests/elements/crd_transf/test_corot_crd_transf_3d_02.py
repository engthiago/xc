# -*- coding: utf-8 -*-
# home made test

from __future__ import print_function
from __future__ import division
import xc_base
import geom
import xc
from model import predefined_spaces
from materials import typical_materials

__author__= "Luis C. Pérez Tato (LCPT) and Ana Ortega (AOO)"
__copyright__= "Copyright 2015, LCPT and AOO"
__license__= "GPL"
__version__= "3.0"
__email__= "l.pereztato@gmail.com"

# Problem type
feProblem= xc.FEProblem()
preprocessor=  feProblem.getPreprocessor   
nodes= preprocessor.getNodeHandler
modelSpace= predefined_spaces.StructuralMechanics3D(nodes)
nodes.defaultTag= 1 #First node number.
nod= nodes.newNodeXYZ(0,0,0)
nod= nodes.newNodeXYZ(5,5,5)

# Materials
section= typical_materials.defElasticSection3d(preprocessor, "section",1,1,1,1,1,1)

# Geometric transformations
corot= modelSpace.newCorotCrdTransf("corot",xc.Vector([0,1,0]))    
# Elements definition
elements= preprocessor.getElementHandler

elements.defaultTransformation= corot.name # Coordinate transformation for the new elements
elements.defaultMaterial= section.name
elements.defaultTag= 1 #Tag for the next element.
beam3d= elements.newElement("ElasticBeam3d",xc.ID([1,2]))

crdTransf= beam3d.getCoordTransf

# \print{"vector I:",getIVector
#print("vector J:",getJVector)
#print("vector K:",getKVector})
vIGlobal= crdTransf.getVectorGlobalCoordFromLocal(xc.Vector([1,0,0]))
vJGlobal= crdTransf.getVectorGlobalCoordFromLocal(xc.Vector([0,1,0]))
vKGlobal= crdTransf.getVectorGlobalCoordFromLocal(xc.Vector([0,0,1]))
# \print{"vector I en globales:",vIGlobal
#print("vector J en globales:",vJGlobal)
#print("vector K en globales:",vKGlobal})
ratio1= (vIGlobal-crdTransf.getIVector).Norm()
ratio2= (vJGlobal-crdTransf.getJVector).Norm()
ratio3= (vKGlobal-crdTransf.getKVector).Norm()




    


import os
from misc_utils import log_messages as lmsg
fname= os.path.basename(__file__)
if (abs(ratio1)<1e-12) & (abs(ratio2)<1e-12) & (abs(ratio3)<1e-12):
    print('test '+fname+': ok.')
else:
    lmsg.error(fname+' ERROR.')
