#pragma once

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

class G4VPhysicalVolume;

class DetectorConstruction :public G4VUserDetectorConstruction
{
public:
	DetectorConstruction();
	~DetectorConstruction();

	G4VPhysicalVolume* Construct();

	void ConstructSDandField();

	void DefineMaterial();
};