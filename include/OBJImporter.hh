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


//�Զ�����OBJģ�Ͳ�ָ�����ϣ�Ҫ�󹹽�ģ��ʱָ����ʵ������ƣ����Ƹ�ʽΪ������ʷ������aaa_bbb_ccc��
//aaaΪbbb��ĸ��,bbbΪccc��ĸ��
//��ʵ��Ĳ�������ָ������Ĳ���
//ʹ�÷��� ��DetectorConstruction��Construct������  ����OBJImporter::modelbulider(�ļ����������PhysicalVolume)��

namespace OBJImporter
{
	//�ָ��ַ��������������ַ����ͷָ��ʶ�����ɷָ� ����Ĭ�ϲ��á�_���ָ�
	//���ڽ�ģ�͹�����ʷ��aaa_bbb_ccc�����Ϊ��aaa������bbb������ccc��
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


	//��ȡobj�ļ��е�ģ�͹�����ʷ�Լ���Ӧ���ϣ���������ʷ���ų�vector����ʽ���Թ�����ʷvectorΪkey������Ϊvalue���map���
	//uniform����ָ���Ƿ����ͳһ�Ĳ��ϣ�ѡtrue�����ͳһ���ϣ�materialname��Ϊͳһ���ϵ����ƣ�uniformĬ��Ϊfalse
	std::map<std::vector<std::string>, std::string> ParseOBJ(std::string FileName, bool uniform, std::string materialname )
	{
		std::cout << std::endl << "**************************************************" << std::endl;
		std::cout << "��ʼ����OBj�ļ��е�ģ��......" << std::endl << std::endl;
		std::map<std::vector<std::string>, std::string> ObjInform;
		std::string LineInform;
		std::ifstream InputFile;
		InputFile.open(FileName);
		if (!InputFile.is_open())
		{
			std::cout << "OBJ�ļ���ʧ�ܣ�" << std::endl;
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
				std::cout << "�ɹ���λ����"<<*(objhistory.end()-1) << ", �乹����ʷΪ��";
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
					std::cout << "����ͳһ�Ĳ���: " << materialname << "��" << std::endl;
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
					std::cout << "��ȷ�ϲ���Ϊ��" << objmaterial << "��" << std::endl;
					ObjInform.insert(std::map<std::vector<std::string>, std::string>::value_type(objhistory, objmaterial));
				}
				else
				{
					std::cout << "��δ�ҵ���Ӧ����" << std::endl;
				}
				flag = false;
			}
		}
		InputFile.close();
		std::cout << std::endl << "�������!" << std::endl;
		std::cout << "**************************************************" << std::endl << std::endl;
		return ObjInform;
	}


	//���ڻ��ģ�͹����������
	int getNOfLayers(std::map<std::vector<std::string>, std::string> ObjInform)
	{
		int currentLayers;
		int maxLayers(0);
		for (const std::pair<std::vector<std::string>, std::string> it : ObjInform)
		{
			currentLayers = it.first.size();
			if (currentLayers > maxLayers)maxLayers = currentLayers;
		}
		std::cout << "ģ�͹����������Ϊ�� " << maxLayers << std::endl;
		return maxLayers;
	}


	//��������������obj�ļ����͵���ģ�͵�ĸ��physicalvolume��һ��Ϊworld�������չ�����ʷ��ģ��һ��һ�㵼�룬��ָ����Ӧ����
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
					G4cout << "���ʣ�" << LogicalVolume[i]->GetMaterial()->GetName() << "�ܶȣ�" << LogicalVolume[i]->GetMaterial()->GetDensity() << G4endl;
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
					G4cout << "���ʣ�" << LogicalVolume[i]->GetMaterial()->GetName() << "�ܶȣ�" << LogicalVolume[i]->GetMaterial()->GetDensity() << G4endl;
					G4cout << G4BestUnit(LogicalVolume[i]->GetMass(false, false, nullptr), "Mass") << G4endl;
					new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), LogicalVolume[i], *((iter->first).end() - 1), G4LogicalVolumeStore::GetInstance()->GetVolume(*(iter->first.end() - 2)), false, 0, true);
					i += 1;
				}

			}
		}


	}

}