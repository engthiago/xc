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
//python_interface.tcc
class_<XC::Material, bases<XC::MovableObject,XC::TaggedObject>, boost::noncopyable >("Material", no_init)
  .def("commitState", &XC::Material::commitState,"Commit material's state.")
  .def("revertToLastCommit", &XC::Material::revertToLastCommit,"Return the material to the last committed state.")
  .def("revertToStart", &XC::Material::revertToStart,"Return the material to its initial state.")
  .def("getName",&XC::Material::getName,"Return the name of the material.")
  .add_property("name",&XC::Material::getName,"Return the name of the material.")
  .def("getGeneralizedStress", make_function(&XC::Material::getGeneralizedStress, return_internal_reference<>() ),"Return the stress in this material point.")
  .def("getGeneralizedStrain", make_function(&XC::Material::getGeneralizedStrain, return_internal_reference<>() ),"Return the strain in this material point.")
   ;
