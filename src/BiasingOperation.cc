#include "BiasingOperation.hh"
#include "G4ParallelGeometriesLimiterProcess.hh"
#include "Randomize.hh"
#include "G4Track.hh"
#include "G4ParticleChange.hh"
#include "G4ParticleChangeForNothing.hh"
#include "G4BiasingProcessInterface.hh"
#include "G4TouchableHistoryHandle.hh"


BiasingOperation::BiasingOperation():G4VBiasingOperation("split and kill"),fsharedata(nullptr)
{
	
	fpc = new G4ParticleChange;
	fpcn = new G4ParticleChangeForNothing;

	stepmanager = new G4SteppingManager;
	
}

BiasingOperation::~BiasingOperation()
{
	delete fpc;
	delete fpcn;
	delete stepmanager;
}


G4double BiasingOperation::DistanceToApplyOperation(const G4Track*, G4double, G4ForceCondition* condition)
{
	fPreStepTouchableHistory =fsharedata->GetParallelGeometriesLimiterProcess()->GetNavigator(0)->CreateTouchableHistoryHandle();
	/*G4cout << "pretouchable:" << fPreStepTouchableHistory->GetVolume()->GetName() << G4endl;*/
	*condition = Forced;
	return DBL_MAX;
}

G4VParticleChange* BiasingOperation::GenerateBiasingFinalState(const G4Track* track, const G4Step* step)
{

	G4bool isLimiting = fsharedata->GetParallelGeometriesLimiterProcess()->GetIsLimiting(0);
	if (!isLimiting)
	{
		fpcn->Initialize(*track);
		return fpcn;
	}
	
	fPostStepTouchableHistory = fsharedata->GetParallelGeometriesLimiterProcess()->GetNavigator(0)->CreateTouchableHistoryHandle();

	if (fPostStepTouchableHistory->GetVolume()!=fPreStepTouchableHistory->GetVolume())
	{
		fpcn->Initialize(*track);
		return fpcn;
	}

	//G4cout << "pretouchable:" << fPreStepTouchableHistory->GetVolume()->GetName() << G4endl;
	//G4cout << "posttouchable:" << fPostStepTouchableHistory->GetVolume()->GetName() << G4endl;

	G4int prex = fPreStepTouchableHistory->GetReplicaNumber(2);
	G4int prey = fPreStepTouchableHistory->GetReplicaNumber(1);
	G4int prez = fPreStepTouchableHistory->GetReplicaNumber(0);
	G4int postx = fPostStepTouchableHistory->GetReplicaNumber(2);
	G4int posty = fPostStepTouchableHistory->GetReplicaNumber(1);
	G4int postz = fPostStepTouchableHistory->GetReplicaNumber(0);


	//G4cout << fPreStepTouchableHistory->GetVolume()->GetName() << G4endl
	//	<< "pre2: " << prex << G4endl
	//	<< "pre1: " << prey << G4endl
	//	<< "pre0: " << prez << G4endl;

	//G4cout<<fPostStepTouchableHistory->GetVolume()->GetName()<<G4endl
	//	<< "post2: " << postx << G4endl
	//	<< "post1: " << posty << G4endl
	//	<< "post0: " << postz << G4endl;
	G4String precoordinate = std::to_string(prex) + std::to_string(prey) + std::to_string(prez);
	G4String postcoordinate = std::to_string(postx) + std::to_string(posty) + std::to_string(postz);

	G4int currentimportance = fimportance[precoordinate];
	G4int nextimportance = fimportance[postcoordinate];

	if (nextimportance == currentimportance)
	{
		fpcn->Initialize(*track);
		return fpcn;
	}

	if (nextimportance > currentimportance)
	{
		G4double splitfactor = nextimportance / currentimportance;
		G4double currentweight = track->GetWeight();
		G4double finalweight = currentweight / splitfactor;
		fpc->Initialize(*track);
		fpc->ProposeParentWeight(finalweight);
		fpc->SetNumberOfSecondaries(splitfactor - 1);
		for (size_t i = 0; i < splitfactor-1; i++)
		{
			G4Track* clone = new G4Track(*track);
			clone->SetWeight(finalweight);
			fpc->AddSecondary(clone);
		}
		fpc->SetSecondaryWeightByProcess(true);
		return fpc;
	}
	else if (nextimportance < currentimportance)
	{
		G4double currentweight = track->GetWeight();
		G4double survivingProbability = nextimportance / currentimportance;
		G4double killingProbability = G4UniformRand();

		fpc->Initialize(*track);
		if (killingProbability > survivingProbability)
		{
			fpc->ProposeTrackStatus(fStopAndKill);
		}
		else
		{
			fpc->ProposeParentWeight(currentweight * currentimportance / nextimportance);
		}
		return fpc;
	}

	fpcn->Initialize(*track);
	return fpcn;
}

G4int BiasingOperation::getCopynum(G4TouchableHistoryHandle touchable)
{
	G4int x = touchable->GetReplicaNumber(2);
	G4int y = touchable->GetReplicaNumber(1);
	G4int z = touchable->GetReplicaNumber(0);
	G4int copynum = x + y * 6 + 36 * z;
	return copynum;
}
