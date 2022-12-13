#pragma once

#include "G4VUserActionInitialization.hh"
#include "globals.hh"


class ActionInitialization : public G4VUserActionInitialization
{
public:
	ActionInitialization();
	~ActionInitialization();

	void Build() const;
	void BuildForMaster() const;

};