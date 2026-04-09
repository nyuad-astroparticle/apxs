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

#include "G4VUserPrimaryGeneratorAction.hh"
#include "DetectorConstruction.hh"

#include <utility>

class G4Event;
class G4String;
class DetectorConstruction;
class G4GeneralParticleSource;

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
    public:
        PrimaryGeneratorAction(DetectorConstruction*);
        ~PrimaryGeneratorAction();

        void GeneratePrimaries(G4Event* event);

    private:
        void setParticleFromMaterial(G4Material * materialName);
        void setParticleFromName(const G4String &materialName);
        G4ThreeVector ComputeCentroidInMother(G4VPhysicalVolume* pv);
        std::pair<G4ThreeVector, G4ThreeVector> ComputeExtentInMother(G4VPhysicalVolume* pv);

        DetectorConstruction*       detectorConstruction;
        G4GeneralParticleSource*    source;
};

#endif
