
#include "MySensitiveDetector.hh"
#include "MyHit.hh"
#include "G4UnitsTable.hh"

#include "G4SDManager.hh"
#include "G4Step.hh"
#include "G4TouchableHistory.hh"
#include "G4VTouchable.hh"
#include "G4ThreeVector.hh"
#include "G4SystemOfUnits.hh"
#include "G4ios.hh"

thread_local MySensitiveDetector* MySensitiveDetector::sInstance = nullptr;

MySensitiveDetector::MySensitiveDetector(const G4String &name)
  : G4VSensitiveDetector(name),
    hitsCollection(nullptr),
    myName(name)
{
  // set collection name (used by SD manager)
  collectionName.insert("MyHitCollection");

  // set the thread-local instance pointer so EventAction can retrieve per-event data
  sInstance = this;
}

MySensitiveDetector::~MySensitiveDetector() {
  // clear the thread-local pointer if it points to this
  if (sInstance == this) sInstance = nullptr;
}

void MySensitiveDetector::Initialize(G4HCofThisEvent* hce)
{
  // create a fresh hits collection for this event
  hitsCollection = new MyHitCollection(SensitiveDetectorName, collectionName[0]);

  if (HCID < 0) {
    HCID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
  }

  if (hce && HCID >= 0) {
    hce->AddHitsCollection(HCID, hitsCollection);
  }

  // clearing maps for new event
  fHitMap.clear();
  fTrackInitKEMap.clear();
}

G4bool MySensitiveDetector::ProcessHits(G4Step* step, G4TouchableHistory* /*history*/)
{
  G4double edep = step->GetTotalEnergyDeposit();
  if (edep <= 0.) return false;

  const G4VTouchable* touch = step->GetPreStepPoint()->GetTouchable();
  G4int detID = touch->GetCopyNumber();
  G4int trackID = step->GetTrack()->GetTrackID();

  // Record the initial kinetic energy for this track the first time it is seen in this event.
  // Using vertex kinetic energy as the "initial" KE of the track.
  if (fTrackInitKEMap.find(trackID) == fTrackInitKEMap.end()) {
    // GetVertexKineticEnergy gives the kinetic energy the particle had at vertex (when created).
    G4double initKE = step->GetTrack()->GetVertexKineticEnergy();
    fTrackInitKEMap.emplace(trackID, initKE);
  }

  //Now we create or update the MyHit for this detector cell
  auto it = fHitMap.find(detID);
  if (it == fHitMap.end()) {
    // creating a new hit with zero initial energy then AddTrackEdep will add edep and track entry
    MyHit* hit = new MyHit(detID, 0.0);

    hit->SetTrackID(trackID);
    hit->SetTime(step->GetPreStepPoint()->GetGlobalTime());
    hit->SetPosition(step->GetPreStepPoint()->GetPosition());
    hit->SetMomentum(step->GetPreStepPoint()->GetMomentum());

    // accumulating per-track and total energy
    hit->AddTrackEdep(trackID, edep);

    hitsCollection->insert(hit);
    fHitMap[detID] = hit;
  } else {
    // updating existing hit then accumulating per-track energy and total
    MyHit* hit = it->second;
    hit->AddTrackEdep(trackID, edep);
  }

  static int sDbgCount = 0;
  if (sDbgCount < 10) {
    G4cout << "[SD] ProcessHits: detID=" << detID
           << " edep=" << G4BestUnit(edep, "Energy")
           << " track=" << trackID
           << " time=" << G4BestUnit(step->GetPreStepPoint()->GetGlobalTime(), "Time")
           << G4endl;
    ++sDbgCount;
  }

  return true;
}

void MySensitiveDetector::EndOfEvent(G4HCofThisEvent* /*hce*/)
{
  // No extra action required here: the hitsCollection and fTrackInitKEMap
  // are kept in the SD instance for EventAction to access.
}
