#include "DetectorConstruction.hh"

#include "G4Box.hh"
#include "G4SystemOfUnits.hh"
#include "G4VSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"

#include "G4Material.hh"
#include "G4NistManager.hh"



DetectorConstruction::DetectorConstruction()
{}

DetectorConstruction::~DetectorConstruction()
{}

void DetectorConstruction::DefineMaterial()
{
	G4NistManager* nistmanager = G4NistManager::Instance();
	G4Material* air = nistmanager->FindOrBuildMaterial("G4_AIR");
}

G4VPhysicalVolume* DetectorConstruction::Construct()
{
	DefineMaterial();
	G4VSolid* world_s = new G4Box("world_s", 10 * cm, 10 * cm, 10 * cm);
	G4LogicalVolume* world_l = new G4LogicalVolume(world_s, G4Material::GetMaterial("G4_AIR"), "world_l");
	G4VPhysicalVolume* world_p = new G4PVPlacement(nullptr, G4ThreeVector(0., 0., 0.), world_l, "world_p", nullptr, false, 0, true);



	


	return world_p;
}

void DetectorConstruction::ConstructSDandField()
{}
