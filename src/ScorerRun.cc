#include "ScorerRun.hh"
#include "G4THitsMap.hh"
#include "G4Event.hh"

ScorerRun::ScorerRun():RunFlux(0),RunFlux2(0)
{}

ScorerRun::~ScorerRun()
{}

void ScorerRun::RecordEvent(const G4Event* anEvent)
{
	G4THitsMap<G4double>* eventmap = (G4THitsMap<G4double>*)anEvent->GetHCofThisEvent()->GetHC(0);
	G4double eventflux(0);
	if (eventmap->GetMap()->begin() != eventmap->GetMap()->end()) eventflux = *eventmap->GetMap()->begin()->second;
	RunFlux += eventflux;
	RunFlux2 += std::pow(eventflux, 2);
	G4Run::RecordEvent(anEvent);
}

void ScorerRun::Merge(const G4Run* run)
{
	ScorerRun* localrun = (ScorerRun*)run;
	RunFlux += localrun->RunFlux;
	RunFlux2 += localrun->RunFlux2;
	G4Run::Merge(run);
}


void ScorerRun::DumpAllScorer()
{
	G4cout << "++++++++++++++++++++++++++++++++++++++" << G4endl;
	G4cout << "RunFlux: " << RunFlux << "  RunFlux2: " << RunFlux2 <<"  error: "<<getError(RunFlux,RunFlux2) << G4endl;
	G4cout << "++++++++++++++++++++++++++++++++++++++" << G4endl;
}