#ifndef MYEVENTACTION_HH
#define MYEVENTACTION_HH

#include "G4UserEventAction.hh"
#include "G4Event.hh"
#include "globals.hh"
#include "G4AnalysisManager.hh"
#include "G4RunManager.hh"
//#include "MyRunAction.hh"
#include "G4SDManager.hh"
#include "MyHit.hh"
#include <TFile.h>
#include <TTree.h>


class MyEventAction : public G4UserEventAction {
public:
    MyEventAction();
    virtual ~MyEventAction();

    virtual void BeginOfEventAction(const G4Event*);
    virtual void EndOfEventAction(const G4Event*);
    

private:
    G4int fTrackID;
    G4double fEdep;      
    G4int fEvent;         
    G4double posPhoton[3] = {0.0, 0.0, 0.0};  
    G4double wlen = 0.0;      
    G4double time = 0.0;       
    G4double posDetector[3] = {0.0, 0.0, 0.0}; 
    G4int fDetectorID;
    TTree *tree;
    TFile *outputFile;
    bool initialized = false;
   
};

#endif 

