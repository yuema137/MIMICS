#include "PhysicsList.hh"
#include "G4PhysicsListHelper.hh"
#include "G4UnitsTable.hh"
#include "G4ParticleTypes.hh"
#include "G4SystemOfUnits.hh"
#include "globals.hh"
#include "G4EmStandardPhysics.hh"
#include "G4DecayPhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"
#include "G4IonPhysics.hh"

// Gamma Process
#include "G4PhotoElectricEffect.hh"
#include "G4ComptonScattering.hh"
#include "G4RayleighScattering.hh"
#include "G4GammaConversion.hh"

// e+- Process
#include "G4eIonisation.hh"
#include "G4eBremsstrahlung.hh"
#include "G4eplusAnnihilation.hh"
#include "G4eMultipleScattering.hh"

namespace SPECT
{
PhysicsList::PhysicsList() : G4VModularPhysicsList()
{
    SetVerboseLevel(1);

    // Remove G4EmStandardPhysics
    // RegisterPhysics(new G4EmStandardPhysics());
    
    RegisterPhysics(new G4DecayPhysics());
    RegisterPhysics(new G4RadioactiveDecayPhysics());
    RegisterPhysics(new G4IonPhysics());
}

void PhysicsList::ConstructParticle()
{
    G4VModularPhysicsList::ConstructParticle();
}

void PhysicsList::ConstructProcess()
{
    G4VModularPhysicsList::ConstructProcess();

    // Add custom EM physics
    G4PhysicsListHelper* ph = G4PhysicsListHelper::GetPhysicsListHelper();

    // Gamma
    G4ParticleDefinition* particle = G4Gamma::GammaDefinition();
    ph->RegisterProcess(new G4PhotoElectricEffect(), particle);
    ph->RegisterProcess(new G4ComptonScattering(),   particle);
    ph->RegisterProcess(new G4RayleighScattering(), particle);
    ph->RegisterProcess(new G4GammaConversion(), particle);

    // Electron
    particle = G4Electron::ElectronDefinition();
    ph->RegisterProcess(new G4eIonisation(),       particle);
    ph->RegisterProcess(new G4eBremsstrahlung(),   particle);
    ph->RegisterProcess(new G4eMultipleScattering(), G4Electron::ElectronDefinition());

    // Positron
    particle = G4Positron::PositronDefinition();
    ph->RegisterProcess(new G4eIonisation(),       particle);
    ph->RegisterProcess(new G4eBremsstrahlung(),   particle);
    ph->RegisterProcess(new G4eplusAnnihilation(), particle);
    ph->RegisterProcess(new G4eMultipleScattering(), G4Electron::ElectronDefinition());
}

void PhysicsList::SetCuts()
{
    SetCutsWithDefault();
    
    SetCutValue(0.1*mm, "gamma");
    SetCutValue(0.1*mm, "e-");
    SetCutValue(0.1*mm, "e+");
}
}