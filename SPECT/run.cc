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


using namespace SPECT;

int main(int argc, char** argv)
{
    G4RunManager *runManager = new G4RunManager();
    DetectorConstruction* detConstruction = new DetectorConstruction();
    runManager->SetUserInitialization(detConstruction);

    runManager->SetUserInitialization(new PhysicsList());
    std::cout << "Physicslist Done" << std::endl;

    runManager->SetUserInitialization(new ActionInitialization());
    std::cout << "ActionInitialization Done" << std::endl;

    runManager->Initialize();
    std::cout << "Initialization Done" << std::endl;

    G4UIExecutive* ui = nullptr;
    if ( argc == 1 ) { ui = new G4UIExecutive(argc, argv); }
    G4VisManager *visManager = new G4VisExecutive();
    visManager->Initialize();

    auto UImanager = G4UImanager::GetUIpointer();
    if ( ! ui ) {
        // batch mode
        G4String command = "/control/execute ";
        G4String fileName = argv[1];
        UImanager->ApplyCommand(command+fileName);
    }
    else {
        // interactive mode
        UImanager->ApplyCommand("/control/execute init_vis.mac");
        ui->SessionStart();
        delete ui;
    }

    delete visManager;
    delete ui;
    delete runManager;
}