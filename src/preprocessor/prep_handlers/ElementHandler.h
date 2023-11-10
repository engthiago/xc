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
//ElementHandler.h

#ifndef ELEMENTHANDLER_H
#define ELEMENTHANDLER_H

#include "preprocessor/prep_handlers/ProtoElementHandler.h"

namespace XC {

//!  @ingroup Ldrs
//! 
//! @brief Element creation manager.
class ElementHandler: public ProtoElementHandler
  {
  public:
    class SeedElemHandler: public ProtoElementHandler
      {
      private:
        Element *seed;
	std::string elementType;
        SeedElemHandler(const SeedElemHandler &);
        SeedElemHandler &operator=(const SeedElemHandler &);
        void free(void);
	void alloc(Element *);
      protected:
        void add(Element *);
      public:
        SeedElemHandler(Preprocessor *preprocessor);
        ~SeedElemHandler(void);
        virtual void setDefaultMaterial(const std::string &);
	Element *getSeedElement(void);
	const Element *getSeedElement(void) const;
        int getDefaultTag(void) const;
        void clearAll(void);
        Element *newElement(const std::string &);
        Element *newElement(const std::string &,const ID &);
      };
  private:
    SeedElemHandler seed_elem_handler; //!< Seed element for meshing.
  protected:
    virtual void add(Element *);
  public:
    ElementHandler(Preprocessor *);
    Element *getElement(int tag);

    void new_element(Element *e);
    inline SeedElemHandler &getSeedElemHandler(void)
      { return seed_elem_handler; }
    const Element *get_seed_element(void) const
      { return seed_elem_handler.getSeedElement(); }

    virtual void Add(Element *);

    int getDefaultTag(void) const;
    void setDefaultTag(const int &tag);

    void clearAll(void);
  };

} // end of XC namespace

#endif
