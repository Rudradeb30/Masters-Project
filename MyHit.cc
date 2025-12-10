#include "MyHit.hh"
#include "G4THitsCollection.hh"
#include "G4ios.hh"
#include <algorithm>

G4ThreadLocal G4Allocator<MyHit>* MyHitAllocator = 0;

// Constructor:
// default layer is calculated as id/10000 if caller didn't pass explicit layer
MyHit::MyHit(G4int id, G4int layer, G4double edep)
  : G4VHit(),
    fDetectorID(id),
    fLayerID(layer >= 0 ? layer : (id/10000)),
    fEdep(edep),
    fTime(0.0),
    fPosition(),
    fMomentum(),
    fWavelength(0.0),
    fDetectorPosition(),
    fTrackID(-1),
    fTrackEdepMap(),
    fTrackInitKE()
{}

MyHit::~MyHit() = default;

// copy ctor: copy all fields (including the per-track maps)
MyHit::MyHit(const MyHit& right) : G4VHit(right) {
  fDetectorID    = right.fDetectorID;
  fLayerID       = right.fLayerID;
  fEdep          = right.fEdep;
  fTime          = right.fTime;
  fPosition      = right.fPosition;
  fMomentum      = right.fMomentum;
  fWavelength    = right.fWavelength;
  fDetectorPosition = right.fDetectorPosition;
  fTrackID       = right.fTrackID;
  fTrackEdepMap  = right.fTrackEdepMap;
  fTrackInitKE   = right.fTrackInitKE;
}

const MyHit& MyHit::operator=(const MyHit& right) {
  if (this != &right) {
    fDetectorID    = right.fDetectorID;
    fLayerID       = right.fLayerID;
    fEdep          = right.fEdep;
    fTime          = right.fTime;
    fPosition      = right.fPosition;
    fMomentum      = right.fMomentum;
    fWavelength    = right.fWavelength;
    fDetectorPosition = right.fDetectorPosition;
    fTrackID       = right.fTrackID;
    fTrackEdepMap  = right.fTrackEdepMap;
    fTrackInitKE   = right.fTrackInitKE;
  }
  return *this;
}

// equality: compare detector+layer (and trackID if you want)
// useful for containers – not virtual in G4VHit so don't use override
G4bool MyHit::operator==(const MyHit& right) const {
  return (this->fDetectorID == right.fDetectorID &&
          this->fLayerID == right.fLayerID &&
          this->fTrackID == right.fTrackID);
}

// --- Setters ---
void MyHit::SetID(G4int id) { fDetectorID = id; }
void MyHit::SetLayer(G4int layer) { fLayerID = layer; }
void MyHit::AddEdep(G4double edep) { fEdep += edep; }
void MyHit::SetTime(G4double time) { fTime = time; }
void MyHit::SetPosition(const G4ThreeVector& pos) { fPosition = pos; }
void MyHit::SetMomentum(const G4ThreeVector& mom) { fMomentum = mom; }
void MyHit::SetWavelength(G4double wlen) { fWavelength = wlen; }
void MyHit::SetDetectorPosition(const G4ThreeVector& detPos) { fDetectorPosition = detPos; }
void MyHit::SetTrackID(G4int tid) { fTrackID = tid; }

// AddTrackEdep: records a deposit by trackID. If initKE>=0 is passed and
// the track does not yet have an entry in fTrackInitKE,it records it there.
void MyHit::AddTrackEdep(G4int trackID, G4double edep, G4double initKE) {
  if (edep <= 0.0) return;

  // update total deposited energy in cell
  fEdep += edep;

  // accumulate per-track deposit
  auto it = fTrackEdepMap.find(trackID);
  if (it == fTrackEdepMap.end()) {
    fTrackEdepMap.emplace(trackID, edep);
  } else {
    it->second += edep;
  }

  // if initial KE was provided and not already recorded,it stores it
  if (initKE >= 0.0) {
    auto itk = fTrackInitKE.find(trackID);
    if (itk == fTrackInitKE.end()) {
      fTrackInitKE.emplace(trackID, initKE);
    } else {
      //
    }
  }
}

// --- Getters ---
G4int MyHit::GetID() const { return fDetectorID; }
G4int MyHit::GetLayer() const { return fLayerID; }
G4double MyHit::GetEdep() const { return fEdep; }
G4double MyHit::GetTime() const { return fTime; }
G4ThreeVector MyHit::GetPosition() const { return fPosition; }
G4ThreeVector MyHit::GetMomentum() const { return fMomentum; }
G4double MyHit::GetWavelength() const { return fWavelength; }
G4ThreeVector MyHit::GetDetectorPosition() const { return fDetectorPosition; }
G4int MyHit::GetTrackID() const { return fTrackID; }

const std::map<G4int, G4double>& MyHit::GetTrackEdepMap() const {
  return fTrackEdepMap;
}
const std::map<G4int, G4double>& MyHit::GetTrackInitKEMap() const {
  return fTrackInitKE;
}
size_t MyHit::GetTrackCount() const {
  return fTrackEdepMap.size();
}
// Return top N contributing tracks sorted by deposited energy (descending)
std::vector<std::pair<G4int, G4double>> MyHit::GetTopNTracks(size_t N) const {
  std::vector<std::pair<G4int, G4double>> vec;
  vec.reserve(fTrackEdepMap.size());
  for (const auto &kv : fTrackEdepMap) vec.emplace_back(kv.first, kv.second);
  std::sort(vec.begin(), vec.end(),
            [](const std::pair<G4int, G4double>& a, const std::pair<G4int, G4double>& b){
              return a.second > b.second;
            });
  if (vec.size() > N) vec.resize(N);
  return vec;
}
// Return single best contributor (trackID, edep)
std::pair<G4int, G4double> MyHit::GetMaxContributor() const {
  if (fTrackEdepMap.empty()) return std::make_pair(-1, 0.0);
  auto it = std::max_element(fTrackEdepMap.begin(), fTrackEdepMap.end(),
                       [](const std::pair<G4int,G4double>& a, const std::pair<G4int,G4double>& b){
                               return a.second < b.second;
                            });
  return *it;
}
