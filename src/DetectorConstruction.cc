#include "DetectorConstruction.hh"
#include "G4VPhysicalVolume.hh"
#include "G4Box.hh"
#include "G4SystemOfUnits.hh"
#include "G4VSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4MultiFunctionalDetector.hh"
#include "G4VPrimitiveScorer.hh"
#include "G4SDManager.hh"
#include "G4PVPlacement.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"
#include "G4Sphere.hh"
#include "G4PSVolumeFlux.hh"


DetectorConstruction::DetectorConstruction()
{

}

DetectorConstruction::~DetectorConstruction()
{}

G4VPhysicalVolume* DetectorConstruction::Construct()
{
	G4NistManager* nistmanager = G4NistManager::Instance();
	G4Material* air = nistmanager->FindOrBuildMaterial("G4_AIR");
	G4Material* water = nistmanager->FindOrBuildMaterial("G4_WATER");
	G4Material* pb = nistmanager->FindOrBuildMaterial("G4_AIR");

	G4VSolid* world_solid = new G4Box("world_solid", 5. * m, 5. * m, 5. * m);
	G4LogicalVolume* world_logical = new G4LogicalVolume(world_solid, air, "world_logical");
	G4VPhysicalVolume* world_physical = new G4PVPlacement(nullptr, G4ThreeVector(0, 0, 0), world_logical, "world_physical", nullptr, false, 0, true);


	world_logical->SetVisAttributes(G4VisAttributes::GetInvisible());
	

	return world_physical;
}

