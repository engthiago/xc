# -*- coding: utf-8 -*-
''' Home made test.'''

from __future__ import print_function

__author__= "Luis C. Pérez Tato (LCPT) and Ana Ortega (AOO)"
__copyright__= "Copyright 2015, LCPT and AOO"
__license__= "GPL"
__version__= "3.0"
__email__= "l.pereztato@gmail.com"

L= 2.0 # Size of element edge.
t= 0.2 # Element thickness.
E= 2.1e6 # Young modulus of the steel.
nu= 0.3 # Poisson's ratio.
# Load
F= 10 # Load magnitude

import xc
from model import predefined_spaces
from materials import typical_materials
from solution import predefined_solutions

# Problem type
feProblem= xc.FEProblem()
preprocessor=  feProblem.getPreprocessor
nodes= preprocessor.getNodeHandler
modelSpace= predefined_spaces.StructuralMechanics3D(nodes)
n1= nodes.newNodeXYZ(0,0,0)
n2= nodes.newNodeXYZ(L,0,0)
n3= nodes.newNodeXYZ(L,L,0)
n4= nodes.newNodeXYZ(0,L,0)


# Materials definition
memb1= typical_materials.defElasticMembranePlateSection(preprocessor, "memb1",E,nu,0.0,t)

elements= preprocessor.getElementHandler
elements.defaultMaterial= memb1.name
elem= elements.newElement("ShellMITC4",xc.ID([n1.tag,n2.tag,n3.tag,n4.tag]))


# Constraints
modelSpace.fixNode000_000(n1.tag)
modelSpace.fixNode000_000(n2.tag)

# Loads definition
lp0= modelSpace.newLoadPattern(name= '0')
lp0.newNodalLoad(n3.tag, xc.Vector([0,0,F,0,0,0]))
lp0.newNodalLoad(n4.tag, xc.Vector([0,0,F,0,0,0]))
# We add the load case to domain.
modelSpace.addLoadCaseToDomain(lp0.name)

# Solution
analysis= predefined_solutions.simple_static_linear(feProblem)
result= analysis.analyze(1)

q13MedioElem= 0.0
q23MedioElem= 0.0
elements= preprocessor.getElementHandler
elem.getResistingForce()
mats= elem.getPhysicalProperties.getVectorMaterials # Materials at gauss points.
# Gauss points iterator
for m in mats:
  q13MedioElem= q13MedioElem+m.getStressResultantComponent("q13")
  q23MedioElem= q23MedioElem+m.getStressResultantComponent("q23")

q13MedioElem= q13MedioElem/4
q23MedioElem= q23MedioElem/4

q13MedioElemTeor= 0.0
q23MedioElemTeor= 2*F/L
ratio1= abs((q13MedioElem-q13MedioElemTeor))
ratio2= abs((q23MedioElem-q23MedioElemTeor)/q23MedioElemTeor)

''' 
print("q13MedioElem= ",q13MedioElem)
print("q13MedioElemTeor= ",q13MedioElemTeor)
print("ratio1= ",ratio1)
print("q23MedioElem= ",q23MedioElem)
print("q23MedioElemTeor= ",q23MedioElemTeor)
print("ratio2= ",ratio2)
  '''

import os
from misc_utils import log_messages as lmsg
fname= os.path.basename(__file__)
if (abs(ratio1)<1e-12) & (abs(ratio2)<1e-12) :
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
