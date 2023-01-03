#include "RunAction.hh"
#include "G4Run.hh"
#include "ScorerRun.hh"
#include "G4StatDouble.hh"


RunAction::RunAction()
{
	fSDName.push_back("probe0");
	fSDName.push_back("probe1");
	fSDName.push_back("probe2");
	fSDName.push_back("probe3");
}
RunAction::~RunAction()
{
	fSDName.clear();
}


G4Run* RunAction::GenerateRun()
{
	return new ScorerRun(fSDName);
}


void RunAction::BeginOfRunAction(const G4Run* run)
{}


void RunAction::EndOfRunAction(const G4Run* run)
{
	if (!IsMaster()) return;
	ScorerRun* scorerrun = (ScorerRun*)run;
	std::vector<G4THitsMap<G4StatDouble>*> runmap = scorerrun->getRunMap();

	double factor[11] = { 0.061,0.83,1.05,0.81,0.64,0.55,0.51,0.53,0.61,0.89,1.2 };
	for(size_t i=0; i<44;++i)
	{
		G4cout << runmap[i]->GetName() << "  " << G4endl;
		for (auto iter = runmap[i]->GetMap()->begin(); iter != runmap[i]->GetMap()->end(); ++iter)
		{
			G4cout << iter->first << ": " << iter->second->sum_wx() << "v2: " << iter->second->sum_wx2() << G4endl;
		}
	}


}