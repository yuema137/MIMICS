/// \file SPECT/include/TargetSD.hh
/// \brief Definition of the SPECT::TargetSD class

#ifndef TARGETSD_HH
#define TARGETSD_HH 1


#include "G4VSensitiveDetector.hh"
#include "globals.hh"

namespace SPECT
{

class TargetSD : public G4VSensitiveDetector
{
    public:
        TargetSD(const G4String&);
        virtual ~TargetSD();

        void Initialize(G4HCofThisEvent*);
        virtual G4bool ProcessHits(G4Step*, G4TouchableHistory*);
        void EndOfEvent(G4HCofThisEvent*);

};

}



#endif