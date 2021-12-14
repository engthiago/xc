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

#include "frictionModel/python_interface.tcc"

class_<XC::FrictionElementBase, bases<XC::Element0D>, boost::noncopyable >("FrictionElementBase", no_init);

class_<XC::FlatSliderSimple2d, bases<XC::FrictionElementBase>, boost::noncopyable >("FlatSliderSimple2d", no_init);

class_<XC::FlatSliderSimple3d, bases<XC::FrictionElementBase>, boost::noncopyable >("FlatSliderSimple3d", no_init);

class_<XC::SingleFPSimple2d , bases<XC::FrictionElementBase>, boost::noncopyable >("SingleFPSimple2d", no_init);

class_<XC::SingleFPSimple3d , bases<XC::FrictionElementBase>, boost::noncopyable >("SingleFPSimple3d", no_init);
