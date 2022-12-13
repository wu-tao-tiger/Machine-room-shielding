#include "PrimaryGeneratorAction.hh"
#include "G4Event.hh"
#include "G4GeneralParticleSource.hh"

PrimaryGeneratorAction::PrimaryGeneratorAction():fgps(new G4GeneralParticleSource)
{}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
	delete fgps;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
	fgps->GeneratePrimaryVertex(anEvent);
}