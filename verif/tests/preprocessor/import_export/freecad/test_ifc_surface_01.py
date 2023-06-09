# -*- coding: utf-8 -*-
from __future__ import division
from __future__ import print_function

__author__= "Luis C. Pérez Tato (LCPT) and Ana Ortega (AO_O)"
__copyright__= "Copyright 2015, LCPT and AO_O"
__license__= "GPL"
__version__= "3.0"
__email__= "l.pereztato@ciccp.es ana.ortega@ciccp.es"

from misc_utils import log_messages as lmsg
from import_export import freecad_reader
from import_export import neutral_mesh_description as nmd
from model import predefined_spaces
import os
import xc

def getRelativeCoo(pt):
    return [pt[0]/1000.0,pt[1]/1000.0,pt[2]/1000.0]

groupsToImport= ['IFC.*']

pth= os.path.dirname(__file__)
if(not pth):
  pth= "."
filePath= pth+'/../../../aux/freecad' 
baseName= 'test_ifc_surface_01'
freeCADFileName= baseName+'.FCStd'
freeCADImport= freecad_reader.FreeCADImport(filePath+'/'+freeCADFileName, groupsToImport, getRelativeCoo, threshold= 0.001)

#Block topology
blocks= freeCADImport.exportBlockTopology('test')

ieData= nmd.XCImportExportData()
ieData.outputFileName= '/tmp/'+baseName+'_blocks'
ieData.problemName= 'FEcase'
ieData.blockData= blocks

ieData.writeToJSON()

FEcase= xc.FEProblem()
FEcase.title= 'Test IFC surfaces.'
preprocessor= FEcase.getPreprocessor
bData= nmd.XCImportExportData()
bData.readJSONFile(fName= ieData.getJSONFileName()) # Read from the previous output.
bData.dumpToXC(FEcase.getPreprocessor)

nodes= preprocessor.getNodeHandler
modelSpace= predefined_spaces.StructuralMechanics3D(nodes) 

xcTotalSet= modelSpace.getTotalSet()

numPoints= len(xcTotalSet.points)
ratio1= abs(numPoints-4)

fname= os.path.basename(__file__)

thk= -1.0
if(len(xcTotalSet.surfaces)>0):
    face= xcTotalSet.surfaces[0]
    thk= face.getProp('thickness')
else:
    lmsg.error(fname+': no surfaces found.')
    quit()
ratio2= (thk-0.2)/0.2

material= face.getProp('attributes')['matId']
matOk= (material=='C25')

'''
print(numPoints)
print(ratio1)
print(thk)
print(ratio2)
print(material)
'''

if(ratio1==0 and matOk and ratio2==0):
    print('test '+fname+': ok.')
else:
    lmsg.error(fname+' ERROR.')
    
# Graphic stuff.
# from postprocess import output_handler
# oh= output_handler.OutputHandler(modelSpace)

# oh.displayBlocks()
