#ifndef __TVADDRESS__
#define __TVADDRESS__
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

#include "TVector3.h"
#include "TObject.h"
#include <vector>


class TVAddress : public TObject {
public:

  TVAddress(Int_t nIDs=0);
  ~TVAddress() { fIDs.clear(); }

  // Future Plan: Override TObject::Compare() later...

  virtual TVector3 GetAddressPosition()                       const = 0;
  virtual Double_t GetDistanceFrom(const TVAddress *addr    ) const = 0;
  virtual Double_t GetDistanceFrom(const TVector3   position) const = 0;

  std::vector<Int_t> GetIDs() const { return fIDs; }
  Int_t GetNIDs() const { return fIDs.size(); }

private:
  std::vector<Int_t> fIDs;
  Int_t fNIDs;

  ClassDef(TVAddress, 1) // hit cell location ID base class
};

#endif
