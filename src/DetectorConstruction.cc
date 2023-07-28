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
#include "G4Tubs.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"
#include "G4SDManager.hh"

// Some STL Headers that are useful
#include <vector>

// Other custom relevant headers
#include "SensitiveDetector.hh"

// Constructor
DetectorConstruction::DetectorConstruction()
{
    // Initialize the Member Variables
    worldSize       = 2.0 * m;
    worldHeight     = 1.0 * m;
    sourceDiameter  = 5.0 * cm;
    sourceThickness = 2.0 * cm;
    sourceRotation  = nullptr;
    sourceLogical   = nullptr;
    sourcePhysical  = nullptr;
    setSourcePosition(G4ThreeVector(-worldSize/6,worldHeight/4,0));
    setSourceMaterial("G4_Cu");
}

//----------------------- 8< -------------[ cut here ]------------------------

// Destructor
DetectorConstruction::~DetectorConstruction()
{
    // Free the memory allocated by the pointers
    delete sourceRotation;
    delete sourceLogical;
    delete sourcePhysical;
}

//----------------------- 8< -------------[ cut here ]------------------------

// Constructs all the Geometry
G4VPhysicalVolume* DetectorConstruction::Construct()
{
    // MATERIALS -----------------------------------------------------------
    // First off invite NIST to the party
    G4NistManager* nist = G4NistManager::Instance();

    // Then add to it the rest of the materials that we might need
    CreateMaterials();

    // WORLD VOLUME --------------------------------------------------------
    G4Material*         worldMaterial   = nist->FindOrBuildMaterial("G4_Galactic");
    G4Box*              worldSolid      = new G4Box("worldSolid", worldSize/2, worldHeight/2, worldSize/2);
    G4VisAttributes*    worldColor      = new G4VisAttributes(false,G4Color(0.0, 1, 0.0, 0.4));
    G4LogicalVolume*    worldLogical    = new G4LogicalVolume(worldSolid,worldMaterial,"worldLogical");
    G4VPhysicalVolume*  worldPhysical   = new G4PVPlacement(
        0,                              // No Rotation Matrix
        G4ThreeVector(0.,0.,0.),        // Position of center
        worldLogical,                   // Logical Volume to place
        "worldPhysical",                // Name of new Physical Volume
        0,                              // Mother Volume Logical
        false,                          // No boolean operation
        0,                              // Copy Number
        true                            // Check for Overlaps
    );
    worldLogical->SetVisAttributes(worldColor);


    // TARGET -------------------------------------------------------------
    G4double            targetThickness = 0.5 * m;
    G4ThreeVector       targetPostition = G4ThreeVector(0., - targetThickness/2, 0.);
    G4Material*         targetMaterial  = nist->FindOrBuildMaterial("Basalt");
    G4Box*              targetSolid     = new G4Box("targetSolid", worldSize/2, targetThickness/2, worldSize/2);
    G4VisAttributes*    targetColor     = new G4VisAttributes(true,G4Color(0.40, 0.45, 0.5, 0.4));
    G4LogicalVolume*    targetLogical   = new G4LogicalVolume(targetSolid, targetMaterial, "targetLogical");
    G4VPhysicalVolume*  targetPhysical  = new G4PVPlacement(
        0,                              // No Rotation Matrix
        targetPostition,                // Position of center
        targetLogical,                  // Logical Volume to place
        "targetPhysical",               // Name of new Physical Volume
        worldLogical,                   // Mother Volume Logical
        false,                          // Boolean operation
        0,                              // Copy Number
        true                            // Check for Overlaps
    );
    targetLogical->SetVisAttributes(targetColor);


    // SOURCE --------------------------------------------------------------
    G4Tubs*             sourceSolid     = new G4Tubs("sourceSolid", 0, sourceDiameter/2, sourceThickness/2, 0, 2*M_PI*rad);
    G4VisAttributes*    sourceColor     = new G4VisAttributes(true,G4Color(0.80, 0.0, 0.0, 0.4));
    sourceLogical                       = new G4LogicalVolume(sourceSolid, sourceMaterial, "sourceLogical");
    sourcePhysical                      = new G4PVPlacement(
        sourceRotation,                 // Rotation Matrix
        sourcePostition,                // Position of center
        sourceLogical,                  // Logical Volume to place
        "sourcePhysical",               // Name of new Physical Volume
        worldLogical,                   // Mother Volume Logical
        false,                          // Boolean operation
        0,                              // Copy Number
        true                            // Check for Overlaps
    );
    sourceLogical->SetVisAttributes(sourceColor);


    // DETECTOR ------------------------------------------------------------
    G4double            detectDiameter  = 10.0 * cm;
    G4double            detectThickness = 3.0 * cm;
    G4ThreeVector       detectPostition = G4ThreeVector(worldSize/6,worldHeight/4,0);
    G4Material*         detectMaterial  = nist->FindOrBuildMaterial("G4_Si");
    G4Tubs*             detectSolid     = new G4Tubs("detectSolid", 0, detectDiameter/2, detectThickness/2, 0, 2*M_PI*rad);
    G4VisAttributes*    detectColor     = new G4VisAttributes(true,G4Color(0.0, 0.0, 0.80, 0.4));
    G4LogicalVolume*    detectLogical   = new G4LogicalVolume(detectSolid, detectMaterial, "detectLogical");
    G4RotationMatrix*   detectRotation  = new G4RotationMatrix(detectPostition.cross(G4ThreeVector(0., 0., 1.)).unit(),90*degree);
    G4VPhysicalVolume*  detectPhysical  = new G4PVPlacement(
        detectRotation,                 // No Rotation Matrix
        detectPostition,                // Position of center
        detectLogical,                  // Logical Volume to place
        "detectPhysical",               // Name of new Physical Volume
        worldLogical,                   // Mother Volume Logical
        false,                          // Boolean operation
        0,                              // Copy Number
        true                            // Check for Overlaps
    );
    detectLogical->SetVisAttributes(detectColor);

    return worldPhysical;

}

//----------------------- 8< -------------[ cut here ]------------------------

void DetectorConstruction::ConstructSDandField()
{
    // Create a master sensitive detector
    SensitiveDetector* detector = new SensitiveDetector("/APXSDetector","APXSHitsCollection");

    // Let Geant4 know about it
    G4SDManager::GetSDMpointer()->AddNewDetector(detector);

    // Attach the sensitive Detector to the relevent volumes
    SetSensitiveDetector(detectLogical, detector);
}

//----------------------- 8< -------------[ cut here ]------------------------

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

//----------------------- 8< -------------[ cut here ]------------------------
// MUTATORS ----------------------------------------------------------------

void DetectorConstruction::setSourceDiameter(G4double diameter) 
{ sourceDiameter = diameter; }

void DetectorConstruction::setSourceThickness(G4double thickness) 
{ sourceThickness = thickness; }

void DetectorConstruction::setSourcePosition(G4ThreeVector position) 
{ 
    sourcePostition = position;

    // Use the position to assign 
    setSourceRotation(position);
}

void DetectorConstruction::setSourceMaterial(const char* name)
{
    // Get the nist pointer
    G4NistManager* nist = G4NistManager::Instance();

    // Assign the material
    sourceMaterial = nist->FindOrBuildMaterial(name);
}

void DetectorConstruction::setSourceRotation(G4ThreeVector normal)
{
    // Remove the previous thing that was stored there
    if (sourceRotation) delete sourceRotation;

    // Sets the new rotation perpendicular to the normal vector
    sourceRotation = new G4RotationMatrix(normal.cross(G4ThreeVector(0., 0., 1.)).unit(),90*degree);
}