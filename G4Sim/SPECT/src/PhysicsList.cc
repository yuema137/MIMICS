#include "PhysicsList.hh"
#include "G4PhysicsListHelper.hh"
#include "G4UnitsTable.hh"
#include "G4ParticleTypes.hh"
#include "G4SystemOfUnits.hh"
#include "globals.hh"
#include "G4EmStandardPhysics.hh"
#include "G4PhysListUtil.hh"


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

// Decay
#include "G4UAtomicDeexcitation.hh"
#include "G4DecayPhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"
#include "G4Radioactivation.hh"
#include "G4IonPhysics.hh"
#include "G4IonTable.hh"
#include "G4GenericIon.hh"
#include "G4Decay.hh"
#include "G4NuclideTable.hh"
#include "G4NuclearLevelData.hh"

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

    // instantiate Physics List infrastructure 
    //
    G4PhysListUtil::InitialiseParameters();
    
    // update G4NuclideTable time limit
    //
    const G4double meanLife = 1*picosecond;  
    G4NuclideTable::GetInstance()->SetMeanLifeThreshold(meanLife);  
    G4NuclideTable::GetInstance()->SetLevelTolerance(1.0*eV);

    // define flags for the atomic de-excitation module
    //
    G4EmParameters::Instance()->SetDefaults();
    G4EmParameters::Instance()->SetAugerCascade(true);
    G4EmParameters::Instance()->SetDeexcitationIgnoreCut(true);    

    // define flags for nuclear gamma de-excitation model
    //
    G4DeexPrecoParameters* deex = G4NuclearLevelData::GetInstance()->GetParameters();
    deex->SetCorrelatedGamma(false);
    deex->SetStoreAllLevels(true);
    deex->SetInternalConversionFlag(true);	  
    deex->SetIsomerProduction(true);  
    deex->SetMaxLifeTime(meanLife);

    // set default cut in range value
    //
    SetDefaultCutValue(0.1*mm);
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

    // Add radioactive decay physics
    G4RadioactiveDecayPhysics* radDecayPhysics = new G4RadioactiveDecayPhysics();
    radDecayPhysics->ConstructProcess();

    // Add general decay process
    G4Decay* decay = new G4Decay();
    auto particleIterator = GetParticleIterator();
    particleIterator->reset();
    while ((*particleIterator)()) {
        G4ParticleDefinition* particle = particleIterator->value();
        if (decay->IsApplicable(*particle)) {
            ph->RegisterProcess(decay, particle);
        }
    }

    // Ensure radioactive decay is registered for ions
    ph->RegisterProcess(new G4RadioactiveDecay(), G4GenericIon::GenericIon());
    
}

void PhysicsList::SetCuts()
{
    SetCutsWithDefault();
    
    SetCutValue(0.1*mm, "gamma");
    SetCutValue(0.1*mm, "e-");
    SetCutValue(0.1*mm, "e+");
}
}