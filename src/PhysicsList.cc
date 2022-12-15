#include "PhysicsList.hh"
#include "G4DecayPhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"
#include "G4EmStandardPhysics.hh"
#include "G4GenericBiasingPhysics.hh"

PhysicsList::PhysicsList()
{
	SetVerboseLevel(1);
	RegisterPhysics(new G4DecayPhysics);
	RegisterPhysics(new G4RadioactiveDecayPhysics);
	RegisterPhysics(new G4EmStandardPhysics);
	G4GenericBiasingPhysics* biasingphysics = new G4GenericBiasingPhysics;
	biasingphysics->NonPhysicsBias("gamma");
	biasingphysics->AddParallelGeometry("gamma", "mseh");
	RegisterPhysics(biasingphysics);

}

PhysicsList::~PhysicsList()
{

}

void PhysicsList::SetCuts()
{
	G4VUserPhysicsList::SetCuts();
}
