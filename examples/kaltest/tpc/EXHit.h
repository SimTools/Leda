#ifndef __EXHIT__
#define __EXHIT__

#include <iostream>
#include <iomanip>
#include "KalTrackDim.h"
#include "TVTrackHit.h"
#include "EXMeasLayer.h"

using namespace std;

class EXHit : public TVTrackHit {
public:
   EXHit(Int_t m = kMdim);

   EXHit(const EXMeasLayer &ms, Double_t *x, Double_t *dx, Int_t side,
         Double_t v, const TVector3 &xx, Double_t b = 30., Int_t m = kMdim);

   ~EXHit();

   inline Int_t    GetSide()      const { return fSide;     }
   inline Double_t GetVdrift()    const { return fVdrift; }

   inline void     SetVdrift (Double_t v)         { fVdrift   = v;  }

   virtual TKalMatrix XvToMv (const TVector3 &xv, Double_t t0)  const;

   virtual void       DebugPrint(Option_t *opt = "")        const;

private:
   inline const EXMeasLayer & GetMeasLayer() const
   {
      return *(const EXMeasLayer *)&TVTrackHit::GetMeasLayer();
   }

private:
   Int_t        fSide;         // (-z,+z) = (-1,+1)
   Double_t     fVdrift;       // drift velocity

   ClassDef(EXHit,1)      // Sample hit class
};

#endif
