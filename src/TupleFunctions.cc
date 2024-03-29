#include "TupleFunctions.hh"
#include "G4RunManager.hh"
#include "G4AnalysisManager.hh"
#include "EventAction.hh"
#include "globals.hh"

void WriteSingleEntry(int tupleID,const G4Step* aStep){
  auto fAnalysisManager = G4AnalysisManager::Instance();
  fAnalysisManager->FillNtupleIColumn(tupleID,0, aStep->GetTrack()->GetParticleDefinition()->GetPDGEncoding());

  fAnalysisManager->FillNtupleDColumn(tupleID,1, aStep->GetPostStepPoint()->GetTotalEnergy()/CLHEP::MeV);

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

void WriteShowerDevEntry(int tupleID,const G4Step* aStep){
  auto fAnalysisManager = G4AnalysisManager::Instance();
  fAnalysisManager->FillNtupleIColumn(tupleID,0, aStep->GetTrack()->GetParticleDefinition()->GetPDGEncoding());
  fAnalysisManager->FillNtupleDColumn(tupleID,1, aStep->GetPostStepPoint()->GetTotalEnergy()/CLHEP::MeV);
  fAnalysisManager->FillNtupleDColumn(tupleID,2,aStep->GetTotalEnergyDeposit()/CLHEP::MeV);
  fAnalysisManager->FillNtupleDColumn(tupleID,3, aStep->GetPostStepPoint()->GetPosition().x());
  fAnalysisManager->FillNtupleDColumn(tupleID,4, aStep->GetPostStepPoint()->GetPosition().y());
  fAnalysisManager->FillNtupleDColumn(tupleID,5, aStep->GetPostStepPoint()->GetPosition().z());
  fAnalysisManager->FillNtupleIColumn(tupleID,6, aStep->GetTrack()->GetTrackID());
  fAnalysisManager->FillNtupleIColumn(tupleID,7, aStep->GetTrack()->GetParentID());
  fAnalysisManager->FillNtupleIColumn(tupleID,8, G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID());
  fAnalysisManager->FillNtupleSColumn(tupleID,9, aStep->GetPostStepPoint()->GetPhysicalVolume()->GetName());
  fAnalysisManager->AddNtupleRow(tupleID);
}

void WriteSingleCalEntry(int tupleID,const G4Step* aStep){
  auto fAnalysisManager = G4AnalysisManager::Instance();
  fAnalysisManager->FillNtupleIColumn(tupleID,0, aStep->GetTrack()->GetParticleDefinition()->GetPDGEncoding());

  fAnalysisManager->FillNtupleDColumn(tupleID,1,aStep->GetPostStepPoint()->GetTotalEnergy()/CLHEP::eV );
  fAnalysisManager->FillNtupleDColumn(tupleID,2,aStep->GetPostStepPoint()->GetTouchable()->GetReplicaNumber(2));  // here the 1 means that it takes the copy numer of its mother volume

  fAnalysisManager->FillNtupleDColumn(tupleID,3, aStep->GetPostStepPoint()->GetPosition().x()/CLHEP::mm);
  fAnalysisManager->FillNtupleDColumn(tupleID,4, aStep->GetPostStepPoint()->GetPosition().y()/CLHEP::mm);
  fAnalysisManager->FillNtupleDColumn(tupleID,5, aStep->GetPostStepPoint()->GetPosition().z()/CLHEP::mm);
  fAnalysisManager->AddNtupleRow(tupleID);
}

void BookSingleTuple(G4String name, G4String title){
  auto fAnalysisManager = G4AnalysisManager::Instance();
  fAnalysisManager->CreateNtuple(name, title);
  fAnalysisManager->CreateNtupleIColumn("pdg");
  fAnalysisManager->CreateNtupleDColumn("E");
  fAnalysisManager->CreateNtupleDColumn("x");
  fAnalysisManager->CreateNtupleDColumn("y");
  fAnalysisManager->CreateNtupleDColumn("z");
  fAnalysisManager->CreateNtupleDColumn("startx");
  fAnalysisManager->CreateNtupleDColumn("starty");
  fAnalysisManager->CreateNtupleDColumn("startz");
  fAnalysisManager->CreateNtupleDColumn("px");
  fAnalysisManager->CreateNtupleDColumn("py");
  fAnalysisManager->CreateNtupleDColumn("pz");
  fAnalysisManager->CreateNtupleDColumn("Polx");
  fAnalysisManager->CreateNtupleDColumn("Poly");
  fAnalysisManager->CreateNtupleDColumn("Polz");
  fAnalysisManager->CreateNtupleDColumn("TrackID");
  fAnalysisManager->CreateNtupleDColumn("ParentID");
  fAnalysisManager->CreateNtupleDColumn("EventID");
  fAnalysisManager->FinishNtuple();
}

void BookSingleCalTuple(G4String name, G4String title){
  auto fAnalysisManager = G4AnalysisManager::Instance();
  fAnalysisManager->CreateNtuple(name,title);
  fAnalysisManager->CreateNtupleIColumn("pdg");
  fAnalysisManager->CreateNtupleDColumn("E");
  fAnalysisManager->CreateNtupleDColumn("CopyNumber");
  fAnalysisManager->CreateNtupleDColumn("x");
  fAnalysisManager->CreateNtupleDColumn("y");
  fAnalysisManager->CreateNtupleDColumn("z");
  fAnalysisManager->FinishNtuple();
}

void BookBunchTuple(G4String name, G4String title){
  auto fAnalysisManager = G4AnalysisManager::Instance();
  fAnalysisManager->CreateNtuple(name, title);
  fAnalysisManager->CreateNtupleDColumn("Esum");
  fAnalysisManager->CreateNtupleIColumn("NP");
  fAnalysisManager->CreateNtupleDColumn("EGammaSum");
  fAnalysisManager->CreateNtupleIColumn("NGamma");
  fAnalysisManager->CreateNtupleDColumn("EeSum");
  fAnalysisManager->CreateNtupleIColumn("Ne");
  fAnalysisManager->FinishNtuple();
}


EventAction* eventAction;
void BookBunchCalTuple(G4String name, G4String title, G4int ncryst){
  auto fAnalysisManager = G4AnalysisManager::Instance();
  fAnalysisManager->CreateNtuple(name, title);
  fAnalysisManager->CreateNtupleDColumn("EIn");
  fAnalysisManager->CreateNtupleDColumn("EPhotonSum");
  for(int i=0; i<ncryst; i++){
    G4String cName = "Edep" + std::to_string(i);
    fAnalysisManager->CreateNtupleDColumn(cName);
  }
  fAnalysisManager->FinishNtuple();
}

void BookShowerDevTuple(G4String name, G4String title){
  auto fAnalysisManager = G4AnalysisManager::Instance();
  fAnalysisManager->CreateNtuple(name, title);
  fAnalysisManager->CreateNtupleIColumn("pdg");
  fAnalysisManager->CreateNtupleDColumn("E");
  fAnalysisManager->CreateNtupleDColumn("Edep");
  fAnalysisManager->CreateNtupleDColumn("x");
  fAnalysisManager->CreateNtupleDColumn("y");
  fAnalysisManager->CreateNtupleDColumn("z");
  fAnalysisManager->CreateNtupleIColumn("TrackID");
  fAnalysisManager->CreateNtupleIColumn("ParentID");
  fAnalysisManager->CreateNtupleIColumn("EventID");
  fAnalysisManager->CreateNtupleSColumn("Volume");
  fAnalysisManager->FinishNtuple(0);
}
