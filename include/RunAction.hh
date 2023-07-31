/****************************
 ╦═╗╦ ╦╔╗╔  ╔═╗╔═╗╔╦╗╦╔═╗╔╗╔
 ╠╦╝║ ║║║║  ╠═╣║   ║ ║║ ║║║║
 ╩╚═╚═╝╝╚╝  ╩ ╩╚═╝ ╩ ╩╚═╝╝╚╝

Describes what should be
done before a run starts.

Includes setting up analusis
and other goodies.
****************************/

#ifndef RunAction_hh
#define RunAction_hh

// Include the header we will be working with
#include "G4UserRunAction.hh"

// Other Useful headers
#include "G4String.hh"

// Inline definition for Parallel simulations
#ifdef MPI_ENABLE
class G4MPIntupleMerger;        
#endif

class RunAction : public G4UserRunAction 
{
    public:
        RunAction();                                        // Constructor
        ~RunAction();                                       // Destructor

        // Methods to override
        void BeginOfRunAction(const G4Run*) override;       // Start the Analysis Manager
        void   EndOfRunAction(const G4Run*) override;       // Save the data and go home

    private:
        // This static method starts an analysis manager with a particular filename
        // at the beginning of the run
        static void BookAnalysis(const G4String& filename, G4bool ntupleMerging = false);

        // If you are using parallel run, create a manager for merging the output files
        #ifdef MPI_ENABLE
        G4MPIntupleMerger* ntupleMerger;
        #endif

};

#endif