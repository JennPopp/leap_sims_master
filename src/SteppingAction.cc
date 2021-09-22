//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
/// \file polarisation/Pol01/src/SteppingAction.cc
/// \brief Implementation of the SteppingAction class
//
// $Id: SteppingAction.cc 98772 2016-08-09 14:25:31Z gcosmo $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "SteppingAction.hh"
#include "DetectorConstruction.hh"
#include "EventAction.hh"

#include "G4RunManager.hh"
#include "G4PolarizationHelper.hh"

#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

// all header files below are coppied from OpNoviceStackingAction.cc
#include "G4VProcess.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"
#include "G4Track.hh"
// heder files below are coppied from QuaSiPMT.cc
#include "G4OpticalPhoton.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::SteppingAction(DetectorConstruction* det,
                               EventAction* eventAction,
                               RunAction* ruAct, G4String outType, G4String version
                             )
 : G4UserSteppingAction(),
   fDetector(det), fRunAction(ruAct),fEventAction(eventAction){
outputType=outType;
versionType=version;
 }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::~SteppingAction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SteppingAction::UserSteppingAction(const G4Step* aStep)
{
  G4StepPoint* endPoint = aStep->GetPostStepPoint();

  G4String procName = endPoint->GetProcessDefinedStep()->GetProcessName();
  fRunAction->CountProcesses(procName);

  // get pre-step-volume of the current aStep
  auto prevolume = aStep->GetPreStepPoint()->GetTouchableHandle()->GetVolume();
  //get post-step-volume of the current aStep
  auto postvolume = aStep->GetPostStepPoint()->GetTouchableHandle()->GetVolume();

  auto aTrack = aStep->GetTrack();

  G4TouchableHistory* theTouchable = (G4TouchableHistory*)(aStep->GetPostStepPoint()->GetTouchable());

  auto fAnalysisManager = G4AnalysisManager::Instance();

  if (outputType == "bunch"){
   // now separating the differnt versions
   if(versionType=="Pol"){
     auto VacStep2PV=fDetector->GetVacStep2PV();
     if ( postvolume == VacStep2PV && prevolume !=VacStep2PV ) {
            // get analysis manager
            auto Eval=aStep->GetPostStepPoint()->GetKineticEnergy()/MeV;
            fEventAction->AddVals(Eval,1);}

     if ( postvolume == VacStep2PV && prevolume !=VacStep2PV && aTrack->GetParticleDefinition()->GetPDGEncoding() == 22) {
            // get analysis manager
            auto EGamma=aStep->GetPostStepPoint()->GetKineticEnergy()/MeV;
            fEventAction->AddGammaVals(EGamma,1);}
    }

   else if(versionType=="Cal"){
      auto CrystalPV=fDetector->GetDetectorPV();
      auto VacStep3PV=fDetector->GetVacStep3PV();
      auto VacStep4PV=fDetector->GetVacStep4PV();
      auto AluwrapPV =fDetector->GetAluwrapPV();

      if (prevolume == CrystalPV) { // her i think we have to use prevolume???
            auto edep = aStep->GetTotalEnergyDeposit();
            fEventAction->AddEnergyCalo(edep);}

      if (postvolume == VacStep3PV && prevolume !=VacStep3PV && aTrack->GetDefinition() == G4OpticalPhoton::OpticalPhotonDefinition()){
            auto ephot = aStep->GetPostStepPoint()->GetTotalEnergy()/eV;
            fEventAction->AddPhotonEnergy(ephot);   // here the Photon Energy will be added up
            fAnalysisManager->FillH1(0, ephot);}


      if (postvolume == VacStep4PV && prevolume !=VacStep4PV && prevolume !=AluwrapPV && aTrack->GetParticleDefinition()->GetPDGEncoding() == 22){
            auto egamma = aStep->GetPostStepPoint()->GetTotalEnergy()/MeV;
            fEventAction->AddGammaEnergy(egamma);}

      // here I kill the eletrons
      // if (postvolume == VacStep4PV && prevolume !=VacStep4PV && prevolume !=AluwrapPV && (aTrack->GetParticleDefinition()->GetPDGEncoding() == 11 || aTrack->GetParticleDefinition()->GetPDGEncoding()==-11)){
      //                 aTrack->SetTrackStatus(fStopAndKill);}
      // else if (postvolume == VacStep4PV && prevolume !=VacStep4PV && prevolume !=AluwrapPV && aTrack->GetParticleDefinition()->GetPDGEncoding() == -11){
      //                 aTrack->SetTrackStatus(fStopAndKill);}

    }
    else if(versionType="PolCal"){
      auto VacStep2PV=fDetector->GetVacStep2PV();
      auto CrystalPV=fDetector->GetDetectorPV();
      auto VacStep3PV=fDetector->GetVacStep3PV();
      auto VacStep4PV=fDetector->GetVacStep4PV();
      auto AluwrapPV =fDetector->GetAluwrapPV();

      if ( postvolume == VacStep2PV && prevolume !=VacStep2PV ) {
             // get analysis manager
             auto Eval=aStep->GetPostStepPoint()->GetKineticEnergy()/MeV;
             fEventAction->AddVals(Eval,1);}

      if ( postvolume == VacStep2PV && prevolume !=VacStep2PV && aTrack->GetParticleDefinition()->GetPDGEncoding() == 22) {
             // get analysis manager
             auto EGamma=aStep->GetPostStepPoint()->GetKineticEnergy()/MeV;
             fEventAction->AddGammaVals(EGamma,1);}

      if (prevolume == CrystalPV) { // her i think we have to use prevolume???
             auto edep = aStep->GetTotalEnergyDeposit();
             fEventAction->AddEnergyCalo(edep);}
      //
      if (postvolume == VacStep3PV && prevolume !=VacStep3PV && aTrack->GetDefinition() == G4OpticalPhoton::OpticalPhotonDefinition()){
            auto ephot = aStep->GetPostStepPoint()->GetTotalEnergy()/eV;
            fEventAction->AddPhotonEnergy(ephot);   // here the Photon Energy will be added up
            fAnalysisManager->FillH1(0, ephot);}
      //
      if (postvolume == VacStep4PV && prevolume !=VacStep4PV && prevolume !=AluwrapPV && aTrack->GetParticleDefinition()->GetPDGEncoding() == 22){
            auto egamma = aStep->GetPostStepPoint()->GetTotalEnergy()/MeV;
            fEventAction->AddGammaEnergy(egamma);}

      // // here I kill the eletrons
      // if (postvolume == VacStep4PV && prevolume !=VacStep4PV && prevolume !=AluwrapPV && (aTrack->GetParticleDefinition()->GetPDGEncoding() == 11 || aTrack->GetParticleDefinition()->GetPDGEncoding()==-11)){
      //                 aTrack->SetTrackStatus(fStopAndKill);}

    }

   }

  else if (outputType == "single"){
    if (versionType=="Pol"){
      auto VacStep1PV=fDetector->GetVacStep1PV();
      auto VacStep2PV=fDetector->GetVacStep2PV();
      if ( postvolume == VacStep1PV && prevolume !=VacStep1PV ) {

         // fill ntuple id=0
         fAnalysisManager->FillNtupleIColumn(0,0, aStep->GetTrack()->GetParticleDefinition()->GetPDGEncoding());

         fAnalysisManager->FillNtupleDColumn(0,1, aStep->GetPostStepPoint()->GetKineticEnergy()/MeV);

         fAnalysisManager->FillNtupleDColumn(0,2, aStep->GetPostStepPoint()->GetPosition().x());
         fAnalysisManager->FillNtupleDColumn(0,3, aStep->GetPostStepPoint()->GetPosition().y());
         fAnalysisManager->FillNtupleDColumn(0,4, aStep->GetPostStepPoint()->GetPosition().z());

         fAnalysisManager->FillNtupleDColumn(0,5, aStep->GetTrack()->GetVertexPosition().x());
         fAnalysisManager->FillNtupleDColumn(0,6, aStep->GetTrack()->GetVertexPosition().y());
         fAnalysisManager->FillNtupleDColumn(0,7, aStep->GetTrack()->GetVertexPosition().z());

         fAnalysisManager->FillNtupleDColumn(0,8, aStep->GetPostStepPoint()->GetMomentumDirection().x());
         fAnalysisManager->FillNtupleDColumn(0,9, aStep->GetPostStepPoint()->GetMomentumDirection().y());
         fAnalysisManager->FillNtupleDColumn(0,10, aStep->GetPostStepPoint()->GetMomentumDirection().z());

         fAnalysisManager->FillNtupleDColumn(0,11, aStep->GetTrack()->GetPolarization().x());
         fAnalysisManager->FillNtupleDColumn(0,12, aStep->GetTrack()->GetPolarization().y());
         fAnalysisManager->FillNtupleDColumn(0,13, aStep->GetTrack()->GetPolarization().z());

         fAnalysisManager->FillNtupleDColumn(0,14, aStep->GetTrack()->GetTrackID());
         fAnalysisManager->FillNtupleDColumn(0,15, aStep->GetTrack()->GetParentID());
         fAnalysisManager->FillNtupleDColumn(0,16, G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID());

         fAnalysisManager->AddNtupleRow(0);

    //G4cout<< " This part of the code you are currently testing is executed"  << G4endl;
       }

       if ( postvolume == VacStep2PV && prevolume !=VacStep2PV ) {

           //~//~//~//
           fAnalysisManager->FillNtupleIColumn(1,0, aStep->GetTrack()->GetParticleDefinition()->GetPDGEncoding());

           fAnalysisManager->FillNtupleDColumn(1,1, aStep->GetPostStepPoint()->GetKineticEnergy()/MeV);

           fAnalysisManager->FillNtupleDColumn(1,2, aStep->GetPostStepPoint()->GetPosition().x());
           fAnalysisManager->FillNtupleDColumn(1,3, aStep->GetPostStepPoint()->GetPosition().y());
           fAnalysisManager->FillNtupleDColumn(1,4, aStep->GetPostStepPoint()->GetPosition().z());

           fAnalysisManager->FillNtupleDColumn(1,5, aStep->GetTrack()->GetVertexPosition().x());
           fAnalysisManager->FillNtupleDColumn(1,6, aStep->GetTrack()->GetVertexPosition().y());
           fAnalysisManager->FillNtupleDColumn(1,7, aStep->GetTrack()->GetVertexPosition().z());

           fAnalysisManager->FillNtupleDColumn(1,8, aStep->GetPostStepPoint()->GetMomentumDirection().x());
           fAnalysisManager->FillNtupleDColumn(1,9, aStep->GetPostStepPoint()->GetMomentumDirection().y());
           fAnalysisManager->FillNtupleDColumn(1,10, aStep->GetPostStepPoint()->GetMomentumDirection().z());

           fAnalysisManager->FillNtupleDColumn(1,11, aStep->GetTrack()->GetPolarization().x());
           fAnalysisManager->FillNtupleDColumn(1,12, aStep->GetTrack()->GetPolarization().y());
           fAnalysisManager->FillNtupleDColumn(1,13, aStep->GetTrack()->GetPolarization().z());

           fAnalysisManager->FillNtupleDColumn(1,14, aStep->GetTrack()->GetTrackID());
           fAnalysisManager->FillNtupleDColumn(1,15, aStep->GetTrack()->GetParentID());
           fAnalysisManager->FillNtupleDColumn(1,16, G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID());

           fAnalysisManager->AddNtupleRow(1);


      //G4cout<< " This part of the code you are currently testing is executed"  << G4endl;
         }
    }
    else if (versionType=="Cal"){
      auto VacStep3PV=fDetector->GetVacStep3PV();
      auto VacStep4PV=fDetector->GetVacStep4PV();
      auto AluwrapPV =fDetector->GetAluwrapPV();

      if ( postvolume == VacStep3PV && prevolume !=VacStep3PV && aTrack->GetDefinition() == G4OpticalPhoton::OpticalPhotonDefinition()) {

          // fill ntuple id=0
          fAnalysisManager->FillNtupleIColumn(0,0, aStep->GetTrack()->GetParticleDefinition()->GetPDGEncoding());

          fAnalysisManager->FillNtupleDColumn(0,1,aStep->GetPostStepPoint()->GetTotalEnergy()/eV );
          fAnalysisManager->FillNtupleDColumn(0,2,theTouchable->GetReplicaNumber(2));  // here the 1 means that it takes the copy numer of its mother volume

          fAnalysisManager->FillNtupleDColumn(0,3, aStep->GetPostStepPoint()->GetPosition().x()/mm);
          fAnalysisManager->FillNtupleDColumn(0,4, aStep->GetPostStepPoint()->GetPosition().y()/mm);
          fAnalysisManager->FillNtupleDColumn(0,5, aStep->GetPostStepPoint()->GetPosition().z()/mm);
          fAnalysisManager->AddNtupleRow(0);}

      if( postvolume == VacStep4PV && prevolume !=VacStep4PV && prevolume != AluwrapPV && aTrack->GetParticleDefinition()->GetPDGEncoding() == 22) {

          // fill ntuple id=0
          fAnalysisManager->FillNtupleIColumn(1,0, aStep->GetTrack()->GetParticleDefinition()->GetPDGEncoding());

          fAnalysisManager->FillNtupleDColumn(1,1,aStep->GetPostStepPoint()->GetTotalEnergy()/MeV );
          fAnalysisManager->FillNtupleDColumn(1,2,theTouchable->GetReplicaNumber(1));  // here the 1 means that it takes the copy numer of its mother volume

          fAnalysisManager->FillNtupleDColumn(1,3, aStep->GetPostStepPoint()->GetPosition().x()/mm);
          fAnalysisManager->FillNtupleDColumn(1,4, aStep->GetPostStepPoint()->GetPosition().y()/mm);
          fAnalysisManager->FillNtupleDColumn(1,5, aStep->GetPostStepPoint()->GetPosition().z()/mm);
          fAnalysisManager->AddNtupleRow(1);}
      //
      // if(postvolume == VacStep4PV && prevolume !=VacStep4PV && prevolume !=AluwrapPV && (aTrack->GetParticleDefinition()->GetPDGEncoding() == 11 || aTrack->GetParticleDefinition()->GetPDGEncoding()==-11)) {
      //             aTrack->SetTrackStatus(fStopAndKill);}


      }

      else if (versionType=="PolCal"){
        auto VacStep1PV=fDetector->GetVacStep1PV();
        auto VacStep2PV=fDetector->GetVacStep2PV();
        auto VacStep3PV=fDetector->GetVacStep3PV();
        auto VacStep4PV=fDetector->GetVacStep4PV();
        auto AluwrapPV =fDetector->GetAluwrapPV();

        // G4cout<< " THIS PART OF THE CODE IS RUNNING"  << G4endl;
        if ( postvolume == VacStep1PV && prevolume !=VacStep1PV ) {

           // fill ntuple id=0
           fAnalysisManager->FillNtupleIColumn(0,0, aStep->GetTrack()->GetParticleDefinition()->GetPDGEncoding());

           fAnalysisManager->FillNtupleDColumn(0,1, aStep->GetPostStepPoint()->GetKineticEnergy()/MeV);

           fAnalysisManager->FillNtupleDColumn(0,2, aStep->GetPostStepPoint()->GetPosition().x());
           fAnalysisManager->FillNtupleDColumn(0,3, aStep->GetPostStepPoint()->GetPosition().y());
           fAnalysisManager->FillNtupleDColumn(0,4, aStep->GetPostStepPoint()->GetPosition().z());

           fAnalysisManager->FillNtupleDColumn(0,5, aStep->GetTrack()->GetVertexPosition().x());
           fAnalysisManager->FillNtupleDColumn(0,6, aStep->GetTrack()->GetVertexPosition().y());
           fAnalysisManager->FillNtupleDColumn(0,7, aStep->GetTrack()->GetVertexPosition().z());

           fAnalysisManager->FillNtupleDColumn(0,8, aStep->GetPostStepPoint()->GetMomentumDirection().x());
           fAnalysisManager->FillNtupleDColumn(0,9, aStep->GetPostStepPoint()->GetMomentumDirection().y());
           fAnalysisManager->FillNtupleDColumn(0,10, aStep->GetPostStepPoint()->GetMomentumDirection().z());

           fAnalysisManager->FillNtupleDColumn(0,11, aStep->GetTrack()->GetPolarization().x());
           fAnalysisManager->FillNtupleDColumn(0,12, aStep->GetTrack()->GetPolarization().y());
           fAnalysisManager->FillNtupleDColumn(0,13, aStep->GetTrack()->GetPolarization().z());

           fAnalysisManager->FillNtupleDColumn(0,14, aStep->GetTrack()->GetTrackID());
           fAnalysisManager->FillNtupleDColumn(0,15, aStep->GetTrack()->GetParentID());
           fAnalysisManager->FillNtupleDColumn(0,16, G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID());

           fAnalysisManager->AddNtupleRow(0);

      //G4cout<< " This part of the code you are currently testing is executed"  << G4endl;
         }

         if ( postvolume == VacStep2PV && prevolume !=VacStep2PV ) {

             //~//~//~//
             fAnalysisManager->FillNtupleIColumn(1,0, aStep->GetTrack()->GetParticleDefinition()->GetPDGEncoding());

             fAnalysisManager->FillNtupleDColumn(1,1, aStep->GetPostStepPoint()->GetKineticEnergy()/MeV);

             fAnalysisManager->FillNtupleDColumn(1,2, aStep->GetPostStepPoint()->GetPosition().x());
             fAnalysisManager->FillNtupleDColumn(1,3, aStep->GetPostStepPoint()->GetPosition().y());
             fAnalysisManager->FillNtupleDColumn(1,4, aStep->GetPostStepPoint()->GetPosition().z());

             fAnalysisManager->FillNtupleDColumn(1,5, aStep->GetTrack()->GetVertexPosition().x());
             fAnalysisManager->FillNtupleDColumn(1,6, aStep->GetTrack()->GetVertexPosition().y());
             fAnalysisManager->FillNtupleDColumn(1,7, aStep->GetTrack()->GetVertexPosition().z());

             fAnalysisManager->FillNtupleDColumn(1,8, aStep->GetPostStepPoint()->GetMomentumDirection().x());
             fAnalysisManager->FillNtupleDColumn(1,9, aStep->GetPostStepPoint()->GetMomentumDirection().y());
             fAnalysisManager->FillNtupleDColumn(1,10, aStep->GetPostStepPoint()->GetMomentumDirection().z());

             fAnalysisManager->FillNtupleDColumn(1,11, aStep->GetTrack()->GetPolarization().x());
             fAnalysisManager->FillNtupleDColumn(1,12, aStep->GetTrack()->GetPolarization().y());
             fAnalysisManager->FillNtupleDColumn(1,13, aStep->GetTrack()->GetPolarization().z());

             fAnalysisManager->FillNtupleDColumn(1,14, aStep->GetTrack()->GetTrackID());
             fAnalysisManager->FillNtupleDColumn(1,15, aStep->GetTrack()->GetParentID());
             fAnalysisManager->FillNtupleDColumn(1,16, G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID());

             fAnalysisManager->AddNtupleRow(1);
          }

          if ( postvolume == VacStep3PV && prevolume !=VacStep3PV && aTrack->GetDefinition() == G4OpticalPhoton::OpticalPhotonDefinition()) {

              // fill ntuple id=2
              fAnalysisManager->FillNtupleIColumn(2,0, aStep->GetTrack()->GetParticleDefinition()->GetPDGEncoding());

              fAnalysisManager->FillNtupleDColumn(2,1,aStep->GetPostStepPoint()->GetTotalEnergy()/eV );
              fAnalysisManager->FillNtupleDColumn(2,2,theTouchable->GetReplicaNumber(2));  // here the 1 means that it takes the copy numer of its mother volume

              fAnalysisManager->FillNtupleDColumn(2,3, aStep->GetPostStepPoint()->GetPosition().x()/mm);
              fAnalysisManager->FillNtupleDColumn(2,4, aStep->GetPostStepPoint()->GetPosition().y()/mm);
              fAnalysisManager->FillNtupleDColumn(2,5, aStep->GetPostStepPoint()->GetPosition().z()/mm);
              fAnalysisManager->AddNtupleRow(2);}


            if( postvolume == VacStep4PV && prevolume !=VacStep4PV && prevolume != AluwrapPV && aTrack->GetParticleDefinition()->GetPDGEncoding() == 22) {

              // fill ntuple id=3
              fAnalysisManager->FillNtupleIColumn(3,0, aStep->GetTrack()->GetParticleDefinition()->GetPDGEncoding());

              fAnalysisManager->FillNtupleDColumn(3,1,aStep->GetPostStepPoint()->GetTotalEnergy()/MeV );
              fAnalysisManager->FillNtupleDColumn(3,2,theTouchable->GetReplicaNumber(1));  // here the 1 means that it takes the copy numer of its mother volume

              fAnalysisManager->FillNtupleDColumn(3,3, aStep->GetPostStepPoint()->GetPosition().x()/mm);
              fAnalysisManager->FillNtupleDColumn(3,4, aStep->GetPostStepPoint()->GetPosition().y()/mm);
              fAnalysisManager->FillNtupleDColumn(3,5, aStep->GetPostStepPoint()->GetPosition().z()/mm);
              fAnalysisManager->AddNtupleRow(3);}

            // if(postvolume == VacStep4PV && prevolume !=VacStep4PV && prevolume !=AluwrapPV && (aTrack->GetParticleDefinition()->GetPDGEncoding() == 11 || aTrack->GetParticleDefinition()->GetPDGEncoding()==-11)) {
            //       aTrack->SetTrackStatus(fStopAndKill);}


        }
    }
  }


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
