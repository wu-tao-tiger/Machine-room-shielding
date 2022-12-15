#pragma once

#include "G4VBiasingOperator.hh"
#include <map>

class G4VBiasingOperation;
class G4Track;
class G4BiasingProcessInterface;
class BiasingOperation;
class G4BiasingProcessSharedData;

class BiasingOperator :public G4VBiasingOperator
{
public:
	BiasingOperator();
	~BiasingOperator();


	G4VBiasingOperation* ProposeNonPhysicsBiasingOperation(const G4Track* track, const G4BiasingProcessInterface* callingProcess);

	G4VBiasingOperation* ProposeOccurenceBiasingOperation(const G4Track* track, const G4BiasingProcessInterface* callingProcess)
	{
		return nullptr;
	}
	G4VBiasingOperation* ProposeFinalStateBiasingOperation(const G4Track* track, const G4BiasingProcessInterface* callingProcess)
	{
		return nullptr;
	}


	void StartRun();

	void setImportance(std::map<G4String, G4int> im)
	{
		fimportance = im;
	}


private:
	BiasingOperation* foperation;
	std::map<G4String, G4int> fimportance;
	const G4BiasingProcessSharedData* fsharedata;
};
