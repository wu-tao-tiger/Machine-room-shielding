#pragma once

#include "G4Run.hh"
#include "globals.hh"

class ScorerRun :public G4Run
{
public:
	ScorerRun();
	~ScorerRun();


	void RecordEvent(const G4Event* anEvent);

	void Merge(const G4Run* run);

	void DumpAllScorer();

	G4double getError(G4double v, G4double v2)
	{
		G4double nOfEvent = GetNumberOfEvent();
		G4double rms = v2 - std::pow(v, 2) / nOfEvent;
		G4double error(0);
		if (rms > 0)
		{
			error = std::sqrt(rms) / v;
		}
		else
		{ 
			error = 0; 
		}
		return error;
	}

private:
	G4double RunFlux;
	G4double RunFlux2;
};
