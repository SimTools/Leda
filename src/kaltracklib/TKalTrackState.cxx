//*************************************************************************
//* ======================
//*  TKalTrackState Class
//* ======================
//*
//* (Description)
//*   Track state vector class used in Kalman Filter.
//* (Requires)
//*     TVKalState
//* (Provides)
//*     class TKalTrackState
//* (Update Recored)
//*   2003/09/30  Y.Nakashima       Original version.
//*
//*************************************************************************
//
#include <iostream>

#include "TVKalDetector.h"
#include "TKalTrackState.h"
#include "TKalTrackSite.h"

using namespace std;

//_____________________________________________________________________
//  -----------------------------------
//  Base Class for Kalman state vector
//  -----------------------------------
//
// --------------------------------------------------------------------
// Ctors and Dtor
//
TKalTrackState::TKalTrackState(Int_t p) 
           : TVKalState(p), fX0()
{
}

TKalTrackState::TKalTrackState(const TKalMatrix &sv, Int_t type, Int_t p) 
           : TVKalState(sv,type,p), fX0()
{
}

TKalTrackState::TKalTrackState(const TKalMatrix &sv, const TKalMatrix &c,
                                     Int_t type, Int_t p) 
           : TVKalState(sv,c,type,p), fX0()
{
}

TKalTrackState::TKalTrackState(const TKalMatrix &sv, const TVKalSite &site,
                                     Int_t type, Int_t p) 
           : TVKalState(sv,site,type,p), 
             fX0(((TKalTrackSite *)&site)->GetPivot())
{
}

TKalTrackState::TKalTrackState(const TKalMatrix &sv, const TKalMatrix &c,
                               const TVKalSite &site, Int_t type, Int_t p) 
           : TVKalState(sv,c,site,type,p),
             fX0(((TKalTrackSite *)&site)->GetPivot())
{
}

//
// --------------------------------------------------------------------
// Implementation of base-class pure virtuals
//

TKalTrackState * TKalTrackState::MoveTo(const TVKalSite  &to,
                                              TKalMatrix &F,
                                              TKalMatrix *QPtr) const
{
   TKalTrackSite &siteto = *(TKalTrackSite *)&to;
   TVector3       x0to   = siteto.GetPivot();

   TVTrack       *vtrkPtr = 0;
   THelicalTrack  hl      = GetHelix();
   TStraightTrack sl      = GetLine ();

   if ((*dynamic_cast<const TKalTrackSite *> (&GetSite())).IsInB()) {
      vtrkPtr = &hl;
   } else {
      vtrkPtr = &sl;
   }

   TVTrack       &helto   = *vtrkPtr;

   TKalMatrix  av(5,1), Fto(5,5);
   Double_t    fid;
   helto.MoveTo(x0to,fid,&Fto);
   helto.PutInto(av);

   Int_t sdim = GetDimension();
   TKalMatrix sv(sdim,1);
   for (Int_t i=0; i<5; i++) {
      sv(i,0) = av(i,0);
      for (Int_t j=0; j<5; j++) {
         F(i,j) = Fto(i,j);
      }
   }
   if (sdim == 6) {
      sv(5,0) = (*this)(5,0);
      F (5,5) = 1.;
   }

   if (QPtr) {
      *QPtr = CalcProcessNoise(to,fid); //  fid = | phi_to - phi_from |
                                        //  assuming that pivots are hits
      return (new TKalTrackState(sv,to,TVKalSite::kPredicted));
   } else {
      return 0;
   }
}

TKalTrackState & TKalTrackState::MoveTo(const TVKalSite  &to,
                                              TKalMatrix &F,
                                              TKalMatrix &Q) const
{
   return *MoveTo(to, F, &Q);
}

TKalMatrix TKalTrackState::CalcProcessNoise(const TVKalSite &to, Double_t dfi) const
{
   static const Double_t kMpi  = 0.13957018;
   static const Double_t kMpi2 = kMpi*kMpi;

   const TKalTrackSite &from   = *(TKalTrackSite *)&GetSite();
   const TVMeasLayer   &mlfrom = from.GetHit().GetMeasLayer();
   const TVKalDetector &det    = *(TVKalDetector *)&mlfrom.GetParent();
   const TVMeasLayer   &mlto   = (*(TKalTrackSite *)&to).GetHit().GetMeasLayer();

   Double_t tnl    = (*this)(4,0);
   Double_t cpa    = (*this)(2,0);
   Double_t tnl2   = tnl*tnl; 
   Double_t tnl21  = 1 + tnl2;
   Double_t cslinv = TMath::Sqrt(1 + tnl2);

   Double_t mom    = TMath::Abs(1/cpa) * cslinv;
   Double_t beta   = mom/TMath::Sqrt(mom*mom + kMpi2); // pion assumed

   Double_t path;
   if ((*dynamic_cast<const TKalTrackSite *> (&GetSite())).IsInB()) {
      path = TMath::Abs(GetHelix().GetRho()*dfi)*cslinv;
   } else {
      path = TMath::Abs(dfi)*cslinv;
   }

   Double_t sgms   = det.CalcSigmaMS0(mlfrom,mlto,path)/(mom*beta);
   Double_t sgms2  = sgms*sgms;
   
   Double_t cpatnl = cpa*tnl;

   Int_t p = GetDimension();
   TKalMatrix Q(p,p);
   Q(1,1) = sgms2 * tnl21;
   Q(2,2) = sgms2 * cpatnl * cpatnl;
   Q(2,4) = sgms2 * cpatnl * tnl21;
   Q(4,2) = sgms2 * cpatnl * tnl21;
   Q(4,4) = sgms2 * tnl21  * tnl21;
   return Q;
}

void TKalTrackState::DebugPrint() const
{
   cerr << "          +-     -+   " << "+-" <<  endl
        << "          | drho  |   " << "| " << (*this)(0,0) << endl
        << "          | phi0  |   " << "| " << (*this)(1,0) << endl
        << " a      = | kappa | = " << "| " << (*this)(2,0) << endl
        << "          | dz    |   " << "| " << (*this)(3,0) << endl
        << "          | tanl  |   " << "| " << (*this)(4,0) << endl;
   if (GetDimension() == 6) {
      cerr 
        << "          | t0    |   " << "| " << (*this)(5,0) << endl;
   }
   cerr << "          +-     -+   " << "+-" << endl;
   cerr << "          +-" << endl 
        << " X0     = | " << fX0.X() << endl
        << "          | " << fX0.Y() << endl
        << "          | " << fX0.Z() << endl
        << "          +-" << endl;
   GetCovMat().DebugPrint(" covMat = ", 6);
}

//
// --------------------------------------------------------------------
// Derived class methods
//

THelicalTrack TKalTrackState::GetHelix() const
{
   TKalMatrix a(5,1);
   for (Int_t i=0; i<5; i++) a(i,0) = (*this)(i,0);
   return THelicalTrack(a,fX0,((TKalTrackSite *)&GetSite())->GetBfield());
}

TStraightTrack TKalTrackState::GetLine() const
{
   TKalMatrix a(5,1);
   for (Int_t i=0; i<5; i++) a(i,0) = (*this)(i,0);
   return TStraightTrack(a,fX0,((TKalTrackSite *)&GetSite())->GetBfield());
}
