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
#include <iostream>

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
    // Change the source material currently not used
        setSourceMaterial               = new G4UIcmdWithAString("/apxs/setSourceMaterial",this);
        setSourceMaterial               ->SetGuidance("Change the material of the source into any of the predefined ones.");
        setSourceMaterial               ->SetParameterName("Material",false);
        setSourceMaterial               ->AvailableForStates(G4State_PreInit,G4State_Idle);

    // Select which volume should be the source at the moment
        setSourceVolume                 = new G4UIcmdWithAString("/apxs/setSourceVolume",this);
        setSourceVolume                 ->SetGuidance("Change which disk is the source");
        setSourceVolume                 ->SetParameterName("Disk volume",false);
        setSourceVolume                 ->AvailableForStates(G4State_PreInit,G4State_Idle);
    
    // Set the material of a picked volume
        setSourceMaterialAndName        = new G4UIcommand("/apxs/setSourceMaterialAndName", this);
        setSourceMaterialAndName        ->SetGuidance("Change the material of the source and set the source name.");
        setSourceMaterialAndName        ->SetGuidance("This command requires two parameters: Material and SourceName.");

        G4UIparameter* materialParam    = new G4UIparameter("Material", 's', false);
        materialParam                   ->SetGuidance("Material name.");
        setSourceMaterialAndName        ->SetParameter(materialParam);

        G4UIparameter* sourceNameParam  = new G4UIparameter("SourceName", 's', false);
        sourceNameParam                 ->SetGuidance("Source name.");
        setSourceMaterialAndName        ->SetParameter(sourceNameParam);

        setSourceMaterialAndName        ->AvailableForStates(G4State_PreInit, G4State_Idle);

    // Set target material duh
        setTargetMaterial               = new G4UIcmdWithAString("/apxs/setTargetMaterial",this);
        setTargetMaterial               ->SetGuidance("Change the material of the Target into any of the predefined ones.");
        setTargetMaterial               ->SetParameterName("Material",false);
        setTargetMaterial               ->AvailableForStates(G4State_PreInit,G4State_Idle);

    // Add a number of disk sources symmetrically spread around the circle
        createMultipleSources           = new G4UIcmdWithAString("/apxs/createMultipleSources", this);
        createMultipleSources           ->SetGuidance("Add a number of disks around the detector");
        createMultipleSources           ->SetParameterName("Number", false);
        createMultipleSources           ->AvailableForStates(G4State_PreInit,G4State_Idle);

    // Tilt the target surface

        setTiltAngle        = new G4UIcommand("/apxs/setTiltAngle", this);
        setTiltAngle        ->SetGuidance("Title the target below about x and z axis by this angle in degrees.");
        setTiltAngle        ->SetGuidance("This command requires two parameters: x angle and z angle.");

        G4UIparameter* x    = new G4UIparameter("xAngle", 's', false);
        x                   ->SetGuidance("Material name.");
        setSourceMaterialAndName        ->SetParameter(x);

        G4UIparameter* z  = new G4UIparameter("zAngle", 's', false);
        z                 ->SetGuidance("Source name.");
        setTiltAngle        ->SetParameter(z);

        setTiltAngle        ->AvailableForStates(G4State_PreInit, G4State_Idle);

        // setTiltAngle = new G4UIcmdWithAString("/apxs/setTiltAngle",this);
        // setTiltAngle->SetGuidance("Title the target below about x axis by this angle in degrees");
        // setTiltAngle->SetParameterName("Angle", false);
        // setTiltAngle->AvailableForStates(G4State_PreInit, G4State_Idle);

}

//----------------------- 8< -------------[ cut here ]------------------------

// Destructor
DetectorMessenger::~DetectorMessenger()
{
    delete setSourceMaterial;
    delete setTargetMaterial;
    delete createMultipleSources;
    delete directory;
    delete setSourceMaterialAndName;
}

//----------------------- 8< -------------[ cut here ]------------------------

// Parse the commands
void DetectorMessenger::SetNewValue(G4UIcommand* command, G4String value)
{
    // PARSING ---------------------------------------------------------------
    if (command == setSourceMaterialAndName) {
        std::istringstream iss(value);
        G4String material, sourceName;
        iss >> material >> sourceName;
        detectorConstruction->setSourceMaterialAndName(material, sourceName);
    }
    if (command == setSourceMaterial){
        detectorConstruction->setSourceMaterial(value);
    }
    if (command == setTargetMaterial){
        detectorConstruction->setTargetMaterial(value);
    }
    if (command == createMultipleSources){
        detectorConstruction->createMultipleSources(std::stoi(value));
    }
    if (command == setSourceVolume)
    {
        detectorConstruction->setSourceVolume(value);
    }
    if (command == setTiltAngle)
    {
        // detectorConstruction->tiltTarget(std::stoi(value));

        std::istringstream iss(value);
        G4String x, z;
        iss >> x >> z;
        detectorConstruction->tiltTarget(std::stoi(x), std::stoi(z));
    }
}