/// \file SPECT/include/PrimaryGeneratorAction.hh
/// \brief Definition of the SPECT::PrimaryGeneratorAction class

#ifndef PRIMARY_GENERATOR_ACTION_HH
#define PRIMARY_GENERATOR_ACTION_HH 1


#include "G4VUserPrimaryGeneratorAction.hh"
#include "DetectorConstruction.hh"
#include "G4ParticleGun.hh"
#include "globals.hh"
#include "G4Box.hh"

namespace SPECT
{

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
    public:
        PrimaryGeneratorAction();
        ~PrimaryGeneratorAction() override;

        // method for the base class
        void GeneratePrimaries(G4Event*) override;

        // method to access particle gun
        const G4ParticleGun* GetParticleGun() const {return fParticleGun;}

    private:
        G4ParticleGun* fParticleGun = nullptr;
        G4Box* fPhantom = nullptr;

};

}


#endif