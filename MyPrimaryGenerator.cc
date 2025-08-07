#include "MyPrimaryGenerator.hh"
#include "G4ParticleTable.hh"
#include "G4SystemOfUnits.hh"

MyPrimaryGenerator::MyPrimaryGenerator() : currentIndex(0) {
    fParticleGun = new G4ParticleGun(1);

    auto particleTable = G4ParticleTable::GetParticleTable();
    auto particle = particleTable->FindParticle("mu+");

    fParticleGun->SetParticleDefinition(particle);
    fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0., 0., 1.));
    fParticleGun->SetParticleEnergy(5. * GeV);  /

   
    double xsiz = 2.2 * cm;
    double ysiz = 2.2 * cm;
    double zpos = -140.0 * cm;
    double xstart = -0.5 * xsiz * (7 - 1);
    double ystart = -0.5 * ysiz * (7 - 1);

    for (int i = 0; i < 7; ++i) {
        for (int j = 0; j < 7; ++j) {
            double xpos = xstart + i * xsiz;
            double ypos = ystart + j * ysiz;
            positions.emplace_back(xpos, ypos, zpos);
        }
    }
}

MyPrimaryGenerator::~MyPrimaryGenerator() {
    delete fParticleGun;
}

void MyPrimaryGenerator::GeneratePrimaries(G4Event* event) {
    
    if (currentIndex >= positions.size()) currentIndex = 0;

    fParticleGun->SetParticlePosition(positions[currentIndex]);
    fParticleGun->GeneratePrimaryVertex(event);
    ++currentIndex;
}

