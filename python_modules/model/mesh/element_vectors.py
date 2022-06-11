# -*- coding: utf-8 -*-

# Direction Vectors for uni-dimensional elements.
def getVectorIElem(preprocessor,idElem):
  '''return the vector in the I direction of the 1D element 
  '''
  elem= preprocessor.getElementHandler.getElement(idElem)
  return elem.getCoordTransf.getIVector


def getVectorJElem(preprocessor,idElem):
  '''return the vector in the J direction of the 1D element 
  '''
  elem= preprocessor.getElementHandler.getElement(idElem)
  return elem.getCoordTransf.getJVector

def getVectorKElem(preprocessor,idElem):
  '''return the vector in the K direction of the 1D element 
  '''
  elem= preprocessor.getElementHandler.getElement(idElem)
  return elem.getCoordTransf.getKVector

