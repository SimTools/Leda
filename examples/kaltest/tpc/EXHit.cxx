#include "EXHit.h"
#include "EXMeasLayer.h"

ClassImp(EXHit)

//_____________________________________________________________________
//  ----------------------------------
//  Ctors and Dtor
//  ----------------------------------

EXHit::EXHit(Int_t m)
        : TVTrackHit(m), fVdrift(0.7e-3)
{
}
      
EXHit::EXHit(const EXMeasLayer &ms,
                   Double_t    *x,
                   Double_t    *dx, 
                   Int_t        side,
                   Double_t     v,
             const TVector3    &xx,
                   Double_t     b,
                   Int_t        m)
        : TVTrackHit(ms,x,dx,xx,b,m), fSide(side), fVdrift(v)
{
}

EXHit::~EXHit()
{
}

//_____________________________________________________________________
//  ----------------------------------
//  Implementation of public methods  
//  ----------------------------------

TKalMatrix EXHit::XvToMv(const TVector3 &xv, Double_t t0) const
{
   TKalMatrix h = GetMeasLayer().XvToMv(xv,GetSide());

#if 0
   Double_t r    = GetMeasLayer().GetR();
   Double_t phih = h(0,0) / r;
   Double_t phim = (*this)(0,0) / r;
   Double_t dphi = phih - phim;

   static Double_t kPi    = TMath::Pi();
   static Double_t kTwoPi = 2 * kPi;

   while (dphi < -kPi) {
      dphi += kTwoPi;
   }
   while (dphi >  kPi) {
      dphi -= kTwoPi;
   }

   h(0,0)  = r * (phim + dphi);
#endif
   h(1,0) += fVdrift * t0;       // T0 shift

   return h;
}

void EXHit::DebugPrint(Option_t *) const
{
   cerr << "------------------- Site Info -------------------------" << endl;

   for (Int_t i=0; i<GetDimension(); i++) {
      Double_t x  = (*this)(i,0);
      Double_t dx = (*this)(i,1);
      cerr << " x[" << i << "] = " << setw(8) << setprecision(5) << x
           << "    "
           << "dx[" << i << "] = " << setw(6) << setprecision(2) << dx
           << setprecision(7)
           << resetiosflags(ios::showpoint)
           << endl;
   }
   cerr << "-------------------------------------------------------" << endl;
}
