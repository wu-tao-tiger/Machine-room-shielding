#pragma once

#include "globals.hh"
#include "G4VUserParallelWorld.hh"
#include "G4ThreeVector.hh"

class G4VPhysicalVolume;

class BiasingWorld :public G4VUserParallelWorld
{
public:
	BiasingWorld();
	~BiasingWorld();

	void Construct();
	void ConstructSD();


	void setNBin(int x, int* xImp, int y, int* yImp, int z, int* zImp);

	G4int getCopyNum(G4int i, G4int j, G4int k);

	G4ThreeVector getPosition(G4int i, G4int j, G4int k);

	G4bool setName(G4String name) 
	{
		fname = name;
		return true;
	}


private:
	G4int xbin;
	G4int ybin;
	G4int zbin;
	G4double xsize;
	G4double ysize;
	G4double zsize;
	G4String fname;
	std::map<G4String, int> fImportanceMap;
};