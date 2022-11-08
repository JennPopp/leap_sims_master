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
#include "TupleFunctions.hh"

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

  //G4TouchableHistory* theTouchable = (G4TouchableHistory*)(aStep->GetPostStepPoint()->GetTouchable());

  auto fAnalysisManager = G4AnalysisManager::Instance();

  if (outputType == "bunch"){
   // now separating the differnt versions
   if(versionType=="Pol" || versionType=="PolCal"){
     auto VacStep2PV=fDetector->GetVacStep2PV();
     if ( postvolume == VacStep2PV && prevolume !=VacStep2PV ) {
            // get analysis manager
            auto Eval=aStep->GetPostStepPoint()->GetKineticEnergy()/MeV;
            fEventAction->AddVals(Eval,1);}

     if ( postvolume == VacStep2PV && prevolume !=VacStep2PV && aTrack->GetParticleDefinition()->GetPDGEncoding() == 22) {
            // get analysis manager
            auto EGamma=aStep->GetPostStepPoint()->GetKineticEnergy()/MeV;
            fEventAction->AddGammaVals(EGamma,1);}
     if ( postvolume == VacStep2PV && prevolume !=VacStep2PV && aTrack->GetParticleDefinition()->GetPDGEncoding() == 11) {
           // get analysis manager
           auto Ee=aStep->GetPostStepPoint()->GetKineticEnergy()/MeV;
           fEventAction->AddeVals(Ee,1);}
    }


   if(versionType=="Cal" || versionType=="PolCal"){
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
      //if (postvolume == VacStep4PV && prevolume !=VacStep4PV && prevolume !=AluwrapPV && (aTrack->GetParticleDefinition()->GetPDGEncoding() == 11 || aTrack->GetParticleDefinition()->GetPDGEncoding()==-11)){
      //                aTrack->SetTrackStatus(fStopAndKill);}
      // else if (postvolume == VacStep4PV && prevolume !=VacStep4PV && prevolume !=AluwrapPV && aTrack->GetParticleDefinition()->GetPDGEncoding() == -11){
      //                 aTrack->SetTrackStatus(fStopAndKill);}

    }


   }

  else if (outputType == "single"){
    if (versionType=="Pol" || versionType=="PolCal"){
      auto VacStep1PV=fDetector->GetVacStep1PV();
      auto VacStep2PV=fDetector->GetVacStep2PV();
      if ( postvolume == VacStep1PV && prevolume !=VacStep1PV ) {
        tupleID = 0;
        WriteSingleEntry(tupleID, aStep);

    //G4cout<< " This part of the code you are currently testing is executed"  << G4endl;
       }

       if ( postvolume == VacStep2PV && prevolume !=VacStep2PV ) {
         tupleID = 1;
         WriteSingleEntry(tupleID, aStep);


      //G4cout<< " This part of the code you are currently testing is executed"  << G4endl;
         }
    }
    if (versionType=="Cal" || versionType == "PolCal"){
      auto VacStep3PV=fDetector->GetVacStep3PV();
      auto VacStep4PV=fDetector->GetVacStep4PV();
      auto AluwrapPV =fDetector->GetAluwrapPV();

      if ( postvolume == VacStep3PV && prevolume !=VacStep3PV && aTrack->GetDefinition() == G4OpticalPhoton::OpticalPhotonDefinition()) {
          if (versionType=="Cal"){
            tupleID = 0;
          }
          else {
            tupleID = 2;
          }
          WriteSingleCalEntry(tupleID, aStep);

          }

      if( postvolume == VacStep4PV && prevolume !=VacStep4PV && prevolume != AluwrapPV && aTrack->GetParticleDefinition()->GetPDGEncoding() == 22) {

        if (versionType=="Cal"){
          tupleID = 1;
        }
        else {
          tupleID = 3;
        }
        WriteSingleCalEntry(tupleID, aStep);
      }
      /*
      if(postvolume == VacStep4PV && prevolume !=VacStep4PV && prevolume !=AluwrapPV && (aTrack->GetParticleDefinition()->GetPDGEncoding() == 11 || aTrack->GetParticleDefinition()->GetPDGEncoding()==-11)) {
                  aTrack->SetTrackStatus(fStopAndKill);
                }
                
*/
      }
    }
  }


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
