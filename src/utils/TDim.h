#ifndef __TDIM__
#define __TDIM__
//*************************************************************************
//* ============
//*  TDim Class
//* ============
//*
//* (Description)
//*    TDim class comprises an array element to build TDynArrayX.
//* (Requires)
//* 	TObjArray:
//* (Provides)
//* 	class TDim
//* (Update Recored)
//*    2000/05/18  K.Fujii	Original very primitive version.
//*    2001/05/06  K.Fujii	Replace Expand by AddAtAndExpand to
//*				force fLast to update.
//*
//*************************************************************************
//
#include <TObjArray.h>

class TDim : public TObjArray {
friend class TDynArray3;
friend class TDynArray2;
public:
   TDim() {}
   virtual ~TDim() { Delete(); }
private:
   virtual TObject * SafeAt(Int_t i) {
      if (i>GetLast()) {
		TObject *dummy = 0;
		AddAtAndExpand(dummy,i+1);
	}
	return At(i);
   }

   ClassDef(TDim,1)		// 1-dim array element
};

#endif




