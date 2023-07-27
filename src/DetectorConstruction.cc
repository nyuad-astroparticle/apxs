/************************
•     ┓             •    
┓┏┳┓┏┓┃┏┓┏┳┓┏┓┏┓╋┏┓╋┓┏┓┏┓
┗┛┗┗┣┛┗┗ ┛┗┗┗ ┛┗┗┗┻┗┗┗┛┛┗
    ┛                    
  Detector Construction

The geometry is a small
radioactive source at
45° wrt a rough rock
surface, and 90° wrt a
block of silicon as used
as a detector
************************/

// Include the header that we are working on
#include "DetectorConstruction.hh"

// Relevant Geant4 Headers
#include "G4NistManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4Box.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"

// Some STL Headers that are useful
#include <vector>

// Constructor
DetectorConstruction::DetectorConstruction()
{}

//-----------------------8<-------------[ cut here ]------------------------

// Destructor
DetectorConstruction::~DetectorConstruction()
{}

//-----------------------8<-------------[ cut here ]------------------------

// Constructs all the Geometry
G4VPhysicalVolume* DetectorConstruction::Construct()
{
    // MATERIALS -----------------------------------------------------------
    // First off invite NIST to the party
    G4NistManager* nist = G4NistManager::Instance();

    // Then add to it the rest of the materials that we might need
    CreateMaterials();

    // WORLD VOLUME --------------------------------------------------------
    G4double            worldSize       = 2.0 * m;
    G4Material*         worldMaterial   = nist->FindOrBuildMaterial("G4_Galactic");
    G4Box*              worldSolid      = new G4Box("worldSolid", worldSize/2, worldSize/2, worldSize/2);
    G4VisAttributes*    worldColor      = new G4VisAttributes(true,G4Color(0.0, 0.1, 0.0, 0.1));
    G4LogicalVolume*    worldLogical    = new G4LogicalVolume(worldSolid,worldMaterial,"worldLogical");
    G4VPhysicalVolume*  worldPhysical   = new G4PVPlacement(
        0,                                                          // No Rotation Matrix
        G4ThreeVector(0.,0.,0.),                                    // Position of center
        worldLogical,                                               // Logical Volume to place
        "worldPhysical",                                            // Name of new Physical Volume
        0,                                                          // Mother Volume Logical
        false,                                                      // No boolean operation
        0,                                                          // Copy Number
        true                                                        // Check for Overlaps
    );
    worldLogical->SetVisAttributes(worldColor);


    // TARGET -------------------------------------------------------------
    G4double            targetThickness = 0.5 * m;
    G4Material*         targetMaterial  = nist->FindOrBuildMaterial("Basalt");
    G4Box*              targetSolid     = new G4Box("targetSolid", worldSize/2, targetThickness/2, worldSize/2);
    G4VisAttributes*    targetColor     = new G4VisAttributes(1,G4Color(0.7, 0.7, 0.7, 0.3));
    G4LogicalVolume*    targetLogical   = new G4LogicalVolume(targetSolid, targetMaterial, "targetLogical");
    G4VPhysicalVolume*  targetPhysical  = new G4PVPlacement(
        0,                                                          // No Rotation Matrix
        G4ThreeVector(0., (-worldSize + targetThickness)/2, 0.),    // Position of center
        targetLogical,                                              // Logical Volume to place
        "targetPhysical",                                           // Name of new Physical Volume
        worldLogical,                                               // Mother Volume Logical
        false,                                                      // Boolean operation
        0,                                                          // Copy Number
        true                                                        // Check for Overlaps
    );


}

//-----------------------8<-------------[ cut here ]------------------------

void DetectorConstruction::ConstructSDandField()
{}

//-----------------------8<-------------[ cut here ]------------------------

// Adds materials that are not in NIST's libary to the Materials Manager
void DetectorConstruction::CreateMaterials()
{
    // Get the NIST Manager
    G4NistManager* nist = G4NistManager::Instance();

    // Define a vector to store the elements
    std::vector<G4String>   elements;
    std::vector<G4double>   massFraction;

    // BASALT --------------------------------------------------------------
    G4double density = 3.0 * g/cm3;
    elements.push_back("Fe");       massFraction.push_back(0.1200); 
    elements.push_back("Ti");       massFraction.push_back(0.0160);   
    elements.push_back("Ca");       massFraction.push_back(0.0750); 
    elements.push_back("Si");       massFraction.push_back(0.2160);   
    elements.push_back("Al");       massFraction.push_back(0.0710); 
    elements.push_back("Mg");       massFraction.push_back(0.0590);   
    elements.push_back("O" );       massFraction.push_back(0.4430);

    // add it to the material
    nist->ConstructNewMaterial("Basalt", elements, massFraction, density);
    elements.clear();
    massFraction.clear();

    return;
}