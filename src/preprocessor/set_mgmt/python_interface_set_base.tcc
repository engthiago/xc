//----------------------------------------------------------------------------
//  XC program; finite element analysis code
//  for structural analysis and design.
//
//  Copyright (C)  Luis C. Pérez Tato
//
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
//python_interface_set_containers.tcc


bool (XC::SetBase::*isNodeIn)(const XC::Node *) const= &XC::SetBase::In;
bool (XC::SetBase::*isElementIn)(const XC::Element *) const= &XC::SetBase::In;
bool (XC::SetBase::*isPntIn)(const XC::Pnt *) const= &XC::SetBase::In;
bool (XC::SetBase::*isEdgeIn)(const XC::Edge *) const= &XC::SetBase::In;
bool (XC::SetBase::*isFaceIn)(const XC::Face *) const= &XC::SetBase::In;
bool (XC::SetBase::*isBodyIn)(const XC::Body *) const= &XC::SetBase::In;
bool (XC::SetBase::*isUniformGridIn)(const XC::UniformGrid *) const= &XC::SetBase::In;
class_<XC::SetBase, XC::SetBase *, bases<XC::EntMdlrBase>, boost::noncopyable >("SetBase", no_init)
  .def("genMesh", &XC::SetBase::genMesh,"Triggers mesh generation.")
  .def("getNodeTags",&XC::SetBase::getNodeTags,"return a set of node tags.")
  .def("getNodeTagList",&XC::SetBase::getNodeTagsPy,"return a Python list containing the node tags.")
  .def("getIdNodeTags", make_function(&XC::SetBase::getIdNodeTags, return_internal_reference<>() ),"return ID containing the tags of the nodes.")
  .def("getElementTags",&XC::SetBase::getElementTags,"return set of element tags.")
  .def("getElementTagList",&XC::SetBase::getElementTagsPy,"return a Python list containing the element tags.")
  .def("getIdElementTags",make_function(&XC::SetBase::getIdElementTags, return_internal_reference<>() ),"return ID containing the tags of the elements.")
  .def("resetTributaries",&XC::SetBase::resetTributaries, "Assigns zero to the tributary lengths (or areas or volumes) of all the nodes in the set.")
  .def("computeTributaryLengths",&XC::SetBase::computeTributaryLengths, "Computethe tributary lengths for all the nodes in the set. WARNING: the tributary length is cumulative (to avoid discontinuities for the nodes at the set boundaries), so if you call it twice, you'll get this value doubled.")
  .def("computeTributaryAreas",&XC::SetBase::computeTributaryAreas, "Compute the tributary areas for all the nodes in the set. WARNING: the tributary area is cumulative (to avoid discontinuities for the nodes at the set boundaries), so if you call it twice, you'll get this value doubled.")
  .def("computeTributaryVolumes",&XC::SetBase::computeTributaryVolumes, "Compute the tributary volumes for all the nodes in the set. WARNING: the tributary volume is cumulative (to avoid discontinuities for the nodes at the set boundaries), so if you call it twice, you'll get this value doubled.")
  .def("isNodeIn",isNodeIn,"True if set contains node.")
  .def("isElementIn",isElementIn,"True if set contains the element argument.")
  .def("isPntIn",isPntIn,"True if sets contains the point argument.")
  .def("isEdgeIn",isEdgeIn,"True if set contains the edge argument.")
  .def("isFaceIn",isFaceIn,"True if set contains the face argument.")
  .def("isBodyIn",isBodyIn,"True if set contains the body argument.")
  .def("isUniformGridIn",isUniformGridIn,"True if set contains the uniform grid argument.")
  .add_property("getNumNodes",&XC::SetBase::getNumberOfNodes,"Return the number or nodes.")
  .add_property("getNumElements",&XC::SetBase::getNumberOfElements,"Return the number or elements.")
  .add_property("color", make_function(&XC::SetBase::getColor, return_internal_reference<>() ),&XC::SetBase::setColor,"stores a color for the objects of the set.")
  .def("setColorComponents",make_function(&XC::SetBase::setColorComponents, return_internal_reference<>() ),"set the components of the color for the objects of the set.")
  .def("createInertiaLoads", &XC::SetBase::createInertiaLoads, "Create the inertia load for the given acceleration vector.")
  ;

class_<XC::SetEstruct, XC::SetEstruct *, bases<XC::SetBase>, boost::noncopyable >("SetEstruct", no_init)
  .add_property("getNumNodeLayers", &XC::SetEstruct::getNumNodeLayers,"Return the number of node layers.")
  .add_property("getNumNodeRows", &XC::SetEstruct::getNumNodeRows,"Return the number of node rows .")
  .add_property("getNumNodeColumns", &XC::SetEstruct::getNumNodeColumns,"Return the number of node columns.")
  .add_property("getNumElementLayers", &XC::SetEstruct::getNumElementLayers,"Return the number of element layers.")
  .add_property("getNumElementRows", &XC::SetEstruct::getNumElementRows,"Return the number of element rows.")
  .add_property("getNumElementColumns", &XC::SetEstruct::getNumElementColumns,"Return the number of element columns.")
  .add_property("isIRow", &XC::SetEstruct::isIRow)
  .add_property("isJRow", &XC::SetEstruct::isJRow)
  .add_property("isKRow", &XC::SetEstruct::isKRow)
  .add_property("getDimension", &XC::SetEstruct::Dimension)
  .add_property("getStrType", &XC::SetEstruct::getStrType,"Return a string describing the set type i_row, j_row, k_row,...")
  .add_property("getNodes", &XC::SetEstruct::getNodes, "Return entity nodes. DEPRECATED use nodes.")
  .add_property("getElements", &XC::SetEstruct::getElements, "Return entity elements. DEPRECATED use elements.")
  .add_property("nodes", &XC::SetEstruct::getNodes, "Return entity nodes.")
  .add_property("elements", &XC::SetEstruct::getElements, "Return entity elements.")
  .def("getNodeI", &XC::SetEstruct::getNodeI, return_internal_reference<>(), "Return node in position (i)." )
  .def("getNodeIJ", &XC::SetEstruct::getNodeIJ, return_internal_reference<>(), "Return node in position (i,j)." )
  .def("getNodeIJK", &XC::SetEstruct::getNodeIJK, return_internal_reference<>(), "Return node in position (i,j,k).")
  .def("getElementI", &XC::SetEstruct::getElementI, return_internal_reference<>(), "Return element in position (i)." )
  .def("getElementIJ", &XC::SetEstruct::getElementIJ, return_internal_reference<>(), "Return element in position (i,j)." )
  .def("getElementIJK", &XC::SetEstruct::getElementIJK, return_internal_reference<>(), "Return element in position (i,j,k)." )
   ;
