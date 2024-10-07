# -*- coding: utf-8 -*-
from __future__ import division
from __future__ import print_function

__author__= "Luis C. Pérez Tato (LCPT) and Ana Ortega (AO_O)"
__copyright__= "Copyright 2015, LCPT and AO_O"
__license__= "GPL"
__version__= "3.0"
__email__= "l.pereztato@ciccp.es ana.ortega@ciccp.es"

from import_export import load_combinations as lc
import os

fName= 'load_combinations.xlsx'
pth= os.path.dirname(__file__)
if(not pth):
  pth= "."
fPath= pth+'/../aux/spreadsheets/'+fName

combNameColumn= 'ID'
actions= ['selfWeight', 'deadLoad', 'earthPressure', 'pedestrianLoad', 'singleAxeLoad', 'LM1', 'nosingLoad', 'roadTrafficLoad', 'derailment', 'earthquake']
sheets= ['ELS_qp','ELU_T2','ELU_A','ELS_rare','ELS_freq']


combs= lc.import_load_combinations_from_book(fPath, sheets, nameColumsRow= 1, combNameColumn= 'ID',actionLabels= actions, dataOnly= True)

count= 0
for key in combs.keys():
    count+= len(combs[key])

ratio1= count-76

'''
print(combs)
print(count)
'''


from misc_utils import log_messages as lmsg
fname= os.path.basename(__file__)
if(ratio1==0):
    print('test '+fname+': ok.')
else:
    lmsg.error(fname+' ERROR.')
