#pragma once

#include "G4Run.hh"
#include "globals.hh"
#include <vector>
#include "G4THitsMap.hh"

class G4StatDouble;

class ScorerRun :public G4Run
{
public:
	ScorerRun(const std::vector<G4String> mfdName);
	~ScorerRun();


	void RecordEvent(const G4Event* anEvent);

	void Merge(const G4Run* run);

	void ConstructMFD(const std::vector<G4String>&);

	std::vector<G4THitsMap<G4StatDouble>*> getRunMap() { return fRunMap; }

private:

	std::vector<G4THitsMap<G4StatDouble>*> fRunMap;
};
