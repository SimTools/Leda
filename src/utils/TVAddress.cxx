//*****************************************************************************
//* ==============
//* TVAddress
//* ==============
//*   Base class for METIS (smeared) hit cell location IDs.
//*   Mother class: TObject
//*
//* (Update Record)
//*   2004.07.31  A.Sanchez  Primitive version.
//*****************************************************************************
#include "TVAddress.h"

ClassImp(TVAddress)

TVAddress::TVAddress(Int_t nIDs)
  : fNIDs(nIDs)
{
}

