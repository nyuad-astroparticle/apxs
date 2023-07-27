/****************************
          ┏┓┏┓┏┓┏┓┏┓
          ┣┫┃┃ ┃┃ ┗┓
          ┛┗┣┛┗┛┗┛┗┛
              •   
           ┏┳┓┏┓┓┏┓ 
          •┛┗┗┗┻┗┛┗•

This is where the simulation
starts. All modules are
loaded here

****************************/

// Import Geant4 Headers
#include "G4RunManagerFactory.hh"
#include "G4UIExecutive.hh"
#include "G4UImanager.hh"
#include "G4VisManager.hh"
#include "G4VisExecutive.hh"

// Import Custom Headers
#include "ActionInitialization.hh"
#include "DetectorConstruction.hh"
#include "PhysicsList.hh"


int main(int argc, char** argv) {
    
    // Set up the simulation
    // i.e. Configure the Run
    auto runManager = G4RunManagerFactory::CreateRunManager();  // This will automatically pick Sequential or MT Run Manager

    // Configure how each run is initialized
    runManager->SetUserInitialization(new ActionInitialization);
    runManager->SetUserInitialization(new DetectorConstruction);
    runManager->SetUserInitialization(new PhysicsList);

    // Punch it!
    runManager->Initialize();


    // Evaluate the arguments
    G4UIExecutive*  ui          = nullptr;                      // Controls the commands you input on the text field
    G4VisManager*   visManager  = nullptr;                      // Controls what you see
    G4UImanager*    uiManager   = G4UImanager::GetUIpointer();  // Controls executing commands in general


    // If there are no arguments run in visualization mode
    if (argc == 1){
        ui          = new G4UIExecutive(argc,argv);
        visManager  = new G4VisExecutive();
        visManager->Initialise();

        // Print stuff for debugging
        uiManager->ApplyCommand("/run/verbose 1");

        // Start the UI
        // Show Geometry and set parameters for viewing particle tracks
		uiManager->ApplyCommand("/vis/open OGL");
		uiManager->ApplyCommand("/vis/drawVolume");
		uiManager->ApplyCommand("/vis/viewer/set/autoRefresh true");
		uiManager->ApplyCommand("/vis/scene/add/trajectories smooth");
        ui->SessionStart();

        delete ui;
        delete visManager;
    }

    // Otherwise Run in Batch mode by loading the macro file
    // provided in the argument
    else if (argc > 1) {
        // Don't print anything
        // Unless it's a serious error
        uiManager->ApplyCommand("/run/verbose 0");
		uiManager->ApplyCommand("/hits/verbose 0");

        //Run the commands in batch mode
        for (int i=1;i<argc;i++){                               // For each input
            G4String command  = "/control/execute ";            // The command to execute it in Geant4
            G4String filename = argv[1];                        // The input filename
            uiManager->ApplyCommand(command + filename);        // Execute it
        }
    }

    delete runManager;
    return 0;

}