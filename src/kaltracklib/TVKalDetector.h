#ifndef __TVKALDETECTOR__
#define __TVKALDETECTOR__
//*************************************************************************
//* =====================
//*  TVKalDetector Class
//* =====================
//*
//* (Description)
//*   Base class to hold information of detector system
//*   used in Kalman filter classes.
//* (Requires)
//* 	TObjArray
//* (Provides)
//* 	class TVKalDetector
//* (Update Recored)
//*   2003/09/30  K.Fujii	Original version.
//*   2005/02/23  A.Yamaguchi	Moved most methods to TKalDetCradle.
//*
//*************************************************************************

#include "TObjArray.h"
#include "TVMeasLayer.h"

//_____________________________________________________________________
//  ------------------------------
//  Detector system class
//  ------------------------------
//

class TVKalDetector : public TObjArray {
public:
   // Ctors and Dtor
   TVKalDetector(Int_t n = 1) : TObjArray(n) {}
   virtual ~TVKalDetector() {}

   
   ClassDef(TVKalDetector,1)  // Base class for detector system
};

#endif
