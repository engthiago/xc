# -*- coding: utf-8 -*-
''' Interaction diagram computation. Home made test.'''
from __future__ import print_function
from __future__ import division

import geom
import xc

from materials.ehe import EHE_materials
from materials.sia262 import SIA262_materials
from materials.sections.fiber_section import def_simple_RC_section

__author__= "Luis C. Pérez Tato (LCPT) and Ana Ortega (AOO)"
__copyright__= "Copyright 2015, LCPT and AOO"
__license__= "GPL"
__version__= "3.0"
__email__= "l.pereztato@gmail.com"

areaFi6= SIA262_materials.section_barres_courantes[6e-3]
areaFi8= SIA262_materials.section_barres_courantes[8e-3]
areaFi10= SIA262_materials.section_barres_courantes[10e-3]
areaFi12= SIA262_materials.section_barres_courantes[12e-3]
areaFi14= SIA262_materials.section_barres_courantes[14e-3]
areaFi16= SIA262_materials.section_barres_courantes[16e-3]
areaFi18= SIA262_materials.section_barres_courantes[18e-3]
areaFi20= SIA262_materials.section_barres_courantes[20e-3]
areaFi22= SIA262_materials.section_barres_courantes[22e-3]
areaFi26= SIA262_materials.section_barres_courantes[26e-3]
areaFi30= SIA262_materials.section_barres_courantes[30e-3]
areaFi34= SIA262_materials.section_barres_courantes[34e-3]
areaFi40= SIA262_materials.section_barres_courantes[40e-3]

concrete= EHE_materials.HA30
concrete.alfacc=0.85    # f_maxd= 0.85*fcd concrete long term compressive strength factor (normally alfacc=1)

reinfSteel= EHE_materials.B500S

sccData=def_simple_RC_section.RCRectangularSection()
sccData.name= "sccData"
sccData.sectionDescr= "Prueba."
sccData.fiberSectionParameters.concrType= concrete
sccData.h= 0.5
sccData.b= 1.0
sccData.fiberSectionParameters.reinfSteelType= reinfSteel
sccData.negatvRebarRows= def_simple_RC_section.LongReinfLayers([def_simple_RC_section.ReinfRow(rebarsDiam=40e-3,areaRebar= areaFi40,rebarsSpacing=0.15,width=1.0,nominalCover=0.25-0.19)])
sccData.positvRebarRows= def_simple_RC_section.LongReinfLayers([def_simple_RC_section.ReinfRow(rebarsDiam=6e-3,areaRebar= areaFi6,rebarsSpacing=0.15,width=1.0,nominalCover=0.25-0.19)])
#sccData.setMainReinfNeg(40e-3,areaFi40,0.15,0.25-0.19)
#sccData.setMainReinfPos(6e-3,areaFi6,0.15,0.25-0.19)


feProblem= xc.FEProblem()
feProblem.logFileName= "/tmp/erase.log" # Don't print(warnings.)
feProblem.errFileName= "/tmp/erase.err" # Don't print(errors.)

preprocessor=  feProblem.getPreprocessor
sccData.defRCSection(preprocessor, 'd')
param= xc.InteractionDiagramParameters()
diag= sccData.defInteractionDiagramNMy(preprocessor)
#from materials.sections.fiber_section import plotSectionGeometry as pg
#pg.plotInteractionDiagram2D(diag)



fcELU13= diag.getCapacityFactor(geom.Pos2d(136.78e3,24.71e3))
fcELU14= diag.getCapacityFactor(geom.Pos2d(1197.13e3,65.98e3))

''' Test to be replaced LCPT 02/10/2015'''
ratio1= abs(fcELU13-0.777283365776)
ratio2= abs(fcELU14-4.4411488676)

'''
sccData.plot(preprocessor) # Display section geometry
print("fcELU13= ", fcELU13)
print("ratio1= ", ratio1)
print("fcELU14= ", fcELU14)
print("ratio2= ", ratio2)
'''


feProblem.errFileName= "cerr" # Display errors if any.
import os
from misc_utils import log_messages as lmsg
fname= os.path.basename(__file__)
if((abs(ratio1)<1e-2) & (abs(ratio2)<1e-2)):
    print('test '+fname+': ok.')
else:
    lmsg.error(fname+' ERROR.')
