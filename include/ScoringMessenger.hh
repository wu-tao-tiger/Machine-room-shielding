#pragma once

#include "G4UImessenger.hh"
#include "ScoringWorld.hh"



class G4UIcmdWith3VectorAndUnit;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithAnInteger;
class G4UIdirectory;

class ScoringMessenger : public G4UImessenger
{
public:
	ScoringMessenger();
	~ScoringMessenger();

	void SetNewValue(G4UIcommand* command, G4String newValue);

private:
	ScoringWorld* fscoringworld;
	G4UIdirectory* fdirectory;
	G4UIcmdWith3VectorAndUnit* fPosCMD;
	G4UIcmdWithADoubleAndUnit* fSizeCMD;
	G4UIcmdWithAnInteger* fTypeCMD;
};