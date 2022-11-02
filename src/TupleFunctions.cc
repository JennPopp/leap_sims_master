#include "TupleFunctions.hh"
#include "G4RunManager.hh"
#include "G4AnalysisManager.hh"

#include "globals.hh"

void WriteSingleEntry(int tupleID,const G4Step* aStep){
  auto fAnalysisManager = G4AnalysisManager::Instance();
  fAnalysisManager->FillNtupleIColumn(tupleID,0, aStep->GetTrack()->GetParticleDefinition()->GetPDGEncoding());

  fAnalysisManager->FillNtupleDColumn(tupleID,1, aStep->GetPostStepPoint()->GetKineticEnergy()/CLHEP::MeV);

  fAnalysisManager->FillNtupleDColumn(tupleID,2, aStep->GetPostStepPoint()->GetPosition().x());
  fAnalysisManager->FillNtupleDColumn(tupleID,3, aStep->GetPostStepPoint()->GetPosition().y());
  fAnalysisManager->FillNtupleDColumn(tupleID,4, aStep->GetPostStepPoint()->GetPosition().z());

  fAnalysisManager->FillNtupleDColumn(tupleID,5, aStep->GetTrack()->GetVertexPosition().x());
  fAnalysisManager->FillNtupleDColumn(tupleID,6, aStep->GetTrack()->GetVertexPosition().y());
  fAnalysisManager->FillNtupleDColumn(tupleID,7, aStep->GetTrack()->GetVertexPosition().z());

  fAnalysisManager->FillNtupleDColumn(tupleID,8, aStep->GetPostStepPoint()->GetMomentumDirection().x());
  fAnalysisManager->FillNtupleDColumn(tupleID,9, aStep->GetPostStepPoint()->GetMomentumDirection().y());
  fAnalysisManager->FillNtupleDColumn(tupleID,10, aStep->GetPostStepPoint()->GetMomentumDirection().z());

  fAnalysisManager->FillNtupleDColumn(tupleID,11, aStep->GetTrack()->GetPolarization().x());
  fAnalysisManager->FillNtupleDColumn(tupleID,12, aStep->GetTrack()->GetPolarization().y());
  fAnalysisManager->FillNtupleDColumn(tupleID,13, aStep->GetTrack()->GetPolarization().z());

  fAnalysisManager->FillNtupleDColumn(tupleID,14, aStep->GetTrack()->GetTrackID());
  fAnalysisManager->FillNtupleDColumn(tupleID,15, aStep->GetTrack()->GetParentID());
  fAnalysisManager->FillNtupleDColumn(tupleID,16, G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID());

  fAnalysisManager->AddNtupleRow(tupleID);
}

void WriteSingleCalEntry(int tupleID,const G4Step* aStep){
  auto fAnalysisManager = G4AnalysisManager::Instance();
  fAnalysisManager->FillNtupleIColumn(tupleID,0, aStep->GetTrack()->GetParticleDefinition()->GetPDGEncoding());

  fAnalysisManager->FillNtupleDColumn(tupleID,1,aStep->GetPostStepPoint()->GetTotalEnergy()/eV );
  fAnalysisManager->FillNtupleDColumn(tupleID,2,theTouchable->GetReplicaNumber(2));  // here the 1 means that it takes the copy numer of its mother volume

  fAnalysisManager->FillNtupleDColumn(tupleID,3, aStep->GetPostStepPoint()->GetPosition().x()/mm);
  fAnalysisManager->FillNtupleDColumn(tupleID,4, aStep->GetPostStepPoint()->GetPosition().y()/mm);
  fAnalysisManager->FillNtupleDColumn(tupleID,5, aStep->GetPostStepPoint()->GetPosition().z()/mm);
  fAnalysisManager->AddNtupleRow(tupleID);
}
