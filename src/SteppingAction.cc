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
  //get vacstep volume
  auto VacStep1PV=fDetector->GetVacStep1PV();
  auto VacStep2PV=fDetector->GetVacStep2PV();
  if (outputType == "bunch"){
  if ( postvolume == VacStep2PV && prevolume !=VacStep2PV ) {
         // get analysis manager
         auto Eval=aStep->GetPostStepPoint()->GetKineticEnergy()/MeV;
         fEventAction->AddVals(Eval,1);

    //G4cout<< " This part of the code you are currently testing is executed"  << G4endl;
    }
  }
  else if (outputType == "single"){
    if ( postvolume == VacStep1PV && prevolume !=VacStep1PV ) {
         // get analysis manager
         auto fAnalysisManager = G4AnalysisManager::Instance();

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

            auto fAnalysisManager = G4AnalysisManager::Instance();
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
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
