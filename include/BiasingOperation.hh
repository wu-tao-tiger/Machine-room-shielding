#pragma once

#include "G4VBiasingOperation.hh"
#include <map>
#include "G4TouchableHistoryHandle.hh"
#include "G4SteppingManager.hh"


class G4ParticleChange;
class G4ParticleChangeForNothing;
class G4BiasingProcessSharedData;

class BiasingOperation :public G4VBiasingOperation
{
public:
	BiasingOperation();
	~BiasingOperation();

	const G4VBiasingInteractionLaw* ProvideOccurenceBiasingInteractionLaw(const G4BiasingProcessInterface* ,G4ForceCondition&)
	{
		return nullptr;
	}

	G4VParticleChange* ApplyFinalStateBiasing(const G4BiasingProcessInterface*,const G4Track*,const G4Step*,G4bool& )
	{
		return nullptr;
	}

	G4double DistanceToApplyOperation(const G4Track* ,G4double,G4ForceCondition*);

	G4VParticleChange* GenerateBiasingFinalState(const G4Track* ,const G4Step* );


	void setImportance(std::map<G4String, G4int> importance)
	{
		fimportance = importance;
	}

	void setShareData(const G4BiasingProcessSharedData* sharedata)
	{
		fsharedata = sharedata;
	}

	G4int getCopynum(G4TouchableHistoryHandle touchable);
	

private:
	G4ParticleChange* fpc;
	G4ParticleChangeForNothing* fpcn;
	std::map<G4String, G4int> fimportance;
	const G4BiasingProcessSharedData* fsharedata;

	G4TouchableHistoryHandle fPreStepTouchableHistory;
	G4TouchableHistoryHandle fPostStepTouchableHistory;

	G4SteppingManager* stepmanager;
};