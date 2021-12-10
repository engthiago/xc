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
/*
//================================================================================
# COPYRIGHT (C):     :-))                                                        #
# PROJECT:           Object Oriented Finite XC::Element Program                      #
# PURPOSE:           General platform for elaso-plastic constitutive model       #
#                    implementation                                              #
# CLASS:             DPEPState (the base class for all Elasto-plastic state)     #
#                                                                                #
# VERSION:                                                                       #
# LANGUAGE:          C++.ver >= 2.0 ( Borland C++ ver=3.00, SUN C++ ver=2.1 )    #
# TARGET OS:         DOS || UNIX || . . .                                        #
# DESIGNER(S):       Boris Jeremic, Zhaohui Yang                                 #
# PROGRAMMER(S):     Boris Jeremic, Zhaohui Yang                                 #
#                                                                                #
#                                                                                #
# DATE:              08-03-2000                                                  #
# UPDATE HISTORY:    May 2004 Guanzhou changed Commit to be consistent           #
#                             with theory                                        #
#		     May 2004, Zhao Cheng splitted the elastic part	         #
#                                                                                #
#                                                                                #
#                                                                                #
# SHORT EXPLANATION: This class is used to hold all state parameters and internal#
#                    variables in an elasto-plastic constitutive model!          #
#                                                                                #
//================================================================================
*/

#ifndef EPState_CPP
#define EPState_CPP

#include "material/nD/Template3Dep/EPState.h"
#include <iostream>

//BJtensor  XC::EPState::Eep( 4, def_dim_4, 0.0 );
//BJtensor  XC::EPState::Eep_commit( 4, def_dim_4, 0.0 );
//BJtensor  XC::EPState::Eep_init( 4, def_dim_4, 0.0 );


//Normal Constructor 1


//ZC05/2004 XC::EPState::EPState(double               Eod,
//ZC05/2004                  double               Ed,
//ZC05/2004                  double               nu,
//ZC05/2004                  double               rho,
XC::EPState::EPState(
                 const stresstensor  &stressp,
                 const straintensor  &strainp,
                 const straintensor  &Estrainp,
                 const straintensor  &Pstrainp,
                 const straintensor  &dEstrainp,
                 const straintensor  &dPstrainp,
                 int                  NScalarp,
                 const std::vector<double> & Scalarp,
                 int                  NTensorp,
                 const std::vector<stresstensor> & Tensorp,
                 const BJtensor       & Eepp,
                 const stresstensor & Stress_commitp,
                 const straintensor & Strain_commitp,
		 const straintensor & ElasticStrain_commitp,
                 const std::vector<double> & Scalar_commitp,
                 const std::vector<stresstensor> & Tensor_commitp,
                 const BJtensor       & Eep_commitp,
                 const stresstensor & Stress_initp,
                 const straintensor & Strain_initp,
                 const std::vector<double> & Scalar_initp,
                 const std::vector<stresstensor> & Tensor_initp,
                 const BJtensor       & Eep_initp,
                 bool                 Convergedp,
//ZC05/2004           int                  Elasticflagp,
//ZC05/2004           double         Evp,
//ZC05/2004           double               nuhvp,
//ZC05/2004           double               Ghvp,
//ZC05/2004           double               eop,
//ZC05/2004           double               ecp,
//ZC05/2004           double               Lamp,
//ZC05/2004           double               pop,
                 double               ep,
                 double               psip,
		 int flag )
//ZC05/2004           double          ap
//ZC05/2004     )
//ZC05/2004: Eo(Eod), E_Young(Ed), nu_Poisson(nu), rho_mass_density(rho), CurrentStress(stressp),
//ZC05/2004  CurrentStrain(strainp), ElasticStrain(Estrainp), PlasticStrain(Pstrainp),
//ZC05/2004  dElasticStrain(dEstrainp), dPlasticStrain(dPstrainp), Eep(Eepp),
//ZC05/2004  Stress_commit(Stress_commitp), Strain_commit(Strain_commitp),
//ZC05/2004  Eep_commit(Eep_commitp), Stress_init(Stress_initp), Strain_init(Strain_initp),
//ZC05/2004  Eep_init(Eep_initp), Converged (Convergedp),
//ZC05/2004  Elasticflag(Elasticflagp),Ev(Evp),nuhv(nuhvp),Ghv(Ghvp),
//ZC05/2004  eo(eop), ec(ecp), Lambda(Lamp), po(pop), e(ep), psi(psip), a(ap)
: CurrentStress(stressp),
  CurrentStrain(strainp), ElasticStrain(Estrainp), PlasticStrain(Pstrainp),
  dElasticStrain(dEstrainp), dPlasticStrain(dPstrainp), Eep(Eepp),
  NScalarVar(NScalarp), ScalarVar(Scalarp), NTensorVar(NTensorp), TensorVar(Tensorp),
  Stress_commit(Stress_commitp), Strain_commit(Strain_commitp), ElasticStrain_commit(ElasticStrain_commitp),
  ScalarVar_commit(Scalar_commitp), TensorVar_commit(Tensor_commitp),
  Eep_commit(Eep_commitp), Stress_init(Stress_initp), Strain_init(Strain_initp),
  ScalarVar_init(Scalar_initp), TensorVar_init(Tensor_initp),
  Eep_init(Eep_initp), Converged (Convergedp), Delta_lambda(0.0),
  e(ep), psi(psip),
  integratorFlag(flag)
  {
      //Eo               = Eod;
      //E_Young          = Ed;
      //nu_Poisson       = nu;
      //rho_mass_density = rho;
  }


//Normal Constructor 11


//ZC05/2004EPState::EPState(double              Eod,
//ZC05/2004                 double              Ed,
//ZC05/2004                 double              nu,
//ZC05/2004                 double              rho,
XC::EPState::EPState(
                 const stresstensor  stressp,
                 const straintensor  strainp,
                 const straintensor  Estrainp,
                 const straintensor  Pstrainp,
                 int                 NScalarp,
                 const std::vector<double> &Scalarp,
                 int                 NTensorp,
                 const std::vector<stresstensor> &Tensorp,
		 double             ep,
		 double             psip,
		 int flag )
//ZC05/2004                 int                 Elasticflagp,
//ZC05/2004                 double              Evp,
//ZC05/2004                 double              nuhvp,
//ZC05/2004                 double              Ghvp,
//ZC05/2004                 double              eop,
//ZC05/2004                 double              ecp,
//ZC05/2004                 double              Lamp,
//ZC05/2004                 double              pop,
//ZC05/2004                 double              ap
//ZC05/2004     )
//: Eo(Eod), E_Young(Ed), nu_Poisson(nu), rho_mass_density(rho),
//  CurrentStress(stressp), CurrentStrain(strainp), ElasticStrain(Estrainp),
//  PlasticStrain(Pstrainp), Stress_commit(stressp), Strain_commit(strainp),
//  Stress_init(stressp), Strain_init(strainp),
//  Elasticflag(Elasticflagp),Ev(Evp),nuhv(nuhvp),Ghv(Ghvp),
//  eo(eop), ec(ecp), Lambda(Lamp),po(pop), e(eop), a(ap)
  : ScalarVar(Scalarp), TensorVar(Tensorp),
    ScalarVar_commit(Scalarp), TensorVar_commit(Tensorp),
    ScalarVar_init(Scalarp), TensorVar_init(Tensorp)
  {

  //ZC05/2004Eo  = Eod;
  //ZC05/2004E_Young  = Ed;
  //ZC05/2004nu_Poisson  = nu;
  //ZC05/2004rho_mass_density  = rho;

  CurrentStress  = stressp;
  CurrentStrain  = strainp;
  ElasticStrain  = Estrainp;
  ElasticStrain_commit = Estrainp;//Guanzhou Mar2005
  Delta_lambda = 0.0;

  PlasticStrain  = Pstrainp;
  Stress_commit  = stressp;
  Strain_commit  = strainp;

  Stress_init  = stressp;
  Strain_init  = strainp;
  integratorFlag = flag;//Guanzhou March2005

  //ZC05/2004Elasticflag  = Elasticflagp;
  //ZC05/2004Ev  = Evp;
  //ZC05/2004nuhv  = nuhvp;
  //ZC05/2004Ghv  = Ghvp;

  //ZC05/2004eo  = eop;
  //ZC05/2004ec  = ecp;
  //ZC05/2004Lambda  = Lamp;
  //ZC05/2004po  = pop;
  //ZC05/2004e  = eop;
  //ZC05/2004a  = ap;

  e = ep;    //ZC
  psi = psip;  //ZC

	//Eo               = Eod;
	//E_Young          = Ed;
	//nu_Poisson       = nu;
	//rho_mass_density = rho;

	//CurrentStress    = stressp;

	//std::cerr << "stressp " << stressp;
	//CurrentStress.null_indices();

	//CurrentStrain    =  strainp;
	//ElasticStrain    =  Estrainp;
	//PlasticStrain    =  Pstrainp;
	//Eep = Eepp;
	Eep = BJtensor( 4, def_dim_4, 0.0 ); // need to be initialized as 4th order BJtensor

	//std::cerr << "strainp " << strainp;
	//CurrentStrain.null_indices();

	NScalarVar = NScalarp;

	NTensorVar = NTensorp;
	//TensorVar = new stresstensor[ NTensorVar ];
	//if ( !TensorVar ) {
	//   std::cerr << "EPState::EPState insufficient memory for Tensor hardening vars";
	//   ::exit(1);
	//}

       Converged = false;
  //ZC05/2004     psi = e - ec;
  }


//Normal Constructor 2
XC::EPState::EPState(
           int                 NScalarp,
           const std::vector<double> &Scalarp,
           int                 NTensorp,
           const std::vector<stresstensor> &Tensorp
//ZC05/2004           int                 Elasticflagp,
//ZC05/2004           double        Evp,
//ZC05/2004           double              nuhvp,
//ZC05/2004           double              Ghvp,
//ZC05/2004           double              eop,
//ZC05/2004           double              ecp,
//ZC05/2004           double              Lamp,
//ZC05/2004           double              pop,
//ZC05/2004           double         ap
     )
//: Eo(Eod), E_Young(Ed), nu_Poisson(nu), rho_mass_density(rho),
//  Elasticflag(Elasticflagp),Ev(Evp),nuhv(nuhvp),Ghv(Ghvp),
//  eo(eop), ec(ecp), Lambda(Lamp), po(pop), e(eop), a(ap)
  : NScalarVar(NScalarp), ScalarVar(Scalarp),
    NTensorVar(NTensorp), TensorVar(Tensorp),
    ScalarVar_commit(Scalarp), TensorVar_commit(Tensorp),
    ScalarVar_init(Scalarp), TensorVar_init(Tensorp)
  {

  //ZC05/2004Eo = Eod;
  //ZC05/2004E_Young = Ed;
  //ZC05/2004nu_Poisson = nu;
  //ZC05/2004rho_mass_density = rho;
  //ZC05/2004Elasticflag = Elasticflagp;
  //ZC05/2004Ev = Evp;
  //ZC05/2004nuhv = nuhvp;
  //ZC05/2004Ghv = Ghvp;
  //ZC05/2004eo = eop;
  //ZC05/2004ec = ecp;
  //ZC05/2004Lambda = Lamp;
  //ZC05/2004po = pop;
  //ZC05/2004e = eop;
  //ZC05/2004a = a;
	Delta_lambda = 0.0;

	e = 0.85;    //ZC
	psi  = 0.05; //ZC
	integratorFlag = 0;//ForwardEuler assumed

	//Eo               = Eod;
	//E_Young          = Ed;
	//nu_Poisson       = nu;
	//rho_mass_density = rho;

	//CurrentStress    = stressp;
	//std::cerr << "CurrentStress " << CurrentStress;
	//CurrentStress.null_indices();
	//CurrentStrain    =  strainp;
	//ElasticStrain    =  Estrainp;
	//PlasticStrain    =  Pstrainp;
	//dElasticStrain   =  dEstrainp;
	//dPlasticStrain   =  dPstrainp;

	Eep = BJtensor( 4, def_dim_4, 0.0 ); // need to be initialized as 4th order BJtensor
	//std::cerr << "strainp " << strainp;
	//CurrentStrain.null_indices();

	Converged = false;
  //ZC05/2004      psi = e - ec;
  }



//Normal Constructor --no parameters


XC::EPState::EPState(void)
//ZC05/2004: Eo(30000.0), E_Young(30000.0), nu_Poisson(0.3), rho_mass_density(0.0),
//ZC05/2004  Converged(false),
//ZC05/2004  Elasticflag(0),Ev(0.0),nuhv(0.0),Ghv(0.0),
//ZC05/2004  eo(0.85), ec(0.80), Lambda(0.025), po(100.0), e(0.85), psi(0.05), a(0.5)
  : NScalarVar(MaxNScalarVar), ScalarVar(MaxNScalarVar),
    NTensorVar(MaxNTensorVar), TensorVar(MaxNTensorVar), 
    ScalarVar_commit(MaxNScalarVar), TensorVar_commit(MaxNTensorVar),
    ScalarVar_init(MaxNScalarVar), TensorVar_init(MaxNTensorVar),
    Converged(false), e(0.85), psi(0.05) //ZC05/2004
    
  {
	//Eo               = 30000.0;
	//E_Young          = 30000.0;
	//nu_Poisson       = 0.3;
	//rho_mass_density = 0.0;
	Eep = BJtensor( 4, def_dim_4, 0.0 );
	integratorFlag = 0;//ForwardEuler assumed
	Delta_lambda = 0.0;
  }


//create a clone of itself


//! @brief Virtual constructor.
XC::EPState* XC::EPState::getCopy(void)
  { return new EPState(*this); }

// Destructor

XC::EPState::~EPState(void)
  {}


//ZC05/2004 
//ZC05/2004 int XC::EPState::getElasticflag(void) const {
//ZC05/2004       return Elasticflag;
//ZC05/2004 }

//ZC05/2004 
//ZC05/2004 double XC::EPState::getE() const {
//ZC05/2004       return E_Young;
//ZC05/2004 }

//ZC05/2004 
//ZC05/2004 // Ev: Young's modulus in a vertical direction -- [out-of-plane]
//ZC05/2004 double XC::EPState::getEv() const {
//ZC05/2004       return Ev;
//ZC05/2004 }

//ZC05/2004 
//ZC05/2004 double XC::EPState::getEo() const {
//ZC05/2004       return Eo;
//ZC05/2004 }

//ZC05/2004 
//ZC05/2004 double XC::EPState::getnu() const {
//ZC05/2004       return nu_Poisson;
//ZC05/2004 }
//ZC05/2004 
//ZC05/2004 
//ZC05/2004 // nuhv: Poisson's ratio for strain in the vertical direction due to a horizontal direct stress -- [out-of-plane]
//ZC05/2004 double XC::EPState::getnuhv() const {
//ZC05/2004       return nuhv;
//ZC05/2004 }

//ZC05/2004 
//ZC05/2004 // Ghv: Modulus for shear deformation in a vertical direction plane-- [out-of-plane]
//ZC05/2004 double XC::EPState::getGhv() const {
//ZC05/2004       return Ghv;
//ZC05/2004 }

//ZC05/2004 
//ZC05/2004 double XC::EPState::getrho() const {
//ZC05/2004       return rho_mass_density;
//ZC05/2004 };


int XC::EPState::getNScalarVar() const
  { return NScalarVar; }


int XC::EPState::getNTensorVar() const
  { return NTensorVar; }


bool XC::EPState::getConverged() const
  { return Converged; }

//ZC05/2004 
//ZC05/2004 double XC::EPState::geteo() const {
//ZC05/2004       return eo;
//ZC05/2004 }

//ZC05/2004 
//ZC05/2004 double XC::EPState::getec() const {
//ZC05/2004       return ec;
//ZC05/2004 }


double XC::EPState::gete() const
  { return e; }

double XC::EPState::getpsi() const
  { return psi; }

int XC::EPState::getIntegratorFlag() const
  { return integratorFlag; }

//ZC05/2004 
//ZC05/2004 double XC::EPState::getLam() const
//  {
//ZC05/2004       return Lambda;
//ZC05/2004 }

//ZC05/2004 
//ZC05/2004 double XC::EPState::getpo() const
//  {
//ZC05/2004       return po;
//ZC05/2004 }

//ZC05/2004 
//ZC05/2004 double XC::EPState::geta() const
//  {
//ZC05/2004       return a;
//ZC05/2004 }


const XC::stresstensor &XC::EPState::getStress(void) const
  { return CurrentStress; }


const XC::stresstensor &XC::EPState::getStress_commit(void) const
  { return Stress_commit; }


const XC::stresstensor &XC::EPState::getStress_init(void) const
  { return Stress_init; }


const XC::stresstensor &XC::EPState::getIterativeStress() const
  { return IterativeStress; }


const XC::straintensor &XC::EPState::getStrain() const
  { return CurrentStrain; }


const XC::straintensor &XC::EPState::getStrain_commit() const
  { return Strain_commit; }


const XC::straintensor &XC::EPState::getStrain_init() const
  { return Strain_init; }


const XC::straintensor &XC::EPState::getElasticStrain() const
  { return ElasticStrain; }

//GZ Mar2005
const XC::straintensor &XC::EPState::getElasticStrain_commit() const
  { return ElasticStrain_commit; }


const XC::straintensor &XC::EPState::getPlasticStrain() const
  { return PlasticStrain; }


const XC::straintensor &XC::EPState::getdElasticStrain() const
  { return dElasticStrain; }



const XC::straintensor &XC::EPState::getdPlasticStrain() const
  { return dPlasticStrain; }



const XC::BJtensor &XC::EPState::getEep() const
  { return Eep; }


const XC::BJtensor &XC::EPState::getEep_commit() const
  { return Eep_commit; }


const XC::BJtensor &XC::EPState::getEep_init() const
  { return Eep_init; }

//ZC05/2004 
//ZC05/2004 void XC::EPState::setElasticflag( int efd ) {
//ZC05/2004       Elasticflag = efd;
//ZC05/2004 }


//ZC05/2004 
//ZC05/2004 void XC::EPState::setEo( double Eod ) {
//ZC05/2004       Eo = Eod;
//ZC05/2004 }


//ZC05/2004 
//ZC05/2004 void XC::EPState::setE( double Ey ) {
//ZC05/2004       E_Young = Ey;
//ZC05/2004 }

//ZC05/2004 
//ZC05/2004 void XC::EPState::setEv( double Evd ) {
//ZC05/2004       Ev = Evd;
//ZC05/2004 }

//ZC05/2004 
//ZC05/2004 void XC::EPState::setGhv( double Ghvd ) {
//ZC05/2004       Ghv = Ghvd;
//ZC05/2004 }

//ZC05/2004 
//ZC05/2004 void XC::EPState::setnu( double nud ) {
//ZC05/2004       nu_Poisson = nud;
//ZC05/2004 }

//ZC05/2004 
//ZC05/2004 void XC::EPState::setnuhv( double nuhvd ) {
//ZC05/2004       nuhv = nuhvd;
//ZC05/2004 }


void XC::EPState::setStress(const stresstensor &newstress )
  { CurrentStress = newstress; }


void XC::EPState::setIterativeStress(const stresstensor &newstress )
  { IterativeStress = newstress; }



void XC::EPState::setStrain(const straintensor &newstrain )
  { CurrentStrain = newstrain; }


void XC::EPState::setStress_commit(const stresstensor &newstress )
  { Stress_commit = newstress; }


void XC::EPState::setStrain_commit(const straintensor &newstrain )
  { Strain_commit = newstrain; }

void XC::EPState::setElasticStrain_commit(const straintensor &newstrain )
  { ElasticStrain_commit = newstrain; }



void XC::EPState::setStress_init(const stresstensor &newstress )
  { Stress_init = newstress; }


void XC::EPState::setStrain_init(const straintensor &newstrain )
  { Strain_init = newstrain; }


void XC::EPState::setElasticStrain(const straintensor &newEstrain )
  { ElasticStrain = newEstrain; }


void XC::EPState::setPlasticStrain(const straintensor &newPstrain )
  { PlasticStrain = newPstrain; }



void XC::EPState::setdElasticStrain(const straintensor &newdEstrain )
  { dElasticStrain = newdEstrain; }


void XC::EPState::setdPlasticStrain(const straintensor &newdPstrain )
  { dPlasticStrain = newdPstrain; }


void XC::EPState::setEep(const BJtensor &newEep )
  { Eep = newEep; }


void XC::EPState::setConverged( bool b)
  { Converged = b; }

//ZC05/2004 
//ZC05/2004 void XC::EPState::seteo( double eod ) {
//ZC05/2004       eo = eod;
//ZC05/2004 }

//ZC05/2004 
//ZC05/2004 void XC::EPState::setec( double ecd ) {
//ZC05/2004       ec = ecd;
//ZC05/2004 }

//ZC05/2004 
//ZC05/2004 void XC::EPState::setLam( double Lamd ) {
//ZC05/2004       Lambda = Lamd;
//ZC05/2004 }

//ZC05/2004 
//ZC05/2004 void XC::EPState::setpo( double pod ) {
//ZC05/2004       po = pod;
//ZC05/2004 }

//ZC05/2004 
//ZC05/2004 void XC::EPState::seta( double ad ) {
//ZC05/2004       a = ad;
//ZC05/2004 }


void XC::EPState::sete( double ed )
  { e = ed; }


void XC::EPState::setpsi( double psid )
  { psi = psid; }


// Return the nth Scalar Var.... Starting from 1!!

double XC::EPState::getScalarVar( int WhichOne) const
  {

      if ( WhichOne <= getNScalarVar() )
         return ScalarVar[ WhichOne - 1 ];
      else
      {
   std::cerr << "EPState::getScalarVar Out of ScalarVar's range: " <<  getNScalarVar()  << std::endl;
         return 0.0;
   exit(1);
      }

      return 0.0;

  }



// Return the nth Tensor Var.... Starting from 1!!

XC::stresstensor XC::EPState::getTensorVar(int WhichOne) const
  {
      stresstensor st;

      if ( WhichOne <= getNTensorVar() )
         return TensorVar[ WhichOne - 1 ];
      else
      {
  std::cerr << "EPState::getTensorVar No. %d: Out of Tensortial Var's range " << WhichOne << " out of " <<  getNTensorVar() << std::endl;
   exit(1);
      }

      return st;
  }


// Return Scalar pointer

const std::vector<double> &XC::EPState::getScalarVar() const
  { return ScalarVar; }


void XC::EPState::setNScalarVar(int rval)
  { NScalarVar = rval; }


void XC::EPState::setNTensorVar(int rval)
  { NTensorVar = rval; }


const std::vector<double> & XC::EPState::getScalarVar_commit(void) const
  { return ScalarVar_commit; }


double XC::EPState::getScalarVar_commit( int i) const
  { return ScalarVar_commit[i-1]; }


const std::vector<double> & XC::EPState::getScalarVar_init(void) const
  { return ScalarVar_init; }


double XC::EPState::getScalarVar_init(int i) const
  { return ScalarVar_init[i]; }


// Return Tensor pointer

const std::vector<XC::stresstensor> &XC::EPState::getTensorVar(void) const
  { return TensorVar; }


const std::vector<XC::stresstensor> &XC::EPState::getTensorVar_commit() const
  { return TensorVar_commit; }


XC::stresstensor XC::EPState::getTensorVar_commit(int i) const
  { return TensorVar_commit[i-1]; }


XC::stresstensor XC::EPState::getTensorVar_init(int i) const
  { return TensorVar_init[i-1]; }


const std::vector<XC::stresstensor> & XC::EPState::getTensorVar_init() const
  { return TensorVar_init; }


// set nth Scalar Var.... Starting from 1!!

void XC::EPState::setScalarVar(int WhichOne, double rval)
  {

      if ( WhichOne <= getNScalarVar() )
         ScalarVar[ WhichOne - 1 ] = rval;
      else
      {
         std::cerr << "EPState::setScalarVar Out of ScalarVar's range " << getNScalarVar() << std::endl;
         //std::cerr << " Out of ScalarVar's range!";
   exit(1);
      }
}
void XC::EPState::setScalarVar_commit(int WhichOne, double rval) {

      if ( WhichOne <= getNScalarVar() )
         ScalarVar_commit[ WhichOne - 1 ] = rval;
      else
      {
  std::cerr << "EPState::setScalarVar Out of ScalarVar's range " <<  getNScalarVar() << std::endl;
         //std::cerr << " Out of ScalarVar's range!";
   exit(1);
      }
}

void XC::EPState::setScalarVar_init(int WhichOne, double rval) {

      if ( WhichOne <= getNScalarVar() )
         ScalarVar_init[ WhichOne - 1 ] = rval;
      else
      {
  std::cerr << "EPState::setScalarVar Out of ScalarVar's range " <<  getNScalarVar() << std::endl;
         //std::cerr << " Out of ScalarVar's range!";
   exit(1);
      }
}



// set nth Tensor Var.... Starting from 1!!

void XC::EPState::setTensorVar(int WhichOne, const stresstensor &rval) {

      if ( WhichOne <= getNTensorVar() )
         TensorVar[ WhichOne - 1 ] = rval;
      else
      {
  std::cerr << "EPState::setTensorVar Out of Tensor Var's range " <<  getNTensorVar() << std::endl;
   exit(1);
      }

}

void XC::EPState::setTensorVar_commit(int WhichOne, const stresstensor &rval) {

      if ( WhichOne <= getNTensorVar() )
         TensorVar_commit[ WhichOne - 1 ] = rval;
      else
      {
  std::cerr << "EPState::setTensorVar Out of Tensor Var's range " <<  getNTensorVar()<< std::endl;
   exit(1);
      }

  }

void XC::EPState::setTensorVar_init(int WhichOne, const stresstensor &rval)
  {

      if ( WhichOne <= getNTensorVar() )
         TensorVar_init[ WhichOne - 1 ] = rval;
      else
      {
  std::cerr << "EPState::setTensorVar Out of Tensor Var's range " <<  getNTensorVar() << std::endl;
   exit(1);
      }

  }

//! @brief set all Scalar Vars ..No boundary checking!
void XC::EPState::setScalarVar(const std::vector<double> &rval)
  { ScalarVar= rval; }


//! @brief set all Scalar Vars
void XC::EPState::setTensorVar(const std::vector<stresstensor> &rval)
  { TensorVar= rval; }


void XC::EPState::print()
  { std::cerr << *this; }



//! @brief Set all state variables to initials
void XC::EPState::setInit()
  {
    stresstensor Stress0;
    straintensor Strain0;

    CurrentStress   = Stress_init;
    CurrentStrain   = Strain_init;
    ElasticStrain   = Strain0;
    PlasticStrain   = Strain0;
    dElasticStrain  = Strain0;
    dPlasticStrain  = Strain0;
    Eep = Eep_init;

    Stress_commit   = Stress_init;
    Strain_commit   = Strain_init;
    Eep_commit = Eep_init;
    ElasticStrain_commit = Strain0;

    ScalarVar= ScalarVar_init;
    ScalarVar_commit= ScalarVar_init;

    TensorVar= TensorVar_init;
    TensorVar_commit= TensorVar_init;

    Converged = false;
  }


int XC::EPState::commitState(void)
  {


//      std::cerr << "\n\n\n\n int EPState::commitState ()\n\n\n\n";


    int err = 0;
      // commit the variables state
//Guanzhou out 5-6-2004      CurrentStress   = Stress_init;
//Guanzhou out 5-6-2004      CurrentStrain   = Strain_init;
//Guanzhou out 5-6-2004      Eep = Eep_init;

    Stress_commit   = CurrentStress;
    Strain_commit   = CurrentStrain;
    Eep_commit = Eep;


    //GZ Mar2005
    ElasticStrain_commit = ElasticStrain;

    //ScalarVar = ScalarVar_init;
    ScalarVar_commit= ScalarVar;
    //TensorVar= TensorVar_init;
    TensorVar_commit= TensorVar;
    return err;
  }


int XC::EPState::revertToLastCommit () {
      int err = 0;
      // revert the variables state  to last committed
      CurrentStress   = Stress_commit;
      CurrentStrain   = Strain_commit;
      Eep = Eep_commit;

      ElasticStrain = ElasticStrain_commit;

    int i;
      for (i = 0; i < NScalarVar; i++) {
          //ScalarVar[i] = ScalarVar_init[i];
          ScalarVar[i] = ScalarVar_commit[i];
      }

      for (i = 0; i < NTensorVar; i++) {
         //TensorVar[i] = TensorVar_init[i];
         TensorVar[i] = TensorVar_commit[i];
      }

      return err;

}

//! @brief Revert the material to its initial state.
int XC::EPState::revertToStart(void)
  {
    int err = 0;
    // revert the variables state to the initials
    CurrentStress   = Stress_init;
    CurrentStrain   = Strain_init;
    Eep = Eep_init;

    Stress_commit   = Stress_init;
    Strain_commit   = Strain_init;
    Eep_commit = Eep_init;
    //ElasticStrain_commit = Strain0;

  int i;
    for (i = 0; i < NScalarVar; i++) {
	ScalarVar[i] = ScalarVar_init[i];
	ScalarVar_commit[i] = ScalarVar_init[i];
    }

    for (i = 0; i < NTensorVar; i++) {
       TensorVar[i] = TensorVar_init[i];
       TensorVar_commit[i] = TensorVar_init[i];
    }

    return err;
  }






// Overloaded Insertion Operator
// prints an XC::EPState's contents

std::ostream &XC::operator<<(std::ostream &os, const EPState & EPS)
    {
      //        os.setf( ios::showpos | ios::scientific);
      os.precision(4);
        os.width(10);
//ZC05/2004         os << std::endl << "Elastic plastic state parameters: "  << std::endl;
//ZC05/2004 
//ZC05/2004   int ef = EPS.getElasticflag();
//ZC05/2004   os << "\tElastic Flag = " << ef << ";";
//ZC05/2004   if (ef == 1)
//ZC05/2004      os << " pressure dependent isotropic material (default case, for soil)." << std::endl;
//ZC05/2004   else if (ef == 2)
//ZC05/2004      os << " pressure independent isotropic material." << std::endl;
//ZC05/2004   else if (ef == 3)
//ZC05/2004      os << " pressure independent cross-anisotropic material." << std::endl;
//ZC05/2004   else if (ef == 4)
//ZC05/2004      os << " pressure dependent cross-anisotropic material." << std::endl;
//ZC05/2004   else
//ZC05/2004      os << " elastic portion code not correct. Flag must be 1, 2, 3 or 4." << std::endl;


//ZC05/2004         os << "\tEo = " << EPS.getEo() << ";";
//ZC05/2004         os << " E_Young = " << EPS.getE() << ";";
//ZC05/2004         //os.width(10);
//ZC05/2004   os << " nu_Poisson = " << EPS.getnu() << ";";
//ZC05/2004         os << " \tE_v = " << EPS.getEv() << ";";
//ZC05/2004         os << " nu_hv = " << EPS.getnuhv() << ";";
//ZC05/2004   os << " G_hv = " << EPS.getGhv() << ";";
//ZC05/2004   os << " rho = " << EPS.getrho() << std::endl;
//ZC05/2004 
//ZC05/2004         os << "\teo = " << EPS.geteo() << ";";
//ZC05/2004         os << " ec = " << EPS.getec() << ";";
//ZC05/2004         os << " Lambda = " << EPS.getLam() << ";";
//ZC05/2004         os << " po = " << EPS.getpo() << ";";
  os << " e = " << EPS.gete() << std::endl;
  os << " psi = " << EPS.getpsi() << std::endl;
//ZC05/2004         os << " a = " << EPS.geta() << ";";

  if ( EPS.getConverged() )
     os << "\tConverged = ok! ";
  else
     os << "\tConverged = no! " << std::endl;

        //os.width(10);
        os << std::endl << "\tCurrent Stress:" << EPS.getStress() << std::endl;
        os << "\tIterati Stress:" << EPS.getIterativeStress() << std::endl;

  os << "\tCurrent Strain:" << EPS.getStrain() << std::endl;
  os << "\tElasticStrain :" << EPS.getElasticStrain() << std::endl;
  os << "\tPlasticStrain :" << EPS.getPlasticStrain() << std::endl;
  os << "\tdElasticStrain:" << EPS.getdElasticStrain() << std::endl;
  os << "\tdPlasticStrain:" << EPS.getdPlasticStrain() << std::endl;
  os << "\tEep.rank():" << EPS.getEep().rank() << std::endl;

  //        os.unsetf( ios::showpos );
  int NS = EPS.getNScalarVar();
  int NT = EPS.getNTensorVar();

  os << std::endl << "\tNScalarVar = " << NS << std::endl;

    int i;
        for (i = 0; i < NS; i++) {
            os << "\tNo." << i+1 << " " << EPS.ScalarVar[i] << "; ";
  }
        os << std::endl << std::endl;

        os << "\tNTensorVar = " << NT;
        for (i = 0; i < NT; i++) {
    //           os.unsetf( ios::showpos);
           os << std::endl << "\tNo." << i+1 << " tensorial var:";
     //           os.setf( ios::showpos);
           os << EPS.TensorVar[i];
        }

        os << std::endl;
        return os;
    }

#endif

