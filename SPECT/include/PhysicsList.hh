/// \file SPECT/include/PhysicsList.hh
/// \brief Definition of the SPECT::PhysicsList class

#ifndef PHYSICS_HH
#define PHYSICS_HH 1

#include "G4VModularPhysicsList.hh"
#include "G4EmStandardPhysics_option4.hh"
#include "G4DecayPhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"

namespace SPECT
{

class PhysicsList : public G4VModularPhysicsList
{
public:
    PhysicsList();
    virtual ~PhysicsList() = default;

    virtual void SetCuts() override;

protected:
    virtual void ConstructParticle() override;
    virtual void ConstructProcess() override;
};

}

#endif