#include "MySteppingAction.hh"
#include "G4Step.hh"
#include "G4RunManager.hh"
#include "MyEventAction.hh"
#include "MyDetectorConstruction.hh"
#include "MyHit.hh"
MySteppingAction::MySteppingAction(MyEventAction *eventAction)
{
    fEventAction = eventAction;
}

MySteppingAction::~MySteppingAction()
{}

void MySteppingAction::UserSteppingAction(const G4Step *step)
{
    G4LogicalVolume *volume = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume();
    if (!fScoringVolume) return;

    
    G4double edep = step->GetTotalEnergyDeposit();
    //fEventAction->AddEdep(edep);
    
}
