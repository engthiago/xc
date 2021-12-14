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
/* ****************************************************************** **
**    OpenSees - Open System for Earthquake Engineering Simulation    **
**          Pacific Earthquake Engineering Research Center            **
**                                                                    **
**                                                                    **
** (C) Copyright 1999, The Regents of the University of California    **
** All Rights Reserved.                                               **
**                                                                    **
** Commercial use of this program without express permission of the   **
** University of California, Berkeley, is strictly prohibited.  See   **
** file 'COPYRIGHT'  in main directory for information on usage and   **
** redistribution,  and for a DISCLAIMER OF ALL WARRANTIES.           **
**                                                                    **
** Developed by:                                                      **
**   Frank McKenna (fmckenna@ce.berkeley.edu)                         **
**   Gregory L. Fenves (fenves@ce.berkeley.edu)                       **
**   Filip C. Filippou (filippou@ce.berkeley.edu)                     **
**                                                                    **
** ****************************************************************** */
                                                                        
// $Revision: 1.2 $
// $Date: 2003/02/25 23:33:14 $
// $Source: /usr/local/cvs/OpenSees/SRC/handler/OPS_Stream.h,v $

#ifndef _OPS_Stream
#define _OPS_Stream

#include <string>
#include <fstream>

namespace XC{
enum openMode  {OVERWRITE, APPEND};
enum floatField {FIXEDD, SCIENTIFIC};

//! @ingroup Utils
//!
//! @defgroup StreamsGrp OpenSees streams abstraction.
//
//! @ingroup StreamsGrp
//
//! @brief Base class for OpenSees streams.
class OPS_Stream
  {
  protected:
    std::ofstream theFile;
    bool fileOpen;
  public:
    OPS_Stream(void);
    virtual ~OPS_Stream(void);

    int close(void);
    
    virtual int setFile(const std::string &fileName, openMode mode = OVERWRITE);
    virtual int setPrecision(int precision);
    virtual int setFloatField(floatField);
    virtual int precision(int precision) {return 0;};
    virtual int width(int width) {return 0;};

    virtual OPS_Stream &write(const char *s, int n);
    virtual OPS_Stream &write(const unsigned char *s, int n);
    virtual OPS_Stream &write(const signed char *s, int n);
    virtual OPS_Stream &write(const void *s, int n);
    virtual OPS_Stream &write(const char &c);
    virtual OPS_Stream &write(const unsigned char &c);
    virtual OPS_Stream &write(const signed char &c);
    virtual OPS_Stream &write(const char *s);
    virtual OPS_Stream &write(const std::string &s);
    virtual OPS_Stream &write(const unsigned char *s);
    virtual OPS_Stream &write(const signed char *s);
    virtual OPS_Stream &write(const void *p);
    virtual OPS_Stream &write(const int &n);
    virtual OPS_Stream &write(const unsigned int &n);
    virtual OPS_Stream &write(const long &n);
    virtual OPS_Stream &write(const unsigned long &n);
    virtual OPS_Stream &write(const short &n);
    virtual OPS_Stream &write(const unsigned short &n);
    virtual OPS_Stream &write(const bool &b);
    virtual OPS_Stream &write(const double &d);
    virtual OPS_Stream &write(const float &f);

    OPS_Stream& operator<<(const char &c);
    OPS_Stream& operator<<(const unsigned char &c);
    OPS_Stream& operator<<(const signed char &c);
    OPS_Stream& operator<<(const char *s);
    OPS_Stream& operator<<(const std::string &s);
    OPS_Stream& operator<<(const unsigned char *s);
    OPS_Stream& operator<<(const signed char *s);
    OPS_Stream& operator<<(const void *p);
    OPS_Stream& operator<<(const int &n);
    OPS_Stream& operator<<(const unsigned int &n);
    OPS_Stream& operator<<(const long &n);
    OPS_Stream& operator<<(const unsigned long &n);
    OPS_Stream& operator<<(const short &n);
    OPS_Stream& operator<<(const unsigned short &n);
    OPS_Stream& operator<<(const bool &b);
    OPS_Stream& operator<<(const double &n);
    OPS_Stream& operator<<(const float &n);
    
    // OPS_Stream& operator<<(__omanip func);
    // OPS_Stream& operator<<(__manip func);
    // OPS_Stream& operator<<(streambuf*);
    // OPS_Stream& ends(OPS_Stream& outs);
    // OPS_Stream& flush(OPS_Stream& outs);
    // OPS_Stream& (OPS_Stream& outs);
  };

} //end of XC namespace

#endif
