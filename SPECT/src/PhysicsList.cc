/// \file PhysicsList.cc
/// \brief Implementation of the PhysicsList class

#include "PhysicsList.hh"
#include "G4UnitsTable.hh"
#include "G4ParticleTypes.hh"
#include "G4SystemOfUnits.hh"

#include "globals.hh"

namespace SPECT
{
PhysicsList::PhysicsList():G4VModularPhysicsList()
{
    RegisterPhysics (new G4EmStandardPhysics());

}
}

