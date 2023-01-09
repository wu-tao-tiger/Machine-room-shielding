#include "DetectorConstruction.hh"
#include "PhysicsList.hh"
#include "ActionInitialization.hh"


#include "G4UIExecutive.hh"
#include "G4UImanager.hh"
#include "G4MTRunManager.hh"
#include "G4VisExecutive.hh"
#include "G4VisManager.hh"
#include "G4ScoringManager.hh"

#include <time.h>
#include "Randomize.hh"



class DetectorConstruction;
class PhysicsList;
class ActionInitialization;

int main(int argc, char** argv)
{
	CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine());
	G4long seed = time(NULL);
	CLHEP::HepRandom::setTheSeed(seed);

	G4UIExecutive* uiexecutive = nullptr;
	if (argc == 1) uiexecutive = new G4UIExecutive(argc, argv);

	G4MTRunManager* runmanager = new G4MTRunManager;

	G4ScoringManager* scoringmanager = G4ScoringManager::GetScoringManager();

	if (argc == 3) runmanager->SetNumberOfThreads(atoi(argv[2]));
	else runmanager->SetNumberOfThreads(1);

	DetectorConstruction* detectorconstrcution = new DetectorConstruction;

	runmanager->SetUserInitialization(detectorconstrcution);
	runmanager->SetUserInitialization(new PhysicsList);
	runmanager->SetUserInitialization(new ActionInitialization);

	G4VisManager* vismanager = new G4VisExecutive;
	vismanager->Initialize();
	G4UImanager* uimanager = G4UImanager::GetUIpointer();
	if (!uiexecutive)
	{
		G4String file_name = argv[1];
		G4String command = "/control/execute ";
		uimanager->ApplyCommand(command + file_name);
	}
	else
	{
		uimanager->ApplyCommand("/control/execute vis.mac");
		uiexecutive->SessionStart();
		delete uiexecutive;
	}

	delete vismanager;
	delete runmanager;
}