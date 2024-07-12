#ifndef PHYSICS_HH
#define PHYSICS_HH 1

#include "G4VUserPhysicsList.hh"
#include "G4VModularPhysicsList.hh"
#include "G4EmStandardPhysics.hh"
#include "G4OpticalPhysics.hh"

namespace SPECT{
class PhysicsList: public G4VModularPhysicsList
{
  public:
    PhysicsList();
   ~PhysicsList() override = default; // set as default, so no need to specify agin in .cc

};
}
#endif