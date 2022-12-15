#include "PrimaryGeneratorAction.hh"
#include "G4Event.hh"

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