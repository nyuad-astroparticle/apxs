/************************
•     ┓             •
┓┏┳┓┏┓┃┏┓┏┳┓┏┓┏┓╋┏┓╋┓┏┓┏┓
┗┛┗┗┣┛┗┗ ┛┗┗┗ ┛┗┗┗┻┗┗┗┛┛┗
    ┛
Primary Generator Action
************************/

#include "PrimaryGeneratorAction.hh"

#include "G4GeneralParticleSource.hh"
#include "G4ParticleDefinition.hh"
#include "G4IonTable.hh"
#include "G4NistManager.hh"
#include "G4Gamma.hh"
#include "G4Event.hh"
#include "DetectorConstruction.hh"
#include "G4SystemOfUnits.hh"
#include "G4String.hh"
#include "G4VoxelLimits.hh"
#include "G4VSolid.hh"
#include "G4AffineTransform.hh"
#include "globals.hh"
#include <algorithm>
#include <cmath>
#include <cctype>

PrimaryGeneratorAction::PrimaryGeneratorAction(DetectorConstruction* construction)
{
    detectorConstruction = construction;
    source = new G4GeneralParticleSource();
}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
    delete source;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* event)
{
#ifndef X_RAY

    G4VPhysicalVolume* physVol =
        detectorConstruction->GetDaughterPhysicalByName(
            detectorConstruction->worldLogical,
            detectorConstruction->sourceVolume);

    if (!physVol) {
        G4Exception("PrimaryGeneratorAction::GeneratePrimaries",
                    "SourceNotFound", FatalException,
                    "Could not find source physical volume.");
        return;
    }

    G4Material* material = physVol->GetLogicalVolume()->GetMaterial();
    setParticleFromName(material->GetName());

    source->SetNumberOfParticles(1);
    source->GetCurrentSource()->GetEneDist()->SetMonoEnergy(0.0 * keV);

    auto extent = ComputeExtentInMother(physVol);
    const auto& min = extent.first;
    const auto& max = extent.second;
    const G4double radius = 0.5 * std::max(max.x() - min.x(), max.y() - min.y());
    const G4double halfZ = 0.5 * (max.z() - min.z());
    const G4ThreeVector centroid = ComputeCentroidInMother(physVol);

    // Emit just inside the Be-window-side face of the active Fe55 layer.
    source->GetCurrentSource()->GetPosDist()->SetPosDisType("Plane");
    source->GetCurrentSource()->GetPosDist()->SetPosDisShape("Circle");
    source->GetCurrentSource()->GetPosDist()->SetCentreCoords(
        G4ThreeVector(centroid.x(), centroid.y(), centroid.z() + halfZ - 0.001 * mm));
    source->GetCurrentSource()->GetPosDist()->SetRadius(radius);
    source->GetCurrentSource()->GetPosDist()->SetRadius0(0.0 * mm);

#endif

    source->GeneratePrimaryVertex(event);
}

void PrimaryGeneratorAction::setParticleFromMaterial(G4Material* material)
{
    G4double Z = material->GetZ();
    G4double A = material->GetA();
    G4ParticleDefinition* particle = G4IonTable::GetIonTable()->GetIon(Z, A, 0);
    source->SetParticleDefinition(particle);
}

void PrimaryGeneratorAction::setParticleFromName(const G4String &materialName)
{
    G4String symbol;
    G4String massDigits;
    for (char ch : materialName)
    {
        if (std::isalpha(static_cast<unsigned char>(ch)))
        {
            symbol += ch;
        }
        else if (std::isdigit(static_cast<unsigned char>(ch)))
        {
            massDigits += ch;
        }
    }

    if (symbol.empty() || massDigits.empty())
    {
        auto* volume = detectorConstruction->GetDaughterPhysicalByName(
            detectorConstruction->worldLogical,
            detectorConstruction->sourceVolume);
        if (!volume)
        {
            G4Exception("PrimaryGeneratorAction::setParticleFromName",
                        "SourceNotFound", FatalException,
                        "Could not find source physical volume.");
            return;
        }
        setParticleFromMaterial(volume->GetLogicalVolume()->GetMaterial());
        return;
    }

    G4Material* baseMaterial = G4NistManager::Instance()->FindOrBuildMaterial("G4_" + symbol);
    if (!baseMaterial)
    {
        G4Exception("PrimaryGeneratorAction::setParticleFromName",
                    "UnknownElement", FatalException,
                    ("Could not resolve element for source material " + materialName).c_str());
        return;
    }

    G4int Z = static_cast<G4int>(std::lround(baseMaterial->GetZ()));
    G4int A = std::stoi(massDigits);
    G4ParticleDefinition* particle = G4IonTable::GetIonTable()->GetIon(Z, A, 0.0);
    source->SetParticleDefinition(particle);
}

std::pair<G4ThreeVector, G4ThreeVector>
PrimaryGeneratorAction::ComputeExtentInMother(G4VPhysicalVolume* pv)
{
    auto* solid = pv->GetLogicalVolume()->GetSolid();
    if (!solid) {
        return {};
    }

    const G4RotationMatrix* Rptr = pv->GetObjectRotation();
    G4RotationMatrix R = (Rptr ? *Rptr : G4RotationMatrix());
    R.invert();

    const G4ThreeVector T = pv->GetObjectTranslation();
    G4AffineTransform tr(R, T);

    G4VoxelLimits lim;
    G4double xmin, xmax, ymin, ymax, zmin, zmax;

    solid->CalculateExtent(kXAxis, lim, tr, xmin, xmax);
    solid->CalculateExtent(kYAxis, lim, tr, ymin, ymax);
    solid->CalculateExtent(kZAxis, lim, tr, zmin, zmax);

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
        (min.x() + max.x()) / 2.0,
        (min.y() + max.y()) / 2.0,
        (min.z() + max.z()) / 2.0
    );
}
