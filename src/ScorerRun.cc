#include "ScorerRun.hh"
#include "G4THitsMap.hh"
#include "G4Event.hh"
#include "G4SDManager.hh"
#include "G4VSensitiveDetector.hh"
#include "G4MultiFunctionalDetector.hh"
#include "G4VPrimitiveScorer.hh"
#include "G4StatDouble.hh"

ScorerRun::ScorerRun(const std::vector<G4String> mfdName) : G4Run()
{
    ConstructMFD(mfdName);
}

ScorerRun::~ScorerRun()
{
    size_t Nmap = fRunMap.size();
    for (size_t i = 0; i < Nmap; ++i)
    {
        if (fRunMap[i]) fRunMap[i]->clear();
    }
    fRunMap.clear();
}

void ScorerRun::RecordEvent(const G4Event* anEvent)
{
    G4HCofThisEvent* HCE = anEvent->GetHCofThisEvent();
    if (!HCE) return;

    //=======================================================
    // Sum up HitsMap of this Event  into HitsMap of this RUN
    //=======================================================
    size_t Ncol = fRunMap.size();
    for (size_t i = 0; i < Ncol; ++i)  // Loop over HitsCollection
    {
        G4THitsMap<G4double>* EvtMap = nullptr;

        EvtMap = static_cast<G4THitsMap<G4double>*>(HCE->GetHC(i));

        if (EvtMap)
        {
            *fRunMap[i] += *EvtMap;
        } 
    }

    G4Run::RecordEvent(anEvent);
}

void ScorerRun::Merge(const G4Run* run)
{
    const ScorerRun* localRun = static_cast<const ScorerRun*>(run);

    // copy function returns the fRunMap size if all data is copied
    // so this loop isn't executed the first time around
    for (size_t i = 0; i < fRunMap.size(); ++i)
    {
        *fRunMap[i] += *localRun->fRunMap[i];
    }
    G4Run::Merge(run);
}

void ScorerRun::ConstructMFD(const std::vector<G4String>& mfdName)
{

    G4SDManager* SDman = G4SDManager::GetSDMpointer();
    size_t Nmfd = mfdName.size();
    for (size_t idet = 0; idet < Nmfd; ++idet)  // Loop for all MFD.
    {
        G4String detName = mfdName[idet];
        //--- Seek and Obtain MFD objects from SDmanager.
        G4MultiFunctionalDetector* mfd =
            (G4MultiFunctionalDetector*)(SDman->FindSensitiveDetector(detName));
        //
        if (mfd)
        {
            //--- Loop over the registered primitive scorers.
            for (G4int icol = 0; icol < mfd->GetNumberOfPrimitives(); ++icol)
            {
                G4VPrimitiveScorer* scorer = mfd->GetPrimitive(icol);
                G4String collectionName = scorer->GetName();
                G4String fullCollectionName = detName + "/" + collectionName;
                G4int    collectionID = SDman->GetCollectionID(fullCollectionName);
                if (collectionID >= 0)
                {
                    G4cout << "++ " << fullCollectionName << " id " << collectionID
                        << G4endl;
                    fRunMap.push_back(new G4THitsMap<G4StatDouble>(detName, collectionName));
                }
                else
                {
                    G4cout << "** collection " << fullCollectionName << " not found. "
                        << G4endl;
                }
            }
        }
    }
}
