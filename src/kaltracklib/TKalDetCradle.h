#ifndef __TKALDETCRADLE__
#define __TKALDETCRADLE__
//*************************************************************************
//* =====================
//*  TKalDetCradle Class
//* =====================
//*
//* (Description)
//*   A sigleton to hold information of detector system
//*   used in Kalman filter classes.
//* (Requires)
//* 	TObjArray
//* 	TVKalDetector
//* (Provides)
//* 	class TKalDetCradle
//* (Update Recored)
//*   2005/02/23  A.Yamaguchi	Original Version.
//*
//*************************************************************************

#include "TObjArray.h"
#include "TVKalDetector.h"
#include <utility>
#include <vector>

class TKalTrackSite;

//_____________________________________________________________________
//  ------------------------------
//  Detector system class
//  ------------------------------
//

class TKalDetCradle : public TObjArray, public TAttElement {
public:
   TKalDetCradle(Int_t n = 1);
   virtual ~TKalDetCradle();

   // Utility methods
   virtual void Install(TVKalDetector &det);

   inline virtual void   SwitchOnMS ()       { fIsMSON = kTRUE;  }
   inline virtual void   SwitchOffMS()       { fIsMSON = kFALSE; }
   inline virtual Bool_t IsMSOn     () const { return fIsMSON;   }

   // RadLPair holds (1/X0_k, dphi_k) where X0_k is the radiation length
   // and dphi_k is the deflection angle in k-th region between from and
   // to sites.
   // GetX0Table(from, to) returns an array/vector of RadPairs between
   // from and to sites. This information on the material distribution
   // is used in TKalTrackState::CalcProcessNoice(to,mass).
   typedef std::pair<Double_t, Double_t> RadLPair;
   const std::vector<RadLPair> &GetX0Table(const TKalTrackSite &from,
                                           const TKalTrackSite &to);

private:
   void Update();

private:
   std::vector<RadLPair> fRadLPairs; //! material info carrier
   Bool_t                fIsMSON;    //! switch for multiple scattering
   Bool_t                fDone;      //! flag to tell if sorting done

   ClassDef(TKalDetCradle,1)  // Base class for detector system
};

//=======================================================
// inline functions, if any
//=======================================================

#endif
