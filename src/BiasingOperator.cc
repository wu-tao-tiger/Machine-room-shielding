#include "BiasingOperator.hh"
#include "G4Track.hh"
#include "G4BiasingProcessInterface.hh"
#include "BiasingOperation.hh"
#include "G4VBiasingOperation.hh"
#include "G4ParticleTable.hh"
#include "G4BiasingProcessSharedData.hh"
#include "G4ParallelGeometriesLimiterProcess.hh"

BiasingOperator::BiasingOperator():G4VBiasingOperator("split and kill"),fsharedata(nullptr)
{
	foperation = new BiasingOperation;
}

BiasingOperator::~BiasingOperator()
{
	delete foperation;
}



G4VBiasingOperation* BiasingOperator::ProposeNonPhysicsBiasingOperation(const G4Track* track, const G4BiasingProcessInterface* callingProcess)
{
	return foperation;
}

void BiasingOperator::StartRun()
{
	fsharedata= G4BiasingProcessInterface::GetSharedData(G4ParticleTable::GetParticleTable()->FindParticle("gamma")->GetProcessManager());
	foperation->setShareData(fsharedata);
	foperation->setImportance(fimportance);
}


