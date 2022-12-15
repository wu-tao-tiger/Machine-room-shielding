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


//自动导入OBJ模型并指定材料，要求构建模型时指定各实体的名称，名称格式为构建历史，例“aaa_bbb_ccc”
//aaa为bbb的母体,bbb为ccc的母体
//在实体的材质名称指定物体的材料
//使用方法 在DetectorConstruction的Construct函数中  输入OBJImporter::modelbulider(文件名，世界的PhysicalVolume)；

namespace OBJImporter
{
	//分割字符串函数，输入字符串和分割标识符即可分割 这里默认采用“_”分割
	//用于将模型构建历史“aaa_bbb_ccc”拆分为“aaa”、“bbb”、“ccc”
	std::vector<std::string> Stringsplit(std::string str, const const char split)
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


	//读取obj文件中的模型构建历史以及对应材料，将构建历史编排成vector的形式，以构建历史vector为key，材料为value组成map输出
	//uniform用于指定是否采用统一的材料，选true则采用统一材料，materialname则为统一材料的名称，uniform默认为false
	std::map<std::vector<std::string>, std::string> ParseOBJ(std::string FileName, bool uniform, std::string materialname )
	{
		std::cout << std::endl << "**************************************************" << std::endl;
		std::cout << "开始检索OBj文件中的模型......" << std::endl << std::endl;
		std::map<std::vector<std::string>, std::string> ObjInform;
		std::string LineInform;
		std::ifstream InputFile;
		InputFile.open(FileName);
		if (!InputFile.is_open())
		{
			std::cout << "OBJ文件打开失败！" << std::endl;
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
				std::cout << "成功定位物体"<<*(objhistory.end()-1) << ", 其构建历史为：";
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
					std::cout << "采用统一的材料: " << materialname << "。" << std::endl;
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
					std::cout << "并确认材料为：" << objmaterial << "。" << std::endl;
					ObjInform.insert(std::map<std::vector<std::string>, std::string>::value_type(objhistory, objmaterial));
				}
				else
				{
					std::cout << "但未找到对应材料" << std::endl;
				}
				flag = false;
			}
		}
		InputFile.close();
		std::cout << std::endl << "检索完毕!" << std::endl;
		std::cout << "**************************************************" << std::endl << std::endl;
		return ObjInform;
	}


	//用于获得模型构建最深层数
	int getNOfLayers(std::map<std::vector<std::string>, std::string> ObjInform)
	{
		int currentLayers;
		int maxLayers(0);
		for (const std::pair<std::vector<std::string>, std::string> it : ObjInform)
		{
			currentLayers = it.first.size();
			if (currentLayers > maxLayers)maxLayers = currentLayers;
		}
		std::cout << "模型构建最深层数为： " << maxLayers << std::endl;
		return maxLayers;
	}


	//构建函数，输入obj文件名和导入模型的母体physicalvolume（一般为world），按照构建历史将模型一层一层导入，并指定对应材料
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
					G4cout << "物质：" << LogicalVolume[i]->GetMaterial()->GetName() << "密度：" << LogicalVolume[i]->GetMaterial()->GetDensity() << G4endl;
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
					G4cout << "物质：" << LogicalVolume[i]->GetMaterial()->GetName() << "密度：" << LogicalVolume[i]->GetMaterial()->GetDensity() << G4endl;
					G4cout << G4BestUnit(LogicalVolume[i]->GetMass(false, false, nullptr), "Mass") << G4endl;
					new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), LogicalVolume[i], *((iter->first).end() - 1), G4LogicalVolumeStore::GetInstance()->GetVolume(*(iter->first.end() - 2)), false, 0, true);
					i += 1;
				}

			}
		}


	}

}