#include "RunAction.hh"

namespace SPECT
{
RunAction::RunAction()
{   
    auto analysis_manager = G4AnalysisManager::Instance();
}

RunAction::~RunAction()
{}

void RunAction::BeginOfRunAction(const G4Run*)
{
    G4AnalysisManager *analysisManager= G4AnalysisManager::Instance();
    analysisManager->SetVerboseLevel(1);

    G4cout << "Analysis manager instance created" << G4endl;
    
    G4String fileName = "output.root";
    G4cout << "Attempting to open file: " << fileName << G4endl;
    
    // Try to open the file
    G4bool fileOpened = analysisManager->OpenFile(fileName);
    G4cout << "File opened successfully: " << (fileOpened ? "Yes" : "No") << G4endl;
    
    G4cout << "Analysis manager type: " << analysisManager->GetType() << G4endl;

    analysisManager->CreateNtuple("Hits", "Hits");
    analysisManager->CreateNtupleIColumn("eventID");           // 0
    analysisManager->CreateNtupleDColumn("preStepX");          // 1
    analysisManager->CreateNtupleDColumn("preStepY");          // 2
    analysisManager->CreateNtupleDColumn("preStepZ");          // 3
    analysisManager->CreateNtupleDColumn("postStepX");         // 4
    analysisManager->CreateNtupleDColumn("postStepY");         // 5
    analysisManager->CreateNtupleDColumn("postStepZ");         // 6
    analysisManager->CreateNtupleDColumn("energyDeposit");     // 7
    analysisManager->CreateNtupleSColumn("processName");       // 8
    analysisManager->CreateNtupleSColumn("particleName");      // 9
    analysisManager->CreateNtupleSColumn("parentParticleName");// 10
    analysisManager->CreateNtupleIColumn("trackID");           // 11
    analysisManager->CreateNtupleIColumn("parentTrackID");     // 12
    analysisManager->CreateNtupleIColumn("particlePDG");       // 13
    analysisManager->CreateNtupleDColumn("preStepKineticEnergy");  // 14
    analysisManager->CreateNtupleDColumn("postStepKineticEnergy"); // 15
    analysisManager->FinishNtuple();

}

void RunAction::EndOfRunAction(const G4Run*)
{
    G4AnalysisManager *analysis_manager = G4AnalysisManager::Instance();
    analysis_manager->Write();
    analysis_manager->CloseFile();
    G4cout << "Closed File" << G4endl;

}

}