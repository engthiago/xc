# -*- coding: utf-8 -*-
from __future__ import print_function
''' Reinforced concrete section verification test.
   Home made test. '''


import geom
import xc

nmbHorm= "HA25"
from materials.ehe import EHE_materials
from materials import concrete_base

__author__= "Luis C. Pérez Tato (LCPT) and Ana Ortega (A_OO)"
__copyright__= "Copyright 2015, LCPT and AO_O"
__license__= "GPL"
__version__= "3.0"
__email__= "l.pereztato@gmail.com ana.ortega.ort@gmal.com"

width= 0.2 # Section width expressed in meters.
depth= 0.4 # Section width expressed in meters.
cover= 0.05 # Concrete cover expressed in meters.
diam= 16e-3 # Diameter of the bars expressed in meters.
areaFi16= 2.01e-4 # Rebar area expressed in square meters.

e1= -10e-3 # Axial trial strain 1.
sgc1= EHE_materials.HA25.sigmac(e1)
sgs1= concrete_base.sigmaDReinfSteel(e1,EHE_materials.B500S)

e2= -3e-3 # Axial trial strain 1.
sgc2= EHE_materials.HA25.sigmac(e2)
sgs2= concrete_base.sigmaDReinfSteel(e2,EHE_materials.B500S)

e3= -1e-3 # Axial trial strain 1.
sgc3= EHE_materials.HA25.sigmac(e3)
sgs3= concrete_base.sigmaDReinfSteel(e3,EHE_materials.B500S)

e4= 1e-3 # Axial trial strain 1.
sgc4= EHE_materials.HA25.sigmac(e4)
sgs4= concrete_base.sigmaDReinfSteel(e4,EHE_materials.B500S)

e5= 3e-3 # Axial trial strain 1.
sgc5= EHE_materials.HA25.sigmac(e5)
sgs5= concrete_base.sigmaDReinfSteel(e5,EHE_materials.B500S)

e6= 10e-3 # Axial trial strain 1.
sgc6= EHE_materials.HA25.sigmac(e6)
sgs6= concrete_base.sigmaDReinfSteel(e6,EHE_materials.B500S)

feProblem= xc.FEProblem()
feProblem.logFileName= "/tmp/erase.log" # Don't print(warnings.)

preprocessor=  feProblem.getPreprocessor
# Define materials
tag= EHE_materials.HA25.defDiagD(preprocessor)
dgDHA25= EHE_materials.HA25.getDiagD(preprocessor)
Ec= dgDHA25.getTangent

tag= EHE_materials.B500S.defDiagD(preprocessor)
dgDB500S= EHE_materials.B500S.getDiagD(preprocessor)
#Es= dgDB500S.getTangent

# Section geometry
#creation
geomSecHA= preprocessor.getMaterialHandler.newSectionGeometry("geomSecHA")
#filling with regions
regions= geomSecHA.getRegions
#generation of a quadrilateral region of the specified sizes and number of
#divisions for the cells (fibers) generation
rg= regions.newQuadRegion(EHE_materials.HA25.nmbDiagD)  #name of the quadrilateral region ==(EHE_materials.HA25.nmbDiagD
rg.nDivIJ= 10
rg.nDivJK= 10
rg.pMin= geom.Pos2d(-depth/2,-width/2)
rg.pMax= geom.Pos2d(depth/2,width/2)


reinforcement= geomSecHA.getReinfLayers
reinforcementInf= reinforcement.newStraightReinfLayer(EHE_materials.B500S.nmbDiagD)
reinforcementInf.numReinfBars= 2
reinforcementInf.barDiam= 16e-3
reinforcementInf.barArea= areaFi16
reinforcementInf.p1= geom.Pos2d(cover-depth/2,width/2-cover) # bottom layer.
reinforcementInf.p2= geom.Pos2d(cover-depth/2,cover-width/2)
reinforcementSup= reinforcement.newStraightReinfLayer(EHE_materials.B500S.nmbDiagD)
reinforcementSup.numReinfBars= 2
reinforcementSup.barDiam= 16e-3
reinforcementSup.barArea= areaFi16
reinforcementSup.p1= geom.Pos2d(depth/2-cover,width/2-cover) # top layer.
reinforcementSup.p2= geom.Pos2d(depth/2-cover,cover-width/2)

materialHandler= preprocessor.getMaterialHandler
secHA= materialHandler.newMaterial("fiber_section_3d","secHA")
fiberSectionRepr= secHA.getFiberSectionRepr()
fiberSectionRepr.setGeomNamed(geomSecHA.name)
secHA.setupFibers()

secHA.setTrialSectionDeformation(xc.Vector([e1,0,0]))
secHA.commitState()
P1= secHA.getStressResultantComponent("N")

secHA.revertToStart()
secHA.setTrialSectionDeformation(xc.Vector([e2,0,0]))
secHA.commitState()
P2= secHA.getStressResultantComponent("N")

secHA.revertToStart()
secHA.setTrialSectionDeformation(xc.Vector([e3,0,0]))
secHA.commitState()
P3= secHA.getStressResultantComponent("N")

secHA.revertToStart()
secHA.setTrialSectionDeformation(xc.Vector([e4,0,0]))
secHA.commitState()
P4= secHA.getStressResultantComponent("N")

secHA.revertToStart()
secHA.setTrialSectionDeformation(xc.Vector([e5,0,0]))
secHA.commitState()
P5= secHA.getStressResultantComponent("N")

secHA.revertToStart()
secHA.setTrialSectionDeformation(xc.Vector([e6,0,0]))
secHA.commitState()
P6= secHA.getStressResultantComponent("N")


P1teor= sgc1*width*depth+sgs1*4.0*areaFi16
ratio1= (P1-P1teor)/P1teor
P2teor= sgc2*width*depth+sgs2*4.0*areaFi16
ratio2= (P2-P2teor)/P2teor
P3teor= sgc3*width*depth+sgs3*4.0*areaFi16
ratio3= (P3-P3teor)/P3teor
P4teor= sgc4*width*depth+sgs4*4.0*areaFi16
ratio4= (P4-P4teor)/P4teor
P5teor= sgc5*width*depth+sgs5*4.0*areaFi16
ratio5= (P5-P5teor)/P5teor
P6teor= sgc6*width*depth+sgs6*4.0*areaFi16
ratio6= (P6-P6teor)/P6teor

''' 
print("sgc1= ",(sgc1/1e6),"MPa \n")
print("sgs1= ",(sgs1/1e6),"MPa \n")
print("theoretical axial force; P1teor= ",(P1teor/1e3)," kN \n")
print("computed axial force; P1= ",(P1/1e3)," kN \n")
print("ratio1= ",(ratio1))

print("sgc2= ",(sgc2/1e6),"MPa \n")
print("sgs2= ",(sgs2/1e6),"MPa \n")
print("theoretical axial force; P2teor= ",(P2teor/1e3)," kN \n")
print("computed axial force; P2= ",(P2/1e3)," kN \n")
print("ratio2= ",(ratio2))

print("sgc3= ",(sgc3/1e6),"MPa \n")
print("sgs3= ",(sgs3/1e6),"MPa \n")
print("theoretical axial force; P3teor= ",(P3teor/1e3)," kN \n")
print("computed axial force; P3= ",(P3/1e3)," kN \n")
print("ratio3= ",(ratio3))

print("sgc4= ",(sgc4/1e6),"MPa \n")
print("sgs4= ",(sgs4/1e6),"MPa \n")
print("theoretical axial force; P4teor= ",(P4teor/1e3)," kN \n")
print("computed axial force; P4= ",(P4/1e3)," kN \n")
print("ratio4= ",(ratio4))

print("sgc5= ",(sgc5/1e6),"MPa \n")
print("sgs5= ",(sgs5/1e6),"MPa \n")
print("theoretical axial force; P5teor= ",(P5teor/1e3)," kN \n")
print("computed axial force; P5= ",(P5/1e3)," kN \n")
print("ratio5= ",(ratio5))

print("sgc6= ",(sgc6/1e6),"MPa \n")
print("sgs6= ",(sgs6/1e6),"MPa \n")
print("theoretical axial force; P6teor= ",(P6teor/1e3)," kN \n")
print("computed axial force; P6= ",(P6/1e3)," kN \n")
print("ratio6= ",(ratio6))
 '''

import os
from misc_utils import log_messages as lmsg
fname= os.path.basename(__file__)
if (abs(ratio1)<5e-3) & (abs(ratio2)<5e-3) & (abs(ratio3)<5e-3) & (abs(ratio4)<5e-3) & (abs(ratio5)<5e-3) & (abs(ratio6)<5e-3) :
    print('test '+fname+': ok.')
else:
    lmsg.error(fname+' ERROR.')
    
# # Section output
# import matplotlib.pyplot as plt
# from materials.sections.fiber_section import plot_fiber_section
# fig = plt.figure()
# ax= fig.add_subplot(111)
# plot_fiber_section.mplot_section_geometry(ax, sectionGeometry= geomSecHA)
# plt.show()
