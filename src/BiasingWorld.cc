#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4VSolid.hh"
#include "G4Box.hh"
#include "G4SystemOfUnits.hh"
#include "G4PVPlacement.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"
#include "BiasingOperator.hh"
#include "G4LogicalVolumeStore.hh"
#include <map>
#include "G4PVReplica.hh"
#include "BiasinglWorld.hh"



BiasingWorld::BiasingWorld():G4VUserParallelWorld("mseh"),xbin(1),ybin(1),zbin(1),xsize(0),ysize(0),zsize(0)
{}

BiasingWorld::~BiasingWorld()
{}

void BiasingWorld::Construct()
{
	G4VPhysicalVolume* parallel_world_physical = GetWorld();
	G4LogicalVolume* parallel_world_logical = parallel_world_physical->GetLogicalVolume();
	G4Box* parallel_world_solid = (G4Box*)parallel_world_logical->GetSolid();

	G4Box* envelope_solid=(G4Box*)G4LogicalVolumeStore::GetInstance()->GetVolume("envelope_logical")->GetSolid();
	G4LogicalVolume* envelope_logical = new G4LogicalVolume(envelope_solid, nullptr, "envelope_logical");
	new G4PVPlacement(nullptr, G4ThreeVector(0, 0, 0), envelope_logical, "envelope_physical", parallel_world_logical, false, 0, true);


	xsize = envelope_solid->GetXHalfLength();
	ysize = envelope_solid->GetYHalfLength();
	zsize = envelope_solid->GetZHalfLength();

	parallel_world_logical->SetVisAttributes(G4VisAttributes::GetInvisible());


	//x
	G4VSolid* xslice_s = new G4Box("xslice_s", xsize / xbin, ysize, zsize);
	G4LogicalVolume* xslice_l = new G4LogicalVolume(xslice_s, nullptr, "xslice_l");
	new G4PVReplica("xslice_p", xslice_l, envelope_logical, kXAxis, xbin, 2 * xsize / xbin, 0);

	//y
	G4VSolid* yslice_s = new G4Box("yslice_s", xsize / xbin, ysize/ybin, zsize);
	G4LogicalVolume* yslice_l = new G4LogicalVolume(yslice_s, nullptr, "yslice_l");
	new G4PVReplica("yslice_p", yslice_l, xslice_l, kYAxis, ybin, 2 * ysize / ybin, 0);

	//z
	G4VSolid* zslice_s = new G4Box("zslice_s", xsize / xbin, ysize / ybin, zsize/zbin);
	G4LogicalVolume* zslice_l = new G4LogicalVolume(zslice_s, nullptr, "zslice_l");
	new G4PVReplica("zslice_p", zslice_l, yslice_l, kZAxis, zbin, 2 * zsize / zbin, 0);


}


void BiasingWorld::setNBin(G4int x, G4int y, G4int z)
{
	xbin = x;
	ybin = y;
	zbin = z;
}

G4int BiasingWorld::getCopyNum(G4int i,G4int j,G4int k)
{
	G4int copynum = (i + xbin * j) + xbin * ybin * k;
	return copynum;
}

G4ThreeVector BiasingWorld::getPosition(G4int i,G4int j,G4int k)
{
	G4double x = (2 * i + 1) * xsize / xbin - xsize;
	G4double y = (2 * j + 1) * ysize / ybin - ysize;
	G4double z = (2 * k + 1) * zsize / zbin - zsize;
	return G4ThreeVector(x, y, z);
}


void BiasingWorld::ConstructSD()
{
	BiasingOperator* biasingoperator = new BiasingOperator;
	
	std::map<G4String, G4int> a;


	for (G4int i = 0; i < xbin; i++)
	{
		for (G4int j = 0; j < ybin; j++)
		{
			for (G4int k = 0; k < zbin; k++)
			{
				if (i == 2 || i == xbin - 3 || j == 2 || j == ybin - 3 || k == 2 || k == zbin - 3)
				{
					G4String coordinate = std::to_string(i) + std::to_string(j) + std::to_string(k);
					a[coordinate] = 1;
				}
				if (i == 1 || i == xbin - 2 || j == 1 || j == ybin - 2 || k == 1 || k == zbin - 2)
				{
					G4String coordinate = std::to_string(i) + std::to_string(j) + std::to_string(k);
					a[coordinate] = 4;
				}

				if (i == 0 || i == xbin - 1 || j == 0 || j == ybin - 1 || k == 0 || k == zbin - 1)
				{
					G4String coordinate = std::to_string(i) + std::to_string(j) + std::to_string(k);
					a[coordinate] = 16;
				}
			}
		}
	}
	biasingoperator->setImportance(a);
	biasingoperator->AttachTo(G4LogicalVolumeStore::GetInstance()->GetVolume("zslice_l"));
}