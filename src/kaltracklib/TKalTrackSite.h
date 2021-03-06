#ifndef __TKALTRACKSITE__
#define __TKALTRACKSITE__
//*************************************************************************
//* =====================
//*  TKalTrackSite Class
//* =====================
//*
//* (Description)
//*   Track measurement site class used by Kalman filter.
//* (Requires)
//*     TKalTrackState
//* (Provides)
//*     class TKalTrackSite
//* (Update Recored)
//*   2003/09/30  Y.Nakashima       Original version.
//*   2004/09/17  K.Fujii           Added ownership flag.
//*
//*************************************************************************

#include "TVector3.h"    // from ROOT
#include "TVKalSite.h"   // from KalLib
#include "TVTrackHit.h"  // from KalTrackLib

class TVKalState;
class TKalTrackState;

//_________________________________________________________________________
//  ---------------------------------
//  Class for Kalman measurement site
//  ---------------------------------
//
class TKalTrackSite : public TVKalSite {
public:
   TKalTrackSite(Int_t m = kMdim, Int_t p = kSdim);
   TKalTrackSite( const TVTrackHit &ht, 
                        Int_t       m = kMdim, 
                        Int_t       p = kSdim);
   ~TKalTrackSite();

   Int_t        CalcExpectedMeasVec  (const TVKalState &a, TKalMatrix &h);
   Int_t        CalcMeasVecDerivative(const TVKalState &a, TKalMatrix &H);
   Bool_t       IsAccepted();

   void         DebugPrint() const;

   inline const TVTrackHit & GetHit    () const { return *fHitPtr;             }
   inline const TVector3   & GetPivot  () const { return fX0;                  }
   inline       Double_t     GetBfield () const { return fHitPtr->GetBfield(); }
   inline       Bool_t       IsInB     () const { return GetBfield() != 0.;    }
   inline       Bool_t       IsHitOwner() const { return fIsHitOwner;          }

   inline       void   SetHitOwner(Bool_t b=kTRUE) { fIsHitOwner = b; }

private:
   TVKalState & CreateState(const TKalMatrix &sv, Int_t type = 0);
   TVKalState & CreateState(const TKalMatrix &sv,
                            const TKalMatrix &C,
                                  Int_t       type = 0);
   Int_t        CalcXexp   (const TVKalState &a, 
                                  TVector3   &xx,
                                  Double_t   &phi) const;

private:
   TVTrackHit  *fHitPtr;       // pointer to corresponding hit
   TVector3     fX0;           // pivot
   Bool_t       fIsHitOwner;   // true if site owns hit

   ClassDef(TKalTrackSite,1)  // sample measurement site class
};
                                                                                
#endif
