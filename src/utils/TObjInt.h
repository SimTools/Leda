#ifndef TOBJINT_H
#define TOBJINT_H

//*************************************************************************
//* -------
//* TObjInt
//* -------
//* (Description)
//*     A simple class to hold an integer.
//* (Update Record)
//*	2000/05/16  K.Fujii	Original version.
//*************************************************************************


#include "TObject.h"
#include <iostream>

class TObjInt : public TObject {
private:
  Int_t num; //! data
public:
  TObjInt(Int_t i = 0) : num(i) {}
  ~TObjInt() {}
  void     SetNum(Int_t i) { num = i; }
  Int_t    GetNum() { return num; }
  void     Print(Option_t *) { std::cerr << "TObjInt = " << num << std::endl; }
  Bool_t   IsEqual(TObject *obj) { return num == ((TObjInt*)obj)->num; }
  Bool_t   IsSortable() const    { return kTRUE; }
  Int_t    Compare(const TObject *obj) const { 
    if      (num > ((TObjInt*)obj)->num)  return  1;
    else if (num < ((TObjInt*)obj)->num)  return -1;
    else                                  return  0; 
  }
  ClassDef(TObjInt, 1) // Int_t object
};

#endif
