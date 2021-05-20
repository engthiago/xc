// -*-c++-*-
//----------------------------------------------------------------------------
//  XC program; finite element analysis code
//  for structural analysis and design.
//
//  Copyright (C)  Luis Claudio Pérez Tato
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
//DqGroundMotions.h

#ifndef DqGroundMotions_h
#define DqGroundMotions_h

#include "utility/kernel/CommandEntity.h"
#include <deque>
#include "utility/actor/actor/MovableObject.h"


namespace XC {

  class GroundMotion;

//! @ingroup GMotion
//
//! @brief Seismic definitions container.
  class DqGroundMotions: public CommandEntity, public MovableObject
  {
  protected:
    typedef std::deque<GroundMotion *> dq_ptr_GroundMotions;
    typedef dq_ptr_GroundMotions::const_reference const_reference;
    typedef dq_ptr_GroundMotions::reference reference;
    typedef dq_ptr_GroundMotions::iterator iterator;
    typedef dq_ptr_GroundMotions::const_iterator const_iterator;

    dq_ptr_GroundMotions gMotions; //!< Pointers to GroundMotion container.



    int sendData(Communicator &);
    int recvData(const Communicator &);

    
  public:
    DqGroundMotions(const size_t &num= 0);
    DqGroundMotions(const DqGroundMotions &);
    DqGroundMotions &operator=(const DqGroundMotions &);
    ~DqGroundMotions(void);

    inline bool empty(void) const
      { return gMotions.empty(); }
    inline size_t getNumGroundMotions(void) const
      { return gMotions.size(); }

    inline iterator begin(void)
      { return gMotions.begin(); }
    inline const_iterator begin(void) const
      { return gMotions.begin(); }
    inline iterator end(void)
      { return gMotions.end(); }
    inline const_iterator end(void) const
      { return gMotions.end(); }

    inline reference operator[](const size_t &n)
      { return gMotions[n]; }
    inline const_reference operator[](const size_t &n) const
      { return gMotions[n]; }

    void addMotion(GroundMotion &f);

    void clear(void);

    int sendSelf(Communicator &);  
    int recvSelf(const Communicator &);


    void Print(std::ostream &s,const int &flag) const;
  };
} // end of XC namespace

#endif
