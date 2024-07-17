#ifndef RUNACTION_HH
#define RUNACTION_HH

#include "G4UserRunAction.hh"
#include "G4Run.hh"
#include "G4AnalysisManager.hh"
#include "g4root_defs.hh"


namespace SPECT
{

class RunAction : public G4UserRunAction
{
    public:
        RunAction();
        ~RunAction();

        virtual void BeginOfRunAction(const G4Run*);
        virtual void EndOfRunAction(const G4Run*);
};

}


#endif
