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
//*    2002/02/10  K.Fujii	Protect against invalid access to
//*				non-existent entires.
//*
//*************************************************************************
//
#include "TDynArray2.h"
#include <iostream>
//_____________________________________________________________________
//  -------------------------
//  2-dim Dynamic Array Class
//  -------------------------
//

ClassImp(TDynArray2)

TObject * TDynArray2::operator() (Int_t i, Int_t j) const
{
	if (!At(i)) return 0;
	return ((TDim *)At(i))->At(j);
}

TObject *& TDynArray2::Set(Int_t i, Int_t j)
{
	TDim *d1p = (TDim *)SafeAt(i);	// Force i to dimension 1
	if (!d1p) {			// If no 2nd dim. at i
		d1p = new TDim();	// create one
		AddAt(d1p,i);		// and add it to i.
	}
	d1p->SafeAt(j);			// Force j to dimension 2

	return d1p->operator[](j);
}

