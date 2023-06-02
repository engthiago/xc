# -*- coding: utf-8 -*-
from __future__ import division
from __future__ import print_function

__author__= "Luis C. Pérez Tato (LCPT) and Ana Ortega (AO_O)"
__copyright__= "Copyright 2015, LCPT and AO_O"
__license__= "GPL"
__version__= "3.0"
__email__= "l.pereztato@ciccp.es ana.ortega@ciccp.es"

import os
import sys
import json
import geom
import xc
from import_export import dxf_reader
from import_export import neutral_mesh_description as nmd
from import_export import block_topology_entities as bte
from model import predefined_spaces
# from postprocess import output_handler
import logging


def getRelativeCooFunc(pt):
    return [pt[0],pt[1],pt[2]] #No modification.

def importMultiBlockTopology(dxfFileName, outputFileName, layerNamesToImport, getRelativeCoo):
    ''' Loads DXF entities and writes XC multi-block topology entities
        in a Python file.

       :param dxfFileName: Drawing exchange format file name.
       :param ouputFileName: 
    '''
    logger= logging.getLogger('ezdxf')
    logger.setLevel(level=logging.WARNING) #Avoid logging info messages.
    dxfImport= dxf_reader.DXFImport(dxfFileName, layerNamesToImport,getRelativeCoo, importLines= True, polylinesAsSurfaces= False, threshold= 0.001, tolerance= .001)

    #Block topology
    blocks= dxfImport.exportBlockTopology('test')

    ieData= nmd.XCImportExportData()
    ieData.outputFileName= outputFileName
    ieData.problemName= 'FEcase'
    ieData.blockData= blocks

    #ieData.writeToXCFile()
    ieData.writeToJSON()
    return ieData

#baseName= 'wireframe_model_rev01'
baseName= 'test_split_lines_01'
dxfFName= baseName+'.dxf'
xcBlocksFileName= baseName+'_blocks'
pth= os.path.dirname(__file__)
if(not pth):
    pth= "."
dxfFilePath= pth+'/../../../aux/dxf/'+dxfFName
ieData= importMultiBlockTopology(dxfFileName= dxfFilePath, outputFileName= xcBlocksFileName, layerNamesToImport= ['0*'], getRelativeCoo= getRelativeCooFunc)

FEcase= xc.FEProblem()
FEcase.title= 'Split lines test'
jsonBlocksPath= './'+xcBlocksFileName+'.json'
f= open(jsonBlocksPath,'r') # Open JSON file
data= json.load(f) # returns JSON object as a dictionary.
bData= nmd.XCImportExportData()
bData.setFromDict(data)


# Create xc problem.
feProblem= xc.FEProblem()
# Dump the topology into this problem.
bData.dumpToXC(feProblem.getPreprocessor)

points= feProblem.getPreprocessor.getMultiBlockTopology.getPoints
lines= feProblem.getPreprocessor.getMultiBlockTopology.getLines

numberOfLinesBeforeSplitting= len(lines)
numberOfPointsBeforeSplitting= len(points)
numberOfClips= numberOfLinesBeforeSplitting/4.0
xcTotalSet= feProblem.getPreprocessor.getSets.getSet('total')
xcTotalSet.getEntities.splitLinesAtIntersections(1e-3)
numberOfLinesAfterSplitting= len(lines)
numberOfPointsAfterSplitting= len(points)
numberOfLinesToGet= numberOfLinesBeforeSplitting*(numberOfClips+1)
numberOfPointsToGet= 2*numberOfLinesBeforeSplitting+2*numberOfClips**2
ratio1= abs(numberOfLinesToGet-numberOfLinesAfterSplitting)/numberOfLinesToGet
ratio2= abs(numberOfPointsToGet-numberOfPointsAfterSplitting)/numberOfPointsToGet

# nodes= preprocessor.getNodeHandler
# modelSpace= predefined_spaces.StructuralMechanics3D(nodes) 

'''
print('number of lines before splitting: ', numberOfLinesBeforeSplitting)
print('number of lines after splitting: ', numberOfLinesAfterSplitting)
print('number of lines to get: ', numberOfLinesToGet)
print(ratio1)
print('number of points before splitting: ', numberOfPointsBeforeSplitting)
print('number of points after splitting: ', numberOfPointsAfterSplitting)
print('number of points to get: ', numberOfPointsToGet)
print(ratio2)
'''

# # Graphic stuff.
# oh= output_handler.OutputHandler(modelSpace)

# oh.displayBlocks()

from misc_utils import log_messages as lmsg
fname= os.path.basename(__file__)
if((abs(ratio1)<1e-15) and (abs(ratio2)<1e-15)):
    print('test '+fname+': ok.')
else:
    lmsg.error(fname+' ERROR.')
    
os.system('rm -f '+jsonBlocksPath) # Your garbage you clean it
