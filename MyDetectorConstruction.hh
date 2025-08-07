#ifndef CONSTRUCTION_HH
#define CONSTRUCTION_HH

#include "G4SystemOfUnits.hh"
#include "G4VUserDetectorConstruction.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4PVPlacement.hh"
#include "G4NistManager.hh"
#include "MySensitiveDetector.hh"
#include "G4GenericMessenger.hh"
#include "G4Tubs.hh"

class MyDetectorConstruction : public G4VUserDetectorConstruction {
public:
  MyDetectorConstruction();
  ~MyDetectorConstruction();
    
   G4LogicalVolume* GetScoringVolume() const { return fScoringVolume; } 

  virtual G4VPhysicalVolume *Construct();
  virtual void ConstructSDandField();

private:
  G4LogicalVolume *logicDetector;
    
  G4int nCols, nRows, nLayers;
    
  void DefineMaterial();

  G4Box *solidWorld,  *solidDetector, *solidRadiator;
  G4LogicalVolume   *logicWorld, *logicRadiator;
  G4VPhysicalVolume   *physWorld, *physRadiator;

  

  G4Material *Air, *PbWO4, *Aluminium;
  G4Element *Ar, *O, *N, *Pb, *W;

  G4double xWorld, yWorld, zWorld;
    G4GenericMessenger *fMessenger;

protected:
    G4LogicalVolume* fScoringVolume = nullptr;
}; 
#endif
