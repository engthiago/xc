# -*- coding: utf-8 -*-
''' Home made test.'''

from __future__ import print_function

__author__= "Luis C. Pérez Tato (LCPT) and Ana Ortega (AOO)"
__copyright__= "Copyright 2015, LCPT and AOO"
__license__= "GPL"
__version__= "3.0"
__email__= "l.pereztato@gmail.com"

# Testing hinge development in a cantilever.

import geom
import xc

from materials.sections.structural_shapes import arcelor_metric_shapes
from materials.ec3 import EC3_materials
from model import predefined_spaces
from solution import predefined_solutions

test= xc.FEProblem()
preprocessor=  test.getPreprocessor

S355JR= EC3_materials.S355JR
S355JR.gammaM= 1.05
epp= S355JR.getDesignElasticPerfectlyPlasticMaterial(preprocessor, "epp")
IPE200= arcelor_metric_shapes.IPEShape(S355JR,'IPE_200')
fs3d= IPE200.getFiberSection3d(preprocessor,'epp')

L= 1.0
nodes= preprocessor.getNodeHandler
modelSpace= predefined_spaces.StructuralMechanics2D(nodes)
nod1= nodes.newNodeXY(0,0.0)
nod2= nodes.newNodeXY(L,0.0)

# Geometric transformations
lin= modelSpace.newLinearCrdTransf("lin")
# Elements definition
elements= preprocessor.getElementHandler
elements.defaultTransformation= lin.name # Coordinate transformation for the new elements
elements.defaultMaterial= IPE200.fiberSection3dName
beam2d= elements.newElement("ForceBeamColumn2d",xc.ID([nod1.tag,nod2.tag]))

# Constraints
constraints= preprocessor.getBoundaryCondHandler
modelSpace.fixNode000(nod1.tag)

# Loads definition
WzplTeor= IPE200.get('Wzpl')
M0Teor= -WzplTeor*S355JR.fyd()
F= M0Teor*0.87
# Load definition.
lp0= modelSpace.newLoadPattern(name= '0')
lp0.newNodalLoad(nod2.tag,xc.Vector([0,F,0]))
# We add the load case to domain.
modelSpace.addLoadCaseToDomain(lp0.name)

# Solution procedure
analysis= predefined_solutions.plain_static_modified_newton(test)
#analysis= predefined_solutions.penalty_modified_newton(test, convergenceTestTol= 1e-2)
result= analysis.analyze(10)

elem1= elements.getElement(0)
elem1.getResistingForce()
scc= elem1.getSections()[0]
M0= scc.getStressResultantComponent("Mz")
M= F*L
ratio1= (M0-M)/M
ratio2= (M0-M0Teor)/M0Teor

'''
print('M0Teor= ', M0Teor/1e3, ' kNm')
print('M0= ', M0/1e3, ' kNm')
print('M= ', M/1e3, ' kNm')
print('ratio1= ', ratio1)
print('ratio2= ', ratio2)
'''

import os
from misc_utils import log_messages as lmsg
fname= os.path.basename(__file__)
if (ratio1<1e-12) & (ratio2<0.2):
    print('test '+fname+': ok.')
else:
    lmsg.error(fname+' ERROR.')
