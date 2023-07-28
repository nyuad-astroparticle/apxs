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

    // Set the energy of the initial particle at 0
    source->GetCurrentSource()->GetEneDist()->SetMonoEnergy(0.0 * keV);

    // Confine the particle source to the geometry of the source block
    source->GetCurrentSource()->GetPosDist()->SetPosDisType("Volume");
	source->GetCurrentSource()->GetPosDist()->SetPosDisShape("Cylinder");
	source->GetCurrentSource()->GetPosDist()->SetCentreCoords(G4ThreeVector( 35.2 * cm / 2 + 1.*cm, -106. * cm / 2 + 11.5*cm + 17*cm, 0));
	source->GetCurrentSource()->GetPosDist()->SetRadius0(0);
    source->GetCurrentSource()->GetPosDist()->SetRadius(detectorConstruction->sourceDiameter/2);
    source->GetCurrentSource()->GetPosDist()->SetHalfY(detectorConstruction->sourceThickness/2);
    source->GetCurrentSource()->GetPosDist()->SetCentreCoords(detectorConstruction->sourcePostition);
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
    if (!material.compare("G4_Cu"))
    {
        G4ParticleDefinition* curium242 = G4IonTable::GetIonTable()->GetIon(96, 242, 0);
        source->SetParticleDefinition(curium242);
    }

    // DEFAULT -------------------------------------------------------------
    else {
        G4cerr << "The material selected for the source is not in the candidates. Please select another one" << G4endl;
    }
}