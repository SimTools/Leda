//-------------------
// VERY TEMPORARY
//-------------------
//#define __LRBYWIRE__
//-------------------

#include "EXKalDetector.h"

ClassImp(EXKalDetector)

// ------------------------------------------------------------------
// A very crude model of JLC-CDC
// ------------------------------------------------------------------
//

EXKalDetector::EXKalDetector(Int_t    nlayers,
                             Double_t lhalf,
                             Double_t rmin,
                             Double_t rstep,
                             Double_t rt0det)
	: TVKalDetector(nlayers+1), fRT0Det(rt0det)
{
   // create inner timing detector
   if (rt0det > 0.) {
         Add(new EXMeasLayer(rt0det, lhalf));
   }
   // create measurement layers of central tracker

   Double_t r    = rmin;
   for (Int_t layer=0; layer<nlayers; layer++) {
      Add(new EXMeasLayer(r, lhalf));
      r += rstep;
   }
   SetOwner();
}

Double_t EXKalDetector::CalcRadLength(const TVMeasLayer &from,
                                      const TVMeasLayer &to) const
{
   Double_t xl = 1.8182e+4; // [cm] for CO2/iso-C4H10 (90:10)
   return xl;
}

Double_t EXKalDetector::CalcSigmaMS0(const TVMeasLayer &vfrom,
                                     const TVMeasLayer &vto,
                                           Double_t     pathlen) const
{
   static const Double_t kMS1  = 0.0136;
   static const Double_t kMS2  = 0.038;

   const EXMeasLayer &from = *dynamic_cast<const EXMeasLayer *> (&vfrom);
   const EXMeasLayer &to   = *dynamic_cast<const EXMeasLayer *> (&vto);

   Double_t xl    = pathlen / CalcRadLength(from,to);
   // ------------------------------------------------------------------
            xl    = TMath::Max(xl,1.e-4); // very crude treatment that
					  // should be improved!
   // ------------------------------------------------------------------

   // ------------------------------------------------------------------
   if (fRT0Det > 0.) {			// VERY TEMPORARY
      Double_t rfrom = from.GetR();
      Double_t rto   = to.GetR();
      Double_t eps   = 1.e-2;
      if ((fRT0Det+eps - rfrom)*(fRT0Det+eps - rto) <= 0.) {
#if 1
         xl = 0.006;  // CFRP or Field cage
#else
         xl = 0.03;  // CFRP or Field cage
#endif
      }
   }
   // ------------------------------------------------------------------

   Double_t sgms0 = kMS1 * TMath::Sqrt(xl) * (1 + kMS2*TMath::Log(xl));

   if (IsMSOn()) return sgms0;
   else          return 0.;
}
