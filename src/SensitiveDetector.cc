/************************
•     ┓             •    
┓┏┳┓┏┓┃┏┓┏┳┓┏┓┏┓╋┏┓╋┓┏┓┏┓
┗┛┗┗┣┛┗┗ ┛┗┗┗ ┛┗┗┗┻┗┗┗┛┛┗
    ┛                    
    Sensitive Detector

Attach to a volume to
create hits every time a 
particle passes through
************************/

// Include the necessary header
#include "SensitiveDetector.hh"

// Other useful Geant4 headers
#include "G4SDManager.hh"
#include "G4String.hh"
#include "TrackInformation.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include <cmath>

// Constructor Calls the parent constructor with the same name and...
SensitiveDetector::SensitiveDetector(const G4String& name, const G4String& hitsCollectionName)
: G4VSensitiveDetector(name)
{
    // The G4VSensitiveDetector has a static member called CollectionName and it stores and  
    // manages all the collections, here we add this detector's collection name to the manager.
    collectionName.insert(hitsCollectionName);
}

//----------------------- 8< -------------[ cut here ]------------------------

// Run once before the event is processed
// The parameter passed is the big hits collection that should contain everything at the end of the run
void SensitiveDetector::Initialize(G4HCofThisEvent* hitsCollection)
{
    // Set the detector's hit collection to the name we provided in the constructor 
    // (but access though the collectionName member)
    this->hitsCollection = new HitsCollection(SensitiveDetectorName, collectionName[0]);

    // We can extract the unique id of our hits collection by asking the Sensitive Detector Manager
    G4int hitsCollectionId = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);

    // Add our hits collection to the Event's Hits collection
    hitsCollection->AddHitsCollection(hitsCollectionId, this->hitsCollection);
}

//----------------------- 8< -------------[ cut here ]------------------------

// What happens after each step?
G4bool SensitiveDetector::ProcessHits(G4Step* step, G4TouchableHistory* history)
{
    // Check if particle is not neutrino
    G4String particleType = step->GetTrack()->GetParticleDefinition()->GetParticleName();
    if (G4StrUtil::contains(particleType,"nu")) return false;

    // Check if the particle has a parent volume
    G4String parentVolume = "NA";
    if (step->GetTrack()->GetOriginTouchable()){
        parentVolume = step->GetTrack()->GetOriginTouchable()->GetVolume()->GetName();
    }

    // Get the migrantID if any
    G4int migrantID = -1;
    if (step->GetTrack()->GetUserInformation()){
        TrackInformation* info  = static_cast<TrackInformation*> (step->GetTrack()->GetUserInformation());
        migrantID               = info->GetMigrantID();
    }

    G4String processName = "Primary";
    if (step->GetTrack()->GetCreatorProcess())
    {
        processName = step->GetTrack()->GetCreatorProcess()->GetProcessName();
    }

    // Create the hit
    DetectorHit* hit        = new DetectorHit();
    hit->setTrackID         (step->GetTrack()->GetTrackID());
    hit->setMigrantID       (migrantID);
    hit->setParticle        (step->GetTrack()->GetParticleDefinition()->GetParticleName());
    hit->setInitialEnergy   (step->GetTrack()->GetVertexKineticEnergy());
    hit->setInitialPosition (step->GetTrack()->GetVertexPosition());
    hit->setDepositedEnergy (step->GetTotalEnergyDeposit());
    hit->setPosition        (step->GetPostStepPoint()->GetPosition());
    hit->setTime            (step->GetPostStepPoint()->GetGlobalTime());
    hit->setVolume          (step->GetPreStepPoint()->GetPhysicalVolume()->GetName());
    hit->setParentVolume    (parentVolume);
    hit->setParentID        (step->GetTrack()->GetParentID());
    hit->setProcessName     (processName);

    // Momenta: initial from vertex KE + mass, final from post-step point
    const auto* definition    = step->GetTrack()->GetParticleDefinition();
    const G4double mass       = definition->GetPDGMass();
    const G4double ke0        = step->GetTrack()->GetVertexKineticEnergy(); // MeV
    const G4ThreeVector dir0  = step->GetTrack()->GetVertexMomentumDirection(); // unit vector
    // if (ke0 >= 0.0) {
        // if (mass > 0.0) {
        //     const G4double e0 = ke0 + mass; // total energy
        //     const G4double p2 = e0*e0 - mass*mass;
        //     const G4double p0 = (p2 > 0.0) ? std::sqrt(p2) : 0.0; // MeV (Geant4 momentum units)
        //     hit->setInitialMomentum(dir0 * p0);
        // } else {
            // massless: p = E/c, but in G4 units momentum is also in MeV, so p = E
    hit->setInitialMomentum(dir0);
        // }
    // } else {
        // hit->setInitialMomentum(G4ThreeVector(0.,0.,0.));
    // }

    // Final momentum at detector: use pre-step momentum (entering step) so it is non-zero even if the track is killed at this step.
    // hit->setMomentum(step->GetPreStepPoint()->GetMomentum()); // MeV

    // Add the hit to the hits collection
    hitsCollection->insert(hit);

    return true;
}

//----------------------- 8< -------------[ cut here ]------------------------

// What happens after the event is done
void SensitiveDetector::EndOfEvent(G4HCofThisEvent* hitsCollection)
{
    // If you are allowed to talk say that you're done
    if (verboseLevel > 1) {
        // Get the number of hits recorded for all detectors
        G4int nofHits = this->hitsCollection->entries();
        G4cout  << G4endl
                << "-------->Hits Collection: in this event they are " << nofHits
                << " hits on the detector. Here they are: " << G4endl;

        // Print the hits
        for (G4int i=0; i < nofHits; i++) (*(this->hitsCollection))[i]->Print();
    }
}