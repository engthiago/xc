# -*- coding: utf-8 -*-
from __future__ import print_function
'''Elastic 3D section verification test.'''

import xc
from model import predefined_spaces
from materials import typical_materials
from materials.sections import section_properties
from misc import scc3d_testing_bench

__author__= "Luis C. Pérez Tato (LCPT) and Ana Ortega (AOO)"
__copyright__= "Copyright 2015, LCPT and AOO"
__license__= "GPL"
__version__= "3.0"
__email__= "l.pereztato@gmail.com"

feProblem= xc.FEProblem()
feProblem.logFileName= "/tmp/erase.log" # Ignore warning messages
preprocessor=  feProblem.getPreprocessor

# Rectangular cross-section definition
# XXX Is not a rectangular sections so THE TEST SEEMS NOT HAVE MUCH SENSE
# At last we must create a class for this kind to be consistent with
# the classes already defined to deal with steel profiles.
SHS50x50x2_5=  section_properties.RectangularSection("SHS50x50x2_5",b=0.05,h=0.05) # Section geometry.
matSHS50x50x2_5=  typical_materials.MaterialData(name='matSec',E=210000e6,nu=0.3e6,rho=2500) # Section material.

SHS50x50x2_5.name= "SHS50x50x2_5"
SHS50x50x2_5.b= 0.05
SHS50x50x2_5.h= 0.05
SHS50x50x2_5.e= 2.5e-3
SHS50x50x2_5.P= 3.54
# SHS50x50x2_5.A= 4.51e-4
# SHS50x50x2_5.Iz= 17.9e-8
# SHS50x50x2_5.Iy= 17.9e-8
SHS50x50x2_5.Wzel= 7.16e-6
SHS50x50x2_5.Wzpl= 8.47e-6
SHS50x50x2_5.iz= 1.99e-2
SHS50x50x2_5.iy= 1.99e-2
# SHS50x50x2_5.J= 26.8e-8
SHS50x50x2_5.E= 210000e6
SHS50x50x2_5.nu= 0.3e6
# SHS50x50x2_5.G= SHS50x50x2_5.E/(2*(1+SHS50x50x2_5.nu))
SHS50x50x2_5.alpha= 5/6 # XXX Corregir


# Materials definition
mat= SHS50x50x2_5.defElasticSection3d(preprocessor,matSHS50x50x2_5)
elemZLS= zlElement, nodA, nodB= scc3d_testing_bench.sectionModel(preprocessor, SHS50x50x2_5.name)

# Constraints
modelSpace= predefined_spaces.getStructuralMechanics3DSpace(preprocessor)
modelSpace.fixNode000_000(nodA.tag)
modelSpace.fixNodeF00_00F(nodB.tag)

# Loads definition
lp0= modelSpace.newLoadPattern(name= '0')

loadMz= 1e3
lp0.newNodalLoad(nodB.tag,xc.Vector([0,0,0,0,0,loadMz]))

# We add the load case to domain.
modelSpace.addLoadCaseToDomain(lp0.name)


# Solution
result= modelSpace.analyze(calculateNodalReactions= True)

nodes= preprocessor.getNodeHandler
RM= nodA.getReaction[5] 

elements= preprocessor.getElementHandler

zlElement.getResistingForce()
scc0= zlElement.getSection()

esfMz= scc0.getStressResultantComponent("Mz")

ratio1= (RM+loadMz)/loadMz
ratio2= (esfMz-loadMz)/loadMz
import os
from misc_utils import log_messages as lmsg
fname= os.path.basename(__file__)
if (abs(ratio1)<1e-5) & (abs(ratio2)<1e-5) :
    print('test '+fname+': ok.')
else:
    lmsg.error(fname+' ERROR.')
