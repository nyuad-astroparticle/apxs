/************************
•     ┓             •    
┓┏┳┓┏┓┃┏┓┏┳┓┏┓┏┓╋┏┓╋┓┏┓┏┓
┗┛┗┗┣┛┗┗ ┛┗┗┗ ┛┗┗┗┻┗┗┗┛┛┗
    ┛                    
Primary Generator Action
************************/

// Include the relevant Header file
#include "PrimaryGeneratorAction.hh"

// Include Other useful Geant4 Headers
#include "G4GeneralParticleSource.hh"
#include "G4ParticleDefinition.hh"
#include "G4IonTable.hh"
#include "G4Gamma.hh"
#include "G4Event.hh"
#include "DetectorConstruction.hh"
#include "G4SystemOfUnits.hh"
#include "G4String.hh"


// Constructor
PrimaryGeneratorAction::PrimaryGeneratorAction(DetectorConstruction* construction)
{
    // Initialize Class variables
    detectorConstruction    = construction;
    source                  = new G4GeneralParticleSource();
}

//----------------------- 8< -------------[ cut here ]------------------------

//Destructor
PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
    delete source;
}

//----------------------- 8< -------------[ cut here ]------------------------


// Function that sets up the primary particles
void PrimaryGeneratorAction::GeneratePrimaries(G4Event* event)
{
    
#ifndef X_RAY
//     

        G4VPhysicalVolume* physVol = detectorConstruction->GetDaughterPhysicalByName(detectorConstruction->worldLogical, detectorConstruction->sourceVolume);
        // G4cout << detectorConstruction->sourceVolume << G4endl;
        G4VSolid *solid = physVol->GetLogicalVolume()->GetSolid();
        
        const G4double thicknessZ = ComputeExtentInMother(physVol).second.z() - ComputeExtentInMother(physVol).first.z();
        G4ThreeVector pos = ComputeCentroidInMother(physVol) + G4ThreeVector(0,0,thicknessZ/2 + 0.1*mm);
        // G4cout << "The source position was found to be " << pos << G4endl;

        
        G4Material * material = physVol->GetLogicalVolume()->GetMaterial();
        setParticleFromMaterial(material);
        source->SetNumberOfParticles(1);
        source->GetCurrentSource()->GetEneDist()->SetMonoEnergy(0.0 * keV);
        source->GetCurrentSource()->GetPosDist()->SetCentreCoords(pos);
        source->GetCurrentSource()->GetPosDist()->SetPosDisType("Plane");
        source->GetCurrentSource()->GetPosDist()->SetPosDisShape("Circle");
        source->GetCurrentSource()->GetPosDist()->SetRadius(2.5*mm);
        source->GetCurrentSource()->GetPosDist()->SetRadius0(0);
//         // source->GetCurrentSource()->GetPosDist()->SetHalfZ(detectorConstruction->sourceThickness/2);
//         // source->GetCurrentSource()->GetPosDist()->SetPosRot1(detectorConstruction->sourceRotation->colX());
//         // source->GetCurrentSource()->GetPosDist()->SetPosRot2(detectorConstruction->sourceRotation->colY());
//     }

#endif
    // Generate the particle
	source->GeneratePrimaryVertex(event);
    // G4cout << "generate primary vertex active" << G4endl;    

}


void PrimaryGeneratorAction::setParticleFromMaterial(G4Material * material)
{
    G4double Z = material->GetZ();
    G4double A = material->GetA();
    G4ParticleDefinition* particle = G4IonTable::GetIonTable()->GetIon(Z, A, 0);
    source->SetParticleDefinition(particle);
}


// void PrimaryGeneratorAction::setParticleFromName(const G4String &materialName)
// {
//     G4ParticleDefinition* isotope = G4IonTable::GetIonTable()->GetIon(Z, A, 0);
//     source->SetParticleDefinition(isotope);
// }


#include "G4VoxelLimits.hh"
#include "G4VSolid.hh"
#include "G4TessellatedSolid.hh"
#include "G4AffineTransform.hh"
#include "globals.hh" 


std::pair<G4ThreeVector, G4ThreeVector> PrimaryGeneratorAction::ComputeExtentInMother(G4VPhysicalVolume* pv)
{
    // 1) Get the solid (your tessellated)
    auto* tess = dynamic_cast<G4TessellatedSolid*>(pv->GetLogicalVolume()->GetSolid());
    if (!tess) { return {}; }

    // 2) Build the placement transform (solid → mother)
    const G4RotationMatrix* Rptr = pv->GetObjectRotation();
    G4RotationMatrix        R    = (Rptr ? *Rptr : G4RotationMatrix()); // identity if null
    R.invert(); // convert stored mother→daughter rotation into the needed daughter→mother
    const G4ThreeVector     T    = pv->GetObjectTranslation();
    G4AffineTransform       tr(R, T);

    // 3) Ask Geant4 for extents of the *transformed* solid
    G4VoxelLimits lim; // no limits
    G4double xmin, xmax, ymin, ymax, zmin, zmax;
    tess->CalculateExtent(kXAxis, lim, tr, xmin, xmax);
    tess->CalculateExtent(kYAxis, lim, tr, ymin, ymax);
    tess->CalculateExtent(kZAxis, lim, tr, zmin, zmax);

    // 4) Return min/max corners in the mother frame
    return {
        G4ThreeVector(xmin, ymin, zmin),
        G4ThreeVector(xmax, ymax, zmax)
    };
}

G4ThreeVector PrimaryGeneratorAction::ComputeCentroidInMother(G4VPhysicalVolume* pv)
{
    auto extent = ComputeExtentInMother(pv);
    const auto& min = extent.first;
    const auto& max = extent.second;
    return G4ThreeVector(
        (min.x() + max.x())/2.,
        (min.y() + max.y())/2.,
        (min.z() + max.z())/2.
    );
}
