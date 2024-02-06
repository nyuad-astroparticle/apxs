/***************************
  ╔╦╗╔═╗╔╦╗╔═╗╔═╗╔╦╗╔═╗╦═╗   
   ║║║╣  ║ ║╣ ║   ║ ║ ║╠╦╝   
  ═╩╝╚═╝ ╩ ╚═╝╚═╝ ╩ ╚═╝╩╚═   
 ╔╦╗╔═╗╔═╗╔═╗╔═╗╔╗╔╔═╗╔═╗╦═╗
 ║║║║╣ ╚═╗╚═╗║╣ ║║║║ ╦║╣ ╠╦╝
 ╩ ╩╚═╝╚═╝╚═╝╚═╝╝╚╝╚═╝╚═╝╩╚═

Used to manage the detector
class. In particular it sets
the material of the source
***************************/

#ifndef DetectorMessenger_hh
#define DetectorMessenger_hh

// The Geant4 class we built on
#include "G4UImessenger.hh"

// Other useful Geant4 Classes
#include "G4String.hh"

// Inline definitions because I don't want to load their headers here
class DetectorConstruction;
class G4UIcmdWithAString;


class DetectorMessenger : public G4UImessenger
{
    public:
        DetectorMessenger(DetectorConstruction*);               // Constructor
        ~DetectorMessenger() override;                          // Destructor

        // The default method to override
        void SetNewValue(G4UIcommand*, G4String) override;      // Parses the command

    private:
        DetectorConstruction*   detectorConstruction;           // Detector construction object we are managing

        G4UIdirectory*          directory;                      // Name of the command directory i.e. /apxs/<command>
        G4UIcmdWithAString*     setSourceMaterial;              // Command that changes the material of the source
        G4UIcmdWithAString*     setTargetMaterial;              // Command that changes the material of the source
        G4UIcmdWithAString*     createMultipleSources;          // Add many disk source around at the same time
        G4UIcommand*            setSourceMaterialAndName;
        G4UIcmdWithAString*     setSourceVolume;
        G4UIcommand*            setTiltAngle;
};

#endif