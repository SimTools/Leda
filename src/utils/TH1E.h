#ifndef __TH1E__
#define __TH1E__
//*************************************************************************
//* ============
//*  TH1E Class
//* ============
//*
//* (Description)
//*    TH1E class produces an efficiency plot.
//* (Requires)
//* 	TH1F
//* (Provides)
//* 	class TH1E
//* (Update Recored)
//*    2000/06/14  K.Fujii	Original very primitive version.
//*    2002/02/10  K.Fujii	Added a new ctor: TH1E(const TH2F &h).
//*
//*************************************************************************
//
#include "TH2.h"
#include "TObjString.h"
//_____________________________________________________________________
//  --------------------------
//  Efficiency Histogram Class
//  --------------------------
//
class TH1E : public TH2F {
public:
   TH1E();
   TH1E(const char *name,const char *title,Int_t n,Axis_t xmn,Axis_t xmx);
   TH1E(const char *name,const char *title,Int_t n,Axis_t *xbins);
   TH1E(const TH1E &h);
   TH1E(const TH2F &h);
   virtual ~TH1E();

   virtual void     SetMaximum(Double_t maximum=-1111);
   virtual void     SetMinimum(Double_t minimum=-1111);

   virtual void Copy(TObject &h);
   virtual Int_t Fill(Axis_t x);
   virtual Int_t Fill(Axis_t x, Stat_t w);
   virtual Int_t Fill(Axis_t x, Axis_t wa, Stat_t we);

   virtual void  Draw(Option_t *option="");
   virtual TH1  *DrawCopy(Option_t *option="");

   virtual TH1  *Rebin(Int_t n=2, const char *newname="");

private:
   void Update();
   
private:
   TH1F       *fEff;		// efficiency plot 

   ClassDef(TH1E,1)  		// Efficiency Histogram Class
};

#endif
