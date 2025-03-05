# -*- coding: utf-8 -*-
''' Export internal force values.'''

from __future__ import print_function
from __future__ import division

__author__= "Luis C. Pérez Tato (LCPT)"
__copyright__= "Copyright 2016,LCPT"
__license__= "GPL"
__version__= "3.0"
__email__= "l.pereztato@gmail.com"

import sys
from materials.sections import internal_forces
from misc_utils import log_messages as lmsg
from model import model_inquiry

def get_internal_forces_dict(nmbComb, elems, vonMisesStressId= 'max_von_mises_stress', woodArmerAlsoForAxialForces= False):
    '''Creates a dictionary with the element's internal forces.

    :param nmbComb: combination name.
    :param elems: element set.
    :param vonMisesStressId: identifier of the Von Mises stress to read
                            (see NDMaterial and MembranePlateFiberSection).
    :param woodArmerAlsoForAxialForces: if true, use Wood-Armer method for both
                                        axial and bending internal forces 
                                        otherwise, use it only for bending 
                                        moments.
    '''
    def get_extended_properties(element):
        '''Return the values of the lateral buckling reduction reduction 
           factor (chiLT) and the axial load reduction factor (chiN)
           properties of an element.

        :param element: element to get the properties from.
        '''
        chiLT= None
        if e.hasProp('chiLT'): # lateral buckling reduction factor.
            chiLT= e.getProp('chiLT')
        chiN= None
        if e.hasProp('chiN'): # axial load reduction reduction factor.
            chiN= e.getProp('chiN')
        FcE= None
        if e.hasProp('FcE'): # AWC NDS-2018 critical buckling design values for compression.
            FcE= e.getProp('FcE')
        FbE= None
        if e.hasProp('FbE'): # AWC NDS-2018 critical buckling design value for bending.
            FbE= e.getProp('FbE')
        return chiLT, chiN, FcE, FbE
           
    combInternalForcesDict= dict()
    outDict= dict()
    combInternalForcesDict[nmbComb]= outDict
    for e in elems:
        elemDict= dict()
        outDict[e.tag]= elemDict
        elementType= e.type()
        elemDict['type']= elementType
        if('Shell' in elementType):
            internalForces= internal_forces.ShellMaterialInternalForces()
            internalForces.setFromAverageInShellElement(e)
            internalForces= internalForces.getWoodArmer(alsoForAxialForces= woodArmerAlsoForAxialForces)
            # Silently ask about maximum Von-Mises stress.
            maxVonMisesAtNodes= e.getValuesAtNodes(vonMisesStressId, True)
            avgMaxVM= None
            if(len(maxVonMisesAtNodes)>1): # vonMisesStressId found.
                avgMaxVM= 0.0
                avgMaxVM+= maxVonMisesAtNodes[0][0] # at node 1
                avgMaxVM+= maxVonMisesAtNodes[1][0] # at node 2
                avgMaxVM+= maxVonMisesAtNodes[2][0] # at node 3
                avgMaxVM+= maxVonMisesAtNodes[3][0] # at node 4
                avgMaxVM/= 4.0 # average of the max. value at nodes.
            sz= len(internalForces)
            internalForcesDict= dict()
            for i in range(0,sz):
                force= internalForces[i]
                internalForcesDict[i]= force.getDict()
                if(avgMaxVM):
                    internalForcesDict[i].update({vonMisesStressId:avgMaxVM})
            elemDict['internalForces']= internalForcesDict
        elif('FourNodeQuad' in elementType):
            e.getResistingForce()
            elementStresses= e.physicalProperties.getVectorMaterials.getValues('stress', False)
            stressesDict= dict()
            numGaussPoints= elementStresses.noRows
            for i in range(0, numGaussPoints):
                sX= elementStresses(i,0)
                sY= elementStresses(i,1)
                sXY= elementStresses(i,2)/2.0
                stressesDict[i]= {'sigma_11':sX, 'sigma_22':sY, 'sigma_12':sXY}
            elemDict['stresses']= stressesDict
        elif('Beam2d' in elementType):
            e.getResistingForce()
            internalForcesDict= dict()
            # Internal forces of the bar. 
            [[N1, M1, V1], [N2, M2, V2]]= model_inquiry.get_values_at_nodes(e, ['N', 'M', 'V'], False)
            internalForces= internal_forces.CrossSectionInternalForces(N1,V1,0.0,0.0,0.0,M1) 
            internalForcesDict[0]= internalForces.getDict()
            internalForces= internal_forces.CrossSectionInternalForces(N2,V2,0.0,0.0,0.0,M2) # Internal forces at the end of the bar.
            internalForcesDict[1]= internalForces.getDict()
            chiLT, chiN, FcE, FbE = get_extended_properties(e)
            if(chiLT): # lateral buckling reduction factor.
                internalForcesDict[0]['chiLT']= chiLT
                internalForcesDict[1]['chiLT']= chiLT
            if(chiN): # axial load reduction reduction factor.
                internalForcesDict[0]['chiN']= chiN
                internalForcesDict[1]['chiN']= chiN
            if(FcE!=None): # AWC NDS 2018 critical buckling design values for compression.
                internalForcesDict[0]['FcE']= FcE
                internalForcesDict[1]['FcE']= FcE
            if(FbE!=None): # AWC NDS 2018 critical buckling design values for bending.
                internalForcesDict[0]['FbE']= FbE
                internalForcesDict[1]['FbE']= FbE
            elemDict['internalForces']= internalForcesDict
        elif('Beam' in elementType):
            e.getResistingForce()
            internalForcesDict= dict()
            [[N1, My1, Mz1, Vy1, Vz1, T1], [N2, My2, Mz2, Vy2, Vz2, T2]]= model_inquiry.get_values_at_nodes(e, ['N', 'My', 'Mz', 'Vy', 'Vz', 'T'], False)
            internalForces= internal_forces.CrossSectionInternalForces(N1,Vy1,Vz1,T1,My1,Mz1) # Internal forces at the origin of the bar.
            internalForcesDict[0]= internalForces.getDict()
            internalForces= internal_forces.CrossSectionInternalForces(N2,Vy2,Vz2,T2,My2,Mz2) # Internal forces at the end of the bar.
            internalForcesDict[1]= internalForces.getDict()
            chiLT, chiN, FcE, FbE= get_extended_properties(e)
            if(chiLT): # ateral buckling reduction factor.
                internalForcesDict[0]['chiLT']= chiLT
                internalForcesDict[1]['chiLT']= chiLT
            if(chiN): # axial load reduction reduction factor.
                internalForcesDict[0]['chiN']= chiN
                internalForcesDict[1]['chiN']= chiN
            if(FcE!=None): # AWC NDS 2018 critical buckling design values for compression.
                internalForcesDict[0]['FcE']= FcE
                internalForcesDict[1]['FcE']= FcE
            if(FbE!=None): # AWC NDS 2018 critical buckling design values for bending.
                internalForcesDict[0]['FbE']= FbE
                internalForcesDict[1]['FbE']= FbE
            elemDict['internalForces']= internalForcesDict
        elif('Truss' in elementType):
            e.getResistingForce()
            internalForcesDict= dict()
            [[N1], [N2]]= model_inquiry.get_values_at_nodes(e,['N'], False)
            internalForces= internal_forces.CrossSectionInternalForces(N1) # Internal forces at the origin of the bar.
            internalForcesDict[0]= internalForces.getDict()
            for extProp in ['chiLT', 'chiN', 'FcE', 'FbE']: # Origin extended properties.
                if e.hasProp(extProp):
                    internalForcesDict[0][extProp]= e.getProp(extProp)
            internalForces= internal_forces.CrossSectionInternalForces(N2) # Internal forces at the end of the bar.
            internalForcesDict[1]= internalForces.getDict()
            for extProp in ['chiLT', 'chiN', 'FcE', 'FbE']: # End extended properties.
                if e.hasProp(extProp):
                    internalForcesDict[1][extProp]= e.getProp(extProp)
            elemDict['internalForces']= internalForcesDict
        elif('ZeroLength' in elementType):
            e.getResistingForce()
            F= e.getValuesAtNodes("stress", False)
            internalForcesDict= dict()
            nDOFs= len(F[0]) # Number of degrees of freedom.
            if(nDOFs!= 6):
                methodName= sys._getframe(0).f_code.co_name
                lmsg.warning(methodName+'; for '+str(nDOFs)+ " DOFs in element type: '"+elementType+"' not implemented.")
            else:
                F0= F[0]
                internalForces= internal_forces.CrossSectionInternalForces(N= F0[0],Vy= F0[1], Vz= F0[2],T= F0[3],My= F0[4], Mz= F0[5]) # Internal forces at the origin node.
                internalForcesDict[0]= internalForces.getDict()
                F1= F[1]
                internalForces= internal_forces.CrossSectionInternalForces(N= F1[0],Vy= F1[1], Vz= F1[2],T= F1[3],My= F1[4], Mz= F1[5]) # Internal forces at the end node.
                internalForcesDict[1]= internalForces.getDict()
            elemDict['internalForces']= internalForcesDict
        else:
            methodName= sys._getframe(0).f_code.co_name
            lmsg.error(methodName+"; element type: '"+elementType+"' unknown.")
    return combInternalForcesDict

def export_internal_forces(nmbComb, elems, fDesc, woodArmerAlsoForAxialForces= False):
    '''Writes a comma separated values file with the element's internal forces.

    :param nmbComb: combination name.
    :param elems: element set.
    :param fDesc: file descriptor to write internal forces on.
    :param woodArmerAlsoForAxialForces: if true, use Wood-Armer method for both
                                        axial and bending internal forces 
                                        otherwise, use it only for bending 
                                        moments.
    '''
    errMsg= 'export_internal_forces deprecated use get_internal_forces_dict'
    errMsg+= 'with apropriate arguments'
    for e in elems:
        elementType= e.type()
        if('Shell' in elementType):
          internalForces= internal_forces.ShellMaterialInternalForces()
          internalForces.setFromAverageInShellElement(e)
          forcesOnNodes= internalForces.getWoodArmer(alsoForAxialForces= woodArmerAlsoForAxialForces)
          sz= len(forcesOnNodes)
          for i in range(0,sz):
            force= forcesOnNodes[i]
            outStr= nmbComb+", "+str(e.tag)+", "+str(i)+", "+force.getCSVString()+'\n'
            fDesc.write(outStr)
        elif('Beam2d' in elementType):
            e.getResistingForce()
            [[N1, M1, V1], [N2, M2, V2]]= model_inquiry.get_values_at_nodes(e,['N', 'M', 'V'], False)
            # Internal forces at the origin of the bar. 
            internalForces= internal_forces.CrossSectionInternalForces(N1,V1,0.0,0.0,0.0,M1) 
            fDesc.write(nmbComb+", "+str(e.tag)+", 0, "+internalForces.getCSVString()+'\n')
            internalForces= internal_forces.CrossSectionInternalForces(N2,V2,0.0,0.0,0.0,M2) # Internal forces at the end of the bar.
            fDesc.write(nmbComb+", "+str(e.tag)+", 1, "+internalForces.getCSVString()+'\n')
        elif('Beam' in elementType):
            e.getResistingForce()
            [[N1, My1, Mz1, Vy1, Vz1, T1], [N2, My2, Mz2, Vy2, Vz2, T2]]= model_inquiry.get_values_at_nodes(e, ['N', 'My', 'Mz', 'Vy', 'Vz', 'T'], False)
            internalForces= internal_forces.CrossSectionInternalForces(N1,Vy1,Vz1,T1,My1,Mz1) # Internal forces at the origin of the bar.
            originStr= nmbComb+", "+str(e.tag)+", 0, "+internalForces.getCSVString()
            for extProp in ['chiLT', 'chiN', 'FcE', 'FbE']: # Origin extended properties.
                if e.hasProp(extProp):
                    originStr+= " , "+str(e.getProp(extProp))
            fDesc.write(originStr+'\n')
            internalForces= internal_forces.CrossSectionInternalForces(N2,Vy2,Vz2,T2,My2,Mz2) # Internal forces at the end of the bar.
            endStr= nmbComb+", "+str(e.tag)+", 1, "+internalForces.getCSVString()
            for extProp in ['chiLT', 'chiN', 'FcE', 'FbE']: # End extended properties.
                if e.hasProp(extProp):
                    endStr+= " , "+str(e.getProp(extProp))
            fDesc.write(endStr+'\n')
        elif('Truss' in elementType):
            e.getResistingForce()
            [[N1], [N2]]= model_inquiry.get_values_at_nodes(e,['N'], False)
            internalForces= internal_forces.CrossSectionInternalForces(N1) # Internal forces at the origin of the bar.
            originStr= nmbComb+", "+str(e.tag)+", 0, "+internalForces.getCSVString()
            for extProp in ['chiLT', 'chiN', 'FcE', 'FbE']: # Origin extended properties.
                if e.hasProp(extProp):
                    originStr+= " , "+str(e.getProp(extProp))
            fDesc.write(originStr+'\n')
            internalForces= internal_forces.CrossSectionInternalForces(N2) # Internal forces at the end of the bar.
            endStr= nmbComb+", "+str(e.tag)+", 1, "+internalForces.getCSVString()
            for extProp in ['chiLT', 'chiN', 'FcE', 'FbE']: # End extended properties.
                if e.hasProp(extProp):
                    endStr+= " , "+str(e.getProp(extProp))
            fDesc.write(endStr+'\n')
        elif('ZeroLength' in elementType):
            methodName= sys._getframe(0).f_code.co_name
            lmsg.warning(methodName+"; for element type: '"+elementType+"' not implemented.")
        else:
            methodName= sys._getframe(0).f_code.co_name
            lmsg.error(methodName+"; error; element type: '"+elementType+"' unknown.")
      

def export_shell_internal_forces(nmbComb, elems, fDesc, woodArmerAlsoForAxialForces= False):
    '''Writes a comma separated values file with the element's internal forces.

    :param nmbComb: combination name.
    :param elems: element set whose internal forces will be exported..
    :param fDesc: file descriptor to write internal forces on.
    :param woodArmerAlsoForAxialForces: if true, use Wood-Armer method for both
                                        axial and bending internal forces 
                                        otherwise, use it only for bending 
                                        moments.
    '''
    errMsg= 'export_shell_internal_forces deprecated use export_internal_forces'
    errMsg+= ' with apropriate arguments'
    lmsg.error(errMsg)
    internalForces= internal_forces.ShellMaterialInternalForces()
    for e in elems:
      internalForces.setFromAverageInShellElement(e)
      forcesOnNodes= internalForces.getWoodArmer(alsoForAxialForces= woodArmerAlsoForAxialForces)
      sz= len(forcesOnNodes)
      for i in range(0,sz):
        force= forcesOnNodes[i]
        outStr= nmbComb+", "+str(e.tag)+", "+str(i)+", "+force.getCSVString()+'\n'
        fDesc.write(outStr)

def exporta_esfuerzos_shell_set(preprocessor,nmbComb, st, fDesc, woodArmerAlsoForAxialForces= False):
    '''Writes a comma separated values file with the element's internal forces.

    :param nmbComb: combination name.
    :param st: XC set containing the elements whose internal forces
               will be exported.
    :param fDesc: file descriptor to write internal forces on.
    :param woodArmerAlsoForAxialForces: if true, use Wood-Armer method for
                                        both axial and bending internal
                                        forces otherwise, use it only for 
                                        bending moments.
    '''
    errMsg= 'exporta_esfuerzos_shell_set deprecated use export_internal_forces'
    errMsg+= ' with apropriate arguments'
    lmsg.error(errMsg)
    elems= st.elements
    exportShellInternalForces(nmbComb,elems,fDesc, woodArmerAlsoForAxialForces= woodArmerAlsoForAxialForces)

def export_beam_internal_forces(nmbComb, elems, fName):
    '''Writes a comma separated values file with the element's internal forces.

    :param nmbComb: name of the load combination.
    :param elems: finite elements to export internal forces from.
    '''
    for e in elems:
        e.getResistingForce()
        [[N1, My1, Mz1, Vy1, Vz1, T1], [N2, My2, Mz2, Vy2, Vz2, T2]]= model_inquiry.get_values_at_nodes(e, ['N', 'My', 'Mz', 'Vy', 'Vz', 'T'], False)
        internalForces= internal_forces.CrossSectionInternalForces(N1,Vy1,Vz1,T1,My1,Mz1) # Internal forces at the origin of the bar.
        fName.write(nmbComb+", "+str(e.tag*10+1)+","+internalForces.getCSVString())
        internalForces= internal_forces.CrossSectionInternalForces(N2,Vy2,Vz2,T2,My2,Mz2) # Internal forces at the end of the bar.
        fName.write(nmbComb+", "+str(e.tag*10+2)+","+internalForces.getCSVString())
