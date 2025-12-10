#ifndef MYEVENTACTION_HH
#define MYEVENTACTION_HH

#include "G4UserEventAction.hh"
#include "G4Event.hh"
#include "globals.hh"
#include "G4SDManager.hh"
#include "MyHit.hh"
#include <TFile.h>
#include <TTree.h>

/*
  MyEventAction:
  - Writes two TTrees per thread output file:
      1) "Hits"  (existing) : per-hit rows (same as before)
      2) "Tracks" (new)     : per-track per-event initial KE (trackID, initKE)
  - The "Tracks" tree is filled from the per-event map recorded by MySensitiveDetector.
*/

class MyEventAction : public G4UserEventAction {
public:
    MyEventAction();
    virtual ~MyEventAction();

    virtual void BeginOfEventAction(const G4Event*) override;
    virtual void EndOfEventAction(const G4Event*) override;

private:
    // tree branches (Hits tree)
    G4int fTrackID = -1;
    G4double fEdep = 0.0;
    G4int fEvent = -1;
    G4double posPhoton[3] = {0.0, 0.0, 0.0};
    G4double wlen = 0.0;
    G4double time = 0.0;
    G4double posDetector[3] = {0.0, 0.0, 0.0};
    G4int fDetectorID = -1;

    // Tracks tree branches (new)
    G4int bTrackID = -1;
    G4double bInitKE = 0.0;

    // ROOT IO
    TTree *tree = nullptr;         // Hits tree
    TTree *treeTracks = nullptr;   // Tracks tree (new)
    TFile *outputFile = nullptr;
    bool initialized = false;
};

#endif
