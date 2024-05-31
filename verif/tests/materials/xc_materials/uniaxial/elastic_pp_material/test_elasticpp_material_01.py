# -*- coding: utf-8 -*-
''' Trival test for elastic perfectly plastic material.'''

from __future__ import print_function

__author__= "Luis C. Pérez Tato (LCPT)"
__copyright__= "Copyright 2014, LCPT"
__license__= "GPL"
__version__= "3.0"
__email__= "l.pereztato@gmail.com"

fy= 2600 # Yield stress of the material.
E= 2.1e6 # Young modulus of the material.
Nsteps= 10 # number of analysis steps.

import math
import xc
from model import predefined_spaces
from materials import typical_materials

# Points of the stress-strain function
x_model= [0,0.000242405785552,0.000480663940905,0.000710697802964,0.000928571428571,0.00113055693959,0.00131319830792,0.00147337048911,0.00160833289274,0.00171577627466,0.00179386224882,0.00184125474255,0.00185714285714,0.00184125474255,0.00179386224882,0.00171577627466,0.00160833289274,0.00147337048911,0.00131319830792,0.00113055693959,0.000928571428571,0.000710697802964,0.000480663940905,0.000242405785552,2.27434405556e-19,-0.000242405785552,-0.000480663940905,-0.000710697802964,-0.000928571428571,-0.00113055693959,-0.00131319830792,-0.00147337048911,-0.00160833289274,-0.00171577627466,-0.00179386224882,-0.00184125474255,-0.00185714285714,-0.00184125474255,-0.00179386224882,-0.00171577627466,-0.00160833289274,-0.00147337048911,-0.00131319830792,-0.00113055693959,-0.000928571428571,-0.000710697802964,-0.000480663940905,-0.000242405785552,-4.54868811112e-19,0.000242405785552,0.000480663940905,0.000710697802964,0.000928571428571,0.00113055693959,0.00131319830792]
y_model= [0,509.052149658,1009.3942759,1492.46538622,1950,2374.16957313,2600,2600,2600,2600,2600,2600,2600,2566.63495936,2467.11072253,2303.13017679,2077.49907476,1794.07802714,1457.71644663,1074.16957313,650,192.465386224,-290.6057241,-790.947850342,-1300,-1809.05214966,-2309.3942759,-2600,-2600,-2600,-2600,-2600,-2600,-2600,-2600,-2600,-2600,-2566.63495936,-2467.11072253,-2303.13017679,-2077.49907476,-1794.07802714,-1457.71644663,-1074.16957313,-650,-192.465386224,290.6057241,790.947850342,1300,1809.05214966,2309.3942759,2600,2600,2600,2600]



# Definition of the FE model.
feProblem= xc.FEProblem()
feProblem.logFileName= "/tmp/erase.log" # Ignore warning messages
preprocessor=  feProblem.getPreprocessor
# Define materials
epp= typical_materials.defElasticPPMaterial(preprocessor, name= "epp", E= E, fyp= fy, fyn= -fy)
A= 1.5*epp.eyp
fin= 54
incr= math.pi/24.0
x= []
y= []
j= 0
while (j<=fin):
    epp.setTrialStrain(A*math.sin(j*incr),0.0)
    epp.commitState()
    x.append(epp.getStrain())
    y.append(epp.getStress())
    j= j+1

diff_x= []
diff_y= []
def subtract(x,y): return x-y
diff_x= map(subtract,x,x_model)
diff_y= map(subtract,y,y_model)

ratio1= 0
for d in diff_x:
    ratio1= ratio1+d**2
ratio1= math.sqrt(ratio1)
ratio2= 0
for d in diff_y:
    ratio2= ratio2+d**2
ratio2= math.sqrt(ratio2)

'''
print("ratio1= ",ratio1)
print("ratio2= ",ratio2)
'''

import os
from misc_utils import log_messages as lmsg
fname= os.path.basename(__file__)
if((ratio1<1e-13) & (ratio2<1e-7)):
    print('test '+fname+': ok.')
else:
    lmsg.error(fname+' ERROR.')    

# import matplotlib.pyplot as plt
# import numpy as np
# plt.scatter(x_model, y_model, marker= 'x')
# plt.title(fname+' results')
# plt.xlabel('strain')
# plt.ylabel('stress')
# plt.plot(x, y)
# u= np.diff(x)
# v= np.diff(y)
# pos_x= x[:-1] + u/2
# pos_y= y[:-1] + v/2
# norm= np.sqrt(u**2+v**2) 
# plt.quiver(pos_x, pos_y, u/norm, v/norm, angles="xy", zorder=5, pivot="mid")
# plt.show()
