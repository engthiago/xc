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

// $Revision: 1.3 $
// $Date: 2004/10/06 19:21:45 $
// $Source: /usr/local/cvs/OpenSees/SRC/element/special/joint/BeamColumnJoint2d.cpp,v $

// Written: NM (nmitra@u.washington.edu)
// Created: April 2002
// Revised: August 2004
//
// Description: This file contains the class implementation for beam-column joint.
// This element is a 4 noded 12 dof (3 dof at each node) finite area super-element introduced by
// Prof. Lowes and Arash. The element takes in 13 different material types in order to simulate
// the inelastic action observed in a reinforced beam column joint.
// Updates: Several concerning Joint formulation (presently a revised formulation for joints)


#include <domain/mesh/element/special/joint/BeamColumnJoint2d.h>

#include <domain/domain/Domain.h>
#include <domain/mesh/node/Node.h>
#include <utility/actor/objectBroker/FEM_ObjectBroker.h>
#include <domain/mesh/element/truss_beam_column/nonlinearBeamColumn/matrixutil/MatrixUtil.h>

#include <material/uniaxial/UniaxialMaterial.h>
#include <utility/recorder/response/ElementResponse.h>

// full constructors:
XC::BeamColumnJoint2d::BeamColumnJoint2d(int tag,int Nd1, int Nd2, int Nd3, int Nd4, const UniaxialMaterial &theMat1, const UniaxialMaterial &theMat2, const UniaxialMaterial &theMat3, const UniaxialMaterial &theMat4, const UniaxialMaterial &theMat5, const UniaxialMaterial &theMat6, const UniaxialMaterial &theMat7, const UniaxialMaterial &theMat8, const UniaxialMaterial &theMat9, const UniaxialMaterial &theMat10, const UniaxialMaterial &theMat11, const UniaxialMaterial &theMat12, const UniaxialMaterial &theMat13):
ElemWithMaterial<4,BeamColumnJointPhysicalProperties>(tag,ELE_TAG_BeamColumnJoint2d,BeamColumnJointPhysicalProperties(theMat1, theMat2, theMat3, theMat4, theMat5, theMat6, theMat7, theMat8, theMat9, theMat10, theMat11, theMat12, theMat13)),
  nodeDbTag(0), dofDbTag(0), elemActHeight(0.0), elemActWidth(0.0),
  elemWidth(0.0), elemHeight(0.0), HgtFac(1.0), WdtFac(1.0),
  Uecommit(12), UeIntcommit(4), UeprCommit(12), UeprIntCommit(4),
  BCJoint(13,16), dg_df(4,13), dDef_du(13,4), K(12,12), R(12)
  {
    theNodes.set_id_nodes(Nd1,Nd2,Nd3,Nd4);

    Uecommit.Zero();
    UeIntcommit.Zero();
    UeprCommit.Zero();
    UeprIntCommit.Zero();

    BCJoint.Zero(); dg_df.Zero(); dDef_du.Zero();

    K.Zero();
    R.Zero();
  }

// full constructors:
XC::BeamColumnJoint2d::BeamColumnJoint2d(int tag,int Nd1, int Nd2, int Nd3, int Nd4, const UniaxialMaterial &theMat1, const UniaxialMaterial &theMat2, const UniaxialMaterial &theMat3, const UniaxialMaterial &theMat4, const UniaxialMaterial &theMat5, const UniaxialMaterial &theMat6, const UniaxialMaterial &theMat7, const UniaxialMaterial &theMat8, const UniaxialMaterial &theMat9, const UniaxialMaterial &theMat10, const UniaxialMaterial &theMat11, const UniaxialMaterial &theMat12, const UniaxialMaterial &theMat13, double elHgtFac, double elWdtFac):
ElemWithMaterial<4,BeamColumnJointPhysicalProperties>(tag,ELE_TAG_BeamColumnJoint2d,BeamColumnJointPhysicalProperties(theMat1, theMat2, theMat3, theMat4, theMat5, theMat6, theMat7, theMat8, theMat9, theMat10, theMat11, theMat12, theMat13)),
  nodeDbTag(0), dofDbTag(0), elemActHeight(0.0), elemActWidth(0.0),
  elemWidth(0.0), elemHeight(0.0), HgtFac(elHgtFac), WdtFac(elWdtFac),
  Uecommit(12), UeIntcommit(4), UeprCommit(12), UeprIntCommit(4),
  BCJoint(13,16), dg_df(4,13), dDef_du(13,4), K(12,12), R(12)
  {
    theNodes.set_id_nodes(Nd1,Nd2,Nd3,Nd4);


        Uecommit.Zero();
        UeIntcommit.Zero();
        UeprCommit.Zero();
        UeprIntCommit.Zero();

        BCJoint.Zero(); dg_df.Zero(); dDef_du.Zero();

        K.Zero();
        R.Zero();

  }

// default constructor:
XC::BeamColumnJoint2d::BeamColumnJoint2d(void):
  ElemWithMaterial<4,BeamColumnJointPhysicalProperties>(0,ELE_TAG_BeamColumnJoint2d,BeamColumnJointPhysicalProperties(13,nullptr)),
  nodeDbTag(0), dofDbTag(0), elemActHeight(0.0), elemActWidth(0.0),
  elemWidth(0.0), elemHeight(0.0), HgtFac(1.0), WdtFac(1.0),
  Uecommit(12), UeIntcommit(4), UeprCommit(12), UeprIntCommit(4),
  BCJoint(13,16), dg_df(4,13), dDef_du(13,4), K(12,12), R(12)
  {}

//! @brief Virtual constructor.
XC::Element* XC::BeamColumnJoint2d::getCopy(void) const
  { return new BeamColumnJoint2d(*this); }

int XC::BeamColumnJoint2d::getNumDOF(void) const
  { return 12; }

void XC::BeamColumnJoint2d::setDomain(Domain *theDomain)
  {
    if(theDomain == 0)
        std::cerr << "ERROR: BeamColumnJoint::setDomain -- Domain is null" << std::endl;

    // call the base class method
    ElemWithMaterial<4,BeamColumnJointPhysicalProperties>::setDomain(theDomain);

    // ensure connected nodes have correct dof's
    const int dofNd1= theNodes[0]->getNumberDOF();
    bool equalNumDofs= theNodes.equalNumDOF();
    if((dofNd1 != 3) || (!equalNumDofs))
      {
         std::cerr << "ERROR : BeamColumnJoint::setDomain -- number of DOF associated with the node incorrect"<< std::endl;
         exit(-1); // do not go any further - otherwise segmentation fault
      }


    // obtain the nodal coordinates
    const Vector &end1Crd = theNodes[0]->getCrds();
    const Vector &end2Crd = theNodes[1]->getCrds();
    const Vector &end3Crd = theNodes[2]->getCrds();
    const Vector &end4Crd = theNodes[3]->getCrds();

    Vector Node1(end1Crd);
    Vector Node2(end2Crd);
    Vector Node3(end3Crd);
    Vector Node4(end4Crd);

    // set the height and width of the element and perform check
    Node3 = Node3 - Node1;
    Node2 = Node2 - Node4;

        double beam = 1.0; //HgtFac;
        double col = 1.0; //WdtFac;

        elemActHeight = fabs(Node3.Norm());
        elemActWidth = fabs(Node2.Norm());
        elemHeight = beam*elemActHeight;
        elemWidth = col*elemActWidth;

        if((elemHeight <= 1e-12) || (elemWidth <= 1e-12))
          {
                std::cerr << "ERROR : BeamColumnJoint::setDomain -- length or width not correct, division by zero occurs"<< std::endl;
                exit(-1); // donot go any further - otherwise segmentation fault
        }

        getBCJoint();
        getdg_df();
        getdDef_du();
}

int XC::BeamColumnJoint2d::commitState(void)
  {
    // store committed external nodal displacements
    Uecommit = UeprCommit;
    // store committed internal nodal displacements
    UeIntcommit = UeprIntCommit;

    // store material history data.
    int mcs= ElemWithMaterial<4,BeamColumnJointPhysicalProperties>::commitState();
    return mcs;
  }

int XC::BeamColumnJoint2d::revertToLastCommit(void)
  {
     int mcs= ElemWithMaterial<4,BeamColumnJointPhysicalProperties>::revertToLastCommit();
     UeprCommit = Uecommit;
     UeprIntCommit = UeIntcommit;

     this->update();
     return mcs;
  }

int XC::BeamColumnJoint2d::revertToStart(void)
  {
     int mcs= ElemWithMaterial<4,BeamColumnJointPhysicalProperties>::revertToStart();
     return mcs;
  }

int XC::BeamColumnJoint2d::update(void)
{
                Vector Ue(16);
                Ue.Zero();

        // determine committed displacements given trial displacements
                getGlobalDispls(Ue);

                // update displacements for the external nodes
                UeprCommit.Extract(Ue,0,1.0);
                // update displacement for the internal nodes
                UeprIntCommit.Extract(Ue,12,1.0);

                return 0;
}

const XC::Matrix &XC::BeamColumnJoint2d::getTangentStiff(void) const
  {
    if(isDead())
      K*=dead_srf;
    return K;
  }

const XC::Matrix &XC::BeamColumnJoint2d::getInitialStiff(void) const
  { return getTangentStiff(); }

const XC::Vector &XC::BeamColumnJoint2d::getResistingForce(void) const
  { 
    if(isDead())
      R*=dead_srf;
    return R;
  }

void XC::BeamColumnJoint2d::getGlobalDispls(Vector &dg)
  {
    // local variables that will be used in this method
    int converge = 0;
    int linesearch = 0;
    int totalCount = 0;
        int dtConverge = 0;
        int incCount = 0;
        int count = 0;
        int maxTotalCount = 1000;
        int maxCount = 20;
        double loadStep = 0.0;
        double dLoadStep = 1.0;
        double stepSize = 0.0;

        Vector uExtOld(12);   uExtOld.Zero();
        Vector uExt(12);      uExt.Zero();
        Vector duExt(12);     duExt.Zero();
        Vector uIntOld(4);    uIntOld.Zero();
        Vector uInt(4);       uInt.Zero();
        Vector duInt(4);      duInt.Zero();
        Vector duIntTemp(4);  duIntTemp.Zero();
        Vector intEq(4);      intEq.Zero();
        Vector intEqLast(4);  intEqLast.Zero();
        Vector Uepr(12);      Uepr.Zero();
        Vector UeprInt(4);    UeprInt.Zero();
        Vector Ut(12);        Ut.Zero();


    Vector disp1 = theNodes[0]->getTrialDisp();
    Vector disp2 = theNodes[1]->getTrialDisp();
    Vector disp3 = theNodes[2]->getTrialDisp();
    Vector disp4 = theNodes[3]->getTrialDisp();

        for(int i = 0; i < 3; i++)
    {
      Ut(i)     = disp1(i);
      Ut(i+3)   = disp2(i);
      Ut(i+6)   = disp3(i);
      Ut(i+9)   = disp4(i);
    }

        Uepr = Uecommit;

        UeprInt = UeprIntCommit;

        uExtOld = Uepr;
        duExt = Ut -Uepr;

        uExt = uExtOld;

        uIntOld = UeprInt;
        uInt = uIntOld;

        double tol = 1e-12;
        double tolIntEq = tol;
        double toluInt = (tol>tol*uInt.Norm())? tol:tol*uInt.Norm();
        double tolIntEqdU = tol;
        double ctolIntEqdU = tol;
        double ctolIntEq = tol;
        double normDuInt = toluInt;
        double normIntEq = tolIntEq;
        double normIntEqdU = tolIntEqdU;

        Vector u(16);
        u.Zero();

        double engrLast = 0.0;
        double engr = 0.0;

        Vector fSpring(13);   fSpring.Zero();
        Vector kSpring(13);   kSpring.Zero();
    Matrix dintEq_du(4,4);     dintEq_du.Zero();
    Matrix df_dDef(13,13);     df_dDef.Zero();
    Matrix tempintEq_du (4,13);  tempintEq_du.Zero();


         while ((loadStep < 1.0) && (totalCount < maxTotalCount))
        {
                count = 0;
                converge = 0;
                dtConverge = 0;
                while ((!converge) && (count < maxCount))
                {
                        if(dLoadStep <= 1e-3)
                        {
                                dLoadStep = dLoadStep;
                        }
                        totalCount ++;
                        count ++;

                        for(int ic = 0; ic < 12; ic++ ) {
                                u(ic) = uExt(ic) + duExt(ic);
                        }
                        u(12) = uInt(0);
                        u(13) = uInt(1);
                        u(14) = uInt(2);
                        u(15) = uInt(3);

                        fSpring.Zero(); kSpring.Zero();

                        getMatResponse(u,fSpring,kSpring);

                // performs internal equilibrium

                intEq(0) = -fSpring(2)-fSpring(3)+fSpring(9)-fSpring(12)/elemHeight;
                intEq(1) = fSpring(1)-fSpring(5)-fSpring(7)+fSpring(12)/elemWidth;
                intEq(2) = -fSpring(4)-fSpring(8)+fSpring(10)+fSpring(12)/elemHeight;
                intEq(3) = fSpring(0)-fSpring(6)-fSpring(11)-fSpring(12)/elemWidth;

                df_dDef.Zero();
                matDiag(kSpring, df_dDef);

                //////////////////////// dintEq_du = dg_df*df_dDef*dDef_du
                tempintEq_du.Zero(); dintEq_du.Zero();

                tempintEq_du.addMatrixProduct(0.0,dg_df,df_dDef,1.0);
                dintEq_du.addMatrixProduct(0.0,tempintEq_du,dDef_du,1.0);

                normIntEq = intEq.Norm();
                normIntEqdU = 0.0;
                for(int jc = 0; jc<4 ; jc++)
                {
                        normIntEqdU += intEq(jc)*duInt(jc);
                }
                normIntEqdU = fabs(normIntEqdU);

                if(totalCount == 1)
                {
                        tolIntEq = (tol>tol*normIntEq) ? tol:tol*normIntEq;
                        tolIntEqdU = tol;
                }
                else if(totalCount == 2)
                {
                        tolIntEqdU = (tol>tol*normIntEqdU) ? tol:tol*normIntEqdU;
                }
                ctolIntEq = (tolIntEq*dLoadStep > tol) ? tolIntEq*dLoadStep:tol;
                ctolIntEqdU = (tolIntEqdU*dLoadStep > tol) ? tolIntEqdU*dLoadStep:tol;


                // check for convergence starts

      if((normIntEq < ctolIntEq) || ((normIntEqdU < ctolIntEqdU) && (count >1)) || (normDuInt < toluInt) || (dLoadStep < 1e-3))
                {
                        converge = 1;
                        loadStep = loadStep + dLoadStep;
                        if(fabs(1.0 - loadStep) < tol)
                        {
                                loadStep = 1.0;
                        }
                }
                else
                {
                        ////////////// duInt = -dintEq_du/intEq
                        dintEq_du.Solve(intEq,duInt);
                        duInt *= -1;

                        normDuInt = duInt.Norm();
                        if(!linesearch)
                        {
                                uInt = uInt + duInt;
                        }
                        else
                        {
                                engrLast = 0.0;
                                engr = 0.0;

                                for(int jd = 0; jd<4 ; jd++)
                                {
                                        engrLast += duInt(jd)*intEqLast(jd);
                                        engr += duInt(jd)*intEq(jd);
                                }

                                if(fabs(engr) > tol*engrLast)
                                {
                                        duIntTemp = duInt;
                                        duIntTemp *= -1;
                                        // lineSearch algorithm requirement
                                        stepSize = getStepSize(engrLast,engr,uExt,duExt,uInt,duIntTemp,tol);

                                        if(fabs(stepSize) > 0.001)
                                        {
                                                uInt = uInt + stepSize*duInt;
                                        }
                                        else
                                        {
                                                uInt = uInt + duInt;
                                        }
                                }
                                else
                                {
                                        uInt = uInt + duInt;
                                }
                                intEqLast = intEq;
                        }
                }
        }

                if(!converge && loadStep < 1.0)
                {

                        incCount = 0;
                        maxCount = 25;
                        if(!linesearch)
                        {
                                linesearch = 1;
                                uInt = uIntOld;
                                duInt.Zero();
                        }
                        else
                        {
                                uInt = uIntOld;
                                duInt.Zero();
                                duExt = duExt*0.1;

                                dLoadStep = dLoadStep*0.1;
                        }
                }
                else if(loadStep < 1.0)
                {
                        maxCount = 10;
                        incCount ++;
                        normDuInt = toluInt;
                        if((incCount < maxCount) || dtConverge)
                        {
                                uExt = uExt + duExt;
                                if(loadStep + dLoadStep > 1.0)
                                {
                                        duExt = duExt*(1.0 - loadStep)/dLoadStep;
                                        dLoadStep = 1.0 - loadStep;
                                        incCount = 9;
                                }
                        }
                        else
                        {
                                incCount = 0;

                                uExt = uExt + duExt;
                                dLoadStep = dLoadStep*10;
                                if(loadStep + dLoadStep > 1.0)
                                {
                                        uExt = uExt + duExt*(1.0 - loadStep)/dLoadStep;
                                        dLoadStep = 1.0 - loadStep;
                                        incCount = 9;
                                }
                        }
                }
        }

        // determination of stiffness matrix and the residual force vector for the element

        formR(fSpring);

        formK(kSpring);

        dg.Zero();
        // committed external and internal displacement update
        for(int ig = 0; ig < 13; ig++ ) {
                if(ig<12)
                {
                        dg(ig) = Ut(ig);
                }
        }
        dg(12) = uInt(0);
        dg(13) = uInt(1);
        dg(14) = uInt(2);
        dg(15) = uInt(3);
}

void XC::BeamColumnJoint2d::getMatResponse(Vector U, Vector &fS, Vector &kS)
  {
        double jh = HgtFac;            // factor for beams
        double jw = WdtFac;            // factor for column

        // obtains the material response from the material class
        Vector defSpring(13);
        defSpring.Zero();
        fS.Zero();
        kS.Zero();

        defSpring.addMatrixVector(0.0, BCJoint, U, 1.0);

        // slip @ bar = slip @ spring * tc couple

        defSpring(0) = defSpring(0)*jw;                   // new model applied on
        defSpring(1) = defSpring(1)*jw;                   // 4th June 2004
        defSpring(6) = defSpring(6)*jw;                   // h, h_bar distinction was
        defSpring(7) = defSpring(7)*jw;                   // removed from the previous model

        defSpring(3)  = defSpring(3)*jh;                  // by making h = h_bar
        defSpring(4)  = defSpring(4)*jh;                  // similar case done for width
        defSpring(9) = defSpring(9)*jh;
        defSpring(10) = defSpring(10)*jh;

        for(int j=0; j<13; j++)
          {
            physicalProperties[j]->setTrialStrain(defSpring(j));
            kS(j) = physicalProperties[j]->getTangent();
            fS(j) = physicalProperties[j]->getStress();
          }

        // force @ spring = force @ bar * tc couple

        fS(0) = fS(0)*jw;
        fS(1) = fS(1)*jw;
        fS(6) = fS(6)*jw;
        fS(7) = fS(7)*jw;

        fS(3) = fS(3)*jh;
        fS(4) = fS(4)*jh;
        fS(9) = fS(9)*jh;
        fS(10) = fS(10)*jh;

        // stiffness @ spring = stiffness @ bar * (tc couple)^2

        kS(0) = kS(0)*jw*jw;
        kS(1) = kS(1)*jw*jw;
        kS(6) = kS(6)*jw*jw;
        kS(7) = kS(7)*jw*jw;

        kS(3) = kS(3)*jh*jh;
        kS(4) = kS(4)*jh*jh;
        kS(9) = kS(9)*jh*jh;
        kS(10) = kS(10)*jh*jh;

}

void XC::BeamColumnJoint2d::getdDef_du()
{
        dDef_du.Zero();

        for(int jb=0; jb<13; jb++)
        {
                dDef_du(jb,0) = BCJoint(jb,12);
                dDef_du(jb,1) = BCJoint(jb,13);
                dDef_du(jb,2) = BCJoint(jb,14);
                dDef_du(jb,3) = BCJoint(jb,15);
        }
}

void XC::BeamColumnJoint2d::matDiag(Vector k,Matrix &dfd)
{
        dfd.Zero();
        // takes in a vector and converts it to a diagonal matrix (could have been placed as a method in matrix class)
        for(int ja=0; ja<13; ja++)
        {
                dfd(ja,ja) = k(ja);
        }
}

void XC::BeamColumnJoint2d::formR(Vector f)
  {
        // develops the element residual force vector
        Vector rForceTemp(16);
        rForceTemp.Zero();

        rForceTemp.addMatrixTransposeVector(0.0,BCJoint,f,1.0);   // rForceTemp = BCJoint'*f
        R.Extract(rForceTemp,0,1.0);
  }

void XC::BeamColumnJoint2d::formK(Vector k)
  {
    // develops the element stiffness matrix
        Matrix kSprDiag(13,13);
        kSprDiag.Zero();
    Matrix kRForce(16,16);
        kRForce.Zero();
        Matrix kRFT1(4,12);
        kRFT1.Zero();
        Matrix kRFT2(4,4);
        kRFT2.Zero();
        Matrix kRFT3(12,4);
        kRFT3.Zero();
        Matrix I(4,4);
        I.Zero();
        Matrix kRSTinv(4,4);
        kRSTinv.Zero();
    Matrix kRF(12,12);
        kRF.Zero();
        Matrix K2Temp(12,4);
        K2Temp.Zero();
        Matrix K2(12,12);
        K2.Zero();

        matDiag(k,kSprDiag);

        kRForce.addMatrixTripleProduct(0.0,BCJoint,kSprDiag,1.0);    // kRForce = BCJoint'*kSprDiag*BCJoint
        kRFT2.Extract(kRForce,12,12,1.0);
        kRFT1.Extract(kRForce,12,0,1.0);
        kRFT3.Extract(kRForce,0,12,1.0);
        kRF.Extract(kRForce,0,0,1.0);

    for(int ic=0; ic<4; ic++)
        {
                I(ic,ic) = 1.0;
        }
        kRFT2.Solve(I,kRSTinv);

        K2Temp.addMatrixProduct(0.0,kRFT3,kRSTinv,1.0);

        // loop done for some idiotic reason
        for(int i = 0; i < 12; ++i)
        {
                for(int j = 0; j < 4; ++j)
                {
                        if(fabs(K2Temp(i,j)) < 1e-15)
                                K2Temp(i,j) = 0.;
                }
        }

        K2.addMatrixProduct(0.0,K2Temp,kRFT1,1.0);

        // loop done for some idiotic reason
        for(int i1 = 0; i1 < 12; ++i1)
        {
                for(int j1 = 0; j1 < 12; ++j1)
                {
                        if(fabs(K2(i1,j1)) < 1e-15)
                                K2(i1,j1) = 0.;
                }
        }

        kRF.addMatrix(1.0,K2,-1.0);

        K = kRF;    // K = kRF - kRFT3*kRSTinv*kRFT1
  }

void XC::BeamColumnJoint2d::getdg_df()
{
                dg_df.Zero();        // basically derivative of intEq
                dg_df(0,2) = -1;
                dg_df(0,3) = -1;
                dg_df(0,9) = 1;
                dg_df(0,12) = -1/elemHeight;
                dg_df(1,1) = 1;
                dg_df(1,5) = -1;
                dg_df(1,7) = -1;
                dg_df(1,12) = 1/elemWidth;
                dg_df(2,4) = -1;
                dg_df(2,8) = -1;
                dg_df(2,10) = 1;
                dg_df(2,12) = 1/elemHeight;
                dg_df(3,0) = 1;
                dg_df(3,6) = -1;
                dg_df(3,11) = -1;
                dg_df(3,12) = -1/elemWidth;

}

void XC::BeamColumnJoint2d::getBCJoint()
{
        BCJoint.Zero();                  // basically the transformation matrix for the element
        BCJoint(0,1) =  -1;
        BCJoint(0,2) =  elemWidth/2;
        BCJoint(0,15) = 1;
        BCJoint(1,1) =  -1;
        BCJoint(1,2) =  -elemWidth/2;
        BCJoint(1,13) = 1;
        BCJoint(2,0) =  1;
        BCJoint(2,12) = -1;
        BCJoint(3,3) =  1;
        BCJoint(3,5) =  elemHeight/2;
        BCJoint(3,12) = -1;
        BCJoint(4,3) = 1;
        BCJoint(4,5) = -elemHeight/2;
        BCJoint(4,14) = -1;
        BCJoint(5,4) = 1;
        BCJoint(5,13) = -1;
        BCJoint(6,7) =  1;
        BCJoint(6,8) =  -elemWidth/2;
        BCJoint(6,15) = -1;
        BCJoint(7,7) =  1;
        BCJoint(7,8) =  elemWidth/2;
        BCJoint(7,13) = -1;
        BCJoint(8,6) =  1;
        BCJoint(8,14) = -1;
        BCJoint(9,9) =  -1;
        BCJoint(9,11) =  -elemHeight/2;
        BCJoint(9,12) = 1;
        BCJoint(10,9) = -1;
        BCJoint(10,11) = elemHeight/2;
        BCJoint(10,14) = 1;
        BCJoint(11,10) = 1;
        BCJoint(11,15) = -1;
        BCJoint(12,12) = -1/elemHeight;
        BCJoint(12,13) = 1/elemWidth;
        BCJoint(12,14) = 1/elemHeight;
        BCJoint(12,15) = -1/elemWidth;

        // based upon the new formulation
        BCJoint(2,2) = -(elemActHeight - elemHeight)/2;
        BCJoint(5,5) = -(elemActWidth - elemWidth)/2;
        BCJoint(8,8) = (elemActHeight - elemHeight)/2;;
        BCJoint(11,11) = (elemActWidth - elemWidth)/2;;

}

double XC::BeamColumnJoint2d::getStepSize(double s0,double s1,Vector uExt,Vector duExt,Vector uInt,Vector duInt,double tol)
{
        Vector u(16);    u.Zero();
        Vector fSpr(13); fSpr.Zero();
        Vector kSpr(13); kSpr.Zero();
        Vector intEq(4); intEq.Zero();

        double r0 = 0.0;            // tolerance check for line-search
        double tolerance = 0.8;     // slack region tolerance set for line-search

        if(s0 != 0.0)
                r0 = fabs(s1/s0);

        if(r0 <= tolerance)
                return 1.0;   // Linsearch Not required residual decrease less than tolerance

        if(s1 == s0)
                return 1.0;   // Bisection would have divide by zero error if continued

        // set some variables
        double etaR;
        double eta = 1.0;
        double s = s1;
        double etaU = 1.0;
        double etaL = 0.0;
        double sU = s1;
        double sL = s0;
        double r = r0;
        double etaJ = 1.0;

        //double minEta = 0.1;
        double maxEta = 10.0;
        int maxIter = 20;

        // bracket the region
        int count = 0;
        while ((sU*sL > 0.0) && (etaU < maxEta)) {
                count ++;
                etaU *= 2.0;
                etaR = etaU - etaJ;

                for(int i = 0; i < 12; i++) {
                        u(i) = uExt(i) + duExt(i);
                }
                u(12) = uInt(0) - etaR*duInt(0);
                u(13) = uInt(1) - etaR*duInt(1);
                u(14) = uInt(2) - etaR*duInt(2);
                u(15) = uInt(3) - etaR*duInt(3);

                getMatResponse(u,fSpr,kSpr);

                intEq(0) = -fSpr(2) - fSpr(3) + fSpr(9) - fSpr(12)/elemHeight;
                intEq(1) = fSpr(1) - fSpr(5) - fSpr(7) + fSpr(12)/elemWidth;
                intEq(2) = -fSpr(4) - fSpr(8) + fSpr(10) + fSpr(12)/elemHeight;
                intEq(3) = fSpr(0) - fSpr(6) - fSpr(11) - fSpr(12)/elemWidth;

                sU = duInt^intEq;

                // check if we are happy with the solution
                r = fabs(sU/s0);
                if(r < tolerance)
                        return etaU;

                etaJ = etaU;
        }

        if(sU*sL > 0.0)   // no bracketing could be done
                return 1.0;

        count = 0;
        while (r > tolerance && count < maxIter) {
                count ++;
                eta = (etaU + etaL)/2.0;

                if(r > r0) eta = 1.0;

                etaR = eta - etaJ;

                for(int i = 0; i < 12; i++) {
                        u(i) = uExt(i) + duExt(i);
                }
                u(12) = uInt(0) - etaR*duInt(0);
                u(13) = uInt(1) - etaR*duInt(1);
                u(14) = uInt(2) - etaR*duInt(2);
                u(15) = uInt(3) - etaR*duInt(3);

                getMatResponse(u,fSpr,kSpr);

                intEq(0) = -fSpr(2) - fSpr(3) + fSpr(9) - fSpr(12)/elemHeight;
                intEq(1) = fSpr(1) - fSpr(5) - fSpr(7) + fSpr(12)/elemWidth;
                intEq(2) = -fSpr(4) - fSpr(8) + fSpr(10) + fSpr(12)/elemHeight;
                intEq(3) = fSpr(0) - fSpr(6) - fSpr(11) - fSpr(12)/elemWidth;

                s = duInt^intEq;

                // check if we are happy with the solution
                r = fabs(s/s0);

                // set variables for next iteration
                etaJ = eta;

                if(s*sU < 0.0) {
                        etaL = eta;
                        sL = s;
                } else if(s*sU == 0.0) {
                        count = maxIter;
                } else {
                        etaU = eta;
                        sU = s;
                }

                if(sL == sU)
                        count = maxIter;

        }

        return eta;
}

const XC::Matrix &XC::BeamColumnJoint2d::getDamp(void) const
  {
    //not applicable (stiffness being returned)
    return getTangentStiff();
  }

const XC::Matrix &XC::BeamColumnJoint2d::getMass(void) const
  {
    //not applicable  (stiffness being returned)
    return getTangentStiff();
  }

int XC::BeamColumnJoint2d::addLoad(ElementalLoad *theLoad, double loadFactor)
  {
    return 0;
  }

int XC::BeamColumnJoint2d::addInertiaLoadToUnbalance(const XC::Vector &accel)
  {
    return 0;
  }


const XC::Vector &XC::BeamColumnJoint2d::getResistingForceIncInertia(void) const
  {
    //not applicable (residual being returned)
    return getResistingForce();
  }

int XC::BeamColumnJoint2d::sendSelf(Communicator &comm)
  {
    // yet to do.
    return -1;
  }

int XC::BeamColumnJoint2d::recvSelf(const Communicator &comm)
  {
    // yet to do.
    return -1;
  }

void XC::BeamColumnJoint2d::Print(std::ostream &s, int flag) const
  {
        s << "Element: " << this->getTag() << " Type: Beam Column Joint " << std::endl;
        for(int i = 0; i<4; i++)
        {
                s << "Node :" << theNodes.getTagNode(i);
                s << "DOF :" << theNodes[i]->getNumberDOF();
        }
        return;
  }

XC::Response* XC::BeamColumnJoint2d::setResponse(const std::vector<std::string> &argv, Information &eleInfo)
  {
        // we will compare argv[0] to determine the type of response required

        if(argv[0] == "node1BarSlipL" || argv[0] == "node1BarslipL" || argv[0] == "Node1BarSlipL")
                return setMaterialResponse(physicalProperties[0],argv,1,eleInfo);

        else if(argv[0] == "node1BarSlipR" || argv[0] == "node1BarslipR" || argv[0] == "Node1BarSlipR")
                return setMaterialResponse(physicalProperties[1],argv,1,eleInfo);

        else if(argv[0] == "node1InterfaceShear" || argv[0] == "node1Interfaceshear" || argv[0] == "Node1InterfaceShear" )
                return setMaterialResponse(physicalProperties[2],argv,1,eleInfo);

        else if(argv[0] == "node2BarSlipB" || argv[0] == "node2BarslipB" || argv[0] == "Node2BarSlipB")
                return setMaterialResponse(physicalProperties[3],argv,1,eleInfo);

        else if(argv[0] == "node2BarSlipT" || argv[0] == "node2BarslipT" || argv[0] == "Node2BarSlipT")
                return setMaterialResponse(physicalProperties[4],argv,1,eleInfo);

        else if(argv[0] == "node2InterfaceShear" || argv[0] == "node2Interfaceshear" || argv[0] == "Node2InterfaceShear" )
                return setMaterialResponse(physicalProperties[5],argv,1,eleInfo);

        else if(argv[0] == "node3BarSlipL" || argv[0] == "node3BarslipL" || argv[0] == "Node3BarSlipL")
                return setMaterialResponse(physicalProperties[6],argv,1,eleInfo);

        else if(argv[0] == "node3BarSlipR" || argv[0] == "node3BarslipR" || argv[0] == "Node3BarSlipR")
                return setMaterialResponse(physicalProperties[7],argv,1,eleInfo);

        else if(argv[0] == "node3InterfaceShear" || argv[0] == "node3Interfaceshear" || argv[0] == "Node3InterfaceShear" )
                return setMaterialResponse(physicalProperties[8],argv,1,eleInfo);

        else if(argv[0] == "node4BarSlipB" || argv[0] == "node4BarslipB" || argv[0] == "Node4BarSlipB")
                return setMaterialResponse(physicalProperties[9],argv,1,eleInfo);

        else if(argv[0] == "node4BarSlipT" || argv[0] == "node4BarslipT" || argv[0] == "Node4BarSlipT")
                return setMaterialResponse(physicalProperties[10],argv,1,eleInfo);

        else if(argv[0] == "node4InterfaceShear" || argv[0] == "node4Interfaceshear" || argv[0] == "Node4InterfaceShear" )
                return setMaterialResponse(physicalProperties[11],argv,1,eleInfo);

        else if(argv[0] == "shearpanel" || argv[0] == "shearPanel")
                return setMaterialResponse(physicalProperties[12],argv,1,eleInfo);


        else if(argv[0] == "externalDisplacement" || argv[0] == "externaldisplacement")
                return new ElementResponse(this,1,Vector(12));

        else if(argv[0] == "internalDisplacement" || argv[0] == "internaldisplacement")
                return new ElementResponse(this,2,Vector(4));

        else if(argv[0] == "deformation" || argv[0] == "Deformation")
                return new ElementResponse(this,3,Vector(4));

        else
                return 0;
}

int
XC::BeamColumnJoint2d::getResponse(int responseID, Information &eleInfo)
{
        static XC::Vector delta(13);
        static XC::Vector def(4);
        static XC::Vector U(16);
        int tr,ty;
        double bsFa, bsFb, bsFc, bsFd;
        double bsFac, bsFbd, isFac, isFbd;

        switch (responseID) {
        case 1:
                if(eleInfo.theVector!=0)
                {
                        (*(eleInfo.theVector))(0) = UeprCommit(0);
                        (*(eleInfo.theVector))(1) = UeprCommit(1);
                        (*(eleInfo.theVector))(2) = UeprCommit(2);
                        (*(eleInfo.theVector))(3) = UeprCommit(3);
                        (*(eleInfo.theVector))(4) = UeprCommit(4);
                        (*(eleInfo.theVector))(5) = UeprCommit(5);
                        (*(eleInfo.theVector))(6) = UeprCommit(6);
                        (*(eleInfo.theVector))(7) = UeprCommit(7);
                        (*(eleInfo.theVector))(8) = UeprCommit(8);
                        (*(eleInfo.theVector))(9) = UeprCommit(9);
                        (*(eleInfo.theVector))(10) = UeprCommit(10);
                        (*(eleInfo.theVector))(11) = UeprCommit(11);
                }
                return 0;

        case 2:
                if(eleInfo.theVector !=0) {
                        (*(eleInfo.theVector))(0) = UeprIntCommit(0);
                        (*(eleInfo.theVector))(1) = UeprIntCommit(1);
                        (*(eleInfo.theVector))(2) = UeprIntCommit(2);
                        (*(eleInfo.theVector))(3) = UeprIntCommit(3);
                }
                return 0;

        case 3:

                for(ty =0; ty <12; ty++)
                {
                        U(ty) = UeprCommit(ty);
                }
                for(tr = 0; tr <4 ; tr++)
                {
                        U(12+tr) = UeprIntCommit(tr);
                }

                delta.addMatrixVector(0.0,BCJoint,U,1.0);

                bsFa = fabs(delta(0) - delta(1))/elemWidth;
                bsFc = fabs(delta(7) - delta(6))/elemWidth;
                bsFac = bsFa + bsFc;
                bsFb = fabs(delta(4) - delta(3))/elemHeight;
                bsFd = fabs(delta(10) - delta(9))/elemHeight;
                bsFbd = bsFb + bsFd;

                def(0) = bsFac + bsFbd; // contribution of bar slip deformation


                isFac = (delta(2) + delta(8))/elemHeight;
                isFbd = (delta(5) + delta(11))/elemWidth;

                def(1) = isFac + isFbd;  // contribution due to interface shear spring

                def(2) = delta(12);  // contribution due to shear panel

                def(3) = def(0) + def(1) + def(2);  // total joint deformation


                return eleInfo.setVector(def);
        default:
                return -1;
        }
}

int XC::BeamColumnJoint2d::setParameter(const std::vector<std::string> &argv, Parameter &param)
  { return -1; }

int XC::BeamColumnJoint2d::updateParameter(int parameterID, Information &info)
  { return -1; }
