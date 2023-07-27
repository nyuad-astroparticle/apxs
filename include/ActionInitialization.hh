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

class ActionInitialization : public G4VUserActionInitialization
{
    public:
        ActionInitialization();                 // Constructor
        ~ActionInitialization();                // Destructor

        void Build() const override;            // What needs to be done for every thread
        void BuildForMaster() const override;   // What needs to be done on the master thread (only in case of MT)
};

#endif

