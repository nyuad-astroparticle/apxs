/*********************************
     ╔╦╗╔═╗╔╦╗╔═╗╔═╗╔╦╗╔═╗╦═╗     
      ║║║╣  ║ ║╣ ║   ║ ║ ║╠╦╝     
    o═╩╝╚═╝ ╩ ╚═╝╚═╝ ╩ ╚═╝╩╚═o    
╔═╗╔═╗╔╗╔╔═╗╔╦╗╦═╗╦ ╦╔═╗╔╦╗╦╔═╗╔╗╔
║  ║ ║║║║╚═╗ ║ ╠╦╝║ ║║   ║ ║║ ║║║║
╚═╝╚═╝╝╚╝╚═╝ ╩ ╩╚═╚═╝╚═╝ ╩ ╩╚═╝╝╚╝

How to build the geometry of the
simulation including placement of
volumes as well as sensitive dete-
ctors and materials
*********************************/

#ifndef DetectorConstruction_hh
#define DetectorConstruction_hh

//Include relevant Geant4 Libraries
#include "G4VUserDetectorConstruction.hh"

// Other relevant Geant4 Headers
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"

// Other relevant inline definitions
class DetectorMessenger;

class DetectorConstruction : public G4VUserDetectorConstruction
{
    public:
        DetectorConstruction(G4String filename);                     // Constructor
        ~DetectorConstruction();                    // Destructor

        G4VPhysicalVolume* Construct() override;    // Create the full geometry assembly
        void ConstructSDandField() override;        // Assign certain logical volumes as Sensitive Detectors

        // Setters for Class Members
        void setSourceDiameter(G4double);
        void setSourceThickness(G4double);
        void setSourcePosition(G4ThreeVector);      // Sets the position of the detector and then it's rotation
        void setSourceMaterial(const char*);        // Finds the material given its string name
        void setSourceRotation(G4ThreeVector);      // Sets source rotation 90° perpendicular to a vector
        void setTargetMaterial(const char*);        // Finds the material given its string name
        void setSourceMaterialAndName(const G4String &element, const G4String & volumeName);
        void setSourceVolume(G4String);
        void tiltTarget(G4double, G4double);
        void atmosphere(G4String, G4double);
        void dustLayer(const G4String & dustMaterial, const G4double & thickness_mm);

       G4LogicalVolume* GetDaughterLogicalByName(G4LogicalVolume*, const G4String & );
       G4VPhysicalVolume* GetDaughterPhysicalByName(G4LogicalVolume*, const G4String & );
        
        // Create multiple sources
        void createMultipleSources(G4int numberOfSources);

    private:
        void CreateMaterials();                     // Defines the Simulation Materials that are not on NIST

        G4String filename = "";
        G4double            worldSize;
        G4double            worldHeight;
        G4double            sourceDiameter;         // Parameters of the
        G4double            sourceThickness;        // radioactive source
        G4ThreeVector       sourcePosition;         // that we want to access
        G4Material*         sourceMaterial;         // from other classes i.e.
        G4RotationMatrix*   sourceRotation;         // Generator & Messenger
        G4LogicalVolume*    sourceLogical;
        G4VPhysicalVolume*  sourcePhysical;
        G4LogicalVolume*    detectLogical;          // The logical Volume for the detector
        G4Material*         targetMaterial;   
        G4VPhysicalVolume*  targetPhysical;      
        G4LogicalVolume*    targetLogical;          // The logical Volume of the target
        DetectorMessenger*  detectorMessenger;      // The messenger Class used to change the materials and other parameters of the simulation
        G4NistManager*      nist;
        G4LogicalVolume*    worldLogical;
        G4String            sourceVolume;
        G4LogicalVolume*    logicXrayTube = nullptr;        

        friend class PrimaryGeneratorAction;        // We allow PrimaryGeneratorAction to have access the variables of this class
};

#endif