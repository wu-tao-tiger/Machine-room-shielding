#pragma once

#include "G4UserRunAction.hh"
#include "globals.hh"
#include <vector>
#include <string>

class G4Run;


class RunAction : public G4UserRunAction
{
public:
	RunAction();
	~RunAction();

	G4Run* GenerateRun();

	void BeginOfRunAction(const G4Run* run);
	void EndOfRunAction(const G4Run* run);


private:
	std::vector<G4String> fSDName;

};