#include "MyHit.hh"

G4ThreadLocal G4Allocator<MyHit>* MyHitAllocator = 0;


MyHit::MyHit(G4int id, G4int layer, G4double edep)
    : G4VHit(), fDetectorID(id), fLayerID(layer), fEdep(edep), fTime(0.),
      fPosition(G4ThreeVector()), fMomentum(G4ThreeVector()),
      fWavelength(0.), fDetectorPosition(G4ThreeVector()),
      fTrackID(-1) {}

MyHit::~MyHit() {}

MyHit::MyHit(const MyHit& right) : G4VHit() {
  fDetectorID = right.fDetectorID;
  fLayerID = right.fLayerID;
  fEdep = right.fEdep;
  fTime = right.fTime;
  fPosition = right.fPosition;
  fMomentum = right.fMomentum;
  fWavelength = right.fWavelength;
  fDetectorPosition = right.fDetectorPosition;
  fTrackID = right.fTrackID;
}

const MyHit& MyHit::operator=(const MyHit& right) {
  if (this != &right) {
    fDetectorID = right.fDetectorID;
    fLayerID = right.fLayerID;
    fEdep = right.fEdep;
    fTime = right.fTime;
    fPosition = right.fPosition;
    fMomentum = right.fMomentum;
    fWavelength = right.fWavelength;
    fDetectorPosition = right.fDetectorPosition;
    fTrackID = right.fTrackID;
  }
  return *this;
}

G4bool MyHit::operator==(const MyHit& right) const {
  return (this->fDetectorID == right.fDetectorID &&
          this->fLayerID == right.fLayerID &&
          this->fTrackID == right.fTrackID);
}

// --- Setters ---
void MyHit::SetID(G4int id) {
  fDetectorID = id;
}

void MyHit::SetLayer(G4int layer) {
  fLayerID = layer;
}

void MyHit::AddEdep(G4double edep) {
  fEdep += edep;
}

void MyHit::SetTime(G4double time) {
  fTime = time;
}

void MyHit::SetPosition(const G4ThreeVector& pos) {
  fPosition = pos;
}

void MyHit::SetMomentum(const G4ThreeVector& mom) {
  fMomentum = mom;
}

void MyHit::SetWavelength(G4double wlen) {
  fWavelength = wlen;
}

void MyHit::SetDetectorPosition(const G4ThreeVector& detPos) {
  fDetectorPosition = detPos;
}

void MyHit::SetTrackID(G4int tid) {
  fTrackID = tid;
}

// --- Getters ---
G4int MyHit::GetID() const {
  return fDetectorID;
}

G4int MyHit::GetLayer() const {
  return fLayerID;
}

G4double MyHit::GetEdep() const {
  return fEdep;
}

G4double MyHit::GetTime() const {
  return fTime;
}

G4ThreeVector MyHit::GetPosition() const {
  return fPosition;
}

G4ThreeVector MyHit::GetMomentum() const {
  return fMomentum;
}

G4double MyHit::GetWavelength() const {
  return fWavelength;
}

G4ThreeVector MyHit::GetDetectorPosition() const {
  return fDetectorPosition;
}

G4int MyHit::GetTrackID() const {
  return fTrackID;
}

