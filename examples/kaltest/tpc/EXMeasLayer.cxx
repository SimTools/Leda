//*************************************************************************
//* ===================
//*  EXMeasLayer Class
//* ===================
//*
//* (Description)
//*   Sample measurement layer class used by EXHit.
//* (Requires)
//* (Provides)
//*     class EXMeasLayer
//* (Update Recored)
//*   2003/09/30  Y.Nakashima       Original version.
//*
//*************************************************************************
//

#include "EXMeasLayer.h"
#include "EXHit.h"

ClassImp(EXMeasLayer)
                                                                                
TKalMatrix EXMeasLayer::XvToMv(const TVector3   &xv,
                                     Int_t       side) const
                             
{
   // Calculate hit coordinate information:
   //	mv(0,0) = r * phi 
   //     (1,0) = z from drift length

   Double_t xvx = xv.X();
   Double_t xvy = xv.Y();
   Double_t xvz = xv.Z();

   TKalMatrix mv(kMdim,1);
   mv(0,0)  = GetR() * TMath::ATan2(xvy,xvx);          // r * phi 
   mv(1,0)  = (GetLength() / 2) - side * xvz;    // z from drift length
   return mv;
}

TKalMatrix EXMeasLayer::XvToMv(const TVTrackHit &vht,
                               const TVector3   &xv) const
{
   const EXHit &ht = *dynamic_cast<const EXHit *> (&vht);
   return XvToMv(xv,ht.GetSide());
}

TVector3 EXMeasLayer::HitToXv(const TVTrackHit &vht) const
{
   const EXHit &ht = *dynamic_cast<const EXHit *> (&vht);

   Double_t phi = ht(0,0) / GetR();
   Double_t z   = ht.GetSide() * (GetLength() / 2 - ht(1,0));
   Double_t x   = GetR() * TMath::Cos(phi);
   Double_t y   = GetR() * TMath::Sin(phi);

   return TVector3(x,y,z);
}

void EXMeasLayer::CalcDhDa(const TVTrackHit &vht,
                           const TVector3   &xxv,
                           const TKalMatrix &dxphiada,
                                 TKalMatrix &H)  const
{
   const EXHit &ht = *dynamic_cast<const EXHit *> (&vht);

   // Calculate
   //    H = (@h/@a) = (@phi/@a, @z/@a)^t
   // where
   //        h(a) = (phi, z)^t: expected meas vector
   //        a = (drho, phi0, kappa, dz, tanl, t0)
   //

   Int_t sdim = H.GetNcols();
   Int_t hdim = TMath::Max(5,sdim-1);

   Double_t xv = xxv.X();
   Double_t yv = xxv.Y();
   Double_t xxyy = xv * xv + yv * yv;

   // Set H = (@h/@a) = (@d/@a, @z/@a)^t
   
   for (Int_t i=0; i<hdim; i++) {
      H(0,i) = - (yv / xxyy) * dxphiada(0,i) 
               + (xv / xxyy) * dxphiada(1,i);
      H(0,i) *= GetR();
      H(1,i) = - ht.GetSide() * dxphiada(2,i);
   }
   if (sdim == 6) {
      H(0,sdim-1) = 0.;
      H(1,sdim-1) = ht.GetVdrift();
   }
}
