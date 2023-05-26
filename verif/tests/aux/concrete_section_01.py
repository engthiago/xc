# -*- coding: utf-8 -*-

__author__= "Luis C. Pérez Tato (LCPT) and Ana Ortega (AOO)"
__copyright__= "Copyright 2015, LCPT and AOO"
__license__= "GPL"
__version__= "3.0"
__email__= "l.pereztato@gmail.com"

import geom
from materials.ehe import EHE_materials

defSec= dict(width= 0.2, # Section width expressed in meters.
depth= 0.4, # Section depth expressed in meters.
cover= 0.05, # Concrete cover expressed in meters.
nIJ= 11, # Number of division in the IJ direction.
nJK= 11, # Number of division in the JK direction.
strandsArea= 140e-6, # Strands area expressed in square meters.
rebarDiameter= 16e-3, # Diameter of the bars expressed in meters.
rebarArea= 2.01e-4 # Rebars area expressed in square meters.
  )

def gmSecHA01(preprocessor, nmbGeomSecc,defSec,concrDiagName,reinfSteelDiagramName):
    geomSecc= preprocessor.getMaterialHandler.newSectionGeometry(nmbGeomSecc)
    regions= geomSecc.getRegions
    concrete= regions.newQuadRegion(concrDiagName)
    concrete.nDivIJ= defSec['nIJ']
    concrete.nDivJK= defSec['nJK']
    concrete.pMin= geom.Pos2d(-(defSec['depth']/2.0),-(defSec['width']/2.0))
    concrete.pMax= geom.Pos2d(defSec['depth']/2.0,defSec['width']/2.0)
    reinforcement= geomSecc.getReinfLayers
    reinforcementInf= reinforcement.newStraightReinfLayer(reinfSteelDiagramName)
    reinforcementInf.numReinfBars= 2
    reinforcementInf.barDiam= 16e-3
    reinforcementInf.barArea= defSec['rebarArea']
    reinforcementInf.p1= geom.Pos2d(defSec['cover']-defSec['depth']/2.0,defSec['cover']-defSec['width']/2.0) # bottom layer.
    reinforcementInf.p2= geom.Pos2d(defSec['cover']-defSec['depth']/2.0,defSec['width']/2.0-defSec['cover'])
    reinforcementSup= reinforcement.newStraightReinfLayer(reinfSteelDiagramName)
    reinforcementSup.numReinfBars= 2
    reinforcementSup.barDiam= 16e-3
    reinforcementSup.barArea= defSec['rebarArea']
    reinforcementSup.p1= geom.Pos2d(defSec['depth']/2.0-defSec['cover'],defSec['cover']-defSec['width']/2.0) # top layer.
    reinforcementSup.p2= geom.Pos2d(defSec['depth']/2.0-defSec['cover'],defSec['width']/2.0-defSec['cover'])
    return geomSecc


