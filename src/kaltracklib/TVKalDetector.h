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
//* 	TAttElement
//* 	TVKalSite
//* (Provides)
//* 	class TVKalDetector
//* (Update Recored)
//*   2003/09/30  K.Fujii	Original version.
//*
//*************************************************************************

#include "TObjArray.h"
#include "TVKalSite.h"

#include "TVMeasLayer.h"

//_____________________________________________________________________
//  ------------------------------
//  Detector system class
//  ------------------------------
//

class TVKalDetector : public TObjArray {
public:

   // Ctors and Dtor

   TVKalDetector(Int_t n = 1) : TObjArray(n), fIsMSON(kTRUE) {}
   virtual ~TVKalDetector() {}

   // Utility methods

   inline virtual void Add(TObject *obj);

   inline virtual void   SwitchOnMS ()       { fIsMSON = kTRUE;  }
   inline virtual void   SwitchOffMS()       { fIsMSON = kFALSE; }
   inline virtual Bool_t IsMSOn     () const { return fIsMSON;   }

   virtual Double_t CalcRadLength(const TVMeasLayer &from,
                                  const TVMeasLayer &to) const = 0;
   virtual Double_t CalcSigmaMS0 (const TVMeasLayer &from,
                                  const TVMeasLayer &to,
                                        Double_t     pathlen) const = 0;


private:
   Bool_t   fIsMSON;   // switch for multiple scattering
   
   ClassDef(TVKalDetector,1)  // Base class for detector system
};

//=======================================================
// inline functions
//=======================================================

void TVKalDetector::Add(TObject *obj)
{
   TObjArray::Add(obj);
   TVMeasLayer &ml   = *dynamic_cast<TVMeasLayer *>(obj);
   ml.SetParentPtr(this);
}

#endif
