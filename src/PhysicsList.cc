/************************
•     ┓             •    
┓┏┳┓┏┓┃┏┓┏┳┓┏┓┏┓╋┏┓╋┓┏┓┏┓
┗┛┗┗┣┛┗┗ ┛┗┗┗ ┛┗┗┗┻┗┗┗┛┛┗
    ┛                    
       Physics List
************************/

// Include the relevant header file
#include "PhysicsList.hh" 

// Geant4 Physics Lists Constructors we are using
#include "G4EmStandardPhysics.hh"
#include "G4EmLivermorePhysics.hh"
#include "G4EmPenelopePhysics.hh"
#include "G4OpticalPhysics.hh"
#include "G4DecayPhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"


// Constructor
PhysicsList::PhysicsList() : G4VModularPhysicsList()
{
	// RegisterPhysics (new G4EmStandardPhysics());
	RegisterPhysics (new G4EmLivermorePhysics());
	// RegisterPhysics (new G4EmPenelopePhysics());
	RegisterPhysics (new G4OpticalPhysics());
	RegisterPhysics (new G4DecayPhysics());
	RegisterPhysics (new G4RadioactiveDecayPhysics());
}

//----------------------- 8< -------------[ cut here ]------------------------

// Destructor
PhysicsList::~PhysicsList()
{}