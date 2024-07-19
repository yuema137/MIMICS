/// \file run.cc
/// \brief Main program of the SPECT project

#include "DetectorConstruction.hh"
#include "ActionInitialization.hh"
#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"
#include "PhysicsList.hh"
#include <iostream>
#include <string>

using namespace SPECT;

int main(int argc, char** argv)
{
    G4RunManager *runManager = new G4RunManager();
    DetectorConstruction* detConstruction = new DetectorConstruction();
    runManager->SetUserInitialization(detConstruction);

    runManager->SetUserInitialization(new PhysicsList());
    std::cout << "PhysicsList Done" << std::endl;

    runManager->SetUserInitialization(new ActionInitialization());
    std::cout << "ActionInitialization Done" << std::endl;

    G4UIExecutive* ui = nullptr;
    G4VisManager* visManager = nullptr;

    auto UImanager = G4UImanager::GetUIpointer();

    // Check if --vis argument is provided
    bool visMode = false;
    if (argc > 1 && std::string(argv[1]) == "--vis") {
        visMode = true;
    }

    if (visMode) {
        // Visualization mode
        ui = new G4UIExecutive(argc, argv);
        visManager = new G4VisExecutive();
        visManager->Initialize();

        // Execute init_vis.mac before initializing the run manager
        UImanager->ApplyCommand("/control/execute ./macros/init_vis.mac");
        
        // Initialize the run manager after executing init_vis.mac
        runManager->Initialize();
        std::cout << "Initialization Done" << std::endl;

        ui->SessionStart();
    } else {
        // Batch mode
        if (argc < 2) {
            std::cerr << "Error: No macro file provided for batch mode." << std::endl;
            std::cerr << "Usage: ./run <macro_file> or ./run --vis" << std::endl;
            return 1;
        }
        
        // Execute the provided macro file
        G4String command = "/control/execute ";
        G4String fileName = argv[1];
        UImanager->ApplyCommand(command+fileName);
        
        // Initialize the run manager after executing the macro
        runManager->Initialize();
        std::cout << "Initialization Done" << std::endl;
    }

    delete visManager;
    delete ui;
    delete runManager;

    return 0;
}