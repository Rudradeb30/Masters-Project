#ifndef MYPRIMARYGENERATOR_HH
#define MYPRIMARYGENERATOR_HH


#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ThreeVector.hh"
#include "G4ParticleGun.hh"

#include <vector>

class G4ParticleGun;
class G4ParticleDefinition;

class MyPrimaryGenerator : public G4VUserPrimaryGeneratorAction {
public:
    MyPrimaryGenerator();
    ~MyPrimaryGenerator();

    virtual void GeneratePrimaries(G4Event*);

private:
    G4ParticleGun* fParticleGun;                      // The particle gun
    std::vector<G4ThreeVector> positions;             // 49 generator positions
    size_t currentIndex;                              // Tracks which generator to use
    G4ParticleDefinition* particle = nullptr;         // Particle type (muon)
};

#endif



