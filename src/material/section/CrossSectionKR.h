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
//CrossSectionKR.h

#ifndef CROSS_SECTION_KR_H
#define CROSS_SECTION_KR_H

#include "utility/matrix/Vector.h"
#include "utility/matrix/Matrix.h"
#include "utility/actor/actor/MovableObject.h"

namespace XC {

//! @ingroup MATSCC
//
//! @brief Stiffness matrix and resultant vector for a section.
class CrossSectionKR: public CommandEntity, public MovableObject
  {
    friend class FiberPtrDeque;
    double rData[4]; //!< stress resultant vector data.
    Vector *R; //!< stress resultant vector.
    double kData[16]; //!< Stiffness matrix vector.
    Matrix *K; //!< Stiffness matrix.

    static double value,vas1,vas2,vas1as2;//Para guardar resultados parciales.
  protected:
    void free_mem(void);
    void alloc(const size_t &dim);
    void copy(const CrossSectionKR &other);

    int sendData(Communicator &);  
    int recvData(const Communicator &);

    inline void updateNMz(const double &f,const double &y)
      {
        rData[0]+= f; //N.
        rData[1]+= f*y; //Mz.
      }
    inline void updateNMzMy(const double &f,const double &y,const double &z)
      {
        updateNMz(f,y);
        rData[2]+= f*z; //My.
      }
    static inline void updateK2d(double k[],const double &fiberArea,const double &y,const double &tangent)
      {
        value= tangent*fiberArea;
        vas1= y*value;

        k[0]+= value; //Axial stiffness
        k[1]+= vas1;
        k[3]+= vas1 * y; //Bending stiffness.
      }
    inline void updateK2d(const double &fiberArea,const double &y,const double &tangent)
      { updateK2d(kData,fiberArea,y,tangent); }
    static inline void updateK3d(double k[],const double &fiberArea,const double &y,const double &z,const double &tangent)
      {
        value= tangent * fiberArea;
        vas1= y*value;
        vas2= z*value;
        vas1as2= vas1*z;

        k[0]+= value; //Axial stiffness
        k[1]+= vas1;
        k[2]+= vas2;

        k[4]+= vas1 * y;
        k[5]+= vas1as2;

        k[8]+= vas2 * z;        
      }
    inline void updateK3d(const double &fiberArea,const double &y,const double &z,const double &tangent)
      { updateK3d(kData,fiberArea,y,z,tangent); }
    static inline void updateKGJ(double k[],const double &fiberArea,const double &y,const double &z,const double &tangent)
      {
        value= tangent * fiberArea;
        vas1= y*value;
        vas2= z*value;
        vas1as2= vas1*z;

        k[0]+= value; //(0,0)->0
        k[1]+= vas1; //(0,1)->4 y (1,0)->1
        k[2]+= vas2; //(0,2)->8 y (2,0)->2
    
        k[5]+= vas1 * y; //(1,1)->5
        k[6]+= vas1as2; //(1,2)->9 y (2,1)->6
    
        k[10]+= vas2 * z; //(2,2)->10
      }
    inline void updateKGJ(const double &fiberArea,const double &y,const double &z,const double &tangent)
      { updateKGJ(kData,fiberArea,y,z,tangent); }

  public:
    CrossSectionKR(const size_t &dim);
    CrossSectionKR(const CrossSectionKR &other);
    CrossSectionKR &operator=(const CrossSectionKR &);
    virtual ~CrossSectionKR(void);

    void zero(void);
    inline size_t dim(void) const
      { return ((R) ? R->Size():0); }

    inline const Vector &getResultant(void) const
      { return *R; }
    inline Vector &getResultant(void)
      { return *R; }
    inline const Matrix &Stiffness(void) const
      { return *K; }
    inline Matrix &Stiffness(void)
      { return *K; }

    int sendSelf(Communicator &);
    int recvSelf(const Communicator &);
  };

} // end of XC namespace

#endif
