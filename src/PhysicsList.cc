#include "PhysicsList.hh"
#include "G4DecayPhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"
//#include "G4EmStandardPhysics.hh"
#include "G4GenericBiasingPhysics.hh"
#include "G4ParallelWorldPhysics.hh"
#include "G4EmLivermorePhysics.hh"

PhysicsList::PhysicsList()
{
	SetVerboseLevel(1);
	RegisterPhysics(new G4DecayPhysics);
	RegisterPhysics(new G4RadioactiveDecayPhysics);
	RegisterPhysics(new G4EmLivermorePhysics);
	G4GenericBiasingPhysics* biasingphysics = new G4GenericBiasingPhysics;
	biasingphysics->NonPhysicsBias("gamma");
	biasingphysics->AddParallelGeometry("gamma", "mseh");
	RegisterPhysics(biasingphysics);
	RegisterPhysics(new G4ParallelWorldPhysics("mesh", false));
	RegisterPhysics(new G4ParallelWorldPhysics("score", false));

}

PhysicsList::~PhysicsList()
{

}

void PhysicsList::SetCuts()
{
	G4VUserPhysicsList::SetCuts();
}
