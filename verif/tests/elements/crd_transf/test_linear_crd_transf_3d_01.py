# -*- coding: utf-8 -*-
''' Home made test.'''

from __future__ import print_function
from __future__ import division
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
n1= nodes.newNodeXYZ(0,0,0)
n2= nodes.newNodeXYZ(5,5,5)

# Geometric transformations
lin= modelSpace.newLinearCrdTransf("lin",xc.Vector([0,1,0]))

# Materials
section= typical_materials.defElasticSection3d(preprocessor, "section",1,1,1,1,1,1)

# Elements definition
elements= preprocessor.getElementHandler
elements.defaultTransformation= lin.name # Coordinate transformation for the new elements
elements.defaultMaterial= section.name
beam3d= elements.newElement("ElasticBeam3d",xc.ID([n1.tag,n2.tag]))

crdTransf= beam3d.getCoordTransf
# print("vector I:",getIVector)
# print("vector J:",getJVector)
# print("vector K:",getKVector)
vILocal= crdTransf.getVectorLocalCoordFromGlobal(crdTransf.getIVector)
vJLocal= crdTransf.getVectorLocalCoordFromGlobal(crdTransf.getJVector)
vKLocal= crdTransf.getVectorLocalCoordFromGlobal(crdTransf.getKVector)
# print("vector I en locales:",vILocal)
# print("vector J en locales:",vJLocal)
# print("vector K en locales:",vKLocal)

ratio1= (vILocal-xc.Vector([1,0,0])).Norm()
ratio2= (vJLocal-xc.Vector([0,1,0])).Norm()
ratio3= (vKLocal-xc.Vector([0,0,1])).Norm()

import os
from misc_utils import log_messages as lmsg
fname= os.path.basename(__file__)
if (abs(ratio1)<1e-12) & (abs(ratio2)<1e-12) & (abs(ratio3)<1e-12):
    print('test '+fname+': ok.')
else:
    lmsg.error(fname+' ERROR.')
