#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <map>
#include <fstream>
#include "CADMesh.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4VSolid.hh"
#include "G4Material.hh"
#include "G4PVPlacement.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4UnitsTable.hh"


//Automatically import OBJ models and specify materials. It is required to specify the names of entities when building models. The name format is construction history, such as "aaa_bbbb_ccc"
//Aaa is the parent of bbb and bbb is the parent of ccc
//Specify the material of the object in the material name of the entity
//Use the method to enter OBJImporter:: modelbuilder (file name, PhysicalVolume of the world) in the Constructfunction of DetectorConstruction;

namespace OBJImporter
{
	//The split string function can be split by entering a string and a split identifier. The "_" split is used by default here
	//Used to split the model construction history "aaa_bbbb_ccc" into "aaa", "bbb" and "ccc"
	std::vector<std::string> Stringsplit(std::string str,const char split)
	{
		std::istringstream iss(str);
		std::string token;
		std::vector<std::string> slices;
		while (std::getline(iss, token, split))
		{
			slices.push_back(token);
		}
		return slices;
	}


	//Read the model construction history and corresponding materials in the obj file, arrange the construction history in the form of vector, and use the construction history vector as the key and the materials as the value to form a map output
	//Uniform is used to specify whether unified materials are used. If true is selected, unified materials are used. Materialname is the name of unified materials. Uniform is false by default
	std::map<std::vector<std::string>, std::string> ParseOBJ(std::string FileName, bool uniform, std::string materialname )
	{
		std::cout << std::endl << "**************************************************" << std::endl;
		std::cout << "Start retrieving models in OBj files....." << std::endl << std::endl;
		std::map<std::vector<std::string>, std::string> ObjInform;
		std::string LineInform;
		std::ifstream InputFile;
		InputFile.open(FileName);
		if (!InputFile.is_open())
		{
			std::cout << "OBJ file opening failed!" << std::endl;
			system("pause");
			exit(-1);
		}
		std::vector<std::string> objhistory;
		bool flag = false;
		while (std::getline(InputFile, LineInform))
		{
			if (LineInform.substr(0, 2) == "o ")
			{
				objhistory = Stringsplit(LineInform.substr(2), '_');
				std::cout << "Object successfully located "<<*(objhistory.end()-1) << ", its construction history is: ";
				for (std::vector<std::string>::iterator iter = objhistory.begin(); iter != objhistory.end(); ++iter)
				{
					if (iter != objhistory.end() - 1)
						std::cout << *iter << "--> ";
					else
						std::cout << *iter;
				}
				std::cout << ", ";
				if (uniform)
				{
					std::cout << "Adopt unified materials: " << materialname << "." << std::endl;
					ObjInform.insert(std::map<std::vector<std::string>, std::string>::value_type(objhistory, materialname));
					continue;
				}
				flag = true;
				continue;
			}
			if (flag)
			{
				if (LineInform.substr(0, 7) == "usemtl ")
				{
					std::string objmaterial = LineInform.substr(7);
					std::cout << "And confirm that the materials are:" << objmaterial << "." << std::endl;
					ObjInform.insert(std::map<std::vector<std::string>, std::string>::value_type(objhistory, objmaterial));
				}
				else
				{
					std::cout << "But no corresponding material was found" << std::endl;
				}
				flag = false;
			}
		}
		InputFile.close();
		std::cout << std::endl << "Retrieval completed!" << std::endl;
		std::cout << "**************************************************" << std::endl << std::endl;
		return ObjInform;
	}


	//Used to obtain the deepest number of model construction
	int getNOfLayers(std::map<std::vector<std::string>, std::string> ObjInform)
	{
		int currentLayers;
		int maxLayers(0);
		for (const std::pair<std::vector<std::string>, std::string> it : ObjInform)
		{
			currentLayers = it.first.size();
			if (currentLayers > maxLayers)maxLayers = currentLayers;
		}
		std::cout << "The deepest level of model construction is: " << maxLayers << std::endl;
		return maxLayers;
	}


	//Build the function, input the obj file name and the physical volume (generally the world) of the imported model, import the model layer by layer according to the construction history, and specify the corresponding materials
	void modelbulider(std::string fileName,G4VPhysicalVolume* pMother, bool uniform = false, std::string materialname = " ",int nscale=1.)
	{

		auto mesh = CADMesh::TessellatedMesh::FromOBJ(fileName);
		mesh->SetScale(nscale);
		mesh->SetOffset(0., 0., 0.);

		std::map<std::vector<std::string>, std::string> objinform = ParseOBJ(fileName, uniform, materialname);
		int layernumber = getNOfLayers(objinform);
		size_t objnum = objinform.size();
		std::vector<G4VSolid*> Solid(objnum);
		std::vector<G4LogicalVolume*> LogicalVolume(objnum);
		G4int i = 0;
		for (int j = 0; j != layernumber; ++j)
		{
			for (std::map < std::vector<std::string>, std::string>::iterator iter = objinform.begin(); iter != objinform.end(); ++iter)
			{
				if ((iter->first).size() == 1 && j==0)
				{
					Solid[i] = mesh->GetSolid(*((iter->first).end() - 1));
					LogicalVolume[i] = new G4LogicalVolume(Solid[i], G4Material::GetMaterial(iter->second), *((iter->first).end() - 1));
					G4cout << "material:" << LogicalVolume[i]->GetMaterial()->GetName() << "Density:" << LogicalVolume[i]->GetMaterial()->GetDensity() << G4endl;
					G4cout <<G4BestUnit( LogicalVolume[i]->GetMass(false, false, nullptr),"Mass") << G4endl;
					new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), *((iter->first).end() - 1), LogicalVolume[i], pMother, false, 0, true);
					i += 1;
				}
				else if((iter->first).size() == j+1 && j !=0)
				{
					std::string solidname("");
					std::vector<std::string> solidhistory = iter->first;
					for (std::vector<std::string>::iterator vstor = solidhistory.begin(); vstor != solidhistory.end(); vstor++)
					{
						if (vstor != solidhistory.end() - 1)
							solidname += (*vstor + "_");
						else
							solidname += *vstor;
					}


					Solid[i] = mesh->GetSolid(solidname);
					LogicalVolume[i] = new G4LogicalVolume(Solid[i], G4Material::GetMaterial(iter->second), *((iter->first).end() - 1));
					G4cout << "material:" << LogicalVolume[i]->GetMaterial()->GetName() << "Density:" << LogicalVolume[i]->GetMaterial()->GetDensity() << G4endl;
					G4cout << G4BestUnit(LogicalVolume[i]->GetMass(false, false, nullptr), "Mass") << G4endl;
					new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), LogicalVolume[i], *((iter->first).end() - 1), G4LogicalVolumeStore::GetInstance()->GetVolume(*(iter->first.end() - 2)), false, 0, true);
					i += 1;
				}

			}
		}


	}

}