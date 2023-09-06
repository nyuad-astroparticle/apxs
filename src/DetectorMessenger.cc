/************************
•     ┓             •    
┓┏┳┓┏┓┃┏┓┏┳┓┏┓┏┓╋┏┓╋┓┏┓┏┓
┗┛┗┗┣┛┗┗ ┛┗┗┗ ┛┗┗┗┻┗┗┗┛┛┗
    ┛                    
    Detector Messenger

Sets different materials
during the simulation
runtime
************************/

// Include the relevant header
#include "DetectorMessenger.hh"

// Include other cool Geant4 Headers
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"

// Other relevant headers
#include "DetectorConstruction.hh"


// Constructor
DetectorMessenger::DetectorMessenger(DetectorConstruction* detectorConstruction)
{
    // First assign the detector construction variable
    this->detectorConstruction = detectorConstruction;

    // Create the command directory
    directory = new G4UIdirectory("/apxs/");
    directory->SetGuidance("Manages the materials assigned to different parts of the setup.");

    // COMMANDS --------------------------------------------------------------
    // Change the source material
    setSourceMaterial = new G4UIcmdWithAString("/apxs/setSourceMaterial",this);
    setSourceMaterial->SetGuidance("Change the material of the source into any of the predefined ones.");
    setSourceMaterial->SetParameterName("Material",false);
    setSourceMaterial->AvailableForStates(G4State_PreInit,G4State_Idle);
}

//----------------------- 8< -------------[ cut here ]------------------------

// Destructor
DetectorMessenger::~DetectorMessenger()
{
    delete setSourceMaterial;
    delete directory;
}

//----------------------- 8< -------------[ cut here ]------------------------

// Parse the commands
void DetectorMessenger::SetNewValue(G4UIcommand* command, G4String value)
{
    // PARSING ---------------------------------------------------------------
    if (command == setSourceMaterial){
        detectorConstruction->setSourceMaterial(value);
    }
    if (command == setTargetMaterial){
        detectorConstruction->setTargetMaterial(value);
    }
}