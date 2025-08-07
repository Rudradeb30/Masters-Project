#ifndef DETECTOR_HH
#define DETECTOR_HH

#include "G4VSensitiveDetector.hh"
#include "G4SystemOfUnits.hh"
#include "MyHit.hh"

class MySensitiveDetector : public G4VSensitiveDetector {
public:
  MySensitiveDetector(const G4String &);
  ~MySensitiveDetector();
    
  void Initialize(G4HCofThisEvent* hce) override;
//typedef G4THitsCollection<MyHit> MyHitCollection;
  G4bool ProcessHits(G4Step*, G4TouchableHistory*) override; 

private:
  MyHitCollection* hitsCollection;
  G4int HCID = -1;
  G4String myName; 
};

#endif

