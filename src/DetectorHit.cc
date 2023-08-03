/************************
•     ┓             •    
┓┏┳┓┏┓┃┏┓┏┳┓┏┓┏┓╋┏┓╋┓┏┓┏┓
┗┛┗┗┣┛┗┗ ┛┗┗┗ ┛┗┗┗┻┗┗┗┛┛┗
    ┛                    
       Detector Hit

The object create at each
interaction with a SD
that contains info about 
the interaction.
************************/


// Include the relevant header
#include "DetectorHit.hh"

//Other Useful Geant4 Headers
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4VisAttributes.hh"
#include "G4UnitsTable.hh"

// Firstly Initialize the HitAllocator to nothing, so that the new DetectorHit command doesn't explode
G4ThreadLocal G4Allocator<DetectorHit>* HitAllocator = nullptr;

//----------------------- 8< -------------[ cut here ]------------------------

// Set the equality operator
G4bool DetectorHit::operator==(const DetectorHit& right) const
{
    // Two hits are equal iff they have the same address
    return (this==&right) ? true : false;
}

//----------------------- 8< -------------[ cut here ]------------------------

// How to visualize hits
void DetectorHit::Draw()
{
    // Get the visualization manger for the scene
    G4VVisManager* particleVisManager = G4VVisManager::GetConcreteInstance();

    // If it does exists (i.e. we are running with visualization)
    if (particleVisManager) {
        
        G4Circle circle(position);                  // Add a circle where the hit happened
        circle.SetScreenSize(4.0);                  // Set the size in pixels
        circle.SetFillStyle(G4Circle::filled);      // Set fill
        G4Colour colour(0.6, 0.9, 0.2);             // Create the fill color
        G4VisAttributes attributes(colour);          
        circle.SetVisAttributes(attributes);        // Set the color of the circle

        particleVisManager->Draw(circle);           // Tell the visualization to add the circle
    }
}

// How to print the hit
void DetectorHit::Print(){

    // If we told it to be talkative
    if (verboseLevel > 0) G4cout
        << " trackID: "     << trackID
        << " Particle: "    << particle
        << " Einitial: "    << std::setw(7) << G4BestUnit(initialEnergy,    "Energy")
        << " Edeposited: "  << std::setw(7) << G4BestUnit(depositedEnergy,  "Energy")
        << " Position: "    << std::setw(7) << G4BestUnit(position,         "Length")
        << " Time: "        << std::setw(7) << G4BestUnit(time,             "Time")
        << " Volume: "      << volume
        << " ParentVolume: "<< parentVolume
        << G4endl;

}