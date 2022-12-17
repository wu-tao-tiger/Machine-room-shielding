#include "ScoringMessenger.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIdirectory.hh"

ScoringMessenger::ScoringMessenger()
{
	fdirectory = new G4UIdirectory("/Scoring/");
	fdirectory->SetGuidance("place the scoring probe");

	fPosCMD = new G4UIcmdWith3VectorAndUnit("/Scoring/position", this);
	fPosCMD->SetGuidance("set where the probe locate");
	fPosCMD->SetParameterName("x", "y", "z", false);
	fPosCMD->SetUnitCategory("Length");
	fPosCMD->SetDefaultUnit("cm");
	fPosCMD->AvailableForStates(G4State_PreInit);

	fSizeCMD = new G4UIcmdWithADoubleAndUnit("/Scoring/size", this);
	fSizeCMD->SetGuidance("set the probe size");
	fSizeCMD->SetParameterName("half length or radius", false);
	fSizeCMD->SetUnitCategory("Length");
	fSizeCMD->SetDefaultUnit("cm");
	fSizeCMD->AvailableForStates(G4State_PreInit);

	fTypeCMD = new G4UIcmdWithAnInteger("/Scoring/type", this);
	fTypeCMD->SetGuidance("decide the shape of probe,cube or sphere");
	fTypeCMD->SetParameterName("ty", this);
	fTypeCMD->AvailableForStates(G4State_PreInit);

}

ScoringMessenger::~ScoringMessenger()
{
	delete fdirectory;
	delete fPosCMD;
	delete fSizeCMD;
	delete fTypeCMD;
}

void ScoringMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
	if()
}