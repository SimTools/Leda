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
//*    2002/02/10  K.Fujii	Protect against invalid access to
//*				non-existent entries.
//*
//*************************************************************************
//
#include "TDynArray3.h"
#include <iostream>
//_____________________________________________________________________
//  -------------------------
//  3-dim Dynamic Array Class
//  -------------------------
//
ClassImp(TDynArray3)

TObject * TDynArray3::operator() (Int_t i, Int_t j, Int_t k) const
{
	if (!At(i) || !((TDim *)At(i))->At(j)) return 0; 
	return ((TDim *)((TDim *)At(i))->At(j))->At(k);
}

TObject *& TDynArray3::Set(Int_t i, Int_t j, Int_t k)
{
	TDim *d1p = (TDim *)SafeAt(i);		// Force i to dimension 1
	if (!d1p) {				// If no 2nd dim. at i
		d1p = new TDim();		// create one
		AddAt(d1p,i);			// and add it to i.
	}
	TDim *d2p = (TDim *)d1p->SafeAt(j);	// Force j to dimension 2
	if (!d2p) {				// If no 3rd dim. at j
		d2p = new TDim();		// create one
		d1p->AddAt(d2p,j);		// and add it to j.
	}
	d2p->SafeAt(k);				// Force k to dimension 3

	return d2p->operator[](k);
}
