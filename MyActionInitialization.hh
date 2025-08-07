#ifndef ACTION_HH
#define ACTION_HH

#include "G4VUserActionInitialization.hh"

//#include "MyPrimaryGenerator.hh"
//#include "MyRunAction.hh"
//#include "globals.hh"
//#include "MyEventAction.hh"
//#include "MySteppingAction.hh"

class MyActionInitialization : public G4VUserActionInitialization
{
public:
    MyActionInitialization();
    ~MyActionInitialization();

    virtual void Build() const;
    virtual void BuildForMaster() const;
};

#endif 
