#ifndef __TDYNARRAY2__
#define __TDYNARRAY2__
//*************************************************************************
//* ==================
//*  TDynArray2 Class
//* ==================
//*
//* (Description)
//*    TDynArray2 class supplies a functionality of a 2-dim array
//*    which has a variable size in each dimension.
//* (Requires)
//* 	TObjArray:
//* (Provides)
//* 	class TDynArray2
//* (Update Recored)
//*    2000/05/17  K.Fujii	Original very primitive version.
//*
//*************************************************************************
//
#include "TDim.h"
//_____________________________________________________________________
//  -------------------------
//  2-dim Dynamic Array Class
//  -------------------------
//
class TDynArray2 : public TDim {
public:
   TDynArray2() {}
   virtual ~TDynArray2() { Delete(); }

   virtual TObject *  operator ()(Int_t i, Int_t j) const;
   // Very strange:
   // 	Rcint does not allow operator() on the LFS.
   //
   virtual TObject *& Set(Int_t i, Int_t j);

   ClassDef(TDynArray2,1)  // Base class for lockable objects
};

#endif



