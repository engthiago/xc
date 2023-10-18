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
//Body.h
//Class for block entities. (XXX: rename as Block?)

#ifndef BODY_H
#define BODY_H

#include "preprocessor/multi_block_topology/entities/EntMdlr.h"
#include "preprocessor/multi_block_topology/entities/1d/CmbEdge.h"


class Pos3d;

namespace XC {

class Face;
class Pnt;
class SurfaceMap;

//! @ingroup MultiBlockTopologyEnt
//!
//! @brief Order in edges of an hexahedron face.
struct SideSequence
  {
    size_t l1; //!< Index edge 1.
    size_t l2; //!< Index edge 2.
    size_t l3; //!< Index edge 3.
    size_t l4; //!< Index edge 4.
    bool forward; //!< Forward or reverse sequence.

    SideSequence(const size_t first= 1,const bool &forward= true);
    inline bool operator==(const SideSequence &other) const
      { return ((l1==other.l1) && (l2==other.l2) && (l3==other.l3) && (l4==other.l4)); } 
    //! @brief Return true if the edge sequence is direct (edge1 -> edge4).
    const bool &isDirect(void) const
      { return forward; }
    
    boost::python::dict getPyDict(void) const;
    void setPyDict(const boost::python::dict &);
  };

//! @ingroup MultiBlockTopologyEnt
//!
//! @brief Six-faced solid.
class Body: public EntMdlr
  {
  public:
    //! @ingroup Geom
    //!
    //! @brief Surface that limits the body (face as seen by the body).
    class BodyFace: public CommandEntity
      {
        Face *surface; //!< Face geometry.
        SideSequence side_seq; //!< Edge sequence.
      public:
        BodyFace(Body *b= nullptr, Face *ptr= nullptr,const size_t &p=1,const bool &d=true);
        virtual bool operator==(const BodyFace &) const;
	Face *Surface(void);
        const Face *Surface(void) const;
        void SetSurf(Face *s);

        const std::string &getName(void) const;
        bool Vacia(void) const;
        size_t getNumberOfLines(void) const;
        size_t getNumberOfVertices(void) const;
        const CmbEdge::Side *getSide(const size_t &) const;
        CmbEdge::Side *getSide(const size_t &);
        const Pnt *getVertex(const size_t &) const;
        Pnt *getVertex(const size_t &);
        virtual Pos3dArray get_positions(void) const;
        void create_nodes(void);
	
        std::deque<const Edge *> getNDivErrors(void) const;
        bool checkNDivs(void) const;
	
        Node *getNode(const size_t &,const size_t &);
	
        boost::python::dict getPyDict(void) const;
        void setPyDict(SurfaceMap &, const boost::python::dict &);
      };

  protected:
    void set_surf(Face *s);

    virtual BodyFace *getFace(const size_t &i)= 0;
  public:
    Body(Preprocessor *m,const std::string &name= "");
    //! @brief Return the object dimension.
    inline virtual unsigned short int GetDimension(void) const
      { return 3; }
    
    //! @brief Return the number of lines in the object.
    virtual size_t getNumberOfLines(void) const= 0;
    //! @brief Return the number of vertices in the object.
    virtual size_t getNumberOfVertices(void) const= 0;
    //! @brief Return the number of faces in the object.
    virtual size_t getNumberOfFaces(void) const= 0;
    virtual std::set<const Face *> getSurfaces(void)= 0;
    virtual const BodyFace *getFace(const size_t &i) const= 0;
    virtual const CmbEdge::Side *getEdge(const size_t &i) const= 0;
    virtual const Pnt *getVertex(const size_t &i) const= 0;
    std::vector<int> getIndicesVertices(void) const;
    virtual BND3d Bnd(void) const;
    virtual double getVolume(void) const;
    bool In(const GeomObj3d &, const double &tol= 0.0) const;
    bool Out(const GeomObj3d &, const double &tol= 0.0) const;

    virtual std::deque<const Edge *> getNDivErrors(void) const= 0;
    virtual bool checkNDivs(void) const= 0;

    std::set<SetBase *> get_sets(void) const;
    void add_to_sets(std::set<SetBase *> &);
  };

std::set<const Body *> getConnectedBodies(const Face &s);

} //end of XC namespace

#endif
