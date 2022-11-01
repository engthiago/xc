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
//Set.cc

#include "Set.h"
#include "domain/domain/Domain.h"
#include "domain/mesh/node/Node.h"
#include "domain/mesh/element/Element.h"
#include "preprocessor/Preprocessor.h"
#include "preprocessor/multi_block_topology/entities/0d/Pnt.h"
#include "preprocessor/multi_block_topology/entities/1d/Edge.h"
#include "preprocessor/multi_block_topology/entities/2d/Face.h"
#include "preprocessor/multi_block_topology/entities/3d/Body.h"
#include "preprocessor/multi_block_topology/entities/3d/UniformGrid.h"
#include "preprocessor/multi_block_topology/matrices/ElemPtrArray3d.h"
#include "preprocessor/multi_block_topology/trf/TrfGeom.h"
#include "utility/matrix/ID.h"

#include "utility/geom/pos_vec/SlidingVectorsSystem3d.h"
#include "utility/geom/d2/Plane.h"
#include "utility/geom/d3/HalfSpace3d.h"
#include "utility/geom/d3/BND3d.h"


//! @brief Constructor.
XC::Set::Set(const std::string &nmb,Preprocessor *prep)
  : SetMeshComp(nmb,prep), entities(prep) {}

//! @brief Extend this set with the points of the argument.
void XC::Set::extend(const SetEntities::lst_ptr_points &pts)
  { entities.extend(pts); }

//! @brief Extend this set with the lines of the argument.
void XC::Set::extend(const SetEntities::lst_line_pointers &lns)
  { entities.extend(lns); }

//! @brief Extend this set with the surfaces of the argument.
void XC::Set::extend(const SetEntities::lst_surface_ptrs &sfs)
  { entities.extend(sfs); }

//! @brief Extend this set with the bodies of the argument.
void XC::Set::extend(const SetEntities::lst_body_pointers &bds)
  { entities.extend(bds); }

//! @brief Extend this set with the uniform grids of the argument.
void XC::Set::extend(const SetEntities::lst_ptr_uniform_grids &ugrids)
  { entities.extend(ugrids); }

//! @brief Extend the set with the elements from the argument set.
//!
//! extend the lists with the objects from the argument.
void XC::Set::extend(const Set &other)
  { entities+= other.entities; }


//! @brief Addition assignment operator.
//!
//! Concatenates the name and description of the argument and
//! extend the lists with the objects from the argument.
XC::Set &XC::Set::operator+=(const Set &other)
  {
    SetMeshComp::operator+=(other);
    if(other.description!="")
      description+= "+" + other.description; //Concatenate descriptions.
    entities+= other.entities;
    return *this;
  }

//! @brief -= operator.
XC::Set &XC::Set::operator-=(const Set &other)
  {
    SetMeshComp::operator-=(other);
    if(other.description!="")
      description+= "-" + other.description; //Concatenate descriptions.
    entities-= other.entities;
    return *this;
  }

//! @brief *= operator (intersection).
XC::Set &XC::Set::operator*=(const Set &other)
  {
    SetMeshComp::operator*=(other);
    if(other.description!="")
      description+= "*" + other.description; //Concatenate descriptions.
    entities*= other.entities;
    return *this;
  }

//! @brief Addition (union) operator.
//!
//! Concatenates the name and description of the arguments and
//! extend the lists of the first set with the objects from the argument.
XC::Set &XC::Set::operator+(const Set &b) const
  {
    Set result(*this);
    result+= b;
    Set *retval= result.alloc_set();
    return *retval;
  }

//! @brief Difference operator.
//!
//! Removes the objects that belong also to the argument.
XC::Set &XC::Set::operator-(const Set &b) const
  {
    Set result(*this);
    result-= b;
    Set *retval= result.alloc_set();
    return *retval;
  }

//! @brief Intersection operator.
//!
//! Return the set intersection.
XC::Set &XC::Set::operator*(const Set &b) const
  {
    Set result(*this);
    result*= b;
    Set *retval= result.alloc_set();
    return *retval;
  }

//! @brief Clears the set.
void XC::Set::clear(void)
  {
    SetMeshComp::clear();
    entities.clear();
  }

//! @brief Return true if the set is empty.
bool XC::Set::empty(void) const
  {
    bool retval= SetMeshComp::empty();
    if(retval)
      retval= (retval && entities.empty());
    return retval;
  }

//!  @brief Clears all the objects of the set.
void XC::Set::clearAll(void)
  {
    SetMeshComp::clearAll();
    entities.clearAll();
  }

//! @brief Set indices for the set objects (nodes,elements,points...) to its use in VTK.
void XC::Set::numerate(void)
  {
    SetMeshComp::numerate();
    SetMeshComp::numerate_list(entities.getPoints());
    SetMeshComp::numerate_list(entities.getLines());
    SetMeshComp::numerate_list(entities.getSurfaces());
    //numerate_list(entities.getBodies());
  }

//! @brief Moves the objects of the set.
void XC::Set::move(const Vector3d &desplaz)
  {
    entities.move(desplaz);
    SetMeshComp::move(desplaz);
  }

//! @brief Applies the transformation to the elements of the set.
void XC::Set::Transform(const TrfGeom &trf)
  {
    entities.Transform(trf);
    SetMeshComp::Transform(trf);
  }

//! @brief Tries to allocate a copy this set.
XC::Set *XC::Set::alloc_set(void)
  {
    MapSet &map_set= getPreprocessor()->get_sets();
    Set *retval= map_set.alloc_set(*this);
    if(!retval) //Can't allocate.
      std::cerr << getClassName() << "::" << __FUNCTION__
	        << "; can't allocate this set."
	        << std::endl;
    return retval;
  }

//! @brief Creates a copy of the elements of the set and put them in another
//! set with the name is being passed as parameter. The coordinates of the new
//! points will be obtained by adding to the original coordinates those
//! of the vector being passed as parameter.
void XC::Set::create_copy(const std::string &name,const Vector3d &v= Vector3d())
  {
    Preprocessor *preprocessor= getPreprocessor();
    if(!preprocessor)
      {
        std::cerr << getClassName() << "::" << __FUNCTION__
	          << "; preprocessor not assigned." << std::endl;
	return;
      }
    Set *new_set= getPreprocessor()->get_sets().create_set(name);
    std::clog << getClassName() << "::" << __FUNCTION__
              << "; warning! copy of nodes and elements"
              << " not implemented." << std::endl;
    //Copying entities.
    new_set->entities= entities.create_copy(name,v);
  }

//! @brief Conciliate number of divisions of the lines shared by
//! faces.
void XC::Set::conciliaNDivs(void)
  { entities.conciliaNDivs(); }

//! @brief Set the value of the useGmsh member.
void XC::Set::setUseGmsh(const bool &b)
  { entities.setUseGmsh(b); }

//! @brief Get the value of the useGmsh member.
bool XC::Set::getUseGmsh(void) const
  { return entities.getUseGmsh(); }


//!  @brief Triggers mesh generation from set components.
//!
//! @param dm: Meshing direction.
void XC::Set::genMesh(meshing_dir dm)
  { entities.genMesh(getName(), dm); }

//! @brief Returns true if the point belongs to the set.
bool XC::Set::In(const Pnt *p) const
  { return entities.In(p); }

//! @brief Return a set with the entities of this one.
XC::Set XC::Set::getEntitiesSet(void) const
  {
    Set retval(*this);
    retval.setName(this->getName()+"_entities");
    retval.setDescription(this->getDescription() + " entities");
    retval.getMeshComp().clear();
    return retval;
  }

//! @brief Return a set with the mesh components of this one.
XC::Set XC::Set::getMeshComponentsSet(void) const
  {
    Set retval(*this);
    retval.setName(this->getName()+"_mesh_components");
    retval.setDescription(this->getDescription() + " mesh components");
    retval.entities.clear();
    return retval;
  }

//! @brief Return a new set that contains the points that lie insiof the
//! geometric object.
//!
//! @param newSetName: name for the new set.
//! @param geomObj: geometric object that must contain the nodes.
//! @param tol: tolerance for "In" function.
XC::Set XC::Set::pickPointsInside(const std::string &newSetName, const GeomObj3d &geomObj, const double &tol)
  {
    Set retval(newSetName,getPreprocessor());
    retval.setPoints(entities.getPoints().pickEntitiesInside(geomObj,tol));
    return retval;
  }

//! @brief Returns true if the edge belongs to the set.
bool XC::Set::In(const Edge *e) const
  { return entities.In(e); }

//! @brief Return a new set that contains the lines that lie insiof the
//! geometric object.
//!
//! @param newSetName: name for the new set.
//! @param geomObj: geometric object that must contain the nodes.
//! @param tol: tolerance for "In" function.
XC::Set XC::Set::pickLinesInside(const std::string &newSetName, const GeomObj3d &geomObj, const double &tol)
  {
    Set retval(newSetName,getPreprocessor());
    retval.setLines(entities.getLines().pickEntitiesInside(geomObj,tol));
    return retval;
  }

//! @brief Returns true if the surface belongs to the set.
bool XC::Set::In(const Face *f) const
  { return entities.In(f); }

//! @brief Return a new set that contains the surfaces that lie insiof the
//! geometric object.
//!
//! @param newSetName: name for the new set.
//! @param geomObj: geometric object that must contain the nodes.
//! @param tol: tolerance for "In" function.
XC::Set XC::Set::pickSurfacesInside(const std::string &newSetName, const GeomObj3d &geomObj, const double &tol)
  {
    Set retval(newSetName,getPreprocessor());
    retval.setSurfaces(entities.getSurfaces().pickEntitiesInside(geomObj,tol));
    return retval;
  }

//! @brief Returns true if the body belongs to the set.
bool XC::Set::In(const Body *b) const
  { return entities.In(b); }

//! @brief Return a new set that contains the bodies that lie insiof the
//! geometric object.
//!
//! @param newSetName: name for the new set.
//! @param geomObj: geometric object that must contain the nodes.
//! @param tol: tolerance for "In" function.
XC::Set XC::Set::pickBodiesInside(const std::string &newSetName, const GeomObj3d &geomObj, const double &tol)
  {
    Set retval(newSetName,getPreprocessor());
    retval.setBodies(entities.getBodies().pickEntitiesInside(geomObj,tol));
    return retval;
  }

//! @brief Returns true if the «uniform grid» belongs to the set.
bool XC::Set::In(const UniformGrid *ug) const
  { return entities.In(ug); }

//! @brief Return the set boundary.
//!
//! @param factor: scale factor for the current position
//!                of the nodes: initPos+ factor * nodDisplacement.
BND3d XC::Set::Bnd(const double &factor) const
  {
    BND3d retval= SetMeshComp::Bnd(factor);
    retval+= entities.Bnd();
    return retval;
  }

//! @brief Return a new set that contains the entities that lie insiof the
//! geometric object.
//!
//! @param newSetName: name for the new set.
//! @param geomObj: geometric object that must contain the nodes.
//! @param tol: tolerance for "In" function.
XC::Set XC::Set::pickEntitiesInside(const std::string &newSetName, const GeomObj3d &geomObj, const double &tol)
  {
    Set retval(newSetName,getPreprocessor());
    retval.entities= entities.pickEntitiesInside(geomObj,tol);
    return retval;
  }

//! @brief Appends to the set being passed as parameter
//! the elements that intervene on the definition
//! of those entities that are already in the set.
void XC::Set::fillDownwards(void)
  {
    entities.fillDownwards(*this);
    SetMeshComp::fillDownwards();
  }

//! @brief  Appends to this set the objects that make reference
//! to one or more of the objects that already make part of the set.
void XC::Set::fillUpwards(void)
  {
    SetMeshComp::fillUpwards();
    entities.fillUpwards(*this);
  }

//! @brief Select the points identified by the tags in the parameter.
//!
//! @param tags: identifiers of the points to select.
void XC::Set::sel_points_lista(const ID &tags)
  { entities.sel_points_lista(tags); }

//! @brief Select the lines identified by the tags in the parameter.
//!
//! @param tags: identifiers of the points to select.
void XC::Set::sel_lines_list(const ID &tags)
  { entities.sel_lines_list(tags); }

//! @brief Selects the surfaces with the identifiers being passed as parameter.
void XC::Set::sel_surfaces_lst(const ID &tags)
  { entities.sel_surfaces_lst(tags); }

//! @brief Returns a vector to store the dbTags
//! of the class members.
XC::DbTagData &XC::Set::getDbTagData(void) const
  {
    static DbTagData retval(19);
    return retval;
  }

//! @brief Send members through the communicator argument.
int XC::Set::sendData(Communicator &comm)
  {
    int res= SetMeshComp::sendData(comm);
    //res+= entities.sendData(comm);
    return res;
  }

//! @brief Receives members through the communicator argument.
int XC::Set::recvData(const Communicator &comm)
  {
    ID tmp;
    int res= SetMeshComp::recvData(comm);
    //res+= entities.recvData(comm);
    return res;
  }

//!  @brief Destructor.
XC::Set::~Set(void)
  { clearAll(); }

//! @brief Sends object through the communicator argument.
int XC::Set::sendSelf(Communicator &comm)
  {
    setDbTag(comm);
    const int dataTag= getDbTag();
    inicComm(19);
    int res= sendData(comm);

    res+= comm.sendIdData(getDbTagData(),dataTag);
    if(res < 0)
      std::cerr << getClassName() << "::" << __FUNCTION__
		<< "; failed to send data\n";
    return res;
  }

//! @brief Receives object through the communicator argument.
int XC::Set::recvSelf(const Communicator &comm)
  {
    inicComm(19);
    const int dataTag= getDbTag();
    int res= comm.receiveIdData(getDbTagData(),dataTag);

    if(res<0)
      std::cerr << getClassName() << "::" << __FUNCTION__
	        << "; failed to receive ids.\n";
    else
      {
        //setTag(getDbTagDataPos(0));
        res+= recvData(comm);
        if(res<0)
          std::cerr << getClassName() << "::" << __FUNCTION__
		    << "; failed to receive data.\n";
      }
    return res;
  }
