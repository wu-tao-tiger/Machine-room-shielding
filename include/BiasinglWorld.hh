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


	void setNBin(G4int x, G4int y, G4int z);

	G4int getCopyNum(G4int i, G4int j, G4int k);

	G4ThreeVector getPosition(G4int i, G4int j, G4int k);
	//{
	//	G4double x = (2 * i + 1) * xsize / xbin - xsize;
	//	G4double y = (2 * j + 1) * ysize / ybin - ysize;
	//	G4double z = (2 * k + 1) * zsize / zbin - zsize;
	//	G4ThreeVector pos = G4ThreeVector(x, y, z);
	//	return pos;
	//}
private:
	G4int xbin;
	G4int ybin;
	G4int zbin;
	G4double xsize;
	G4double ysize;
	G4double zsize;
};