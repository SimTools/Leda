#ifndef __TATTELEMENT__
#define __TATTELEMENT__
//*************************************************************************
//* ===================
//*  TAttElement Class
//* ===================
//*
//* (Description)
//*    TAttElement class adds constituent attribute to an object.
//* (Requires)
//* 	none
//* (Provides)
//* 	class TAttElement
//* (Update Recored)
//*    2003/10/10  K.Fujii	Original very primitive version.
//*
//*************************************************************************
//
#include <Rtypes.h>
class TObject;
//_____________________________________________________________________
//  --------------------------------
//  Base Class for Element Objects
//  --------------------------------
//
class TAttElement {
public:
   TAttElement() : fParentPtr(0) {}
   virtual ~TAttElement() {}

   inline virtual const TObject  & GetParent() const { return *fParentPtr; }

   inline virtual void  SetParentPtr(TObject *obj)   { fParentPtr = obj;   }

private:
   TObject *fParentPtr;	 // pointer to parent

   ClassDef(TAttElement,1)  // Base class for lockable objects
};

#endif
