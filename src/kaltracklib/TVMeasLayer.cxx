//*************************************************************************
//* ====================
//*  TVMeasLayer Class
//* ====================
//*
//* (Description)
//*   Measurement layer interface class.
//* (Requires)
//* (Provides)
//*     class TVMeasLayer
//* (Update Recored)
//*   2003/09/30  Y.Nakashima       Original version.
//*   2005/02/23  A.Yamaguchi       Added new data members, fFwdX0Inv,
//*                                 fBwdX0Inv and fIndex, and their
//*                                 corresponding getters and setters.
//*                                 Added a new method, GetX0Inv().
//*
//*************************************************************************
//

#include "TVMeasLayer.h"

ClassImp(TVMeasLayer)

TVMeasLayer::TVMeasLayer(Double_t x0invIn, Double_t x0invOut)
           : fX0InvIn(x0invIn), fX0InvOut(x0invOut), fIndex(0)
{
}

