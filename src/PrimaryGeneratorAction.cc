/************************
•     ┓             •    
┓┏┳┓┏┓┃┏┓┏┳┓┏┓┏┓╋┏┓╋┓┏┓┏┓
┗┛┗┗┣┛┗┗ ┛┗┗┗ ┛┗┗┗┻┗┗┗┛┛┗
    ┛                    
Primary Generator Action
************************/

// Include the relevant Header file
#include "PrimaryGeneratorAction.hh"

// Include Other useful Geant4 Headers
#include "G4GeneralParticleSource.hh"
#include "G4ParticleDefinition.hh"
#include "G4IonTable.hh"
#include "G4Event.hh"
#include "DetectorConstruction.hh"
#include "G4SystemOfUnits.hh"
#include "G4String.hh"


// Constructor
PrimaryGeneratorAction::PrimaryGeneratorAction(DetectorConstruction* construction)
{
    // Initialize Class variables
    detectorConstruction    = construction;
    source                  = new G4GeneralParticleSource();
}

//----------------------- 8< -------------[ cut here ]------------------------

//Destructor
PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
    delete source;
}

//----------------------- 8< -------------[ cut here ]------------------------


// Function that sets up the primary particles
void PrimaryGeneratorAction::GeneratePrimaries(G4Event* event)
{
    // Get the material from Detector construction
    G4String material = detectorConstruction->sourceMaterial->GetName();
    setParticleFromMaterial(material);

    // Select the number of particles to generate per event
    source->SetNumberOfParticles(100);

    // Set the energy of the initial particle at 0
    source->GetCurrentSource()->GetEneDist()->SetMonoEnergy(0.0 * keV);

    // Confine the particle source to the geometry of the source block
    source->GetCurrentSource()->GetPosDist()->SetPosDisType("Surface");
	source->GetCurrentSource()->GetPosDist()->SetPosDisShape("Cylinder");
	source->GetCurrentSource()->GetPosDist()->SetCentreCoords(G4ThreeVector( 35.2 * cm / 2 + 1.*cm, -106. * cm / 2 + 11.5*cm + 17*cm, 0));
	source->GetCurrentSource()->GetPosDist()->SetRadius0(0);
    source->GetCurrentSource()->GetPosDist()->SetRadius(detectorConstruction->sourceDiameter/2);
    source->GetCurrentSource()->GetPosDist()->SetHalfZ(detectorConstruction->sourceThickness/2);
    source->GetCurrentSource()->GetPosDist()->SetCentreCoords(detectorConstruction->sourcePosition);
    source->GetCurrentSource()->GetPosDist()->SetPosRot1(detectorConstruction->sourceRotation->colX());
    source->GetCurrentSource()->GetPosDist()->SetPosRot2(detectorConstruction->sourceRotation->colY());

    // Generate the particle
	source->GeneratePrimaryVertex(event);
}

//----------------------- 8< -------------[ cut here ]------------------------


// Given a material string find the corresponding particle definitions
void PrimaryGeneratorAction::setParticleFromMaterial(G4String material)
{
    // Start checking for the available materials

    // CURIUM --------------------------------------------------------------
    if (!material.compare("G4_Cm"))
    {
        G4ParticleDefinition* curium244 = G4IonTable::GetIonTable()->GetIon(96, 244, 0);
        source->SetParticleDefinition(curium244);
    }

    // IRON ----------------------------------------------------------------
    else if (!material.compare("G4_Fe"))
    {
        G4ParticleDefinition* iron55 = G4IonTable::GetIonTable()->GetIon(26, 55, 0);
        source->SetParticleDefinition(iron55);
    }

    // CADMIUM -------------------------------------------------------------
    else if (!material.compare("G4_Cd"))
    {
        G4ParticleDefinition* cadmium109 = G4IonTable::GetIonTable()->GetIon(48, 109, 0);
        source->SetParticleDefinition(cadmium109);
    }

    // COBALT --------------------------------------------------------------
    else if (!material.compare("G4_Co"))
    {
        G4ParticleDefinition* cobalt60 = G4IonTable::GetIonTable()->GetIon(27, 60, 0);
        source->SetParticleDefinition(cobalt60);
    }

    // AMERICIUM -----------------------------------------------------------
    else if (!material.compare("G4_Am"))
    {
        G4ParticleDefinition* americium241 = G4IonTable::GetIonTable()->GetIon(95, 241, 0);
        source->SetParticleDefinition(americium241);
    }

    // DEFAULT -------------------------------------------------------------
    else {
        G4cerr << "The material selected for the source is not in the candidates. Please select another one" << G4endl;
    }
}