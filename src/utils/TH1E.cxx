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
//*    2000/06/15  K.Fujii	TH1E now inherits from TH2F.
//*    2002/02/10  K.Fujii	Added a new ctor: TH1E(const TH2F &).
//*				Fixed a bug in Copy method.
//*
//*************************************************************************
//
#include "TH1E.h"
#include <string>
#include <iostream>

ClassImp(TH1E)

//______________________________________________________________________________
//                     TH1E methods
//______________________________________________________________________________
TH1E::TH1E() : TH2F(), fEff(0)
{
}

//______________________________________________________________________________
TH1E::TH1E(const char *name,const char *title,Int_t nbins,Axis_t xlow,Axis_t xup)
     : TH2F(name,title,nbins,xlow,xup,2,0.,2.), fEff(0)
{
}

//______________________________________________________________________________
TH1E::TH1E(const char *name,const char *title,Int_t nbins,Axis_t *xbins)
     : TH2F(name,title,nbins,xbins,2,0.,2.), fEff(0)
{
}

//______________________________________________________________________________
TH1E::TH1E(const TH1E &h)
{
   ((TH1E &)h).Copy(*this);
}

//______________________________________________________________________________
TH1E::TH1E(const TH2F &h)
{
   ((TH2F &)h).Copy(*this);
}

//______________________________________________________________________________
TH1E::~TH1E()
{
}

//______________________________________________________________________________
void TH1E::SetMinimum(Double_t m)
{
   if (!fEff) Update();
   fEff->SetMinimum(m);
}

//______________________________________________________________________________
void TH1E::SetMaximum(Double_t m)
{
   if (!fEff) Update();
   fEff->SetMaximum(m);
}

//______________________________________________________________________________
void TH1E::Copy(TObject &h)
{
   if (fEff) {
      fEff->Copy(*((TH1E &)h).fEff);
   }
   TH2F::Copy((TH2F &)h);
}

//______________________________________________________________________________
Int_t TH1E::Fill(Axis_t x)
{
   return Fill(x,1.,1.);
}

//______________________________________________________________________________
Int_t TH1E::Fill(Axis_t x, Stat_t w)
{
   return Fill(x,(Axis_t)w,1.);
}

//______________________________________________________________________________
Int_t TH1E::Fill(Axis_t x, Axis_t wa, Stat_t we)
{
   Int_t bin = -999999;   
   if (we > 0.) {
      bin = TH2F::Fill(x,0.5,we);
      if (wa > 0.) TH2F::Fill(x,1.5,wa);
   }
   return bin;
}

//______________________________________________________________________________
void TH1E::Draw(Option_t *option)
{
   Update();
   fEff->Draw(option);
}

//______________________________________________________________________________
TH1 *TH1E::DrawCopy(Option_t *option)
{
   Update();
   TH1F *h = (TH1F *)fEff->Clone();
   h->Draw(option);
   return h;
}

//______________________________________________________________________________
TH1 *TH1E::Rebin(Int_t n, const char *name)
{
   Int_t  nxbins = GetXaxis()->GetNbins();
   Axis_t xmin   = GetXaxis()->GetXmin();
   Axis_t xmax   = GetXaxis()->GetXmax();
   Axis_t dx     = (xmax-xmin)/nxbins;
   Int_t  nxbso  = nxbins;
   nxbins /= n;
   dx     *= n;
   xmax = xmin + nxbins*dx;

   TH1E *hnew;
   Char_t newname[256];
   if (name) { 
      hnew = new TH1E(name,GetTitle(),nxbins,xmin,xmax);
   } else {
      sprintf(newname,"%s_chbn",GetName());
      hnew = new TH1E(newname,GetTitle(),nxbins,xmin,xmax);
   }

   Stat_t z;
   Int_t i, j;
   for (j=0; j<=3; j++)
   {
      for (i=0; i<=nxbins+1; i++) {
      	 if (i > 0 && i <= nxbins) z  = Integral(n*(i-1)+1,n*i,j,j);
      	 else if (i == 0) z = GetBinContent(GetBin(i,j));
      	 else z = Integral(n*nxbins+1,nxbso);
    	 if (z > 0) {
    	    ((TH2F *)hnew)->Fill(hnew->GetXaxis()->GetBinCenter(i),
    	       	                 hnew->GetYaxis()->GetBinCenter(j),z);
    	 }
      }	
   }
   return hnew;
}

//______________________________________________________________________________
void TH1E::Update()
{     
   if (!fEff) {  
      TString tmpname(GetName());
      fEff = new TH1F(tmpname.Append("_e").Data(),
   		      GetTitle(),
   		      GetNbinsX(),
   		      GetXaxis()->GetXmin(),
   		      GetXaxis()->GetXmax());
   }
   GetXaxis()->Copy(*fEff->GetXaxis());
   Int_t i;
   for (i=0; i<=GetNbinsX()+1; i++) {
      Stat_t n   = GetBinContent(GetBin(i,1));
      if (n > 0) {
         Double_t y = GetBinContent(GetBin(i,2))/n;
         fEff->SetBinContent(i,y);
         fEff->SetBinError(i,TMath::Sqrt(y*(1-y)/n));
      }
   }
}

