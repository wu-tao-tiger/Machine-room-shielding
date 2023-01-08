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
#include "G4SDParticleWithEnergyFilter.hh"
#include "G4PSDoseDeposit.hh"

ScoringWorld::ScoringWorld(G4String worldName):G4VUserParallelWorld(worldName),probeNum(0)
{}

ScoringWorld::~ScoringWorld()
{}

void ScoringWorld::Construct()
{
	setProbe(1228.*mm, 0., 355.*mm, ksphere, 1 * cm);
	setProbe(1528.*mm, 0, 0, ksphere, 1 * cm);
	setProbe(490*mm, 543*mm, 0, ksphere, 1 * cm);
	setProbe(-250*mm, 320*mm, 0, ksphere, 1 * cm);
	//setProbe(500. * mm, 0., 0, ksphere, 1 * cm);
	//setProbe(0. * mm, 0, 0, ksphere, 1 * cm);
	//setProbe(-500 * mm, 0, 0, ksphere, 1 * cm);
	//setProbe(-1000 * mm, 0, 0, ksphere, 1 * cm);
}

void ScoringWorld::ConstructSD()
{
	double energy[22] = { 0,0.01 * MeV,
		0.01 * MeV,0.015 * MeV,
		0.015 * MeV,0.02 * MeV,
		0.02 * MeV,0.03 * MeV,
		0.03 * MeV,0.04 * MeV,
		0.04 * MeV,0.05 * MeV,
		0.05 * MeV,0.06 * MeV,
		0.06 * MeV,0.08 * MeV,
		0.08 * MeV,0.1 * MeV,
		0.1 * MeV,0.15 * MeV,
		0.15 * MeV,0.20 * MeV };

	setProbeSD(0, kflux, 11, energy);
	setProbeSD(1, kflux, 11, energy);
	setProbeSD(2, kflux, 11, energy);
	setProbeSD(3, kflux, 11, energy);
}

G4bool ScoringWorld::setProbe(double x,double y, double z, probeType ktype, G4double size)
{
	if (ktype == kbox)
	{
		probe_s.push_back(new G4Box("probe_s" + std::to_string(probeNum), size, size, size));
	}
	else if (ktype == ksphere)
	{
		probe_s.push_back(new G4Sphere("probe_s" + std::to_string(probeNum), 0, size, 0, 360 * deg, 0, 180 * deg));
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
	probe_p.push_back(new G4PVPlacement(nullptr, G4ThreeVector(x,y,z), "probe_p" + std::to_string(probeNum), probe_l[probeNum], GetWorld(), false, 0, false));
	probeNum++;
	return true;
}

G4bool ScoringWorld::setProbeSD(int probeID, scoreType kScore, int nOfRange, double* energyrange)
{
	G4SDManager* sdmanager = G4SDManager::GetSDMpointer();
	G4MultiFunctionalDetector* probe_MFD = new G4MultiFunctionalDetector("probe" + std::to_string(probeID));
	G4VPrimitiveScorer* probe_scorer;
	G4SDParticleWithEnergyFilter* probe_filter;
	

	for (size_t i = 0; i < nOfRange; i++)
	{
		probe_filter = new G4SDParticleWithEnergyFilter("probe" + std::to_string(probeID) + ":" + std::to_string(energyrange[2 * i]) + "-" + std::to_string(energyrange[2 * i + 1]), energyrange[2 * i], energyrange[2 * i + 1]);
		probe_filter->add("gamma");
		if(kScore == kflux)
		{
			probe_scorer = new G4PSVolumeFlux("probe" + std::to_string(probeID) + ":" + std::to_string(energyrange[2 * i]) + "-" + std::to_string(energyrange[2 * i + 1]));
		}
		else
		{
			probe_scorer = new G4PSDoseDeposit("probe" + std::to_string(probeID) + ":" + std::to_string(energyrange[2 * i]) + "-" + std::to_string(energyrange[2 * i + 1]));
		}
		probe_scorer->SetFilter(probe_filter);
		probe_MFD->RegisterPrimitive(probe_scorer);
	}
	sdmanager->AddNewDetector(probe_MFD);
	SetSensitiveDetector("probe_l" + std::to_string(probeID), probe_MFD);
	//SetSensitiveDetector(probe_l[probeID], probe_MFD);
}