#include "ScoringWorld.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4VSolid.hh"
#include "G4Box.hh"
#include "G4Sphere.hh"
#include "G4SystemOfUnits.hh"
#include "G4PVPlacement.hh"
#include "G4SDManager.hh"
#include "G4MultiFunctionalDetector.hh"
#include "G4VPrimitiveScorer.hh"
#include "G4PSVolumeFlux.hh"

ScoringWorld::ScoringWorld(G4String worldName):G4VUserParallelWorld(worldName),probeNum(0)
{}

ScoringWorld::~ScoringWorld()
{}

void ScoringWorld::Construct()
{}

void ScoringWorld::ConstructSD()
{
	G4bool isSDDone = setProbeSD();
	if (!isSDDone)
	{
		G4cout << "SD did not activate" << G4endl;
	}
}

G4bool ScoringWorld::setProbe(G4ThreeVector pos, probeType ktype, G4double size)
{
	if (ktype == kbox)
	{
		probe_s.push_back(new G4Box("probe_s" + std::to_string(probeNum), size, size, size));
	}
	else if (ktype == ksphere)
	{
		probe_s.push_back(new G4Sphere("probe_s" + std::to_string(probeNum), 0, size, 0, 180 * deg, 0, 360 * deg));
	}
	else
	{
		G4cerr << "no such shape" << G4endl;
		return false;
	}
	probe_l.push_back(new G4LogicalVolume(probe_s[probeNum], nullptr, "probe_l" + std::to_string(probeNum)));
	if (!probe_l[probeNum])
	{
		G4cerr << "Failed to create Logical Volume" << G4endl;
		return false;
	}
	probe_p.push_back(new G4PVPlacement(nullptr, pos, "probe_p" + std::to_string(probeNum), probe_l[probeNum], GetWorld(), false, 0, false));
	probeNum++;
	return true;
}

G4bool ScoringWorld::setProbeSD()
{
	if (probeNum == 0)
	{
		return false;
	}
	G4SDManager* sdmanager = G4SDManager::GetSDMpointer();
	size_t numOfLV = probe_l.size();
	for (size_t i = 0; i < numOfLV; i++)
	{
		probe_SD.push_back(new G4MultiFunctionalDetector("probe_SD" + std::to_string(i)));
		probe_Scorer.push_back(new G4PSVolumeFlux("probe_SD" + std::to_string(i)));
		probe_SD[i]->RegisterPrimitive(probe_Scorer[i]);
		sdmanager->AddNewDetector(probe_SD[i]);
		SetSensitiveDetector(probe_l[i], probe_SD[i]);
	}

}