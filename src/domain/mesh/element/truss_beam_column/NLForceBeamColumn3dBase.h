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
//NLForceBeamColumn3dBase.h

#ifndef NLForceBeamColumn3dBase_h
#define NLForceBeamColumn3dBase_h

#include <domain/mesh/element/truss_beam_column/BeamColumnWithSectionFDTrf3d.h>
#include <utility/matrix/Matrix.h>
#include <utility/matrix/Vector.h>
#include "domain/mesh/element/utils/fvectors/FVectorBeamColumn3d.h"
#include "domain/mesh/element/truss_beam_column/EsfBeamColumn3d.h"
#include "domain/mesh/element/utils/coordTransformation/CrdTransf3d.h"

namespace XC {

//! @ingroup BeamColumnElemGrp
//
//! @brief Base class for 3D force beam column elements with PrismaticBarCrossSection type material.
class NLForceBeamColumn3dBase: public BeamColumnWithSectionFDTrf3d
  {
    NLForceBeamColumn3dBase &operator=(const NLForceBeamColumn3dBase &);
  protected:
    static const size_t NDM; //!< dimension of the problem (3d)
    static const int NND; //!< number of nodal dof's
    static const size_t NEGD; //!< number of element global dof's
    static const size_t NEBD; //!< number of element dof's in the basic system
    static const double DefaultLoverGJ;

    
    // internal data
    int maxIters; //!<maximum number of local iterations
    double tol;	 //!<tolerance for relative energy norm for local iterations

    int initialFlag; //!<indicates if the element has been initialized
    bool isTorsion;
	
    Matrix kv; //!<stiffness matrix in the basic system 
    EsfBeamColumn3d Se; //!<element resisting forces in the basic system

    Matrix kvcommit; //!<committed stiffness matrix in the basic system
    EsfBeamColumn3d Secommit; //!<committed element end forces in the basic system

    std::vector<Matrix> fs; //!<array of section flexibility matrices
    std::vector<Vector> vs; //!<array of section deformation vectors
    std::vector<Vector> Ssr; //!<array of section resisting force vectors
    std::vector<Vector> vscommit; //!<array of committed section deformation vectors

    Matrix sp; //!<Applied section forces due to element loads, 5 x nSections
    FVectorBeamColumn3d p0; //!<Reactions in the basic system due to element loads

    mutable Matrix Ki;

    static Matrix theMatrix;
    static Vector theVector;
    static double workArea[];

    void resizeMatrices(const size_t &nSections);
    void initializeSectionHistoryVariables(void);

    int sendData(Communicator &comm);
    int recvData(const Communicator &comm);

  public:
    NLForceBeamColumn3dBase(int tag,int classTag,int numSec= 0);
    NLForceBeamColumn3dBase(int tag,int classTag,int numSec,const Material *theSection,const CrdTransf *coordTransf);
    NLForceBeamColumn3dBase(const NLForceBeamColumn3dBase &);

    void setSection(const PrismaticBarCrossSection *sccModel);

    int getNumDOF(void) const;

    const Matrix &getTangentStiff(void) const;

    const Vector &getResistingForce(void) const;

    //! Axial force which acts over the element at its back end.
    //! ¡Warning! call "calc_resisting_force" before calling this method.
    inline double getAN1(void) const
      {
        return Secommit.AN1()+p0[0];
      }

    //! @brief Axial force which acts over the element at its front end.
    //! ¡Warning! call "calc_resisting_force" before calling this method.
    inline double getAN2(void) const
      {
        return Secommit.AN2();
      }

    //! @brief Axial force at the front end.
    //! ¡Warning! call "calc_resisting_force" before calling this method.
    inline double getN1(void) const
      {                 
        return -Secommit.AN1()-p0[0];
      }

    //! @brief Axial force at the back end.
    //! ¡Warning! call "calc_resisting_force" before calling this method.
    inline double getN2(void) const
      {
        return Secommit.AN2();
      }

    //! @brief Mean axial force.
    //! ¡Warning! call "calc_resisting_force" before calling this method.
    inline double getN(void) const
      {
        return (-Secommit.AN1()-p0[0]+Secommit.AN2())/2.0;
      }

    //! @brief Moment about z axis drived over the bar at its back end.
    //! ¡Warning! call "calc_resisting_force" before calling this method.
    inline double getAMz1(void) const
      {
        return Secommit.Mz1(); 
      }

    //! @brief Moment about z axis drived over the bar at its front end.
    //! ¡Warning! call "calc_resisting_force" before calling this method.
    inline double getAMz2(void) const
      {
        return Secommit.Mz2(); 
      }

    //! @brief Moment about z axis at its back end.
    //! ¡Warning! call "calc_resisting_force" before calling this method.
    inline double getMz1(void) const
      {
        return -Secommit.Mz1(); 
      }

    //! @brief Moment about z axis at its front end.
    //! ¡Warning! call "calc_resisting_force" before calling this method.
    inline double getMz2(void) const
      {
        return Secommit.Mz2(); 
      }

    //! @brief Mean bending moment.
    //! ¡Warning! call "calc_resisting_force" before calling this method.
    inline double getMz(void) const
      { return (this->getMz1()+this->getMz2())/2.0; }

    //! @brief y shear.
    inline double getVy(void) const
      {
        return -Secommit.Vy(theCoordTransf->getInitialLength()); 
      }

    //! @brief y shear drived over the bar at its back end.
    //! ¡Warning! call "calc_resisting_force" before calling this method.
    inline double getAVy1(void) const
      {
        return Secommit.Vy(theCoordTransf->getInitialLength())+p0[1]; 
      }

    //! @brief y shear drived over the bar at its front end.
    //! ¡Warning! call "calc_resisting_force" before calling this method.
    inline double getAVy2(void) const
      {
        return -Secommit.Vy(theCoordTransf->getInitialLength())+p0[2]; 
      }

    //! @brief y shear at its back end.
    //! ¡Warning! call "calc_resisting_force" before calling this method.
    inline double getVy1(void) const
      {
        return this->getVy()-p0[1]; 
      }

    //! @brief y shear at its front end.
    //! ¡Warning! call "calc_resisting_force" before calling this method.
    inline double getVy2(void) const
      {
        return p0[2]+this->getVy(); 
      }

    //! @brief z shear.
    //! ¡Warning! call "calc_resisting_force" before calling this method.
    inline double getVz(void) const
      {
        return -Secommit.Vz(theCoordTransf->getInitialLength()); 
      }

    //! @brief z shear drived over the bar at its back end.
    //! ¡Warning! call "calc_resisting_force" before calling this method.
    inline double getAVz1(void) const
      { //¡Warning! call "calc_resisting_force" before calling this method.
        return Secommit.Vz(theCoordTransf->getInitialLength())+p0[3]; 
      }

    //! @brief z shear drived over the bar at its front end.
    //! ¡Warning! call "calc_resisting_force" before calling this method.
    inline double getAVz2(void) const
      {
        return -Secommit.Vz(theCoordTransf->getInitialLength())+p0[4]; 
      }

    //! @brief z shear at its back end.
    //! ¡Warning! call "calc_resisting_force" before calling this method.
    inline double getVz1(void) const
      {
        return this->getVz()-p0[3]; 
      }

    //! @brief z shear at its front end.
    //! ¡Warning! call "calc_resisting_force" before calling this method.
    inline double getVz2(void) const
      {
        return p0[4]+this->getVz();
      }

    //! @brief Moment about y axis at the back end.
    //! ¡Warning! call "calc_resisting_force" before calling this method.
    inline double getMy1(void) const
      {
        return -Secommit.My1();
      }

    //! @brief Moment about y axis at the front end.
    //! ¡Warning! call "calc_resisting_force" before calling this method.
    inline double getMy2(void) const
      {
        return Secommit.My2();
      }
    
    //! @brief Mean bending moment.
    //! ¡Warning! call "calc_resisting_force" before calling this method.
    inline double getMy(void) const
      { return (this->getMy1()+this->getMy2())/2.0; }
    
    //! @brief Element's torque.
    //! ¡Warning! call "calc_resisting_force" before calling this method.
    inline double getT(void) const
      {
        return Secommit.T();
      }

    //! @brief Torsor at the back end.
    //! ¡Warning! call "calc_resisting_force" before calling this method.
    inline double getT1(void) const
      {
        return -Secommit.T1(); //+p0[0]; 
      }

    //! @brief Torsor at the front end.
    //! ¡Warning! call "calc_resisting_force" before calling this method.
    inline double getT2(void) const
      {
        return Secommit.T2(); 
      }
    
    boost::python::list getValuesAtNodes(const std::string &, bool silent= false) const;
  };
} // end of XC namespace

#endif

