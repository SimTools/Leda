#ifndef __TDYNARRAY3__
#define __TDYNARRAY3__
//*************************************************************************
//* ==================
//*  TDynArray3 Class
//* ==================
//*
//* (Description)
//*    TDynArray3 class supplies a functionality of a 3-dim array
//*    which has a variable size in each dimension.
//* (Requires)
//* 	TObjArray:
//* (Provides)
//* 	class TDynArray3
//* (Update Recored)
//*    2000/05/17  K.Fujii	Original very primitive version.
//*
//*************************************************************************
//
#include "TDim.h"
//_____________________________________________________________________
//  -------------------------
//  3-dim Dynamic Array Class
//  -------------------------
//
class TDynArray3 : public TDim {
public:
   TDynArray3() {}
   virtual ~TDynArray3() { Delete(); }

   virtual TObject *  operator ()(Int_t i, Int_t j, Int_t k) const;
   // Very strange:
   // 	Rcint does not allow operator() on the LFS.
   //
   virtual TObject *& Set(Int_t i, Int_t j, Int_t k);
   
   ClassDef(TDynArray3,1)	// 3-dim dynamic array.
};

#endif


