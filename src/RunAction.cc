/************************
•     ┓             •    
┓┏┳┓┏┓┃┏┓┏┳┓┏┓┏┓╋┏┓╋┓┏┓┏┓
┗┛┗┗┣┛┗┗ ┛┗┗┗ ┛┗┗┗┻┗┗┗┛┛┗
    ┛                    
        Run Action

Set up the analysis ma-
nager for each of the 
worker threads, and save
the data when done.
************************/

// Include the corresponding header file
#include "RunAction.hh"

// Include other useful headers
#include "G4Run.hh"
#include "G4AnalysisManager.hh"

// Include the Parallel Headers
#ifdef MPI_ENABLE
#include "G4MPImanager.hh"
#include "G4MPIntupleMerger.hh"
#endif

// Constructor
RunAction::RunAction()
{
    // If you are in parallel mode set up how to merge the files
    #ifdef MPI_ENABLE
    // Sets the merging of Ntuples for MPI
    if (G4MPImanager::GetManager()->GetTotalSize() >= 2) {
        G4int numberOfReducedNtupleFiles    = 0;
        G4bool rowWise                      = true;
        ntupleMerger    = new G4MPIntupleMerger(numberOfReducedNtupleFiles, rowWise);
    }

    // Enable table merging
    G4bool mergeNtuple  = false;
    #else
    // Set if tables are merged at the end, can only happen if you output ROOT0
    G4bool mergeNtuple  = false;
    #endif

    // Create a default filename for the output of the analysis manager
    // Can be changed using UI commands
    G4String defaultFileName = "APXS";

    // Initalize tha analysis manager with the default filename
    RunAction::BookAnalysis(defaultFileName,    mergeNtuple);
}

//----------------------- 8< -------------[ cut here ]------------------------

// Destructor
RunAction::~RunAction()
{
    // If you are running in parallel mode delete the table merger
    #ifdef MPI_ENABLE
    delete ntupleMerger;
    #endif
}

//----------------------- 8< -------------[ cut here ]------------------------

// Before the run starts reset the analysis and get ready
void RunAction::BeginOfRunAction(const G4Run* run)
{
    // Tell me you are starting a run
    if (isMaster) G4cout << ">>> Run " << run->GetRunID() << " starting..." << G4endl;

    // Get the analysis Manager
    auto analysisManager = G4AnalysisManager::Instance();

    // Remove any previous data entries
    analysisManager->Reset();

    // Reset the filename
    analysisManager->SetFileName("APXS");

    // Retrive the current filename string with the extension and whatnot
    G4String filename = analysisManager->GetFileName();
    
    // Add the run Number
    filename += "-run";
    filename += std::to_string(run->GetRunID());

    // For parallel mode we need to explicitly separate the files by process
    #ifdef MPI_ENABLE
    // Add the process number (AKA Rank)
    filename += "-rank";
    filename += std::to_string(G4MPImanager::GetManager()->GetRank());
    #endif

    // Open the new Analysis filename
    analysisManager->OpenFile(filename);
}

//----------------------- 8< -------------[ cut here ]------------------------

// At the end of the run save and close everything
void RunAction::EndOfRunAction(const G4Run* run)
{
    // Get the analysis Manager
    auto analysisManager = G4AnalysisManager::Instance();

    // In parallel mode we need to explicitly merge them
    #ifdef MPI_ENABLE
    // Retrive the rank of the current process
    const G4int rank    = G4MPImanager::GetManager()->GetRank();

    // Write the file if you are the main thread. 
    if (!G4MPImanager::GetManager()->IsExtraWorker()){
    	analysisManager->Write();
    	analysisManager->CloseFile(false);
    }
    
    #else
    // Write the leftovers and save the file without removing the data from analysis manager
    analysisManager->Write();
    analysisManager->CloseFile(false);
    #endif
}

//----------------------- 8< -------------[ cut here ]------------------------

// Static Function to Set Up the analysis for a new run
void RunAction::BookAnalysis(const G4String& filename, G4bool ntupleMerging)
{
    // Get the analysis Manager
    auto analysisManager = G4AnalysisManager::Instance();

    // Stop it from talking
    analysisManager->SetVerboseLevel(0);

    // Set the output file parameters
    analysisManager->SetDefaultFileType("csv");
    analysisManager->SetNtupleDirectoryName("output");
    analysisManager->SetFileName(filename);
    analysisManager->SetNtupleMerging(ntupleMerging);

    // Create the table that will store the hits
    analysisManager->CreateNtuple("hits","APXS Detector Hits");
    analysisManager->CreateNtupleIColumn("EventID");
    analysisManager->CreateNtupleIColumn("TrackID");
    analysisManager->CreateNtupleIColumn("MigrantID");
    analysisManager->CreateNtupleSColumn("Particle");
    analysisManager->CreateNtupleDColumn("InitialEnergy");
    analysisManager->CreateNtupleDColumn("DepositedEnergy");
    analysisManager->CreateNtupleDColumn("XPosition");
    analysisManager->CreateNtupleDColumn("YPosition");
    analysisManager->CreateNtupleDColumn("ZPosition");
    analysisManager->CreateNtupleDColumn("LocalTime");
    analysisManager->CreateNtupleSColumn("Volume");
    analysisManager->CreateNtupleSColumn("ParentVolume");
    analysisManager->CreateNtupleIColumn("ParentID");
    analysisManager->CreateNtupleSColumn("ProcessName");
    analysisManager->FinishNtuple();
}
