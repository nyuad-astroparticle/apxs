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


class DetectorConstruction : public G4VUserDetectorConstruction
{
    public:
        DetectorConstruction();                     // Constructor
        ~DetectorConstruction();                    // Destructor

        G4VPhysicalVolume* Construct() override;    // Create the full geometry assembly
        void ConstructSDandField() override;        // Assign certain logical volumes as Sensitive Detectors

    private:
        void CreateMaterials();                     // Defines the Simulation Materials that are not on NIST
};

#endif