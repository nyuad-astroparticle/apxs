/*****************************
      ╔╦╗╦═╗╔═╗╔═╗╦╔═        
       ║ ╠╦╝╠═╣║  ╠╩╗        
       ╩ ╩╚═╩ ╩╚═╝╩ ╩        
╦╔╗╔╔═╗╔═╗╦═╗╔╦╗╔═╗╔╦╗╦╔═╗╔╗╔
║║║║╠╣ ║ ║╠╦╝║║║╠═╣ ║ ║║ ║║║║
╩╝╚╝╚  ╚═╝╩╚═╩ ╩╩ ╩ ╩ ╩╚═╝╝╚╝

An object attached to each
track that stores the first 
generation migrant id for the
that generated it.
*****************************/

#ifndef TrackInformation_hh
#define TrackInformation_hh

// The corresponding Geant4 class to extend
#include "G4VUserTrackInformation.hh"

class TrackInformation : public G4VUserTrackInformation
{
    public:
        TrackInformation(G4int id = -1);        // Constructor
        ~TrackInformation() override;           // Destructor

        // Mutators
        void  SetMigrantID(G4int);
        const G4int GetMigrantID();

    private:
        G4int migrantID;
};

#endif