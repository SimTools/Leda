#ifndef __TPLANE__
#define __TPLANE__
//*************************************************************************
//* ====================
//*  TPlane Class
//* ====================
//*
//* (Description)
//*   A class to implement a flat plane object.
//* (Requires)
//*     TVSurface
//* (Provides)
//*     class TPlane
//* (Update Recored)
//*   2004/10/30  A.Yamaguchi       Original version.
//*
//*************************************************************************
//
#include "TVSurface.h"
#include "TVector3.h"
#include "TMatrixD.h"

class TVTrack;

//_____________________________________________________________________
//  -----------------------------------
//  Plane Class
//  -----------------------------------

class TPlane : public TVSurface {
public:
   TPlane(const TVector3 &n = TVector3(0.));

   virtual ~TPlane() {}

   virtual Double_t CalcS   (const TVector3 &xx) const;
   virtual TMatrixD CalcDSDx(const TVector3 &xx) const;

   inline virtual       Bool_t     IsOnSurface(const TVector3 &xx) const;
   inline virtual       Bool_t     IsOutside  (const TVector3 &xx) const;


private:
   TVector3 fXc;          // center
   TVector3 fNormal;      // normal
 
   ClassDef(TPlane,1)      // plane class
};
//=======================================================
// inline functions
//=======================================================


Bool_t TPlane::IsOnSurface(const TVector3 &xx) const
{
   return (xx - fXc) * fNormal == 0. ? kTRUE : kFALSE; 
} 

Bool_t TPlane::IsOutside(const TVector3 &xx) const
{
   return (xx - fXc) * fNormal > 0. ? kTRUE : kFALSE; 
} 
#endif

