#include "MyEventAction.hh"
#include "G4SDManager.hh"
#include "G4RunManager.hh"
#include "G4Event.hh"
#include "MyHit.hh"
#include "G4HCofThisEvent.hh"
#include "G4ios.hh"
#include "MySensitiveDetector.hh"

#include <TSystem.h>
#include <TThread.h>

MyEventAction::MyEventAction()
    : tree(nullptr), treeTracks(nullptr), outputFile(nullptr), initialized(false)
{
    // Create per-thread output file. Use thread id so parallel jobs don't clash.
    G4int tid = G4Threading::G4GetThreadId();
    // ensure output directory exists (best-effort)
    gSystem->Exec("mkdir -p output");

    outputFile = new TFile(Form("output/output_thread%d.root", tid), "RECREATE");

    // Hits tree
    tree = new TTree("Hits", "Hit Data Tree");
    tree->Branch("fEvent", &fEvent, "fEvent/I");
    tree->Branch("posPhoton", posPhoton, "posPhoton[3]/D");
    tree->Branch("wlen", &wlen, "wlen/D");
    tree->Branch("time", &time, "time/D");
    tree->Branch("posDetector", posDetector, "posDetector[3]/D");
    tree->Branch("fEdep", &fEdep, "fEdep/D");
    tree->Branch("fDetectorID", &fDetectorID, "fDetectorID/I");
    tree->Branch("fTrackID", &fTrackID, "fTrackID/I");

    // Tracks tree 
    treeTracks = new TTree("Tracks", "Initial track kinetic energy per event");
    treeTracks->Branch("fEvent", &fEvent, "fEvent/I");     // reuse fEvent
    treeTracks->Branch("fTrackID", &bTrackID, "fTrackID/I");
    treeTracks->Branch("fInitKE", &bInitKE, "fInitKE/D");

    initialized = true;
}

MyEventAction::~MyEventAction()
{
    if (outputFile) {
        outputFile->cd();
        if (tree)      tree->Write();
        if (treeTracks) treeTracks->Write();
        outputFile->Write();
        outputFile->Close();
        delete outputFile;
        outputFile = nullptr;
    }
}

void MyEventAction::BeginOfEventAction(const G4Event* event)
{
    fEvent = event->GetEventID();
}

void MyEventAction::EndOfEventAction(const G4Event* event)
{
    // Get SD manager and hits collection as before
    G4SDManager* sdManager = G4SDManager::GetSDMpointer();
    if (!sdManager) return;

    G4int hcID = sdManager->GetCollectionID("MyHitCollection");
    if (hcID < 0) {
        G4cerr << "Invalid HitCollection ID!" << G4endl;
        return;
    }

    G4HCofThisEvent* hce = event->GetHCofThisEvent();
    if (!hce) {
        G4cerr << "hce is null!" << G4endl;
        return;
    }

    // retrieve hits collection (may be null if no hits)
    MyHitCollection* hitsCollection = static_cast<MyHitCollection*>(hce->GetHC(hcID));
    if (!hitsCollection) {
        G4cout << "[EventAction] Event " << event->GetEventID() << " has no hits collection." << G4endl;
    } else {
        static int eventPrintCounter = 0;
        if ((eventPrintCounter % 50) == 0) {
            G4cout << "[EventAction] Event " << event->GetEventID()
                   << " has hits = " << hitsCollection->GetSize() << G4endl;
        }
        ++eventPrintCounter;

        // Loop over hits in the collection and fill Hits tree as before
        for (size_t i = 0; i < hitsCollection->GetSize(); ++i) {
            MyHit* hit = static_cast<MyHit*>((*hitsCollection)[i]);
            if (!hit) continue;

            fEdep = hit->GetEdep();                    // total energy deposited in this cell (for the event)
            fDetectorID = hit->GetID();
            fTrackID = hit->GetTrackID();

            posPhoton[0] = hit->GetPosition().x();
            posPhoton[1] = hit->GetPosition().y();
            posPhoton[2] = hit->GetPosition().z();

            posDetector[0] = hit->GetDetectorPosition().x();
            posDetector[1] = hit->GetDetectorPosition().y();
            posDetector[2] = hit->GetDetectorPosition().z();

            wlen = hit->GetWavelength();
            time = hit->GetTime();

            tree->Fill();
        }
    }

    //  write per-event track initial kinetic energies into the Tracks tree ---
    MySensitiveDetector* sd = MySensitiveDetector::Instance();
    if (sd) {
        const auto &trackMap = sd->GetTrackInitKEMap();
        if (!trackMap.empty()) {
            // Fill a row for every track recorded in the SD during this event.
            for (const auto &kv : trackMap) {
                bTrackID = kv.first;
                bInitKE  = kv.second;
                // ensure event number is correct
                fEvent = event->GetEventID();
                treeTracks->Fill();
            }
        }
    } else {
        static bool warned = false;
        if (!warned) {
            G4cerr << "[EventAction] WARNING: MySensitiveDetector::Instance() returned null. "
                   << "Tracks tree will be empty. Check sensitive detector registration." << G4endl;
            warned = true;
        }
    }

    return;
}
