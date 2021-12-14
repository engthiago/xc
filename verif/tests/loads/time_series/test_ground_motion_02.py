# -*- coding: utf-8 -*-
from __future__ import print_function

__author__= "Luis C. Pérez Tato (LCPT) and Ana Ortega (AOO)"
__copyright__= "Copyright 2015, LCPT and AOO"
__license__= "GPL"
__version__= "3.0"
__email__= "l.pereztato@gmail.com"

import xc_base
import geom
import xc

feProblem= xc.FEProblem()
preprocessor=  feProblem.getPreprocessor


#Load modulation.
loadHandler= preprocessor.getLoadHandler
lPatterns= loadHandler.getLoadPatterns
ts= lPatterns.newTimeSeries("constant_ts","ts")
gm= lPatterns.newLoadPattern("uniform_excitation","gm")
mr= gm.motionRecord
hist= mr.history
accel= lPatterns.newTimeSeries("path_ts","accel")
import os
pth= os.path.dirname(__file__)
#print("pth= ", pth)
if(not pth):
  pth= "."
exec(open(pth+"//data/BM68elc_path.py").read())
hist.accel= accel

motionDuration= mr.getDuration() 
motionPath= mr.history.accel.path
motionPathSize= mr.history.getNumberOfDataPoints()
motionFactor= mr.history.accel.getFactor(0.5)
motionPeakFactor= mr.history.accel.getPeakFactor()
motionLastSendCommitTag= mr.history.accel.lastSendCommitTag
motionPathTimeIncrement= mr.history.accel.getTimeIncr(0.5)

ratio1= (motionDuration-4000)/4000
ratio2= (motionFactor+0.0015141295)/0.0015141295
ratio3= (motionPeakFactor-0.056658)/0.056658
ratio4= (motionLastSendCommitTag+1)
ratio5= (motionPathTimeIncrement-1)
ratio6= (motionPathSize-4000)/4000

''' 
print("duration= ",motionDuration)
print("ratio1= ",ratio1)
# print("path= ",motionPath)
print("factor= ",motionFactor)
print("ratio2= ",ratio2)
print("peak factor= ",motionPeakFactor)
print("ratio3= ",ratio3)
print("lastSendCommitTag= ",motionLastSendCommitTag)
print("ratio4= ",ratio4)
print("path time increment= ",motionPathTimeIncrement)
print("ratio5= ",ratio4)
print("path size= ",motionPathSize)
print("ratio6= ",ratio6)
  '''

import os
from misc_utils import log_messages as lmsg
fname= os.path.basename(__file__)
if (abs(ratio1)<1e-15) & (abs(ratio2)<1e-15) & (abs(ratio3)<1e-15) & (abs(ratio4)<1e-15) & (abs(ratio5)<1e-15) & (abs(ratio6)<1e-15):
    print('test '+fname+': ok.')
else:
    lmsg.error(fname+' ERROR.')
