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
///////////////////////////////////////////////////////////////////////////////
//
// COPYRIGHT (C):     :-))
// PROJECT:           Object Oriented Finite Element Program
// FILE:              TwentyNodeBrick.h
// CLASS:             TwentyNodeBrick
// MEMBER FUNCTIONS:
//
// MEMBER VARIABLES
//
// PURPOSE:           Finite Element Class
// RETURN:
// VERSION:
// LANGUAGE:          C++.ver >= 3.0
// TARGET OS:         DOS || UNIX || . . .
// DESIGNER:          Boris Jeremic, Zhaohui Yang and Xiaoyan Wu
// PROGRAMMER:        Boris Jeremic, Zhaohui Yang  and Xiaoyan Wu
// DATE:              Aug. 2001
// UPDATE HISTORY:			 
//
///////////////////////////////////////////////////////////////////////////////
//



#ifndef TWENTYNODEBRICK_H
#define TWENTYNODEBRICK_H

#include <domain/mesh/element/ElementBase.h>
#include "domain/mesh/element/utils/body_forces/BodyForces3D.h"



namespace XC {
class Node;
 class MatPoint3D;
 class BJtensor;
 class NDMaterial;

//! @ingroup ElemVol
//!
//! @brief Twenty node hexahedral element for three-dimensional problems.
class TwentyNodeBrick: public ElementBase<20>
  {
  private:
    // private attributes - a copy for each object of the class

    int numDOF;	//!< Number of element DOF

    Matrix *Ki;

    static Matrix K;    // Element stiffness Matrix
    static Matrix C;    // Element damping matrix
    static Matrix M;    // Element mass matrix
    static Vector P;    // Element resisting force vector
    BodyForces3D bf; //!< Body forces
    
    // double thickness;	// Element thickness
    double rho;		// Mass per unit volume DO WE GET THIS ONE OUT!!!
    double pressure;	// Normal surface traction (pressure) over entire element
    int    order;  	// Order of the quadrature rule

    //Matrix J;		// Jacobian of transformation
    //Matrix L;		// Inverse of J
    //Matrix B;		// Strain interpolation matrix
    
	
  private:
    
    double determinant_of_Jacobian;

    NDMaterial * mmodel;     // pointer to GLOBAL material models
    
    int r_integration_order; // Gauss-Legendre integration order in r direction
    int s_integration_order; // Gauss-Legendre integration order in s direction
    int t_integration_order; // Gauss-Legendre integration order in t direction
    
    // Now I want 3D array of Material points!
    // MatPoint3D[r_integration_order][s_integration_order][t_integration_order]
    // 3D array of Material points
    MatPoint3D ** matpoint;  // pointer to array of Material Points
    
    // this is LM array. This array holds DOFs for this element
    //int  LM[60]; // for 20noded x 3 = 60
  public:
    
    void incremental_Update(void);
    //void iterative_Update(void);

    static BJtensor H_3D(double r1, double r2, double r3);
    BJtensor interp_poli_at(double r, double s, double t);
    static BJtensor dh_drst_at(double r, double s, double t);


    TwentyNodeBrick & operator[](int subscript);
   
    BJtensor getStiffnessTensor(void) const;

    void set_strain_stress_tensor(FILE *fp, double * u);
    BJtensor getMassTensor(void) const;

    BJtensor Jacobian_3D(const BJtensor &dh) const;
    BJtensor Jacobian_3Dinv(const BJtensor &dh) const;
    BJtensor Nodal_Coordinates(void) const;

    BJtensor incr_disp(void) const;
    BJtensor total_disp(void) const;

    BJtensor total_disp(FILE *fp, double * u);

    BJtensor stiffness_matrix(const BJtensor &);
    BJtensor mass_matrix(const BJtensor &);


    int  get_global_number_of_node(int local_node_number);
    int  get_Brick_Number(void);


    //int * get_LM(void);
    //void set_LM(Node * node); // commented out temporarily 09-27-2000 Zhaohui

    //these two files are originally in fe.h
    static double get_Gauss_p_c(short order, short point_numb);
    static double get_Gauss_p_w(short order, short point_numb);

    // returns nodal forces for given stress field in an element
    BJtensor nodal_forces(void) const;
    // returns nodal forces for ITERATIVE stress field in an element
    BJtensor iterative_nodal_forces(void) const;
    // returns nodal forces for given constant stress field in the element
    BJtensor nodal_forces_from_stress(stresstensor &) const;
    // returns nodal forces for given incremental strain field in an element
    // by using the linearized constitutive tensor from the beginning of the step !
    BJtensor linearized_nodal_forces(void) const;

    // updates Material point stresses and strains from given displacements
    BJtensor update_stress_strain(BJtensor &disp);

    void report(const std::string &);
    void reportshort(const std::string &);
    void reportPAK(const std::string &);
    void reportpqtheta(int);
    //void reportLM(const std::string &);
    void computeGaussPoint(void);
    void reportCIPIC(const std::string &);
    void reportTensorF(FILE *);
    Vector getWeightofGP(void);


    // Setting initial E according to the initial pressure
    //void setInitE(void);
    //void reportStressTensorF(FILE *);
  public:
    TwentyNodeBrick(int element_number,
                   int node_numb_1,  int node_numb_2,  int node_numb_3,  int node_numb_4,
                   int node_numb_5,  int node_numb_6,  int node_numb_7,  int node_numb_8,
                   int node_numb_9,  int node_numb_10, int node_numb_11, int node_numb_12,
                   int node_numb_13, int node_numb_14, int node_numb_15, int node_numb_16,
                   int node_numb_17, int node_numb_18, int node_numb_19, int node_numb_20,
		    NDMaterial * Globalmmodel, const BodyForces3D &bForces, double r, double p);

    TwentyNodeBrick(void);
    Element *getCopy(void) const;
    ~TwentyNodeBrick(void);

    int getNumDOF(void) const;	
    void setDomain(Domain *theDomain);

    // public methods to set the state of the element    
    int commitState(void);
    int revertToLastCommit(void);
    int revertToStart(void);

    // update, Guanzhou added Apr. 2004 to update incremental strain in the domain
    int update(void);
    
    // public methods to obtain stiffness, mass, damping and residual information    
    // We haven't build the following functions. 
    // All the value of K M Dmp and F are nothing.
    const Matrix &getTangentStiff(void) const; 
    const Matrix &getInitialStiff(void) const; 
    const Matrix &getMass(void) const; 

    const Matrix &getConsMass(void) const; 
    const Matrix &getLumpedMass(void) const; 

    void alive(void);
    int addLoad(ElementalLoad *theLoad, double loadFactor);    
    //int addLoad(const Vector &addP);
    int addInertiaLoadToUnbalance(const Vector &accel);
    const Vector  FormEquiBodyForce(void);
    const Vector &getResistingForce(void) const;
    const Vector &getResistingForceIncInertia(void) const;

    virtual int sendSelf(Communicator &);
    virtual int recvSelf(const Communicator &);

    void Print(std::ostream &s, int flag =0) const;   
    //    Do nothing with void Print(std::ostream &s, int flag =0);
    //    use Brick3D report.  08/16/00
    Response *setResponse(const std::vector<std::string> &argv, Information &eleInformation);
    int getResponse(int responseID, Information &eleInformation);
};
} // end of XC namespace


#endif

