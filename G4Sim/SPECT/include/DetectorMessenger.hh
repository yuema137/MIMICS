// DetectorMessenger.hh
#ifndef DETECTORMESSENGER_HH
#define DETECTORMESSENGER_HH 1

#include "G4UImessenger.hh"
#include "globals.hh"

class G4UIdirectory;
class G4UIcmdWithABool;

namespace SPECT {

class DetectorConstruction;

class DetectorMessenger: public G4UImessenger
{
  public:
    DetectorMessenger(DetectorConstruction* );
    virtual ~DetectorMessenger();
 
    virtual void SetNewValue(G4UIcommand*, G4String);
 
  private:
    DetectorConstruction*      fDetectorConstruction;
    G4UIdirectory*             fDirectory;
    G4UIcmdWithABool*          fCollimatorCmd;
};

}  // namespace SPECT

#endif