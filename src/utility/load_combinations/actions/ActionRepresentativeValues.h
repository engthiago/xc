// -*-c++-*-
//----------------------------------------------------------------------------
//  xc utils library; general purpose classes and functions.
//
//  Copyright (C)  Luis C. Pérez Tato
//
//  XC utils is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or 
//  (at your option) any later version.
//
//  This software is distributed in the hope that it will be useful, but 
//  WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.  
//
// You should have received a copy of the GNU General Public License 
// along with this program.
// If not, see <http://www.gnu.org/licenses/>.
//----------------------------------------------------------------------------
//ActionRepresentativeValues.hxx
//Representative values of an action.

#ifndef ACTIONREPRESENTATIVEVALUES_H
#define ACTIONREPRESENTATIVEVALUES_H

#include "Action.h"

namespace cmb_acc{

class PartialSafetyFactors;
class CombinationFactors;
class Variations;
class LeadingActionInfo;

//! @ingroup CMBACC
//
//! @brief Representative values of an action.
class ActionRepresentativeValues: public Action
  {
  protected:
    ActionDesignValuesList *acc_familia; //!< Container for the actions of the family.
    const CombinationFactors *combination_factors; //!< Combination factors.

    inline double getCombinationFactor(short int r) const;
    
    friend class ActionDesignValuesList;
    //! @brief Default constructor.
    ActionRepresentativeValues(const std::string &n="", const std::string &descrip="",ActionDesignValuesList *fam= nullptr);
    ActionRepresentativeValues(const Action &a,ActionDesignValuesList *fam= nullptr,const std::string &nmb_comb_factors= "");
  public:
    Action getValue(short int r) const;
    void setCombinationFactors(const std::string &);
    virtual int getIndex(void) const= 0; 
    Action getRepresentativeValue(const LeadingActionInfo &) const;    
    void Print(std::ostream &os) const;    
  };

} //fin namespace nmb_acc.

#endif
