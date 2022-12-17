#pragma once

#include "G4VUserParallelWorld.hh"
#include "globals.hh"
#include <vector>


class G4ThreeVector;
class G4VPhysicalVolume;
class G4VSolid;
class G4LogicalVolume;
class G4MultiFunctionalDetector;
class G4VPrimitiveScorer;

class ScoringWorld :public G4VUserParallelWorld
{
public:
	ScoringWorld(G4String);
	~ScoringWorld();

	void Construct();
	void ConstructSD();

	enum probeType
	{
		kbox,
		ksphere
	};

	G4bool setProbe(G4ThreeVector, probeType,G4double);
	G4bool setProbeSD();

private:
	std::vector<G4VPhysicalVolume*> probe_p;
	std::vector<G4LogicalVolume*> probe_l;
	std::vector<G4VSolid*> probe_s;
	std::vector<G4MultiFunctionalDetector*> probe_SD;
	std::vector<G4VPrimitiveScorer*> probe_Scorer;
	int probeNum;
};