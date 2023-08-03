/************************
•     ┓             •    
┓┏┳┓┏┓┃┏┓┏┳┓┏┓┏┓╋┏┓╋┓┏┓┏┓
┗┛┗┗┣┛┗┗ ┛┗┗┗ ┛┗┗┗┻┗┗┗┛┛┗
    ┛                    
      Event Action

At the end of each event
recover the htis colle-
ction and save the data
to the analysis
************************/

// Include the relevant header
#include "EventAction.hh"

// Other useful Geant4 Headers
#include "G4AnalysisManager.hh"
#include "DetectorHit.hh"
#include "G4SystemOfUnits.hh"

// Before an event do nothing
void EventAction::BeginOfEventAction(const G4Event* event) 
{}

//----------------------- 8< -------------[ cut here ]------------------------

// At the end of an event add the info of each hit in the hitsCollection in the analysis
void EventAction::EndOfEventAction(const G4Event* event)
{
    // Get the master Hits Collection for this events that includes all (currently only 1) detectors
    auto eventHitsCollections   = event->GetHCofThisEvent();

    // Get the Analysis Manager
    auto analysisManager        = G4AnalysisManager::Instance();

    // Each detector has its own hits collection, loop through them
    for (G4int i=0; i< eventHitsCollections->GetNumberOfCollections(); i++){
        // Get the hits collection for the current detector
        auto eventHitsCollection = static_cast<HitsCollection*> (eventHitsCollections->GetHC(i));

        // For all the hits in the collection
        for (G4int j=0; j < eventHitsCollection->GetSize(); j++) {
            // Extract the jth event
            DetectorHit* hit = (*eventHitsCollection)[j];

            // Save the data in the table maintained by the analysis manager
            analysisManager->FillNtupleIColumn(0, event->GetEventID());
            analysisManager->FillNtupleIColumn(1, hit->getTrackID());
            analysisManager->FillNtupleDColumn(2, hit->getMigrantID());
            analysisManager->FillNtupleSColumn(3, hit->getParticle());
            analysisManager->FillNtupleDColumn(4, hit->getInitialEnergy()/keV);         // Divide to return the answer in keV
            analysisManager->FillNtupleDColumn(5, hit->getDepositedEnergy()/keV);       // Divide to return the answer in keV
            analysisManager->FillNtupleDColumn(6, hit->getPosition()[0]);
            analysisManager->FillNtupleDColumn(7, hit->getPosition()[1]);
            analysisManager->FillNtupleDColumn(8, hit->getPosition()[2]);
            analysisManager->FillNtupleDColumn(9, hit->getTime()/year);                 // Divide to return the answer in ns
            analysisManager->FillNtupleSColumn(10,hit->getVolume());
            analysisManager->FillNtupleSColumn(11,hit->getParentVolume());
            
            // Finish By adding a row
            analysisManager->AddNtupleRow();
        }
    } 
}