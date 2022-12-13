#pragma once

#include "globals.hh"
#include "G4VModularPhysicsList.hh"

class PhysicsList :public G4VModularPhysicsList
{
public:
	PhysicsList();
	~PhysicsList();

	void SetCuts();

};