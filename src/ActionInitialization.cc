/************************
•     ┓             •    
┓┏┳┓┏┓┃┏┓┏┳┓┏┓┏┓╋┏┓╋┓┏┓┏┓
┗┛┗┗┣┛┗┗ ┛┗┗┗ ┛┗┗┗┻┗┗┗┛┛┗
    ┛                    
  Action Initialization
************************/

// Import the header file
#include "ActionInitialization.hh"

// Include the rest of our custom initializations
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "EventAction.hh"

// Constructor
ActionInitialization::ActionInitialization(DetectorConstruction* construction)
{
    detectorConstruction = construction;
}

//----------------------- 8< -------------[ cut here ]------------------------

// Destructor
ActionInitialization::~ActionInitialization()
{}

//----------------------- 8< -------------[ cut here ]------------------------

// Build for Master
void ActionInitialization::BuildForMaster() const
{
    SetUserAction(new RunAction());
}

//----------------------- 8< -------------[ cut here ]------------------------

// Build for Master
void ActionInitialization::Build() const
{
    SetUserAction(new RunAction);
    SetUserAction(new EventAction);
    SetUserAction(new PrimaryGeneratorAction(detectorConstruction));
}

