#ifndef __TKALTRACK__
#define __TKALTRACK__
//*************************************************************************
//* =================
//*  TKalTrack Class
//* =================
//*
//* (Description)
//*   Track class for Kalman filter
//* (Requires)
//*     TVKalSystem
//* (Provides)
//*     class TKalTrack
//* (Update Recored)
//*   2003/09/30  Y.Nakashima	Original version.
//*
//*************************************************************************
                                                                                
#include "TVKalSystem.h"
#include "TKalTrackState.h"

//_____________________________________________________________________
//  ------------------------------
//  Sample Kalman Filter class
//  ------------------------------
//
                                                                                
class TKalTrack : public TVKalSystem {
public:
   TKalTrack(Int_t n = 1);
   ~TKalTrack() {} 

   Double_t FitToHelix(TKalTrackState &a, TKalMatrix &C, Int_t &ndf);

private:

   ClassDef(TKalTrack,1)  // Base class for Kalman Filter
};
                                                                                
//=======================================================
// inline functions
//=======================================================
                                                                                
#endif
