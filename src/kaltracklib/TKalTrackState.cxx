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
//*   2005/02/23  Y.Yamaguchi       Improved CalcProcessNoise().
//*
//*************************************************************************
//
#include <iostream>
#include <memory>

#include "TKalTrackState.h"
#include "TKalTrackSite.h"
#include "TKalTrack.h"

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

   std::auto_ptr<TVTrack> helto(&CreateTrack());

   TKalMatrix  av(5,1), Fto(5,5);
   Double_t    fid;
   helto->MoveTo(x0to,fid,&Fto);
   helto->PutInto(av);

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
      TKalTrackState *atop
                   = new TKalTrackState(sv, siteto, TVKalSite::kPredicted);
       //  fid = | phi_to - phi_from |
       //  assuming that pivots are hits
      *QPtr = CalcProcessNoise(siteto, *atop, *helto, fid);
      return atop;
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

TKalMatrix TKalTrackState::CalcProcessNoise(const TKalTrackSite  &to,
                                            const TKalTrackState &ato,
                                            const TVTrack        &tto,
                                                  Double_t        dfi) const
{
   const TKalTrackSite &from = static_cast<const TKalTrackSite &>(GetSite());
   TKalDetCradle       &det  = const_cast<TKalDetCradle &>
                              (static_cast<const TKalDetCradle &>
                              (from.GetHit().GetMeasLayer().GetParent()));
   Int_t p = GetDimension();
   TKalMatrix Q(p,p);
   if (!det.IsMSOn()) return Q;

   const vector<TKalDetCradle::RadLPair> &radpairs
              = det.GetX0Table(from, static_cast<const TKalTrackSite &>(to));

   Bool_t isInB = from.IsInB();
   Int_t  nel   = radpairs.size();
   if (!nel) return Q;

   Double_t delfi = dfi;
   //Double_t dr    = (*this)(0, 0);
   Double_t dr    = 0.;
   Double_t drp   = ato(0, 0);

   for (Int_t i = 0; i < nel; i++) {
#if 1
      TKalMatrix D(p, p);
      tto.CalcDapDa(delfi, dr, drp, D);
      if (p == 6) D(5, 5) = 1.;
#else
      TKalMatrix D  = TKalMatrix(TMatrixD::kUnit, Q);
#endif
      TKalMatrix Dt = TKalMatrix(TMatrixD::kTransposed, D);
      TKalMatrix Qi = CalcQ(radpairs[i], isInB);
      Q += D * Qi * Dt;
      delfi -= radpairs[i].second;
      //dr = drp;
   }
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

TKalMatrix TKalTrackState::CalcQ(const TKalDetCradle::RadLPair &radpair,
                                       Bool_t                   isInB) const
{
   Double_t cpa    = (*this)(2, 0);
   Double_t tnl    = (*this)(4, 0); 
   Double_t tnl2   = tnl * tnl;
   Double_t tnl21  = 1. + tnl2;
   Double_t cpatnl = cpa * tnl;
   Double_t cslinv = TMath::Sqrt(1. + tnl2);
   Double_t mom    = TMath::Abs(1. / cpa) * cslinv;

   static const Double_t kMpi = 0.13957018; // pion mass [GeV]
   TKalTrack *ktp  = static_cast<TKalTrack *>(TVKalSite::GetKalSystemPtr());
   Double_t   mass = ktp ? ktp->GetMass() : kMpi;
   Double_t   beta = mom / TMath::Sqrt(mom * mom + mass * mass);

   // *Calculate sigma_ms0 ---------------------------------------------
   Double_t x0inv = radpair.first;    // radiation length inverse
   Double_t df    = radpair.second;   // delta phi
                                                                                
   static const Double_t kMS1  = 0.0136;
   static const Double_t kMS12 = kMS1 * kMS1;
   static const Double_t kMS2  = 0.038;
   Double_t path = isInB
                 ? TMath::Abs(GetHelix().GetRho()*df)*cslinv
                 : TMath::Abs(df)*cslinv;
   Double_t xl   = path * x0inv;
   // ------------------------------------------------------------------
   // Very Crude Treatment!!
   Double_t tmp = 1. + kMS2 * TMath::Log(TMath::Max(1.e-4, xl));
   tmp /= (mom * beta);
   Double_t sgms2 = kMS12 * xl * tmp * tmp;
   // ------------------------------------------------------------------

   Int_t p = GetDimension();
   TKalMatrix q(p,p);
   q(1,1) = sgms2 * tnl21;
   q(2,2) = sgms2 * cpatnl * cpatnl;
   q(2,4) = sgms2 * cpatnl * tnl21;
   q(4,2) = sgms2 * cpatnl * tnl21;
   q(4,4) = sgms2 * tnl21  * tnl21;
   return q;
}

TVTrack &TKalTrackState::CreateTrack() const
{
   TVTrack *tkp = 0;

   TKalMatrix a(5,1);
   for (Int_t i=0; i<5; i++) a(i,0) = (*this)(i,0);
   Double_t bfield = static_cast<const TKalTrackSite &>(GetSite()).GetBfield();

   if (bfield == 0.) tkp = new TStraightTrack(a,fX0);
   else              tkp = new THelicalTrack(a,fX0, bfield);

   return *tkp;
}
