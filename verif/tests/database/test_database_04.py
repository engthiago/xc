# -*- coding: utf-8 -*-
''' Home made test.'''

from __future__ import print_function

__author__= "Luis C. Pérez Tato (LCPT)"
__copyright__= "Copyright 2014, LCPT"
__license__= "GPL"
__version__= "3.0"
__email__= "l.pereztato@gmail.com"

import xc
import math
from model import predefined_spaces
from materials import typical_materials

E= 2.1e6 # Young modulus of the steel en kg/cm2.
nu= 0.3 # Poisson's ratio.
h= 0.1 # Thickness.
dens= 1.33 # Density kg/m2.


v1= xc.Vector([0,math.sqrt(2)/2,math.sqrt(2)/2])
v2= xc.Vector([0,-math.sqrt(2)/2,math.sqrt(2)/2])

# Problem type
feProblem= xc.FEProblem()
preprocessor=  feProblem.getPreprocessor
nodes= preprocessor.getNodeHandler
modelSpace= predefined_spaces.StructuralMechanics3D(nodes)
n1= nodes.newNodeXYZ(0,0,0)
n2= nodes.newNodeXYZ(2,0,0)
n3= nodes.newNodeXYZ(2,1,1)
n4= nodes.newNodeXYZ(0,1,1)


# Materials definition

memb1= typical_materials.defElasticMembranePlateSection(preprocessor, "memb1",E,nu,dens,h)


elements= preprocessor.getElementHandler
elements.defaultMaterial= memb1.name
elem= elements.newElement("ShellMITC4",xc.ID([n1.tag,n2.tag,n3.tag,n4.tag]))

import os
os.system("rm -f /tmp/test04.db")
db= feProblem.newDatabase("SQLite","/tmp/test04.db")
db.save(100)
feProblem.clearAll()
db.restore(100)

ratio1= (elem.getCoordTransf.getG2Vector-v1).Norm()
ratio2= (elem.getCoordTransf.getG3Vector-v2).Norm()
#ratio1= abs((abs(elem.getCoordTransf.getG2Vector-v1)))
#ratio2= abs((abs(elem.getCoordTransf.getG3Vector-v2)))




''' 
print("v1= ",v1)
print("v2= ",v2)
print("ratio1= ",ratio1)
print("ratio2= ",ratio2)
 '''

import os
from misc_utils import log_messages as lmsg
fname= os.path.basename(__file__)
if (ratio1 < 1e-12) & (ratio2 < 1e-12):
    print('test '+fname+': ok.')
else:
    lmsg.error(fname+' ERROR.')
os.system("rm -f /tmp/test04.db") # Your garbage you clean it

