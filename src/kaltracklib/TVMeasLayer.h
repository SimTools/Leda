#ifndef __TVMEASLAYER__
#define __TVMEASLAYER__
//*************************************************************************
//* ====================
//*  TVMeasLayer Class
//* ====================
//*
//* (Description)
//*   Measurement layer interface class.
//* (Requires)
//* (Provides)
//*     class TVMeasLayer
//* (Update Recored)
//*   2003/09/30  Y.Nakashima       Original version.
//*   2005/02/23  A.Yamaguchi       Added new data members, fFwdX0Inv,
//*                                 fBwdX0Inv and fIndex, and their
//*                                 corresponding getters and setters.
//*                                 Added a new method, GetX0Inv().
//*
//*************************************************************************
//
#include "TVector3.h"

#include "TAttElement.h"
#include "TKalMatrix.h"
#include "KalTrackDim.h"

class TVTrackHit;

class TVMeasLayer : public TAttElement {
public:
   // Ctors and Dtor

   TVMeasLayer(Double_t x0invIn = 0., Double_t x0invOut = 0.);
   virtual ~TVMeasLayer() {}

   // Utiliy Methods

   virtual TKalMatrix XvToMv   (const TVTrackHit &ht,
                                const TVector3   &xv) const = 0;
   virtual TVector3   HitToXv  (const TVTrackHit &ht) const = 0;
   virtual void       CalcDhDa (const TVTrackHit &ht,
                                const TVector3   &xv,
                                const TKalMatrix &dxphiada,
                                      TKalMatrix &H)  const = 0;

   inline virtual Double_t GetX0Inv(Bool_t isfwd) const { return isfwd ? fX0InvOut : fX0InvIn; }

   inline         Int_t    GetIndex()             const { return fIndex; }
   inline         void     SetIndex(Int_t i)            { fIndex = i;    } 

private:
   Double_t fX0InvIn;      // 1/X0 for inner region
   Double_t fX0InvOut;      // 1/X0 for outer reagion
   Int_t    fIndex;         // index in TKalDetCradle

   ClassDef(TVMeasLayer,1) 	// Measurement layer interface class
};

#endif
