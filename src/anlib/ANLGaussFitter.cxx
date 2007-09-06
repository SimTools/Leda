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
//*    2007/09/06  K.Fujii         Allow asymmetirc fit range.
//*
//*************************************************************************
//
#include "ANLGaussFitter.h"
#include <iostream>

ClassImp(ANLGaussFitter)

//=====================================================================
//* constructor -------------------------------------------------------
ANLGaussFitter::ANLGaussFitter(TH1      *hist,
			       Double_t  nsigma,
			       Double_t  xlo,
			       Double_t  xhi,
			       Int_t     ntrials,
			       Double_t  eps)
              :fHistPtr(hist),
               fDone(kFALSE),
               fNsigmaLo(nsigma),
               fNsigmaHi(nsigma),
               fXlo(xlo),
               fXhi(xhi),
               fNtrials(ntrials),
               fEpsilon(eps)
{
}

ANLGaussFitter::ANLGaussFitter(TH1      *hist,
			       Double_t  nsigmalo,
			       Double_t  nsigmahi,
			       Double_t  xlo,
			       Double_t  xhi,
			       Int_t     ntrials,
			       Double_t  eps)
              :fHistPtr(hist),
               fDone(kFALSE),
               fNsigmaLo(nsigmalo),
               fNsigmaHi(nsigmahi),
               fXlo(xlo),
               fXhi(xhi),
               fNtrials(ntrials),
               fEpsilon(eps)
{
}

//=====================================================================
//* GetFunction -------------------------------------------------------
TF1* ANLGaussFitter::GetFunction() 
{ 
  if (!fDone) Update();
  return   fHistPtr->GetFunction("gaus"); 
}

//=====================================================================
//* Update ------------------------------------------------------------

void ANLGaussFitter::Update()
{
  if (fDone) return;
  fDone = kTRUE;

  Double_t xlo = fXlo;
  Double_t xhi = fXhi;
  Double_t sigmab4 = -999.;

  for (Int_t itry = 0; itry < fNtrials; itry++) {
    fHistPtr->Fit("gaus", "R", "", xlo, xhi);
    Double_t sigma = fHistPtr->GetFunction("gaus")->GetParameter(2);
    if (TMath::Abs(sigma - sigmab4) < fEpsilon*TMath::Abs(sigma)) break;
    Double_t mean = fHistPtr->GetFunction("gaus")->GetParameter(1);
    xlo = TMath::Max(fXlo, mean - fNsigmaLo*sigma);
    xhi = TMath::Min(fXhi, mean + fNsigmaHi*sigma);
    sigmab4 = sigma;
  }
}
