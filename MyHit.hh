#ifndef MYHIT_HH
#define MYHIT_HH

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4ThreeVector.hh"
#include "G4Allocator.hh"

class MyHit : public G4VHit {
public:
  // Updated constructor to include layer
  MyHit(G4int id = -1, G4int layer = -1, G4double edep = 0.0);
  virtual ~MyHit();
  MyHit(const MyHit&);
  const MyHit& operator=(const MyHit&);
  G4bool operator==(const MyHit&) const;

  inline void* operator new(size_t);
  inline void operator delete(void*);

  // --- Setters ---
  void SetID(G4int id);
  void SetLayer(G4int layer);
  void AddEdep(G4double edep);
  void SetTime(G4double t);
  void SetPosition(const G4ThreeVector& pos);
  void SetMomentum(const G4ThreeVector& mom);
  void SetWavelength(G4double wl);
  void SetDetectorPosition(const G4ThreeVector& pos);
  void SetTrackID(G4int);

  // --- Getters ---
  G4int GetID() const;
  G4int GetLayer() const;
  G4double GetEdep() const;
  G4double GetTime() const;
  G4ThreeVector GetPosition() const;
  G4ThreeVector GetMomentum() const;
  G4double GetWavelength() const;
  G4ThreeVector GetDetectorPosition() const;
  G4int GetTrackID() const;

private:
  G4int fDetectorID;                   
  G4int fLayerID;                      // NEW: for 3D layered detector
  G4double fEdep;                 
  G4double fTime;                 
  G4ThreeVector fPosition;         
  G4ThreeVector fMomentum;        
  G4double fWavelength;            
  G4ThreeVector fDetectorPosition; 
  G4int fTrackID;
};

// --- Memory allocator ---
extern G4ThreadLocal G4Allocator<MyHit>* MyHitAllocator;

inline void* MyHit::operator new(size_t) {
  if (!MyHitAllocator) MyHitAllocator = new G4Allocator<MyHit>;
  return (void*)MyHitAllocator->MallocSingle();
}

inline void MyHit::operator delete(void* hit) {
  MyHitAllocator->FreeSingle((MyHit*)hit);
}

// --- Hit Collection Typedef ---
typedef G4THitsCollection<MyHit> MyHitCollection;

#endif

