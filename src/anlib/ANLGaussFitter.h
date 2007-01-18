#ifndef __ANLGAUSSFITTER__
#define __ANLGAUSSFITTER__
//*************************************************************************
//* ======================
//*  GaussFitter Classes
//* ======================
//*
//* (Description)
//*    A very primitive class library for pair combiners.
//* (Requires)
//* 
//* (Provides)
//*     class ANLGaussFitter
//* (Update Recored)
//*    2005/10/05  S.Yamamoto      Original very primitive version.
//*
//*************************************************************************
//
#include "TF1.h"
#include "TH1.h"

class ANLGaussFitter {
public:
 ANLGaussFitter(TH1      *hist,
 	        Double_t  nsigma  =  1.e20,  // fit range in sigma
	        Double_t  xlo     = -1.e20,  // initial lower bound
	        Double_t  xhi     =  1.e20,  // initial upper bound
	        Int_t     ntrials =    100,  // max #traials
	        Double_t  eps     =  1.e-5); // convergence criterion
  virtual ~ANLGaussFitter() {}

  TF1      *GetFunction();
  Double_t  GetMean    () { return GetFunction()->GetParameter(1); }
  Double_t  GetSigma   () { return GetFunction()->GetParameter(2); }

private:
  void Update();    // perform Gaussian fit in the specified range

private:
  TH1        *fHistPtr;    //  pointer to input histgram
  Bool_t      fDone;       //  fit done or not
  Double_t    fNsigma;     //  fit range in units of sigma
  Double_t    fXlo;        //  initial lower bound of fit range
  Double_t    fXhi;        //  initial upper bound of fit range
  Double_t    fNtrials;    //  maximum number of trials
  Double_t    fEpsilon;    //  convergence criterion

  ClassDef(ANLGaussFitter,1) // Gauss fitter class
};
#endif
