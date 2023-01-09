#include "DetectorConstruction.hh"
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
#include "OBJImporter.hh"
#include "G4ThreeVector.hh"


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
	G4Material* pb = nistmanager->FindOrBuildMaterial("G4_Pb");
	G4Material* fe = nistmanager->FindOrBuildMaterial("G4_Fe");
	G4Material* vacuum = nistmanager->FindOrBuildMaterial("G4_Galactic");
	G4Material* ti = nistmanager->FindOrBuildMaterial("G4_Ti");
	G4Material* o = nistmanager->FindOrBuildMaterial("G4_O");
	G4Material* sr = nistmanager->FindOrBuildMaterial("G4_Sr");
	G4Material* ba = nistmanager->FindOrBuildMaterial("G4_Ba");
	G4Material* cr = nistmanager->FindOrBuildMaterial("G4_Cr");
	G4Material* mn = nistmanager->FindOrBuildMaterial("G4_Mn");
	G4Material* ni = nistmanager->FindOrBuildMaterial("G4_Ni");

	G4Material* citie = new G4Material("CiTie", 4.9 * g / cm3, 4);
	citie->AddMaterial(ba, 0.0155);
	citie->AddMaterial(sr, 0.0155);
	citie->AddMaterial(o, 0.594);
	citie->AddMaterial(fe, 0.375);

	G4Material* gang = new G4Material("gang", 7.92 * g / cm3, 4);
	gang->AddMaterial(cr, 0.19);
	gang->AddMaterial(mn, 0.02);
	gang->AddMaterial(fe, 0.695);
	gang->AddMaterial(ni, 0.095);


	G4VSolid* world_solid = new G4Box("world_solid", 1.7 * m, 1.1 * m, 0.6 * m);
	G4LogicalVolume* world_logical = new G4LogicalVolume(world_solid, water, "world_logical");
	G4VPhysicalVolume* world_physical = new G4PVPlacement(nullptr, G4ThreeVector(0, 0, 0), world_logical, "world_physical", nullptr, false, 0, true);

	world_logical->SetVisAttributes(G4VisAttributes::GetInvisible());
	
	OBJImporter::modelbulider("jc.obj", world_physical);

	return world_physical;
}

