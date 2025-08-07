#include "MyDetectorConstruction.hh"
#include "MySensitiveDetector.hh"
#include "G4SDManager.hh"

MyDetectorConstruction::MyDetectorConstruction() {
  nCols = 7;
  nRows = 7;
  nLayers = 24;

  DefineMaterial();

  xWorld = 2.0 * m;
  yWorld = 2.05 * m;
  zWorld = 4.0 * m;
}

MyDetectorConstruction::~MyDetectorConstruction() {}

void MyDetectorConstruction::DefineMaterial() {
  G4NistManager* nist = G4NistManager::Instance();

  Ar = new G4Element("Argon", "Ar", 18., 39.948 * g / mole);
  O = new G4Element("Oxygen", "O", 8., 15.999 * g / mole);
  N = new G4Element("Nitrogen", "N", 7., 14.007 * g / mole);

  Aluminium = new G4Material("Al", 2.70 * g / cm3, 1);
  Aluminium->AddElement(nist->FindOrBuildElement("Al"), 1);

  Pb = new G4Element("Lead", "Pb", 82., 207.2 * g / mole);
  W = new G4Element("Tungsten", "W", 74., 183.84 * g / mole);

  Air = new G4Material("Air", 1.214 * mg / cm3, 3);
  Air->AddElement(N, 75 * perCent);
  Air->AddElement(O, 24 * perCent);
  Air->AddElement(Ar, 1 * perCent);

  PbWO4 = new G4Material("PbWO4", 8.300 * g / cm3, 3);
  PbWO4->AddElement(Pb, 1);
  PbWO4->AddElement(W, 2);
  PbWO4->AddElement(O, 2);
}

G4VPhysicalVolume* MyDetectorConstruction::Construct() {
  // World
  solidWorld = new G4Box("World", 0.5 * xWorld, 0.5 * yWorld, zWorld);
  logicWorld = new G4LogicalVolume(solidWorld, Air, "World");
  physWorld = new G4PVPlacement(0, G4ThreeVector(), logicWorld, "World", nullptr, false, 0, true);

  // Aluminium Box
  G4VSolid* solidAlBox = new G4Box("AluminiumBox", 25.2 * cm, 15.2 * cm, 60.95 * cm);
  G4LogicalVolume* logicAlBox = new G4LogicalVolume(solidAlBox, Aluminium, "AluminiumBox");
  new G4PVPlacement(0, G4ThreeVector(), logicAlBox, "AluminiumBox", logicWorld, false, 0, true);

  // Inside Box
  G4VSolid* solidInside = new G4Box("InsideBox", 25.0 * cm, 15.0 * cm, 60.75 * cm);
  G4LogicalVolume* logicInside = new G4LogicalVolume(solidInside, Air, "InsideBox");
  new G4PVPlacement(0, G4ThreeVector(), logicInside, "InsideBox", logicAlBox, false, 0, true);

  // Radiator
  G4VSolid* solidRadiator = new G4Box("Radiator", 5 * cm, 5 * cm, 1 * cm);
  G4LogicalVolume* logicRadiator = new G4LogicalVolume(solidRadiator, PbWO4, "Radiator");
  physRadiator = new G4PVPlacement(0, G4ThreeVector(0., 0., -50.0 * cm),
                                   logicRadiator, "physRadiator", logicWorld, false, 0, true);

  // Detector cells
  G4double xsize = 2.2 * cm;
  G4double ysize = 2.2 * cm;
  G4double zsize = 1.0 * cm;

  G4VSolid* solidDetector = new G4Box("Detector", 0.5 * xsize, 0.5 * ysize, 0.5 * zsize);
  logicDetector = new G4LogicalVolume(solidDetector, PbWO4, "Detector");

  
  for (G4int layer = 0; layer < nLayers; ++layer) {
    G4double zpos = -0.5 * nLayers * zsize + (layer + 0.5) * zsize;

    for (G4int col = 0; col < nCols; ++col) {
      G4double ypos = -0.5 * ysize * (nCols - 1) + col * ysize;

      for (G4int row = 0; row < nRows; ++row) {
        G4double xpos = -0.5 * xsize * (nRows - 1) + row * xsize;

        G4int copyNo = layer * 10000 + col * 100 + row;
        new G4PVPlacement(0, G4ThreeVector(xpos, ypos, zpos),
                          logicDetector, "Detector", logicInside,
                          false, copyNo, true);
      }
    }
  }

  fScoringVolume = logicDetector;
  return physWorld;
}

void MyDetectorConstruction::ConstructSDandField() {
  G4SDManager* sdManager = G4SDManager::GetSDMpointer();
  auto* sd = new MySensitiveDetector("SensitiveDetector");
  sdManager->AddNewDetector(sd);

  if (logicDetector) {
    logicDetector->SetSensitiveDetector(sd);
    G4cout << "[SD] Sensitive detector successfully attached to logicDetector" << G4endl;
  } else {
    G4cerr << "[SD] ERROR: logicDetector is NULL!" << G4endl;
  }
}

