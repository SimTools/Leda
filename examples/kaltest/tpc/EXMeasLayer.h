#ifndef __EXMEASLAYER__
#define __EXMEASLAYER__
//*************************************************************************
//* ===================
//*  EXMeasLayer Class
//* ===================
//*
//* (Description)
//*   Sample measurement layer class used by EXHit.
//* (Requires)
//* (Provides)
//*     class EXMeasLayer
//* (Update Recored)
//*   2003/09/30  Y.Nakashima       Original version.
//*
//*************************************************************************
//
#include <iostream>
#include "TVector3.h"

#include "TKalMatrix.h"
#include "TCylinder.h"
#include "TVMeasLayer.h"
#include "KalTrackDim.h"

using namespace std;

class TVTrackHit;

class EXMeasLayer : public TVMeasLayer, public TCylinder {
public:

   // Ctors and Dtor

   EXMeasLayer(Double_t r0, Double_t lhalf)
              : TCylinder(r0,lhalf)
   {
   }
   virtual ~EXMeasLayer() {}

   // Parrent's pure virtuals that must be implemented

   virtual TKalMatrix XvToMv    (const TVTrackHit &ht,
                                 const TVector3   &xv)     const;
   virtual TKalMatrix XvToMv    (const TVector3   &xv,
                                       Int_t       side)   const;
   virtual TVector3   HitToXv   (const TVTrackHit &ht)     const;
   virtual void       CalcDhDa  (const TVTrackHit &ht,
                                 const TVector3   &xv,
                                 const TKalMatrix &dxphiada,
                                       TKalMatrix &H)      const;

private:
   ClassDef(EXMeasLayer,1) 	// Sample measurement layer class
};

#endif
