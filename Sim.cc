#include <iostream>
#include <cstdlib>
#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4VisManager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"
#include "MySensitiveDetector.hh"
#include "MyDetectorConstruction.hh"
#include "MyPhysicsList.hh"
#include "MyPrimaryGenerator.hh"
#include "MyActionInitialization.hh"
#include "MyHit.hh"
#include "MyEventAction.hh"
#include "MySteppingAction.hh"

int main(int argc, char** argv)
{
     

     G4RunManager *runManager = new G4RunManager();
     
     MyDetectorConstruction* detector = new MyDetectorConstruction();
     G4cout << "Detector: " << detector << G4endl;
     runManager->SetUserInitialization(detector);
     runManager->SetUserInitialization(new MyPhysicsList());

     
     auto eventAction = new MyEventAction();

     
     runManager->SetUserAction(eventAction);

     runManager->SetUserInitialization(new MyActionInitialization());
     runManager->Initialize();
     detector->ConstructSDandField();
     G4UIExecutive *ui = 0;
     if(argc == 1)
     {
          ui = new G4UIExecutive(argc, argv);
     }    
     G4VisManager *visManager= new G4VisExecutive();
     visManager->Initialize();
     G4UImanager *UImanager = G4UImanager::GetUIpointer();
     if(ui)
     {UImanager->ApplyCommand("/control/execute run.mac");
     ui->SessionStart();}
     else
     {
         G4String command = "/control/execute ";
         G4String fileName = argv[1];
         UImanager->ApplyCommand(command+fileName);
     }
     delete runManager;
     return 0;
}
