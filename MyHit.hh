#ifndef MYHIT_HH
#define MYHIT_HH

#include "G4VHit.hh"
#include "G4ThreeVector.hh"
#include "G4THitsCollection.hh" // <-- needed for MyHitCollection typedef
#include <map>
#include <vector>
#include <utility>

/*
  MyHit holds aggregated information for one detector cell for a single event.
  It stores:
   - fEdep: total energy deposited in the cell (summed over tracks)
   - fTrackEdepMap: per-track breakdown of deposited energy (trackID -> edep)
   - fTrackInitKE: optional map of track initial kinetic energies (trackID -> initKE)
  The header also provides a convenient typedef `MyHitCollection` so other parts of
  the code (EventAction, SensitiveDetector, etc.) can use that name directly.
*/

// forward declare to make typedef below valid
class MyHit;

// typedef so you can write `MyHitCollection*` in other files
using MyHitCollection = G4THitsCollection<MyHit>;

class MyHit : public G4VHit {
public:
  // ctor: id = detector identifier, layer = optional layer id, edep = initial total energy
  MyHit(G4int id = -1, G4int layer = -1, G4double edep = 0.0);
  MyHit(const MyHit& right);
  const MyHit& operator=(const MyHit& right);
  virtual ~MyHit();

  // equality used by some containers; not marked override because G4VHit doesn't declare it
  G4bool operator==(const MyHit& right) const;

  // --- Setters ---
  void SetID(G4int id);
  void SetLayer(G4int layer);

  // Record that 'trackID' deposited 'edep' into this cell.
  // If 'initKE' >= 0 it will be recorded in the per-track init energy map.
  void AddTrackEdep(G4int trackID, G4double edep, G4double initKE = -1.0);

  // increment total edep (keeps backward compatibility)
  void AddEdep(G4double edep);

  void SetTime(G4double time);
  void SetPosition(const G4ThreeVector& pos);
  void SetMomentum(const G4ThreeVector& mom);
  void SetWavelength(G4double wlen);
  void SetDetectorPosition(const G4ThreeVector& detPos);
  void SetTrackID(G4int tid);

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

  // Per-track maps and helpers
  const std::map<G4int, G4double>& GetTrackEdepMap() const;
  const std::map<G4int, G4double>& GetTrackInitKEMap() const;

  size_t GetTrackCount() const;

  // Return top N (trackID, edep) pairs sorted by edep (descending)
  std::vector<std::pair<G4int, G4double>> GetTopNTracks(size_t N) const;

  // Return the single (trackID, edep) pair that contributed most energy
  std::pair<G4int, G4double> GetMaxContributor() const;

private:
  G4int fDetectorID;
  G4int fLayerID;
  G4double fEdep; // total energy deposited in this cell (sum over tracks)
  G4double fTime;
  G4ThreeVector fPosition;
  G4ThreeVector fMomentum;
  G4double fWavelength;
  G4ThreeVector fDetectorPosition;
  G4int fTrackID;

  // per-track information
  std::map<G4int, G4double> fTrackEdepMap; // trackID -> energy deposited here
  std::map<G4int, G4double> fTrackInitKE;  // trackID -> initial kinetic energy (if provided)
};

#endif // MYHIT_HH
