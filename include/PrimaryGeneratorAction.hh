/***************************************
            ╔═╗╦═╗╦╔╦╗╔═╗╦═╗╦ ╦    
            ╠═╝╠╦╝║║║║╠═╣╠╦╝╚╦╝    
            ╩  ╩╚═╩╩ ╩╩ ╩╩╚═ ╩     
        ╔═╗╔═╗╔╗╔╔═╗╦═╗╔═╗╔╦╗╔═╗╦═╗
        ║ ╦║╣ ║║║║╣ ╠╦╝╠═╣ ║ ║ ║╠╦╝
        ╚═╝╚═╝╝╚╝╚═╝╩╚═╩ ╩ ╩ ╚═╝╩╚═
            ╔═╗╔═╗╔╦╗╦╔═╗╔╗╔     
            ╠═╣║   ║ ║║ ║║║║     
            ╩ ╩╚═╝ ╩ ╩╚═╝╝╚╝     

Before an event is started  this class 
sets up the particles that are going 
to be in that event.

In this case it sets up the radioactive 
decays of the source material.
***************************************/

#ifndef PrimaryGeneratorAction_hh
#define PrimaryGeneratorAction_hh

// Include the Geant4 library we are extending
#include "G4VUserPrimaryGeneratorAction.hh"

// Other declarations to avoid importing headers for no reason
class G4Event;
class G4String;
class DetectorConstruction;
class G4GeneralParticleSource;

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
    public:
        PrimaryGeneratorAction(DetectorConstruction*);      // Constructor
        ~PrimaryGeneratorAction();                          // Destructor

        void GeneratePrimaries(G4Event* event);             // Runs before each event to set up what particles are going to be created

    private:
        void setParticleFromMaterial(G4String);

        DetectorConstruction*       detectorConstruction;   // The detector construction class that we will steal the source parameters from
        G4GeneralParticleSource*    source;                 // The Particle source class that will generate the decay products.
};

#endif