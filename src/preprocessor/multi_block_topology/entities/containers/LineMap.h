// -*-c++-*-
//----------------------------------------------------------------------------
//  XC program; finite element analysis code
//  for structural analysis and design.
//
//  Copyright (C)  Luis C. Pérez Tato
//
//  This program derives from OpenSees <http://opensees.berkeley.edu>
//  developed by the  «Pacific earthquake engineering research center».
//
//  Except for the restrictions that may arise from the copyright
//  of the original program (see copyright_opensees.txt)
//  XC is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or 
//  (at your option) any later version.
//
//  This software is distributed in the hope that it will be useful, but 
//  WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details. 
//
//
// You should have received a copy of the GNU General Public License 
// along with this program.
// If not, see <http://www.gnu.org/licenses/>.
//----------------------------------------------------------------------------
//LineMap.h

#ifndef LINEMAP_H
#define LINEMAP_H

#include "EntityMap.h"
#include "preprocessor/multi_block_topology/entities/1d/Edge.h"

class Line3d;
class Plane;

namespace XC {

class Line;
class DividedLine;
class CmbEdge;
class CircularArc;

//! @ingroup MultiBlockTopology
//
//! @brief Line container.
class LineMap: public EntityMap<Edge>
  {
  private:
    void updateSets(Edge *) const;
  protected:
    template <class E>
    Edge *Crea(void);

  public:
    LineMap(MultiBlockTopology *mbt= nullptr);

    Vector3d getVector(const Indice &i,const Indice &j) const;
    Line3d getRecta(const Indice &i,const Indice &j) const;
    Plane getPlane(const Indice &i,const Indice &j,const Indice &k) const;

    template <class E>
    Edge *New(void);
    template <class E>
    Edge *New(const size_t &);
    Edge *New(const size_t &, const std::string &);
    Edge *createLine(Pnt *,Pnt *);
    Edge *createDividedLine(Pnt *,Pnt *);
    Edge *createArc(Pnt *,Pnt *,Pnt *);
    Edge *createLineSequence(void);
    Edge *createCopy(const Edge *l);
 
    Line *newLine(const size_t &, const size_t &);
    DividedLine *newDividedLine(const size_t &, const size_t &);
    CircularArc *newCircleArc(const size_t &, const size_t &, const size_t &);
    CmbEdge *newLineSequence(void);
    
    double getAverageLength(void) const;
    
    boost::python::dict getPyDict(void) const;
    void setPyDict(const boost::python::dict &);    
  };

//! @brief Creates a new line.
template <class E>
Edge *XC::LineMap::Crea(void)
  {
    Preprocessor *preprocessor= getPreprocessor();
    assert(preprocessor);
    Edge *retval= new E(preprocessor);
    const size_t tg= this->getTag();
    retval->Name()= "l"+boost::lexical_cast<std::string>(tg);
    (*this)[tg]= retval;
    updateSets(retval);
    tag++;
    return retval;
  }

//! @brief Creates a new Edge.
template <class E>
Edge *LineMap::New(void)
  {
    Edge *retval= busca(getTag());
    if(!retval) // the edge is new.
      retval= Crea<E>();
    return retval;
  }

//! @brief Creates a new edge with the given identifier.
template <class E>
Edge *LineMap::New(const size_t &tag)
  {
    size_t old_tag= getTag();
    Edge *retval= nullptr;
    setTag(tag); // Edge identifier.
    retval= busca(getTag());
    if(retval)
      {
	std::cerr << getClassName() << "::" << __FUNCTION__
		  << "; edge with identifier: " << tag
		  << " already exists. Command ignored." << std::endl;
      }
    else
      {	retval= Crea<E>(); }
    setTag(old_tag);
    return retval;
  }
  
} //end of XC namespace
#endif
