#include "MyEventAction.hh"
#include "G4SDManager.hh"
#include "G4RunManager.hh"
#include "G4Event.hh"
#include "MyHit.hh"
#include "TFile.h"
#include "TTree.h"
#include "G4HCofThisEvent.hh"

MyEventAction::MyEventAction() : tree(nullptr), outputFile(nullptr), initialized(false)
{
    G4int tid = G4Threading::G4GetThreadId();
    outputFile = new TFile(Form("output/output_thread%d.root", tid), "RECREATE");

    tree = new TTree("Hits", "Hit Data Tree");

    tree->Branch("fEvent", &fEvent, "fEvent/I");
    tree->Branch("posPhoton", posPhoton, "posPhoton[3]/D");
    tree->Branch("wlen", &wlen, "wlen/D");
    tree->Branch("time", &time, "time/D");
    tree->Branch("posDetector", posDetector, "posDetector[3]/D");
    tree->Branch("fEdep", &fEdep, "fEdep/D");
    tree->Branch("fDetectorID", &fDetectorID, "fDetectorID/I");
    tree->Branch("fTrackID", &fTrackID, "fTrackID/I");

    initialized = true;
}

MyEventAction::~MyEventAction()
{
    if (outputFile)
    {
        outputFile->Write();
        tree->Write();
        outputFile->Close();
        delete outputFile;
    }
}

void MyEventAction::BeginOfEventAction(const G4Event* event)
{
    fEvent = event->GetEventID();
    fEdep = 0.0;
    G4cout << "MyEventAction::BeginOfEventAction for " << fEvent << G4endl;
}

void MyEventAction::EndOfEventAction(const G4Event* event)
{
    G4SDManager* sdManager = G4SDManager::GetSDMpointer();
    G4int hcID = sdManager->GetCollectionID("SensitiveDetector/MyHitCollection");

    if (hcID < 0)
    {
        G4cerr << "Invalid HitCollection ID!" << G4endl;
        return;
    }

    G4HCofThisEvent* hce = event->GetHCofThisEvent();
    if (!hce)
    {
        G4cerr << "hce is null!" << G4endl;
        return;
    }

    MyHitCollection* hitsCollection = static_cast<MyHitCollection*>(hce->GetHC(hcID));
    if (!hitsCollection)
    {
        G4cerr << "hitsCollection is null!" << G4endl;
        return;
    }

    for (size_t i = 0; i < hitsCollection->GetSize(); ++i)
    {
        MyHit* hit = static_cast<MyHit*>((*hitsCollection)[i]);
        if (!hit) continue;

        fEdep = hit->GetEdep();

        posPhoton[0] = hit->GetPosition().x();
        posPhoton[1] = hit->GetPosition().y();
        posPhoton[2] = hit->GetPosition().z();

        wlen = hit->GetWavelength();
        time = hit->GetTime();

        posDetector[0] = hit->GetDetectorPosition().x();
        posDetector[1] = hit->GetDetectorPosition().y();
        posDetector[2] = hit->GetDetectorPosition().z();

        fDetectorID = hit->GetID();
        fTrackID = hit->GetTrackID();

        tree->Fill();  
    }

    G4cout << "Recorded " << hitsCollection->GetSize() << " hits for event " << fEvent << G4endl;
}

