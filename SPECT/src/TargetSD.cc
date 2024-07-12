#include "TargetSD.hh"
#include "G4RunManager.hh"

namespace SPECT
{

TargetSD::TargetSD(const G4String& name) : G4VSensitiveDetector(name)
{}

TargetSD::~TargetSD()
{}

void TargetSD::Initialize(G4HCofThisEvent*)
{

}

G4bool TargetSD::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
    // Get the track associated with this step
    G4Track* track = aStep->GetTrack();

    // Get the pre-step point (beginning of the step)
    G4StepPoint* preStepPoint = aStep->GetPreStepPoint();

    // Get the post-step point (end of the step)
    G4StepPoint* postStepPoint = aStep->GetPostStepPoint();

    // Get the position of the particle at the beginning of the step
    G4ThreeVector posPhoton = preStepPoint->GetPosition();

    // Print the position of the photon
    G4cout << "Photon position: " << posPhoton << G4endl;

    // Return true to indicate that the hit was processed successfully
    return true;
}

void TargetSD::EndOfEvent(G4HCofThisEvent*)
{}

}