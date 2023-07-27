/************************************
        ╔═╗╦ ╦╦ ╦╔═╗╦╔═╗╔═╗
        ╠═╝╠═╣╚╦╝╚═╗║║  ╚═╗
        ╩  ╩ ╩ ╩ ╚═╝╩╚═╝╚═╝
            ╦  ╦╔═╗╔╦╗     
            ║  ║╚═╗ ║      
            ╩═╝╩╚═╝ ╩      

Define all the physical interactions
needed for the simulation.

Includes Radioactive Decay and some
stuff for correctly releasing energy
in metals.
************************************/

#ifndef PhysicsList_hh
#define PhysicsList_hh

// Geant4 Classes that we are using
#include "G4VModularPhysicsList.hh"
#include "G4EmStandardPhysics.hh"
#include "G4OpticalPhysics.hh"
#include "G4DecayPhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"

class PhysicsList : public G4VModularPhysicsList
{
    public:
        PhysicsList();      // Constructor
        ~PhysicsList();     // Destructor
};

#endif