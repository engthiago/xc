# -*- coding: utf-8 -*-
''' Home made test. Ménsula sometida a carga vertical en su extremo.'''

from __future__ import print_function

__author__= "Luis C. Pérez Tato (LCPT) and Ana Ortega (AOO)"
__copyright__= "Copyright 2015, LCPT and AOO"
__license__= "GPL"
__version__= "3.0"
__email__= "l.pereztato@gmail.com"

import xc
from solution import predefined_solutions
from model import predefined_spaces
from materials.sections.structural_shapes import arcelor_metric_shapes
from materials.ec3 import EC3_materials
from postprocess import def_vars_control as vc
from postprocess import callback_controls as cc

# Geometry
L= 10 # Bar length (m)

# Load
F= 50e3 # Load magnitude (kN)

# Steel shape for the cantilever.
S275JR= EC3_materials.S275JR
gammaM0= 1.05
S275JR.gammaM= gammaM0 
#HE400B= structural_steel.SteelShape(S275JR,"HE_400_B",arcelor_metric_shapes.HE) 
HE400B= arcelor_metric_shapes.HEShape(steel= S275JR, name= "HE_400_B") # Section geometry.

# Problem type
feProblem= xc.FEProblem()
preprocessor=  feProblem.getPreprocessor   
nodes= preprocessor.getNodeHandler
modelSpace= predefined_spaces.StructuralMechanics3D(nodes)
n1= nodes.newNodeXYZ(0,0.0,0.0)
n2= nodes.newNodeXYZ(L,0.0,0.0)

# Geometric transformations
lin= modelSpace.newLinearCrdTransf("lin",xc.Vector([0,0,1]))
shape= HE400B.defElasticShearSection3d(preprocessor)

# Elements definition
elements= preprocessor.getElementHandler
elements.defaultTransformation= lin.name
elements.defaultMaterial= HE400B.name
elem= elements.newElement("ElasticBeam3d",xc.ID([n1.tag, n2.tag]))
elem.rho= HE400B.get('P')
vc.defSteelShapeElasticRangeElementParameters(elem,HE400B)
vc.defVarsControlTensRegElastico3d([elem])

recorder= feProblem.getDomain.newRecorder("element_prop_recorder",None)
recorder.setElements(xc.ID([0]))
recorder.callbackRecord= cc.controTensRecElastico3d()


# Constraints
modelSpace.fixNode000_000(n1.tag)


# Load case definition.
lp0= modelSpace.newLoadPattern(name= '0') 
lp0.newNodalLoad(n2.tag, xc.Vector([0,0,-F,0,0,0]))
# We add the load case to domain.
modelSpace.addLoadCaseToDomain(lp0.name)

# Solution
analysis= predefined_solutions.simple_static_linear(feProblem)
result= analysis.analyze(1)

delta= n2.getDisp[2]  # Node 2 yAxis displacement

elem.getResistingForce()
M= elem.getMy1
V= elem.getVz()
FC= elem.getProp("FCTN")

EI=  HE400B.EIy()
deltateor= F*L**3/3/EI
ratio1= (abs(delta+deltateor)/deltateor)
ratio2= ((M-F*L)/(F*L))
ratio3= ((F+V)/F)
ratio4= ((FC-2.64674)/2.64674)

'''   
print("delta= ",delta)
print("deltaTeor= ",deltateor)
print("ratio1= ",ratio1)
print("M= ",M)
print("ratio2= ",ratio2)
print("V1= ",V)
print("ratio3= ",ratio3)
print("FC= ",FC)
print("ratio4= ",ratio4)
'''

cumple= (abs(ratio1)<1e-8) & (abs(ratio2)<1e-10) & (abs(ratio3)<1e-10) & (abs(ratio4)<1e-5)
import os
from misc_utils import log_messages as lmsg
fname= os.path.basename(__file__)
if cumple:
    print('test '+fname+': ok.')
else:
    lmsg.error(fname+' ERROR.')
