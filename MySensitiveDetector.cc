#include "MySensitiveDetector.hh"
#include "MyHit.hh"
#include "G4SDManager.hh"
#include "G4Step.hh"
#include "G4TouchableHistory.hh"

MySensitiveDetector::MySensitiveDetector(const G4String& name)
    : G4VSensitiveDetector(name) {
    collectionName.insert("MyHitCollection");
}

MySensitiveDetector::~MySensitiveDetector() {}

void MySensitiveDetector::Initialize(G4HCofThisEvent* hce) {
    hitsCollection = new MyHitCollection(SensitiveDetectorName, collectionName[0]);

    if (HCID < 0) {
        HCID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
    }
    hce->AddHitsCollection(HCID, hitsCollection);
}

G4bool MySensitiveDetector::ProcessHits(G4Step* aStep, G4TouchableHistory*) {
    G4double edep = aStep->GetTotalEnergyDeposit();
    if (edep <= 0.) return false;

    auto preStep = aStep->GetPreStepPoint();
    const G4VTouchable* touch = preStep->GetTouchable();

    G4int row   = touch->GetCopyNumber(0);  
    G4int col   = touch->GetCopyNumber(1);  
    G4int layer = touch->GetCopyNumber(2);  

    G4int detID = layer * 10000 + col * 100 + row;
    G4int trackID = aStep->GetTrack()->GetTrackID();

    
    MyHit* hit = nullptr;
    for (size_t i = 0; i < hitsCollection->GetSize(); ++i) {
        MyHit* existingHit = (*hitsCollection)[i];
        if (existingHit->GetID() == detID ) {
            hit = existingHit;
            break;
        }
    }

    if (!hit) {
        hit = new MyHit(detID, edep);
        hit->SetTrackID(trackID);
        hit->SetPosition(preStep->GetPosition());
        hit->SetMomentum(preStep->GetMomentum());
        hit->SetTime(preStep->GetGlobalTime());
        hit->SetWavelength((1.239841939 * eV / preStep->GetMomentum().mag()) * 1E+03);
        hit->SetDetectorPosition(touch->GetTranslation());
        hitsCollection->insert(hit);
    } else {
        hit->AddEdep(edep);
    }

    return true;
}

