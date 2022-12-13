#pragma once

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"

class G4Event;
class G4GeneralParticleSource;

class PrimaryGeneratorAction :public G4VUserPrimaryGeneratorAction
{
public:
	PrimaryGeneratorAction();
	~PrimaryGeneratorAction();
	void GeneratePrimaries(G4Event* anEvent);
private:
	G4GeneralParticleSource* fgps;
};