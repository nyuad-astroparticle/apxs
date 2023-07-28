/****************************
   ╔╦╗╔═╗╔╦╗╔═╗╔═╗╔╦╗╔═╗╦═╗
    ║║║╣  ║ ║╣ ║   ║ ║ ║╠╦╝
   ═╩╝╚═╝ ╩ ╚═╝╚═╝ ╩ ╚═╝╩╚═
           ╦ ╦╦╔╦╗         
           ╠═╣║ ║          
           ╩ ╩╩ ╩          

Sensitive Detectors create
this object when a particle
crosses them. It stores
information about the inte-
raction.
****************************/

#ifndef DetectorHit_hh
#define DetectorHit_hh


// The Geant4 Header we are building on
#include "G4VHit.hh"

// Other useful Geant4 Classes
#include "G4ThreeVector.hh" 
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"

class DetectorHit : public G4VHit
{
    public:
        DetectorHit() = default;                                    // Default Constructor
        DetectorHit(const DetectorHit&) = default;                  // Create an object from another
        ~DetectorHit() override = default;                          // Default Destructor

        DetectorHit&    operator=(const DetectorHit&) = default;    // Assignment Operator
        G4bool          operator==(const DetectorHit&)  const;      // Equality Operator

        // We override these operators to add the HIT to the hits collection!
        inline void* operator new(size_t);                          // Assign enough memory and anonymously create an object
        inline void  operator delete(void*);                        // Delete the memory 

        // Methods to override
        void Draw() override;
        void Print() override;

        // Setters
        void setTrackID         (G4int trackID)             { this->trackID = trackID; }
        void setParticle        (G4String particle)         { this->particle = particle; }
        void setInitialEnergy   (G4double initialEnergy)    { this->initialEnergy = initialEnergy; }
        void setDepositedEnergy (G4double depositedEnergy)  { this->depositedEnergy = depositedEnergy; }
        void setPosition        (G4ThreeVector position)    { this->position = position; }
        void setTime            (G4double time)             { this->time = time; }
        void setVolume          (G4String volume)           { this->volume = volume; }

        // Getters
        G4int                   getTrackID()                { return trackID; }
        G4String                getParticle()               { return particle; }
        G4double                getInitialEnergy()          { return initialEnergy; }
        G4double                getDepositedEnergy()        { return depositedEnergy; }
        G4ThreeVector           getPosition()               { return position; }
        G4double                getTime()                   { return time; }
        G4String                getVolume()                 { return volume; }

    
    private:
    
        // Parameters that are stored in a hit
        G4int           trackID         = -1;
        G4String        particle        = "";
        G4double        initialEnergy   = 0;
        G4double        depositedEnergy = 0;
        G4ThreeVector   position        = G4ThreeVector(0.,0.,0.);
        G4double        time            = -1;
        G4String        volume          = "";

        // Set how much you should be printing out.
        G4int           verboseLevel    = 0;

};

//----------------------- 8< -------------[ cut here ]------------------------

// Define the overriden operators.

// Every time we say HitsCollection we mean to use the template with our custom class
using HitsCollection = G4THitsCollection<DetectorHit>;

// Tell the simulation to recognize an object called HitAllocator that is going to
// assign the hits in the collection above when they are created.
extern G4ThreadLocal G4Allocator<DetectorHit>* HitAllocator;

// What happens when you try to anonymously create a hit
inline void* DetectorHit::operator new(size_t)
{
    // If the first time you try to add a hit there is no allocator create it
    if(!HitAllocator) HitAllocator = new G4Allocator<DetectorHit>;

    // Allocate enough memory for one Hit in the Alocator and return its adress
    return (void*) HitAllocator->MallocSingle();
}

// What happens when we delete a hit object
inline void DetectorHit::operator delete(void* hit)
{
    // Free the memory and remove it from the allocator
    HitAllocator->FreeSingle((DetectorHit*) hit);
}

#endif