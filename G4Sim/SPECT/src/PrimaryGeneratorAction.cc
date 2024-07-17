/// \file SPECT/src/PrimaryGeneratorAction.cc
/// \brief Implementation of the SPECT::PrimaryGeneratorAction class

#include "PrimaryGeneratorAction.hh"

#include "G4Box.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"
#include "G4LogicalVolume.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"


namespace SPECT
{

PrimaryGeneratorAction::PrimaryGeneratorAction()
{
    G4int n_particle = 1;
    fParticleGun = new G4ParticleGun(n_particle);

    // Default particle kinematic
    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
    G4String particleName = "gamma";
    G4ParticleDefinition* particle = particleTable->FindParticle(particleName);
    fParticleGun->SetParticleDefinition(particle);
    fParticleGun->SetParticleEnergy(300*keV);

}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
    delete fParticleGun;
    delete fPhantom;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
    G4double PhantomSizeX = 0;
    G4double PhantomSizeY = 0;
    G4double PhantomSizeZ = 0;

    if ( !fPhantom )
    {
        G4LogicalVolume* phantomLV
            = G4LogicalVolumeStore::GetInstance()->GetVolume("Phantom1");
        if (phantomLV) fPhantom = dynamic_cast<G4Box*>(phantomLV->GetSolid());
    }
    if ( fPhantom )
    {
        PhantomSizeX = fPhantom->GetXHalfLength()*2.;
        PhantomSizeY = fPhantom->GetYHalfLength()*2.;
        PhantomSizeZ = fPhantom->GetZHalfLength()*2.;
    }
      else  
    {
        G4ExceptionDescription msg;
        msg << "Phantom volume of box shape not found.\n";
        msg << "Perhaps you have changed geometry.\n";
        msg << "The gun will be place at the center.";
        G4Exception("PrimaryGeneratorAction::GeneratePrimaries()",
        "MyCode0002",JustWarning,msg);
    }

    // Set the source position
    G4double x0 = PhantomSizeX * (G4UniformRand()-0.5);
    G4double y0 = PhantomSizeY * (G4UniformRand()-0.5);
    G4double z0 = PhantomSizeZ * (G4UniformRand()-0.5);
    fParticleGun->SetParticlePosition(G4ThreeVector(x0,y0,z0));

    // Set the angle
    G4double maxTheta = std::atan(10/20.0); // Your specified angle
    G4double phi = 2 * M_PI * G4UniformRand(); // Random azimuthal angle
    G4double theta = std::acos(1 - G4UniformRand() * (1 - std::cos(maxTheta))); // Random polar angle

    G4double dx = std::sin(theta) * std::cos(phi);
    G4double dy = std::sin(theta) * std::sin(phi);
    G4double dz = -std::cos(theta); // Negative because we want it to point down

    fParticleGun->SetParticleMomentumDirection(G4ThreeVector(dx, dy, dz));

    fParticleGun->GeneratePrimaryVertex(anEvent);
}
   


}