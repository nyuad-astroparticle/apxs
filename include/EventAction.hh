/*******************
  ╔═╗╦  ╦╔═╗╔╗╔╔╦╗
  ║╣ ╚╗╔╝║╣ ║║║ ║ 
  ╚═╝ ╚╝ ╚═╝╝╚╝ ╩ 
  ╔═╗╔═╗╔╦╗╦╔═╗╔╗╔
  ╠═╣║   ║ ║║ ║║║║
  ╩ ╩╚═╝ ╩ ╩╚═╝╝╚╝

What to do before
and after an event

Saves the data from
the hits collection
to the analysis
*******************/

#ifndef EventAction_hh
#define EventAction_hh

// The class we will be inheriting from
#include "G4UserEventAction.hh"

// Other useful headers
#include "G4Event.hh"

class EventAction : public G4UserEventAction
{
    public: 
        EventAction() = default;
        ~EventAction() override = default;

        void BeginOfEventAction(const G4Event*) override;
        void   EndOfEventAction(const G4Event*) override;
};


#endif