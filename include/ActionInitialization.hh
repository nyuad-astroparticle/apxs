/********************************
        ╔═╗╔═╗╔╦╗╦╔═╗╔╗╔          
        ╠═╣║   ║ ║║ ║║║║          
        ╩ ╩╚═╝ ╩ ╩╚═╝╝╚╝          
╦╔╗╔╦╔╦╗╦╔═╗╦  ╦╔═╗╔═╗╔╦╗╦╔═╗╔╗╔  
║║║║║ ║ ║╠═╣║  ║╔═╝╠═╣ ║ ║║ ║║║║  
╩╝╚╝╩ ╩ ╩╩ ╩╩═╝╩╚═╝╩ ╩ ╩ ╩╚═╝╝╚╝

Defines what custom steps are
needed before starting shooting
particles
*******************************/

#ifndef ActionInitialization_hh
#define ActionInitialization_hh

// Include relevant Geant4 Libraries
#include "G4VUserActionInitialization.hh"

// Inline class definitions so that we don't load the headers here
class DetectorConstruction;

class ActionInitialization : public G4VUserActionInitialization
{
    public:
        ActionInitialization(DetectorConstruction*);    // Constructor
        ~ActionInitialization();                        // Destructor

        void Build() const override;                    // What needs to be done for every thread
        void BuildForMaster() const override;           // What needs to be done on the master thread (only in case of MT)

    private:
        DetectorConstruction* detectorConstruction;     // A reference to the detector construction to be passed to the Primary Generator Action
        
};

#endif

