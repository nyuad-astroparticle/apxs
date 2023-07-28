/*************************
 ╔═╗╔═╗╔╗╔╔═╗╦╔╦╗╦╦  ╦╔═╗
 ╚═╗║╣ ║║║╚═╗║ ║ ║╚╗╔╝║╣ 
 ╚═╝╚═╝╝╚╝╚═╝╩ ╩ ╩ ╚╝ ╚═╝
 ╔╦╗╔═╗╔╦╗╔═╗╔═╗╔╦╗╔═╗╦═╗
  ║║║╣  ║ ║╣ ║   ║ ║ ║╠╦╝
 ═╩╝╚═╝ ╩ ╚═╝╚═╝ ╩ ╚═╝╩╚═

Attach to a Logical Volume
to register particles
passing through and record
their properties
*************************/

#ifndef SensitiveDetector_hh
#define SensitiveDetector_hh

// Geant4 Method to override
#include "G4VSensitiveDetector.hh"

// Other Useful headers
#include "DetectorHit.hh"

class SensitiveDetector : public G4VSensitiveDetector 
{
    public:
        SensitiveDetector(const G4String& name, const G4String& hitsCollectionName);    // Constructor
        ~SensitiveDetector() override = default;                                        // Destructor

        // Methods to override from Parent Class
        void Initialize(G4HCofThisEvent* hitsCollection) override;                      // Run at the start of each event. Set up the collection
        G4bool ProcessHits(G4Step* step, G4TouchableHistory* history) override;         // At every step create the hits if the SD is triggered
        void EndOfEvent(G4HCofThisEvent* hitsCollection) override;                      // Run at the end of each event, save the information in the hits

    private:
        // The hits collection of this sensitive detector
        // At the end all the collections of all the sensitive detectors are combined
        HitsCollection* hitsCollection = nullptr;

};



#endif