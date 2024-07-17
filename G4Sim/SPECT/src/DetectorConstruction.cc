/// \file DetectorConstruction.cc
/// \brief Implementation of the DetectorConstruction class

#include "DetectorConstruction.hh"
#include "TargetSD.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4SystemOfUnits.hh"
#include "G4SubtractionSolid.hh"
#include "G4Tubs.hh"
#include "G4PVPlacement.hh"
#include "G4LogicalVolume.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"
#include "G4AssemblyVolume.hh"
#include "G4Transform3D.hh"
#include "G4SDManager.hh"

namespace SPECT
{

DetectorConstruction::DetectorConstruction()
{
    crystalLV = 0;
}

G4VPhysicalVolume* DetectorConstruction::Construct()
{
    // Get nist material manager
    G4NistManager* nist = G4NistManager::Instance();

    G4bool check_overlaps = true;

    // World
    G4Material* world_mat = nist->FindOrBuildMaterial("G4_AIR");
    G4double world_size = 0.5*m;
    auto solidWorld = new G4Box("World", 0.5 * world_size, 0.5 * world_size, 0.5 * world_size);
    
    auto logicWorld = new G4LogicalVolume(solidWorld, // solid
    world_mat, //material
    "World"); //name

    auto physicsWorld = new G4PVPlacement(nullptr, //no rotation
    G4ThreeVector(0,0,0), //position
    logicWorld, //logic volume
    "World", // name
    nullptr, //mother logic volume
    false, //no boolean operation
    0, //copy number
    check_overlaps);

    //  -------------------------------------------------------Phantom 1-------------------------------------------------------------
    G4Material* phantom_mat = nist->FindOrBuildMaterial("G4_WATER");

    G4double phantom1_size_x = 10 * mm;
    G4double phantom1_size_y = 10 * mm;
    G4double phantom1_size_z = 10 * mm;
    auto solid_panthom_1 = new G4Box("Panthom1",
    0.5 * phantom1_size_x, 0.5 * phantom1_size_y, 0.5 * phantom1_size_z
    );
    auto logic_phantom_1 = new G4LogicalVolume(solid_panthom_1,
    phantom_mat, "Phantom1");

    // Choose the Phantom
    const G4Box* fPhantom = solid_panthom_1;

    // Set the color to light blue
    G4VisAttributes* phantom_vis_att = new G4VisAttributes(G4Colour(51/255, 153/255, 255/255)); // RGB for blue
    phantom_vis_att->SetVisibility(true);
    phantom_vis_att->SetForceSolid(true); // This makes it appear as a solid blue volume instead of just wireframe
    logic_phantom_1->SetVisAttributes(phantom_vis_att);

    new G4PVPlacement(nullptr,
        G4ThreeVector(),
        logic_phantom_1,
        "Phantom1",
        logicWorld,
        false,
        0,
        check_overlaps);


    
    // -------------------------------------------------------Collimator-------------------------------------------------------------
    G4cout << "Starting collimator construction..." << G4endl;

    G4Material* lead = nist->FindOrBuildMaterial("G4_Pb");
    if (!lead) {
        G4cerr << "Failed to create lead material. Cannot proceed with collimator construction." << G4endl;
    }

    G4cout << "Successfully created lead material." << G4endl;
    G4cout << "Lead density: " << lead->GetDensity() / (g/cm3) << " g/cm3" << G4endl;

    // Define collimator parameters
    G4double collimator_x = 100.0*mm;
    G4double collimator_y = 100.0*mm;
    G4double collimator_z = 10.0*mm;
    G4double hole_radius = 1*mm;
    G4double hole_spacing = 2.0*mm; // Center-to-center gap between holes

    G4cout << "Collimator dimensions: " << collimator_x/mm << " x " << collimator_y/mm << " x " << collimator_z/mm << " mm" << G4endl;

    // Create the main collimator box
    G4Box* collimator_box = new G4Box("Collimator", collimator_x/2, collimator_y/2, collimator_z/2);
    G4LogicalVolume* collimator_logic = new G4LogicalVolume(collimator_box, lead, "Collimator_Logic");

    // Create a single hole
    G4Tubs* hole_solid = new G4Tubs("Hole", 0., hole_radius, collimator_z/2 + 1*nm, 0., 360.*deg);
    G4LogicalVolume* hole_logic = new G4LogicalVolume(hole_solid, world_mat, "Hole_Logic");

    // Calculate the number of holes
    int num_holes_x = static_cast<int>((collimator_x - hole_spacing) / hole_spacing) + 1;
    int num_holes_y = static_cast<int>((collimator_y - hole_spacing) / hole_spacing) + 1;

    G4cout << "Number of holes: " << num_holes_x << " x " << num_holes_y << G4endl;

    // Create an assembly for the collimator with holes
    G4AssemblyVolume* collimatorAssembly = new G4AssemblyVolume();

    // Add the main collimator to the assembly
    G4ThreeVector collimatorPos(0, 0, 0);
    G4RotationMatrix collimatorRot;
    collimatorAssembly->AddPlacedVolume(collimator_logic, collimatorPos, &collimatorRot);

    // Add holes to the assembly
    for (int i = 0; i < num_holes_x; i++) {
        for (int j = 0; j < num_holes_y; j++) {
            G4double x = (i - (num_holes_x-1)/2.0) * hole_spacing;
            G4double y = (j - (num_holes_y-1)/2.0) * hole_spacing;
            G4ThreeVector holePos(x, y, 0);
            G4RotationMatrix holeRot;
            collimatorAssembly->AddPlacedVolume(hole_logic, holePos, &holeRot);
        }
    }

    // Position the collimator assembly
    G4double col_phan_f2f = 20*cm;
    G4double phantom_thickness = 2 * fPhantom->GetZHalfLength(); // Assuming fPhantom is accessible and contains the phantom
    G4cout << "Phantom Thickness: " << phantom_thickness/mm << " mm" << G4endl;

    G4ThreeVector collimator_position(0, 0, -(col_phan_f2f + 0.5 * phantom_thickness + 0.5 * collimator_z));
    G4RotationMatrix collimatorAssemblyRot;

    // Place the assembly in the world volume
    collimatorAssembly->MakeImprint(logicWorld, collimator_position, &collimatorAssemblyRot);

        // Set the color to light blue
    G4VisAttributes* col_vis_att = new G4VisAttributes(G4Colour(1,1,0)); // RGB for blue
    col_vis_att->SetVisibility(true);
    col_vis_att->SetForceSolid(true); // This makes it appear as a solid blue volume instead of just wireframe
    collimator_logic->SetVisAttributes(col_vis_att);

    
    //  -------------------------------------------------------Crystal-------------------------------------------------------------
    G4Element* ele_Tl = nist->FindOrBuildElement("Tl");
    G4Element* ele_Br = nist->FindOrBuildElement("Br");

    G4double density_TlBr = 7.56*g/cm3;
    G4int ncomponents_TlBr = 2;
    G4Material* TlBr = new G4Material("TlBr",density_TlBr,ncomponents_TlBr);

    TlBr->AddElement(ele_Tl, 1);
    TlBr->AddElement(ele_Br, 1);

    G4double crystalX = 40.0*mm;
    G4double crystalY = 40.0*mm;
    G4double crystalZ = 5.0*mm;

    G4double gap = 5.0*mm;

    // Create the crystal shape
    G4Box* crystalShape = new G4Box("CrystalShape", 0.5*crystalX, 0.5*crystalY, 0.5*crystalZ);

    // Create the crystal material (already defined above as TlBr)
    G4Material* crystalMaterial = TlBr;

    // Create logical volume for the crystal
    crystalLV = new G4LogicalVolume(crystalShape, crystalMaterial, "CrystalLV");

    // Set the color to yellow
    G4VisAttributes* crystal_vis_att = new G4VisAttributes(G4Colour(255/255, 245/255, 238/255)); 
    crystal_vis_att->SetVisibility(true);
    crystal_vis_att->SetForceSolid(true); 
    crystalLV->SetVisAttributes(crystal_vis_att);

    // Calculate positions for the crystals
    G4double startPosX = -0.5 * (crystalX + gap);
    G4double startPosY = -0.5 * (crystalY + gap);

    // Place each crystal in the world, based on the f2f
    G4double col_crystal_f2f = 2*mm;
    G4double posZ = -( 0.5 * phantom1_size_z + col_phan_f2f + collimator_z + 0.5 * crystalZ + col_crystal_f2f);

    for (int ix = 0; ix < 2; ++ix) {
        for (int iy = 0; iy < 2; ++iy) {
            G4double posX = startPosX + ix * (crystalX + gap);
            G4double posY = startPosY + iy * (crystalY + gap);
            G4ThreeVector pos(posX, posY, posZ);

            new G4PVPlacement(0,               // no rotation
                            pos,             // at position
                            crystalLV,       // its logical volume
                            "CrystalPV",     // its name
                            logicWorld,         // its mother  volume
                            false,           // no boolean operation
                            ix*2 + iy,       // copy number
                            true);           // checking overlaps
        }
    }


    return physicsWorld; // return the whole physics world
}


void DetectorConstruction::ConstructSDandField()
{
    // Use G4ThreadLocal to ensure one instance per thread
    static G4ThreadLocal TargetSD* fTargetSD = nullptr;
    if (!fTargetSD)
    {
        // Create the sensitive detector
        fTargetSD = new TargetSD("targetSD");
        
        // Register the sensitive detector with G4SDManager
        G4SDManager::GetSDMpointer()->AddNewDetector(fTargetSD);
        
        // Set the sensitive detector for the logical volume
        if (crystalLV)
        {
            crystalLV->SetSensitiveDetector(fTargetSD);
        }
        else
        {
            G4Exception("DetectorConstruction::ConstructSDandField()",
                        "InvalidSetup", FatalException,
                        "crystalLV is not initialized!");
        }
    }
}



}
