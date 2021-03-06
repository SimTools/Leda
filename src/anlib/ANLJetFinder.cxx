//*************************************************************************
//* ======================
//*  ANLJetFinder Classes
//* ======================
//*
//* (Description)
//*    Jet finder classes for JLC analyses
//* (Requires)
//* 	class TLorentzVector
//* 	class TAttLockable
//* 	class ANL4DVector
//* (Provides)
//* 	class ANLJet
//* 	class ANLJetFinder
//* 	class ANLJadeJetFinder
//* 	class ANLJadeEJetFinder
//* 	class ANLDurhamJetFinder
//* (Usage)
//*     // Example
//*     Double_t ycut = 0.01;
//*	ANLJadeEJetFinder jclust(ycut);
//*     jclust.Initialize(tracks); // tracks: TObjArray of LVector derivatives.
//*     jclust.FindJets();	   // finds jets with ycut = 0.01.
//*     ycut = 0.015;
//*	jclust.SetYcut(ycut);	   // One can make the ycut "bigger"
//*	jclust.FindJets();	   // and resume with the new ycut.
//*     ycut = 0.05;
//*	ANLJadeEJetFinder jclust2(jclust);
//*	jclust2.SetYcut(ycut);	   // One can copy the previous jet finder
//*	jclust2.FindJets();	   // and resume with the new "bigger" ycut.
//*	Int_t njets = 2;	   // One can also force the event to be
//* 	jclust2.ForceNJets(njets); // "njets" jets.
//* (Caution)
//*	One CANNOT decrease the ycut without restoring track information
//*	by reinvoking the "Initialize" member function.
//*	On the other hand, one can increase the ycut and resume jet
//*	finding starting from the last result, thereby saving CPU time.
//* (Update Recored)
//*    1999/07/30  K.Fujii	Original version derived from Rob Shanks's
//*				LCD version. This version heavily uses
//*				ROOT facilities to simplify algorithm.
//*				It allows one to "increase" the last ycut
//*				and resume jet finding. It also allows one
//*				to force an event to be "n" jets.
//*    1999/08/14  K.Fujii	Modified GetYmass as suggested by M.Iwasaki.
//*    1999/09/05  K.Ikematsu   Replaced LockableLVector with ANL4DVector.
//*    1999/09/14  K.Ikematsu   Added GetYmax method.
//*    2000/03/28  K.Ikematsu   Bug fixed about fYmassMax.
//*    2000/10/12  K.Ikematsu   Added maximum trial in ycut search part
//*                             of ForceNJets method. If binary search
//*                             reaches maximum trial, ForceNJets method
//*                             aborts to find ycut making "n" jets.
//*                             So you have to confirm to be "n" jets in
//*                             your analysis program.
//*    2001/10/22  K.Ikematsu   Added virtual NewJet method
//*                             instead of ANLJet *jet = new ANLJet();
//*                             and changed fEvis member to protected
//*                             to overload in ANLCheatedJetFinder class.
//*    2001/10/22  K.Ikematsu   Changed Merge method to virtual
//*                             for overloading in ANLTaggedJet class.
//*    2001/10/24  K.Ikematsu   Added virtual NewJetFinder method.
//*    2008/02/13  D.Jeans      Improved efficiency of jet forcing
//*    2008/03/26  K.Fujii      Fixed operator= to copy fYmassMax.
//*                             Modified to allow restarting from a
//*                             previous state of jet clustering.
//*
//* $Id$
//*************************************************************************
//
#include "ANLJetFinder.h"
//_____________________________________________________________________
//  ------------
//  ANLJet Class
//  ------------
//
ClassImp(ANLJet)

//*--
//*  Constructors
//*--
ANLJet::ANLJet() : ANL4DVector(0.), fParts(1) {}

ANLJet::ANLJet(const TObjArray &parts) : ANL4DVector(0.), fParts(parts) {
   TIter next(&parts);
   TObject *obj;
   while ((obj = next())) *this += *(ANL4DVector *)obj;
}

//*--
//*  Destructor
//*--
ANLJet::~ANLJet() { fParts.Clear(); }

//*--
//*  Getters
//*--
Int_t ANLJet::GetNparticles() const { return fParts.GetEntries(); }

const TObjArray & ANLJet::GetParticlesInJet() const { return fParts; }

const ANL4DVector & ANLJet::operator()() const { return *this; }

//*--
//*  Setters
//*--
void ANLJet::Add(TObject *part) {
   fParts.Add(part);
   *this += *(ANL4DVector *)part;
   // ::operator+=(*(ANL4DVector *)this,*(ANL4DVector *)part);
}

void ANLJet::Merge(TObject *part) { Add(part); }

void ANLJet::Merge(ANLJet *jet) {
   TIter next(&jet->fParts);
   TObject *obj;
   while ((obj = next())) fParts.Add(obj);
   *this += *jet;
}

void ANLJet::Remove(TObject *part) {
   *this -= *(ANL4DVector *)part;
   fParts.Remove(part);
}

//*--
//*  Misc. Services
//*--
void ANLJet::DebugPrint(const Char_t *opt) const {
   cerr << "Number of particles in jets = " << GetNparticles() << endl;
   ANL4DVector::DebugPrint(opt);
   if (TString(opt).Contains("Detailed")) {
      TIter next(&fParts);
      ANL4DVector *p;
      Int_t np = 0;
      while((p = (ANL4DVector *)next())) {
         cerr << "Particle " << ++np << endl;
         p->DebugPrint(opt);
      }
   }
}


//_____________________________________________________________________
//  ------------------
//  ANLJetFinder Class
//  ------------------
//
ClassImp(ANLJetFinder)

//*--
//*  Constructors
//*--
ANLJetFinder::ANLJetFinder(Double_t y)
            : fDone(kFALSE), fYcut(y), fJets(1),
              fYmass(0), fYmassMax(0.), fEvis(0.) {}

ANLJetFinder::ANLJetFinder(const ANLJetFinder &jf)
            : fDone(jf.fDone), fYcut(jf.fYcut), fJets(1),
              fYmass(0), fYmassMax(jf.fYmassMax), fEvis(jf.fEvis) {
   CopyJets(jf.fJets);
}

//*--
//*  Destructor
//*--
ANLJetFinder::~ANLJetFinder() {
   DeleteJets(); 		// clean the array of Jet's
   if (fYmass) delete fYmass;	// clean the pair mass array
}

//*--
//*  Private service methods
//*--
void ANLJetFinder::CopyJets(const TObjArray &jets) {
   DeleteJets();
   TIter next(&jets);
   TObject *obj;
   while ((obj = next())) {
     ANLJet *jet = NewJet();
     jet->Merge((ANLJet *)obj);
     fJets.Add(jet);
   }
}

void ANLJetFinder::DeleteJets() {
   fJets.Delete();
}

//*--
//*  Getters
//*--
Bool_t   ANLJetFinder::IsInitialized()  const {
   if (fJets.GetEntries() > 0) return kTRUE;
   else return kFALSE;
}

Double_t ANLJetFinder::GetYcut() const { return fYcut; }

Double_t ANLJetFinder::GetYmax() {
   if (!fDone) FindJets();
   return fYmassMax/(fEvis*fEvis);
}

Int_t    ANLJetFinder::GetNjets() {
   if (!fDone) FindJets();
   return fJets.GetEntries();
}

TObjArray & ANLJetFinder::GetJets() {
   if (!fDone) FindJets();
   return fJets;
}

//*--
//*  Setters
//*--
void ANLJetFinder::SetYcut(Double_t ycut) {
   if (ycut != fYcut) {
      fDone = kFALSE;
      fYcut = ycut;
   }
}

void ANLJetFinder::Initialize(const TObjArray &parts) {
   fDone = kFALSE;
   DeleteJets();
   fEvis = 0.;
   //
   // Store each unlocked object as a single jet.
   //
   TIter next(&parts);
   TObject *obj;
   while ((obj = next())) {
      if (!obj->IsA()->InheritsFrom("ANL4DVector")) {
         cerr << "ANLJetFinder::Initialize input is not a ANL4DVector" << endl;
	 continue;
      }
      if (((ANL4DVector *)obj)->IsLocked()) continue;
      ANLJet *jet = NewJet();
      jet->Merge(obj);		// obj can be a jet, instead of being a track.
      fEvis += jet->E();
      fJets.Add(jet);
   }
}

ANLJetFinder & ANLJetFinder::operator=(const ANLJetFinder & jf) {
   fDone = jf.fDone;
   fYcut = jf.fYcut;
   fEvis = jf.fEvis;
   fYmassMax = jf.fYmassMax;
   CopyJets(jf.fJets);
   if (fYmass) { delete fYmass; fYmass = 0; }
   return *this;
}

//*--
//*  Basic Services
//*--
void ANLJetFinder::FindJets(Int_t nforcejets) {
  // added njet forcing to this method - djeans 12/2/08
#if 1
   Int_t np  = fJets.GetEntries();
   if (nforcejets > 0 && nforcejets < np) fDone = kFALSE;
#endif
   if (fDone) return;
   if (!IsInitialized()) {
      cout << "ANLJetFinder::FindJets : No particles in the stack" << endl
           << "                         Set them and retry." << endl;
      return;
   }
   fDone     = kTRUE;
#if 0
   Int_t np  = fJets.GetEntries();	// There is yet no gap in fJets here.
#endif
   if (np < 2) return;

   if (nforcejets>0) {
     if (np==nforcejets) {
       cout << "ANLJetFinder::FindJets - nothing to do: passed " << np << " particles and asked to make " << nforcejets << " jets" << endl;
       return;
     }
     else if (np<=nforcejets) {
       cout << "ANLJetFinder::FindJets - warning: passed " << np << " particles and asked to make " << nforcejets << " jets" << endl;
       fYcut = -1;
       return;
     }
   }

   //
   // Initialize pair mass matrix.
   //
   if (fYmass) delete fYmass;
   fYmass = new TMatrix(np,np);
   TMatrix &ymass = *fYmass;
   Int_t i, j;
   for (i = 0; i < np - 1; i++) {
      for (j = i + 1; j < np; j++) {
         ANLJet &jeti = *(ANLJet *)fJets.UncheckedAt(i);
         ANLJet &jetj = *(ANLJet *)fJets.UncheckedAt(j);
         ymass(i,j) = GetYmass(jeti(),jetj());
      }
   }
   //
   // Start jet clustering
   //
   Double_t masscut  = fYcut*fEvis*fEvis;
   while (kTRUE) {
      Int_t i, im = -1;
      Int_t j, jm = -1;
      Double_t minmass = 1.e+10;
      for (i = 0; i < np - 1; i++) {
         if (!(fJets.UncheckedAt(i))) continue;
         for (j = i + 1; j < np; j++) {
            if (!(fJets.UncheckedAt(j))) continue;
            if (ymass(i,j) > minmass) continue;
	    minmass = ymass(i,j);
	    im = i;
	    jm = j;
         }
      }

      if (nforcejets<=0 && minmass > masscut) { // changed djeans: only break if not forcing njets
	fYmassMax = minmass;
	break;
      }

      //
      // Pair (im,jm) accepted.
      //
      ANLJet *oim = (ANLJet *)fJets.UncheckedAt(im);
      ANLJet *ojm = (ANLJet *)fJets.UncheckedAt(jm);
      oim->Merge(ojm);				// Merge Jet j to i.
      fJets.Remove(ojm);			// Remove jet j from fJets
      delete ojm;				// Delete jet j

      //
      // Update the pair mass array.
      //
      for (j = 0; j < np; j++) {
         if (j == im) continue;
         ANLJet *oj = (ANLJet *)fJets.UncheckedAt(j);
         if (oj == 0) continue;
         ymass(TMath::Min(j,im),TMath::Max(j,im)) = GetYmass(*oim,*oj);
      }

      if (nforcejets>0 && fJets.GetEntries()==nforcejets) { // added djeans: break here if forcing and njets correct
	// fill ycut
	fYmassMax = minmass;
	fYcut = minmass/(fEvis*fEvis); // this is the ycut at which we go from njets+1 -> njets
	break;
      }
   }
   //
   // Eliminate empty slots.
   //
   fJets.Compress();
}

void ANLJetFinder::ForceNJets(Int_t njets) {

  if (njets < 1) {
    cout << "ANLJetFinder::ForceNJets : njets = " << njets
	 << " invalied" << endl;
    return;
  }

  // simplified by djeans 12/2/08
  FindJets(njets);

  //if (!fDone) FindJets();		// Find jets if not yet.
  //if (GetNjets() <= njets) return;	// No further clustering necessary.
  //
  // Binary search ycut to make njets Jet's.
  //
  //Double_t   ycutLo = fYcut;
  //Double_t   ycutHi = 1.0;
  //Double_t   ycut;
  //ANLJetFinder *jf  = 0;
  //
  //Int_t ntrial = 0;
  //while (kTRUE) {
  //  ntrial++;
  //  ycut = (ycutLo + ycutHi)/2;
  //  jf   = NewJetFinder(this);
  //  SetYcut(ycut);
  //  FindJets();
  //  Int_t nj = GetNjets();
  //  if (ntrial > 100) cerr << "ANLJetFinder::ForceNJets : Making "
  //			   << njets << "Jets was aborted ! Njets = "
  //			   << nj << endl;
  //  if (nj == njets || ntrial > 100) { delete jf; break; } // Found a valid ycut, quit looping.
  //  if (nj > njets) {
  //    ycutLo = ycut;
  //  } else {
  //    ycutHi = ycut;
  //    *this  = *jf;
  //  }
  //  delete jf;
  //}

  return;
}

Double_t ANLJetFinder::GetYmass(const ANL4DVector &p1,
   			        const ANL4DVector &p2) const { return 0; }


//_____________________________________________________________________
//  ----------------------
//  ANLJadeJetFinder Class
//  ----------------------
//
ClassImp(ANLJadeJetFinder)

ANLJadeJetFinder::ANLJadeJetFinder(Double_t y) : ANLJetFinder(y) {}

Double_t ANLJadeJetFinder::GetYmass(const ANL4DVector &p1,
				     const ANL4DVector &p2) const {
   return 2 * p1.E() * p2.E() * ( 1 - p1.CosTheta(p2) );
}


//_____________________________________________________________________
//  -----------------------
//  ANLJadeEJetFinder Class
//  -----------------------
//
ClassImp(ANLJadeEJetFinder)

ANLJadeEJetFinder::ANLJadeEJetFinder(Double_t y) : ANLJetFinder(y) {}

Double_t ANLJadeEJetFinder::GetYmass(const ANL4DVector &p1,
				     const ANL4DVector &p2) const {
   return (p1+p2).GetMass2();
}


//_____________________________________________________________________
//  ------------------------
//  ANLDurhamJetFinder Class
//  ------------------------
//
ClassImp(ANLDurhamJetFinder)

ANLDurhamJetFinder::ANLDurhamJetFinder(Double_t y) : ANLJetFinder(y) {}

Double_t ANLDurhamJetFinder::GetYmass(const ANL4DVector &p1,
				      const ANL4DVector &p2) const {
   Double_t minE = TMath::Min(p1.E(),p2.E());
   return 2 * minE * minE * ( 1 - p1.CosTheta(p2) );
}
