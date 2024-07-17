#include "TargetSD.hh"
#include "G4RunManager.hh"
#include "G4AnalysisManager.hh"

namespace SPECT
{

TargetSD::TargetSD(const G4String& name) : G4VSensitiveDetector(name)
{}

TargetSD::~TargetSD()
{}

void TargetSD::Initialize(G4HCofThisEvent*)
{

}

G4bool TargetSD::ProcessHits(G4Step* step, G4TouchableHistory*)
{
    // Get the track associated with this step
    G4Track* track = step->GetTrack();

    // Get the pre-step and post-step points
    G4StepPoint* preStepPoint = step->GetPreStepPoint();
    G4StepPoint* postStepPoint = step->GetPostStepPoint();

    // Get positions at the beginning and end of the step
    G4ThreeVector preStepPosition = preStepPoint->GetPosition();
    G4ThreeVector postStepPosition = postStepPoint->GetPosition();

    // Get the process name
    G4String processName = postStepPoint->GetProcessDefinedStep()->GetProcessName();
    
    // Get the energy deposit
    G4double energyDeposit = step->GetTotalEnergyDeposit();

    // Get the analysis manager
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    
    // Get the current event ID
    G4int eventID = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();

    // Get particle information
    G4String particleName = track->GetParticleDefinition()->GetParticleName();
    G4int particlePDG = track->GetDefinition()->GetPDGEncoding();
    G4String parentParticleName = track->GetParentID() == 0 ? "None" : 
        track->GetDynamicParticle()->GetParticleDefinition()->GetParticleName();

    // Get track information
    G4int trackID = track->GetTrackID();
    G4int parentTrackID = track->GetParentID();

    // Get kinetic energies
    G4double preStepKineticEnergy = preStepPoint->GetKineticEnergy();
    G4double postStepKineticEnergy = postStepPoint->GetKineticEnergy();

    // Fill the ntuple
    analysisManager->FillNtupleIColumn(0, eventID);
    analysisManager->FillNtupleDColumn(1, preStepPosition.x());
    analysisManager->FillNtupleDColumn(2, preStepPosition.y());
    analysisManager->FillNtupleDColumn(3, preStepPosition.z());
    analysisManager->FillNtupleDColumn(4, postStepPosition.x());
    analysisManager->FillNtupleDColumn(5, postStepPosition.y());
    analysisManager->FillNtupleDColumn(6, postStepPosition.z());
    analysisManager->FillNtupleDColumn(7, energyDeposit);
    analysisManager->FillNtupleSColumn(8, processName);
    analysisManager->FillNtupleSColumn(9, particleName);
    analysisManager->FillNtupleSColumn(10, parentParticleName);
    analysisManager->FillNtupleIColumn(11, trackID);
    analysisManager->FillNtupleIColumn(12, parentTrackID);
    analysisManager->FillNtupleIColumn(13, particlePDG);
    analysisManager->FillNtupleDColumn(14, preStepKineticEnergy);
    analysisManager->FillNtupleDColumn(15, postStepKineticEnergy);
    analysisManager->AddNtupleRow();

    return true;
}

void TargetSD::EndOfEvent(G4HCofThisEvent*)
{}

}