# -*- coding: utf-8 -*-
# Home made test
from __future__ import print_function

__author__= "Luis C. Pérez Tato (LCPT) and Ana Ortega (AOO)"
__copyright__= "Copyright 2015, LCPT and AOO"
__license__= "GPL"
__version__= "3.0"
__email__= "l.pereztato@gmail.com"

E= 30e6 # Young modulus (psi)
nu= 0.33 # Poisson's ratio
rho= 1.5 # Density

import xc
from model import predefined_spaces
from materials import typical_materials

# Define materials
feProblem= xc.FEProblem()
preprocessor=  feProblem.getPreprocessor
mat= typical_materials.defElasticIsotropic3d(preprocessor, "elastIso3d",E,nu,rho)

import os
from misc_utils import log_messages as lmsg
fname= os.path.basename(__file__)
if (abs(mat.E-E)<1e-15) & (abs(mat.nu-nu)<1e-15) & (abs(mat.rho-rho)<1e-15):
    print('test '+fname+': ok.')
else:
    lmsg.error(fname+' ERROR.')
