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
#include "G4UIExecutive.hh"
#include "G4UImanager.hh"
#include "G4VisManager.hh"
#include "G4VisExecutive.hh"

// Import Custom Headers
#include "ActionInitialization.hh"
#include "DetectorConstruction.hh"
#include "PhysicsList.hh"

#ifdef MPI_ENABLE // For a parallel simulation
// Include headers that create a parallel run
#include "G4MPImanager.hh"
#include "G4MPIsession.hh"
#include "G4RunManager.hh"

#else // For a sequential Simulation
// Include headers that create a sequential or MT run
#include "G4RunManagerFactory.hh"
#endif

int main(int argc, char** argv) {

    // Flags
    G4String world = "./geometry/sdd-C1.gdml";
    G4String macro = "";

    for (G4int i = 1; i < argc; i = i+2)
    {
        G4String flagValue = argv[i];
        if (flagValue == "-g") world = argv[i+1];
        if (flagValue == "-m") macro = argv[i+1];

    }
    
    // Variables to hold the new argc and argv for MPI
    int newArgc = 1; // Start with 1 to include the program name
    char* newArgv[3]; // Assuming a maximum of two additional arguments: flag and its value

    newArgv[0] = argv[0]; // The program name remains unchanged

    // Search for the macro file flag and its value
    for (int i = 1; i < argc; i++) {
        if (std::string(argv[i]) == "-m" && i + 1 < argc) {
            // Found the macro file flag, prepare newArgv for MPI
            // newArgv[newArgc++] = argv[i];     // Include "-m"
            newArgv[newArgc++] = argv[i + 1]; // Include the macro file path
            break; // Assuming only one macro file is specified
        }
    }
    
    // Set up the simulation
    G4VisManager*   visManager  = nullptr;                                  // Controls what you see
    G4UImanager*    uiManager   = G4UImanager::GetUIpointer();              // Controls executing commands in general

    // i.e. Configure the Run
    #ifdef MPI_ENABLE
    G4MPImanager*   mpiManager  = new G4MPImanager(newArgc, newArgv);             // Creates Manager for contacting the OpenMPI library
    G4MPIsession*   ui          = mpiManager->GetMPIsession();              // Manages the UI in Multiprocessing mode
    G4RunManager*   runManager  = new G4RunManager();

    // Set some parameters for the mpi manager
    mpiManager->SetVerbose(1);

    #else
    auto runManager             = G4RunManagerFactory::CreateRunManager();  // This will automatically pick Sequential or MT Run Manager
    G4UIExecutive*  ui          = nullptr;                                  // Controls the commands you input on the text field
    #endif

    // Configure how each run is initialized
    DetectorConstruction* detectorConstruction = new DetectorConstruction(world);
    runManager->SetUserInitialization(new PhysicsList);
    runManager->SetUserInitialization(detectorConstruction);
    runManager->SetUserInitialization(new ActionInitialization(detectorConstruction));

    // Punch it!
    runManager->Initialize();

    // start if you are in parallel mode
    #ifdef MPI_ENABLE
    ui->SessionStart();
    delete mpiManager;

    // Otherwise process the arguments
    #else

    // Evaluate the arguments
    // If there are no arguments run in visualization mode
    if (macro == ""){
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
        uiManager->ApplyCommand("/vis/modeling/trajectories/create/drawByParticleID");
        uiManager->ApplyCommand("/vis/scene/endOfEventAction accumulate -1");
		uiManager->ApplyCommand("/vis/scene/add/trajectories smooth");
        uiManager->ApplyCommand("/vis/geometry/set/forceSolid");
        ui->SessionStart();

        delete ui;
        delete visManager;
    }

    // Otherwise Run in Batch mode by loading the macro file
    // provided in the argument
    else {
        // Don't print anything
        // Unless it's a serious error
        uiManager->ApplyCommand("/run/verbose 0");
		uiManager->ApplyCommand("/hits/verbose 0");

        //Run the commands in batch mode
        // for (int i=1;i<argc;i++){                               // For each input
        //     G4String command  = "/control/execute ";            // The command to execute it in Geant4
        //     G4String filename = macro;                        // The input filename
        //     uiManager->ApplyCommand(command + filename);        // Execute it
        // }

        G4String command  = "/control/execute ";            // The command to execute it in Geant4
        G4String filename = macro;                        // The input filename
        uiManager->ApplyCommand(command + filename);        // Execute it

    }
    #endif

    delete runManager;
    return 0;

}