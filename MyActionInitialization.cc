#include "MyActionInitialization.hh"
#include "MyPrimaryGenerator.hh"
//#include "MyRunAction.hh"
#include "MyEventAction.hh"
#include "MySteppingAction.hh"
#include "G4AnalysisManager.hh"
#include "G4Run.hh"
#include "G4RunManager.hh"

 MyActionInitialization:: MyActionInitialization()
{}

 MyActionInitialization::~MyActionInitialization()
{}

void MyActionInitialization::BuildForMaster() const
{
    //MyRunAction *runAction = new MyRunAction();
    //SetUserAction(runAction);
}


void  MyActionInitialization::Build() const
{
     MyPrimaryGenerator *generator = new MyPrimaryGenerator();
    SetUserAction(generator);

    //MyRunAction *runAction = new MyRunAction();
    //SetUserAction(runAction);

    MyEventAction *eventAction = new MyEventAction();
    SetUserAction(eventAction);

    MySteppingAction *steppingAction = new MySteppingAction(eventAction);
    SetUserAction(steppingAction);
}

