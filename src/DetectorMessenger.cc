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
        // sourceMaterial               = new G4UIcmdWithAString("/apxs/sourceMaterial",this);
        // sourceMaterial               ->SetGuidance("Change the material of the source into any of the predefined ones.");
        // sourceMaterial               ->SetParameterName("Material",false);
        // sourceMaterial               ->AvailableForStates(G4State_PreInit,G4State_Idle);

    // Select which volume should be the source at the moment
        activeVolume                 = new G4UIcmdWithAString("/apxs/activeVolume",this);
        activeVolume                 ->SetGuidance("Change which disk is the source");
        activeVolume                 ->SetParameterName("Disk volume",false);
        activeVolume                 ->AvailableForStates(G4State_PreInit,G4State_Idle);
    
    // Set the material of a picked volume
        sourceMaterialAndName        = new G4UIcommand("/apxs/sourceMaterialAndName", this);
        sourceMaterialAndName        ->SetGuidance("Change the material of the source and set the source name.");
        sourceMaterialAndName        ->SetGuidance("This command requires two parameters: Material and SourceName.");

        G4UIparameter* materialParam    = new G4UIparameter("Material", 's', false);
        materialParam                   ->SetGuidance("Material name.");
        sourceMaterialAndName        ->SetParameter(materialParam);

        G4UIparameter* sourceNameParam  = new G4UIparameter("SourceName", 's', false);
        sourceNameParam                 ->SetGuidance("Source name.");
        sourceMaterialAndName        ->SetParameter(sourceNameParam);

        sourceMaterialAndName        ->AvailableForStates(G4State_PreInit, G4State_Idle);

    // Set target material duh
        targetMaterial               = new G4UIcmdWithAString("/apxs/targetMaterial",this);
        targetMaterial               ->SetGuidance("Change the material of the Target into any of the predefined ones.");
        targetMaterial               ->SetParameterName("Material",false);
        targetMaterial               ->AvailableForStates(G4State_PreInit,G4State_Idle);

    // Add a number of disk sources symmetrically spread around the circle
        createMultipleSources           = new G4UIcmdWithAString("/apxs/createMultipleSources", this);
        createMultipleSources           ->SetGuidance("Add a number of disks around the detector");
        createMultipleSources           ->SetParameterName("Number", false);
        createMultipleSources           ->AvailableForStates(G4State_PreInit,G4State_Idle);

    // Tilt the target surface

        tiltAngle        = new G4UIcommand("/apxs/tiltAngle", this);
        tiltAngle        ->SetGuidance("Tilt the target below about x and z axis by this angle in degrees.");
        tiltAngle        ->SetGuidance("This command requires two parameters: x angle and z angle.");

        G4UIparameter* x    = new G4UIparameter("xAngle", 's', false);
        x                   ->SetGuidance("Material name.");
        tiltAngle        ->SetParameter(x);

        G4UIparameter* z  = new G4UIparameter("zAngle", 's', false);
        z                 ->SetGuidance("Source name.");
        tiltAngle        ->SetParameter(z);

        tiltAngle        ->AvailableForStates(G4State_PreInit, G4State_Idle);

    // Choose atmosphere 
// Choose atmosphere with two components
        atmo = new G4UIcommand("/apxs/atmo", this);
        atmo->SetGuidance("Choose your atmosphere by specifying two components.");
        atmo->SetGuidance("This command requires two parameters: atmosphereName and pressure. For Mars pressure value is irrelevant");

        G4UIparameter* comp1 = new G4UIparameter("atmosphereName", 's', false);
        comp1->SetGuidance("Atmosphere to select: AIR, CO2, Vacuum, and Mars.");
        atmo->SetParameter(comp1);

        G4UIparameter* comp2 = new G4UIparameter("pressure", 's', false);
        comp2->SetGuidance("Pressure in millibars");
        atmo->SetParameter(comp2);

        atmo->AvailableForStates(G4State_PreInit, G4State_Idle);
}

//----------------------- 8< -------------[ cut here ]------------------------

// Destructor
DetectorMessenger::~DetectorMessenger()
{
    // delete sourceMaterial;
    delete targetMaterial;
    delete createMultipleSources;
    delete directory;
    delete sourceMaterialAndName;
    delete activeVolume;
    delete atmo;
}

//----------------------- 8< -------------[ cut here ]------------------------

// Parse the commands
void DetectorMessenger::SetNewValue(G4UIcommand* command, G4String value)
{
    // PARSING ---------------------------------------------------------------
    if (command == sourceMaterialAndName) {
        std::istringstream iss(value);
        G4String material, sourceName;
        iss >> material >> sourceName;
        detectorConstruction->setSourceMaterialAndName(material, sourceName);
    }
    // if (command == sourceMaterial){
    //     detectorConstruction->setSourceMaterial(value);
    // }
    if (command == targetMaterial){
        detectorConstruction->setTargetMaterial(value);
    }
    if (command == createMultipleSources){
        detectorConstruction->createMultipleSources(std::stoi(value));
    }
    if (command == activeVolume)
    {
        detectorConstruction->setSourceVolume(value);
    }
    if (command == tiltAngle)
    {
        // detectorConstruction->tiltTarget(std::stoi(value));

        std::istringstream iss(value);
        G4String x, z;
        iss >> x >> z;
        detectorConstruction->tiltTarget(std::stod(x), std::stod(z));
    }
    if (command == atmo)
    {
        std::istringstream iss(value);
        G4String atmosphereName, pressure;
        iss >> atmosphereName >> pressure;
        detectorConstruction->atmosphere(atmosphereName, std::stod(pressure));
    }
}