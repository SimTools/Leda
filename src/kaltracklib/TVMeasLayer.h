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
//*
//*************************************************************************
//
#include "TVector3.h"

#include "TAttElement.h"
#include "TKalMatrix.h"
#include "THype.h"
#include "KalTrackDim.h"

class TVTrackHit;

class TVMeasLayer : public TAttElement {
public:

   // Ctors and Dtor

   TVMeasLayer() {}
   virtual ~TVMeasLayer() {}

   // Utiliy Methods

   virtual TKalMatrix XvToMv   (const TVTrackHit &ht,
                                const TVector3   &xv) const = 0;
   virtual TVector3   HitToXv  (const TVTrackHit &ht) const = 0;
   virtual void       CalcDhDa (const TVTrackHit &ht,
                                const TVector3   &xv,
                                const TKalMatrix &dxphiada,
                                      TKalMatrix &H)  const = 0;


private:

   ClassDef(TVMeasLayer,1) 	// Measurement layer interface class
};

#endif
