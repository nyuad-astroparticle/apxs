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

// Constructor
RunAction::RunAction()
{
    // Create a default filename for the output of the analysis manager
    // Can be changed using UI commands
    G4String defaultFileName = "APXS";

    // Initalize tha analysis manager with the default filename
    RunAction::BookAnalysis(defaultFileName);
}

//----------------------- 8< -------------[ cut here ]------------------------

// Destructor
RunAction::~RunAction() = default;

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

    // Open the new Analysis filename
    analysisManager->OpenFile(analysisManager->GetFileName()); 
}

//----------------------- 8< -------------[ cut here ]------------------------

// At the end of the run save and close everything
void RunAction::EndOfRunAction(const G4Run* run)
{
    // Get the analysis Manager
    auto analysisManager = G4AnalysisManager::Instance();

    // Write the leftovers and save the file without removing the data from analysis manager
    analysisManager->Write();
    analysisManager->CloseFile(false);
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
    analysisManager->CreateNtupleSColumn("Particle");
    analysisManager->CreateNtupleDColumn("InitialEnergy");
    analysisManager->CreateNtupleDColumn("DepositedEnergy");
    analysisManager->CreateNtupleDColumn("XPosition");
    analysisManager->CreateNtupleDColumn("YPosition");
    analysisManager->CreateNtupleDColumn("ZPosition");
    analysisManager->CreateNtupleDColumn("LocalTime");
    analysisManager->CreateNtupleSColumn("Volume");
    analysisManager->FinishNtuple();
}