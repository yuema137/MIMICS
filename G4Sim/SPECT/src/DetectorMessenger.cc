// DetectorMessenger.cc
#include "DetectorMessenger.hh"
#include "DetectorConstruction.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithABool.hh"

namespace SPECT {

DetectorMessenger::DetectorMessenger(DetectorConstruction* Det)
 : G4UImessenger(),
   fDetectorConstruction(Det),
   fDirectory(nullptr),
   fCollimatorCmd(nullptr)
{
  fDirectory = new G4UIdirectory("/mydetector/");
  fDirectory->SetGuidance("Detector construction control");

  fCollimatorCmd = new G4UIcmdWithABool("/mydetector/includeCollimator", this);
  fCollimatorCmd->SetGuidance("Include collimator in the setup");
  fCollimatorCmd->SetParameterName("Include", false);
  fCollimatorCmd->SetDefaultValue(true);
}

DetectorMessenger::~DetectorMessenger()
{
  delete fCollimatorCmd;
  delete fDirectory;
}

void DetectorMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
  if(command == fCollimatorCmd)
  {
    fDetectorConstruction->SetIncludeCollimator(fCollimatorCmd->GetNewBoolValue(newValue));
  }
}

}  // namespace SPECT