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

// In case we are loading the geometry from a GDML File
#ifdef DETECTOR_GDML
#include "G4GDMLParser.hh"
#endif

// Some STL Headers that are useful
#include <vector>
#include <map>

// Other custom relevant headers
#include "DetectorMessenger.hh"
#include "SensitiveDetector.hh"

// Constructor
DetectorConstruction::DetectorConstruction(G4String aFilename) : filename(aFilename)
{
    // Initialize the Member Variables
    worldSize       = 20.0 * cm;
    worldHeight     = 10.00 * cm;
    sourceDiameter  = 6.00 * mm;
    sourceThickness = 0.24 * mm;
    sourceRotation  = nullptr;
    sourceLogical   = nullptr;
    sourcePhysical  = nullptr;
    detectLogical   = nullptr;
    targetLogical   = nullptr;
    targetPhysical  = nullptr;
    nist = G4NistManager::Instance();
    setSourcePosition(G4ThreeVector(-worldSize/30,worldHeight/8,0));
    // setSourceMaterial("G4_Cm");
    setTargetMaterial("G4_Au");

    // Create the messenger
    detectorMessenger = new DetectorMessenger(this);
}

//----------------------- 8< -------------[ cut here ]------------------------

// Destructor
DetectorConstruction::~DetectorConstruction()
{
    // Free the memory allocated by the pointers
    delete sourceRotation;
    delete sourceLogical;
    delete sourcePhysical;
    delete targetLogical;
    delete targetPhysical;
    delete detectorMessenger;
}

//----------------------- 8< -------------[ cut here ]------------------------

// Constructs all the Geometry
G4VPhysicalVolume* DetectorConstruction::Construct()
{

    // Then add to it the rest of the materials that we might need
    CreateMaterials();

    // WORLD VOLUME --------------------------------------------------------
    G4Material*         worldMaterial   = nist->FindOrBuildMaterial("G4_Galactic");
    G4Box*              worldSolid      = new G4Box("worldSolid", worldSize/2, worldHeight/2, worldSize/2);
    G4VisAttributes*    worldColor      = new G4VisAttributes(false,G4Color(0.0, 1, 0.0, 0.4));
    worldLogical    = new G4LogicalVolume(worldSolid,worldMaterial,"worldLogical");
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
    G4double            targetThickness = 0.5 * cm;
    G4ThreeVector       targetPostition = G4ThreeVector(0., - targetThickness/2, 0.);
    //targetMaterial  =   nist->FindOrBuildMaterial("G4_Au");
    G4Box*              targetSolid     = new G4Box("targetSolid", worldSize/2, targetThickness/2, worldSize/2);
    G4VisAttributes*    targetColor     = new G4VisAttributes(true,G4Color(0.40, 0.45, 0.5, 0.4));
    targetLogical   = new G4LogicalVolume(targetSolid, targetMaterial, "targetLogical");
    targetPhysical  = new G4PVPlacement(
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


    // // SOURCE --------------------------------------------------------------
    // G4Tubs*             sourceSolid     = new G4Tubs("sourceSolid", 0, sourceDiameter/2, sourceThickness/2, 0, 2*M_PI*rad);
    // G4VisAttributes*    sourceColor     = new G4VisAttributes(true,G4Color(0.80, 0.0, 0.0, 0.4));
    // sourceLogical                       = new G4LogicalVolume(sourceSolid, sourceMaterial, "sourceLogical");
    // sourcePhysical                      = new G4PVPlacement(
    //     sourceRotation,                 // Rotation Matrix
    //     sourcePosition,                 // Position of center
    //     sourceLogical,                  // Logical Volume to place
    //     "sourcePhysical",               // Name of new Physical Volume
    //     worldLogical,                   // Mother Volume Logical
    //     false,                          // Boolean operation
    //     0,                              // Copy Number
    //     true                            // Check for Overlaps
    // );
    // sourceLogical->SetVisAttributes(sourceColor);
    
    // // SOURCE SUBSTRATE -----------------------------------------------------
    // G4Material*         subMaterial     = nist->FindOrBuildMaterial("G4_SILICON_DIOXIDE");
    // G4ThreeVector       subPosition     = G4ThreeVector(0.0, sourceThickness/2 + 0.8/2 * mm, 0.0) + sourcePosition;
    // G4Tubs*             subSolid        = new G4Tubs("sourceSolid", 0.0 , 8.0/2*mm, 0.8/2 * mm, 0, 2*M_PI*rad);
    // G4VisAttributes*    subColor        = new G4VisAttributes(true,G4Color(0.80, 0.5, 0.0, 0.4));
    // G4LogicalVolume*    subLogical      = new G4LogicalVolume(subSolid, subMaterial, "sourceLogical");
    // G4VPhysicalVolume*  subPhysical     = new G4PVPlacement(
    //     sourceRotation,                 // Rotation Matrix
    //     subPosition,                    // Position of center
    //     subLogical,                     // Logical Volume to place
    //     "subPhysical",                  // Name of new Physical Volume
    //     worldLogical,                   // Mother Volume Logical
    //     false,                          // Boolean operation
    //     0,                              // Copy Number
    //     true                            // Check for Overlaps
    // );
    // subLogical->SetVisAttributes(subColor);

    // DETECTOR ------------------------------------------------------------
    // By default place a pure volume that is vacuum and simply tracks what particles go through it.
    #ifndef DETECTOR_GDML
    G4double            detectDiameter  = 30.0 * mm;
    G4double            detectThickness = 3.0 * mm;
    G4ThreeVector       detectPostition = G4ThreeVector(worldSize/10,worldHeight/4,0);
    G4Material*         detectMaterial  = nist->FindOrBuildMaterial("G4_Galactic");
    G4Tubs*             detectSolid     = new G4Tubs("detectSolid", 0, detectDiameter/2, detectThickness/2, 0, 2*M_PI*rad);
    G4VisAttributes*    detectColor     = new G4VisAttributes(true,G4Color(0.0, 0.0, 0.80, 0.4));
                        detectLogical   = new G4LogicalVolume(detectSolid, detectMaterial, "detectLogical");
    G4RotationMatrix*   detectRotation  = new G4RotationMatrix(G4ThreeVector(-1., 0., 0.),90*degree); //new G4RotationMatrix(detectPostition.cross(G4ThreeVector(0., 0., 1.)).unit(),90*degree);
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
    #endif

    // Alternatively you can load a GDML File!
    #ifdef DETECTOR_GDML
    // Start the parser
    G4GDMLParser* parser = new G4GDMLParser();

    // Read the file 
    // (the `false` flag turns off the check to make sure the GDML file is properly formatted)
    
    parser->Read(filename,false);

    // Extract the logical volume of the sensitive part of the detector
    detectLogical = parser->GetVolume("detectLogical");

    // Extract the rest of the detector and place it accordingly
    G4ThreeVector       detectPostition = G4ThreeVector(-sourcePosition[0],sourcePosition[1],sourcePosition[2]);
    G4VisAttributes*    detectColor     = new G4VisAttributes(true,G4Color(0.00, 0.00, 0.80, 0.6));
    G4VisAttributes*    windowColor     = new G4VisAttributes(true,G4Color(0.68, 0.93, 0.93, 0.6));
    G4VisAttributes*    caseColor       = new G4VisAttributes(true,G4Color(0.72, 0.54, 0.04, 0.6));
    G4VisAttributes*    containerColor  = new G4VisAttributes(false,G4Color(0.72, 0.54, 0.04, 0.6));
    G4RotationMatrix*   detectRotation  = new G4RotationMatrix(G4ThreeVector(-1., 0., 0.),90*degree); //new G4RotationMatrix(detectPostition.cross(G4ThreeVector(0., 0., -1.)).unit(),90*degree);
    G4LogicalVolume*    sddLogical      = parser->GetVolume("VITUS");
    G4VPhysicalVolume*  sddPhysical     = new G4PVPlacement(
        detectRotation,                 // Rotation Matrix
        detectPostition,                // Position of center
        sddLogical,                     // Logical Volume to place
        "detectPhysical",               // Name of new Physical Volume
        worldLogical,                   // Mother Volume Logical
        false,                          // Boolean operation
        0,                              // Copy Number
        true                            // Check for Overlaps
    );

    // Add some colors
    if (parser->GetVolume("sddWindowLogical")) parser->GetVolume("sddWindowLogical")->SetVisAttributes(windowColor);
    parser->GetVolume("sddCaseLogical")->SetVisAttributes(caseColor);
    parser->GetVolume("detectLogical")->SetVisAttributes(detectColor);
    sddLogical->SetVisAttributes(containerColor);

    #endif

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

    // Ferroan Noritic Anorthosite --------------------------------------------------------------
    elements.push_back("O" );       massFraction.push_back(0.4497951);
    elements.push_back("Si");       massFraction.push_back(0.2070582);   
    elements.push_back("Al");       massFraction.push_back(0.1572021); 
    elements.push_back("Fe");       massFraction.push_back(0.0310920);
    elements.push_back("Mn");       massFraction.push_back(0.0004647); 
    elements.push_back("Mg");       massFraction.push_back(0.0229140); 
    elements.push_back("Ca");       massFraction.push_back(0.1236431); 
    elements.push_back("Na");       massFraction.push_back(0.0015580); 
    elements.push_back("K");       massFraction.push_back(0.0001328); 
    elements.push_back("Cr");       massFraction.push_back(0.0004600); 
    elements.push_back("Sr");       massFraction.push_back(0.0001500); 

    // add it to the material
    nist->ConstructNewMaterial("FerroanNoriticAnorthosite", elements, massFraction, density);
    elements.clear();
    massFraction.clear();

    // Pigeonite Basalt --------------------------------------------------------------
    elements.push_back("O" );       massFraction.push_back(0.4172228);
    elements.push_back("Si");       massFraction.push_back(0.2159855);   
    elements.push_back("Ti");       massFraction.push_back(0.0198968); 
    elements.push_back("Al");       massFraction.push_back(0.0536710); 
    elements.push_back("Fe");       massFraction.push_back(0.1536722);
    elements.push_back("Mn");       massFraction.push_back(0.0021686); 
    elements.push_back("Mg");       massFraction.push_back(0.0492651); 
    elements.push_back("Ca");       massFraction.push_back(0.0786885); 
    elements.push_back("Na");       massFraction.push_back(0.0019289); 
    elements.push_back("K");       massFraction.push_back(0.0005396); 
    elements.push_back("P");       massFraction.push_back(0.0006110); 
    elements.push_back("S");       massFraction.push_back(0.0008000); 
    elements.push_back("V");       massFraction.push_back(0.0001480); 
    elements.push_back("Cr");       massFraction.push_back(0.0033500); 
    elements.push_back("Sr");       massFraction.push_back(0.0001300); 
    elements.push_back("Zr");       massFraction.push_back(0.0001380); 

    // add it to the material
    nist->ConstructNewMaterial("PigeoniteBasalt", elements, massFraction, density);
    elements.clear();
    massFraction.clear();

    // Impact Melt Breccia --------------------------------------------------------------
    elements.push_back("O" );       massFraction.push_back(0.4530908);
    elements.push_back("Si");       massFraction.push_back(0.2075256);   
    elements.push_back("Ti");       massFraction.push_back(0.0017979); 
    elements.push_back("Al");       massFraction.push_back(0.1635537); 
    elements.push_back("Fe");       massFraction.push_back(0.0230858);
    elements.push_back("Mn");       massFraction.push_back(0.0003098); 
    elements.push_back("Mg");       massFraction.push_back(0.0167031); 
    elements.push_back("Ca");       massFraction.push_back(0.1266448); 
    elements.push_back("Na");       massFraction.push_back(0.0043030); 
    elements.push_back("K");       massFraction.push_back(0.0004981); 
    elements.push_back("P");       massFraction.push_back(0.0000873); 
    elements.push_back("Cr");       massFraction.push_back(0.0003700); 
    elements.push_back("Sr");       massFraction.push_back(0.0002500); 
    elements.push_back("Ni");       massFraction.push_back(0.0003600); 
    elements.push_back("Ba");       massFraction.push_back(0.0000920); 


    // add it to the material
    nist->ConstructNewMaterial("ImpactMeltBreccia", elements, massFraction, density);
    elements.clear();
    massFraction.clear();

    // Troctolite --------------------------------------------------------------
    elements.push_back("O" );       massFraction.push_back(0.4465796);
    elements.push_back("Si");       massFraction.push_back(0.2004211);   
    elements.push_back("Ti");       massFraction.push_back(0.0002997); 
    elements.push_back("Al");       massFraction.push_back(0.1097239); 
    elements.push_back("Fe");       massFraction.push_back(0.0387873);
    elements.push_back("Mn");       massFraction.push_back(0.0005422); 
    elements.push_back("Mg");       massFraction.push_back(0.1151127); 
    elements.push_back("Ca");       massFraction.push_back(0.0815473); 
    elements.push_back("Na");       massFraction.push_back(0.0017064); 
    elements.push_back("K");       massFraction.push_back(0.0002491); 
    elements.push_back("P");       massFraction.push_back(0.0001309); 
    elements.push_back("Cr");       massFraction.push_back(0.0007530); 
    elements.push_back("Sr");       massFraction.push_back(0.0001140); 

    // add it to the material
    nist->ConstructNewMaterial("Troctolite", elements, massFraction, density);
    elements.clear();
    massFraction.clear();

    // Apollo 17 Orange --------------------------------------------------------------
    elements.push_back("O" );       massFraction.push_back(0.4007786);
    elements.push_back("Si");       massFraction.push_back(0.1802762);   
    elements.push_back("Ti");       massFraction.push_back(0.0527983); 
    elements.push_back("Al");       massFraction.push_back(0.0334518); 
    elements.push_back("Fe");       massFraction.push_back(0.1713169);
    elements.push_back("Mn");       massFraction.push_back(0.0023235); 
    elements.push_back("Mg");       massFraction.push_back(0.0870732); 
    elements.push_back("Ca");       massFraction.push_back(0.0548890); 
    elements.push_back("Na");       massFraction.push_back(0.0026708); 
    elements.push_back("K");       massFraction.push_back(0.0007472); 
    elements.push_back("P");       massFraction.push_back(0.0001746); 
    elements.push_back("S");       massFraction.push_back(0.0007000); 
    elements.push_back("Cr");       massFraction.push_back(0.0046500); 
    elements.push_back("Ni");       massFraction.push_back(0.0001010); 
    elements.push_back("Zn");       massFraction.push_back(0.0002920); 
    elements.push_back("Sr");       massFraction.push_back(0.0002050); 
    elements.push_back("Zr");       massFraction.push_back(0.0001820); 
    elements.push_back("Ba");       massFraction.push_back(0.0001120); 


    // add it to the material
    nist->ConstructNewMaterial("Apollo17Orange", elements, massFraction, density);
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
    sourcePosition = position;

    // Use the position to assign 
    setSourceRotation(position);
}

void DetectorConstruction::setSourceMaterial(const char* name)
{
    // Assign the material
    sourceMaterial = nist->FindOrBuildMaterial(name);
    
    // If the logical volume already exists
    if (sourceLogical) {
        sourceLogical->SetMaterial(sourceMaterial);                 // Set the new Material
        G4cout << "Source is now made out of " << name << G4endl;   // Let the user know
    }

}

void DetectorConstruction::setSourceMaterialAndName(const char* materialName, G4String volumeName)
{
    // Assign the material
    
    G4Material* material = nist->FindOrBuildMaterial(materialName);
    G4LogicalVolume * volume = DetectorConstruction::GetDaughterLogicalByName(worldLogical, volumeName);
    
    // If the logical volume already exists
    if (volume) {
        volume->SetMaterial(material);                 // Set the new Material
        G4cout << volume->GetName() << " is now made out of " << material->GetName() << G4endl;   // Let the user know
    }

}

void DetectorConstruction::setSourceRotation(G4ThreeVector normal)
{
    // Remove the previous thing that was stored there
    if (sourceRotation) delete sourceRotation;

    // Sets the new rotation perpendicular to the normal vector
    // sourceRotation = new G4RotationMatrix(normal.cross(G4ThreeVector(0., 0., 1.)).unit(),90*degree);
    sourceRotation = new G4RotationMatrix(G4ThreeVector(-1., 0., 0.),90*degree);
}

void DetectorConstruction::setTargetMaterial(const char* name)
{
    // Assign the material
    targetMaterial = nist->FindOrBuildMaterial(name);
    
    // If the logical volume already exists
    if (targetLogical) {
        targetLogical->SetMaterial(targetMaterial);                 // Set the new Material
        G4cout << "Target is now made out of " << name << G4endl;   // Let the user know
    }

}

void DetectorConstruction::createMultipleSources(G4int numberOfSources)
{

    // Define the disk shape
    G4double    innerRadius = 0.0;
    G4double    outerRadius = sourceDiameter/2;
    G4double    thickness = sourceThickness;
    G4Tubs*     diskSolid = new G4Tubs("Disk", innerRadius, outerRadius, thickness/2, 0.0, 360.0*deg);
    // Iron
    sourceMaterial = nist->FindOrBuildMaterial("G4_Fe");

    // SOURCE SUBSTRATE -----------------------------------------------------
    G4Material*         subMaterial     = nist->FindOrBuildMaterial("G4_SILICON_DIOXIDE");
    G4double            subDiameter     = sourceDiameter;
    G4double            subThickness    = sourceThickness;
    G4Tubs*             subSolid        = new G4Tubs("sourceSolid", 0.0 , subDiameter/2, subThickness/2, 0, 2*M_PI*rad);
    G4VisAttributes*    subColor        = new G4VisAttributes(true,G4Color(0.80, 0.5, 0.0, 0.4));
    G4LogicalVolume*    subLogical      = new G4LogicalVolume(subSolid, subMaterial, "sourceLogical");
    subLogical->SetVisAttributes(subColor);
    
    // Beryllium window 
    G4Material*         BeMaterial     = nist->FindOrBuildMaterial("G4_Be");
    G4double            BeDiameter     = sourceDiameter;
    G4double            BeThickness    = sourceThickness;
    G4Tubs*             BeSolid        = new G4Tubs("sourceSolid", 0.0 , BeDiameter/2, BeThickness/2, 0, 2*M_PI*rad);
    G4VisAttributes*    BeColor        = new G4VisAttributes(true,G4Color(0.0, 0.5, 0.5, 0.4));
    G4LogicalVolume*    BeLogical      = new G4LogicalVolume(BeSolid, BeMaterial, "sourceLogical");
    BeLogical->SetVisAttributes(BeColor);
    


    // Create a logical volume

    // Number of disks
    int n = numberOfSources; // Number of copies
    double radius = 2*cm; // Radius of the circle where disks will be placed
    double angleStep = 360.0 * deg / n;

    for (int i = 0; i < n; ++i) {
        
        G4LogicalVolume* diskLV = new G4LogicalVolume(diskSolid, sourceMaterial, "DiskLV" + std::to_string(i));
        double angle = i * angleStep;
        double x = radius * cos(angle);
        double z = radius * sin(angle);
        double y = 0.0;

        // Create a rotation matrix
        G4RotationMatrix rotm = G4RotationMatrix();
        rotm.rotateX(90.0*deg); // Adjust this as needed
        // rotm.rotateZ(angle);

        // Combine rotation and translation
        G4ThreeVector position(x, y, z);
        G4ThreeVector shift(6.7*mm,12.5*mm,0);
        position += shift;
        G4Transform3D transform(rotm, position);

        // Place the copy
        new G4PVPlacement(transform, diskLV, "DiskPV_" + std::to_string(i), worldLogical, false, 0, true);

        // Substrate
            G4ThreeVector subPosition = G4ThreeVector(0.0, sourceThickness/2 + subThickness/2, 0.0) + position;
            G4Transform3D transformSub(rotm, subPosition);        
            new G4PVPlacement(transformSub,subLogical,"subPhysical_" + std::to_string(i),worldLogical,false,i,true);

        // Beryllium
            G4ThreeVector BePosition = position - G4ThreeVector(0,BeThickness/2 + sourceThickness/2,0);
            G4Transform3D transformBe(rotm, BePosition);
            new G4PVPlacement(transformBe, BeLogical, "BePhysics_" + std::to_string(i),worldLogical,false,i,true);
    }
}

// Function to find a daughter logical volume by name from a specific parent logical volume
G4VPhysicalVolume* DetectorConstruction::GetDaughterPhysicalByName(G4LogicalVolume* parentLogical, const G4String& daughterName) {
    // G4cout << "GetDaughterLogicalByName was called\n";
    if (!parentLogical) return nullptr; // Safety check
    // G4cout << "worldLogical was found\n";


    for (G4int i = 0; i < parentLogical->GetNoDaughters(); ++i) {
        G4VPhysicalVolume* daughterPhys = parentLogical->GetDaughter(i);
        // G4cout << daughterPhys->GetName() << "\n";
        if (daughterPhys && daughterPhys->GetName() == daughterName) {
            // G4cout << "the volume was found\n";
            return daughterPhys;
        }
    }

    // G4cout << "the volume was not found\n";
    return nullptr; // If not found
}

G4LogicalVolume* DetectorConstruction::GetDaughterLogicalByName(G4LogicalVolume* parentLogical, const G4String & daughterName)
{
    G4VPhysicalVolume* physVol = DetectorConstruction::GetDaughterPhysicalByName(parentLogical, daughterName);
    
    if (physVol)
    {
    return physVol->GetLogicalVolume();
    }
    else
    {
        return nullptr;
    }
}

void DetectorConstruction::setSourceVolume(G4String volumeName)
{
    sourceVolume = volumeName;
    G4cout << volumeName + " is now the source Volume\n";
}

void DetectorConstruction::tiltTarget(G4double x, G4double z)
{
    G4RotationMatrix* rot = new G4RotationMatrix;
    rot->rotateX(x*deg);
    rot->rotateZ(z*deg);
    targetPhysical->SetRotation(rot);
    G4cout << "Tilted the target about X axis by " + std::to_string(x) + " degrees\n";
    G4cout << "Tilted the target about Z axis by " + std::to_string(z) + " degrees\n";
}

void DetectorConstruction::atmosphere(G4String atmosphereName, G4double pressure_mBar)
{
    G4double P = pressure_mBar * 100 * pascal;
    G4double R = 8.314 * joule/mole/kelvin; // Universal gas constant
    G4double T = 293.0 * kelvin;

    G4Material* CO2 = nist->FindOrBuildMaterial("G4_CARBON_DIOXIDE");
    G4Material* Ar  = nist->FindOrBuildMaterial("G4_Ar");
    G4Material* AIR = nist->FindOrBuildMaterial("G4_AIR");

    G4Element* N = nist->FindOrBuildElement("N");
    G4Element* O = nist->FindOrBuildElement("O");

    G4double fractionmass;

    std::map <G4String, G4Material*> dictionary; 

    // AIR

    if (atmosphereName == "Air")
    {

        G4double M = 28.97 * g/mole;

        // Calculate the density using the ideal gas law
        G4double density = (P * M) / (R * T);

        G4Material* air = new G4Material("Air", density, 1, kStateGas, T, P);
        air->AddMaterial(AIR, fractionmass=1.0);
        dictionary["Air"]       = air;
    }

    // CO2
    if (atmosphereName == "CO2")
    {
        G4double M_CO2 = 44.01 * g/mole;

        // Calculate the density of CO2 using the ideal gas law
        G4double density_CO2 = (P * M_CO2) / (R * T);

        // Define CO2 as a material
        G4Material* customCO2 = new G4Material("CarbonDioxide", density_CO2, 1, kStateGas, T, P);

        // Add elements to CO2 material
        customCO2->AddMaterial(CO2, fractionmass=1.0);
        dictionary["CO2"]       = customCO2;
    }

    // Vacuum
    if (atmosphereName == "Vacuum")
    {
        G4Material* vacuum = nist->FindOrBuildMaterial("G4_Galactic");
        dictionary["Vacuum"]    = vacuum;
    }
    // Mars

    if (atmosphereName == "Mars")
    {

        G4double MarsP = 652.0 * pascal; 
        G4double MarsT = 210.0 * kelvin;
        G4double MarsDensity = 0.020 * kg/m3;

        // Constants for the composition of Mars' atmosphere
        G4double fractionCO2 = 0.9532 + 0.0025;
        G4double fractionN2 = 0.027;
        G4double fractionAr = 0.016;
        G4double fractionO2 = 0.0013;

        // Define Mars atmosphere as a material
        G4Material* marsAtmosphere = new G4Material("MarsAtmosphere", MarsDensity, 4, kStateGas, MarsT, MarsP);
        marsAtmosphere->AddMaterial (CO2,   fractionmass = fractionCO2);
        marsAtmosphere->AddMaterial (Ar,    fractionmass = fractionAr);
        marsAtmosphere->AddElement  (N,     fractionmass = fractionN2);
        marsAtmosphere->AddElement  (O,     fractionmass = fractionO2);
        dictionary["Mars"]      = marsAtmosphere;
    }

    G4Material* material    = dictionary[atmosphereName];
    worldLogical            ->SetMaterial(material);

    G4cout << "Atmosphere has been changed to " << dictionary[atmosphereName]->GetName() << " with pressure " << material->GetPressure()/100/pascal << "millibar" <<"\n";
}