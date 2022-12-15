#include "RunAction.hh"
#include "G4Run.hh"
#include "ScorerRun.hh"


class ScorerRun;

RunAction::RunAction()
{}
RunAction::~RunAction()
{}


G4Run* RunAction::GenerateRun()
{
	return new ScorerRun;
}


void RunAction::BeginOfRunAction(const G4Run* run)
{}


void RunAction::EndOfRunAction(const G4Run* run)
{
	if (!IsMaster()) return;
	ScorerRun* scorerrun = (ScorerRun*)run;
	scorerrun->DumpAllScorer();
}