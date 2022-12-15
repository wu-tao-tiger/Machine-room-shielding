#pragma once

#include "G4UserRunAction.hh"
#include "globals.hh"

class G4Run;


class RunAction : public G4UserRunAction
{
public:
	RunAction();
	~RunAction();

	G4Run* GenerateRun();

	void BeginOfRunAction(const G4Run* run);
	void EndOfRunAction(const G4Run* run);

};