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
                               RunAction* ruAct, G4String outType, G4String version, G4String dipolState
                             )
 : G4UserSteppingAction(),
   fDetector(det), fRunAction(ruAct),fEventAction(eventAction){
outputType=outType;
versionType=version;
dipolStatus = dipolState;
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
  auto postvolume = aStep->GetPostStepPoint()->GetPhysicalVolume();

  auto aTrack = aStep->GetTrack();

  //G4TouchableHistory* theTouchable = (G4TouchableHistory*)(aStep->GetPostStepPoint()->GetTouchable());
  auto core1stat = fRunAction->GetCore1Stat();
  auto core2stat = fRunAction->GetCore2Stat();
  auto dipole1stat = fRunAction->GetDipole1Stat();
  auto dipole2stat = fRunAction->GetDipole2Stat();
  auto dipole3stat = fRunAction->GetDipole3Stat();
  auto cal1stat = fRunAction->GetCal1Stat();
  auto cal2stat = fRunAction->GetCal2Stat();

  auto fAnalysisManager = G4AnalysisManager::Instance();

  if (outputType == "bunch"){
   // now separating the differnt versions
   if((versionType=="Pol" || versionType=="PolCal") && core1stat==1){
     auto VacStep2PV=fDetector->GetVacStep2PV();
     if ( postvolume == VacStep2PV && prevolume !=VacStep2PV ) {
            // get analysis manager
            auto Eval=aStep->GetPostStepPoint()->GetTotalEnergy()/MeV;
            fEventAction->AddVals(Eval,1);}

     if ( postvolume == VacStep2PV && prevolume !=VacStep2PV && aTrack->GetParticleDefinition()->GetPDGEncoding() == 22) {
            // get analysis manager
            auto EGamma=aStep->GetPostStepPoint()->GetTotalEnergy()/MeV;
            fEventAction->AddGammaVals(EGamma,1);}
     if ( postvolume == VacStep2PV && prevolume !=VacStep2PV && aTrack->GetParticleDefinition()->GetPDGEncoding() == 11) {
           // get analysis manager
           auto Ee=aStep->GetPostStepPoint()->GetTotalEnergy()/MeV;
           fEventAction->AddeVals(Ee,1);}
    } // end if version pol|polcal


   if((versionType=="Cal" || versionType=="PolCal")&&(cal1stat==1 || cal2stat==1)){
      auto CrystalPV=fDetector->GetDetectorPV();
      auto VacStep3PV=fDetector->GetVacStep3PV();
      auto VacStep4PV=fDetector->GetVacStep4PV();
      auto AluwrapPV =fDetector->GetAluwrapPV();

      if (prevolume == CrystalPV) { // her i think we have to use prevolume???
            auto edep = aStep->GetTotalEnergyDeposit();
            fEventAction->AddEnergyCalo(edep);}

      // if (cal2stat==1 && postvolume == VacStep3PV && prevolume !=VacStep3PV && aTrack->GetDefinition() == G4OpticalPhoton::OpticalPhotonDefinition()){
      //       //aTrack->SetTrackStatus(fStopAndKill);
      //       auto ephot = aStep->GetTotalEnergyDeposit()/eV;
      //       // G4cout<<ephot<<G4endl;
      //       // auto ephot = aStep->GetPostStepPoint()->GetTotalEnergy()/eV;
      //       fEventAction->AddPhotonEnergy(ephot);   // here the Photon Energy will be added up
      //       fAnalysisManager->FillH1(0, ephot);}
            // G4cout<<ephot<<G4endl;
      if (cal2stat==1 && aStep->GetTotalEnergyDeposit()>0. && aTrack->GetDefinition() == G4OpticalPhoton::OpticalPhotonDefinition()){
            //aTrack->SetTrackStatus(fStopAndKill);
            auto ephot = aStep->GetTotalEnergyDeposit()/eV;
            // G4cout<<ephot<<G4endl;
            // auto ephot = aStep->GetPostStepPoint()->GetTotalEnergy()/eV;
            fEventAction->AddPhotonEnergy(ephot);   // here the Photon Energy will be added up
            fAnalysisManager->FillH1(0, ephot);}

      if (cal1stat==1 && postvolume == VacStep4PV && prevolume !=VacStep4PV && prevolume !=AluwrapPV && aTrack->GetParticleDefinition()->GetPDGEncoding() == 22){
            auto egamma = aStep->GetPostStepPoint()->GetTotalEnergy()/MeV;
            fEventAction->AddGammaEnergy(egamma);}

      // here I kill the electrons
      //if (postvolume == VacStep4PV && prevolume !=VacStep4PV && prevolume !=AluwrapPV && (aTrack->GetParticleDefinition()->GetPDGEncoding() == 11 || aTrack->GetParticleDefinition()->GetPDGEncoding()==-11)){
      //                aTrack->SetTrackStatus(fStopAndKill);}
      // else if (postvolume == VacStep4PV && prevolume !=VacStep4PV && prevolume !=AluwrapPV && aTrack->GetParticleDefinition()->GetPDGEncoding() == -11){
      //                 aTrack->SetTrackStatus(fStopAndKill);}

    } // end if version cal|polcal


  } // end if type bunch

  else if (outputType == "single"){
    if (versionType=="Pol" || versionType=="PolCal"){
      auto VacStep1PV=fDetector->GetVacStep1PV();
      auto VacStep2PV=fDetector->GetVacStep2PV();

      if (core1stat==1 &&  postvolume == VacStep1PV && prevolume !=VacStep1PV ) {
        tupleID = 0;
        WriteSingleEntry(tupleID, aStep);
        }

      if (core2stat==1 && postvolume == VacStep2PV && prevolume !=VacStep2PV ) {
        tupleID = core1stat;
        WriteSingleEntry(tupleID, aStep);
      }

    } // end if version pol|polcal

      if (dipolStatus == "On"){
        auto DipoleVacPV = fDetector->GetBigVacPV();
        auto DipoleVac2PV = fDetector->GetBigVac2PV();
        auto DipoleVac3PV = fDetector->GetDipVac3PV();
        if( dipole1stat==1 && postvolume == DipoleVacPV && prevolume !=DipoleVacPV && aStep->GetPostStepPoint()->GetMomentumDirection().z()>0.) {
          if (versionType == "Pol" || versionType == "PolCal"){
            tupleID = core1stat+core2stat;
          }
          else {
            tupleID = 0;
          }
          WriteSingleEntry(tupleID, aStep);
        } // end if postvolume = DipoleVac1PV
        if( dipole2stat==1 && postvolume == DipoleVac2PV && prevolume !=DipoleVac2PV && aStep->GetPostStepPoint()->GetMomentumDirection().z()>0.) {
          if (versionType == "Pol" || versionType == "PolCal"){
            tupleID = core1stat+core2stat+dipole1stat;
          }
          else {
            tupleID = dipole1stat;
          }
          WriteSingleEntry(tupleID, aStep);
        } // end if postvolume = DipoleVac2PV
        if( dipole3stat==1 && postvolume == DipoleVac3PV && prevolume !=DipoleVac3PV) {
          if (versionType == "Pol" || versionType == "PolCal"){
            tupleID = core1stat+core2stat+dipole1stat+dipole2stat;
          }
          else {
            tupleID = dipole1stat;
          }
          WriteSingleEntry(tupleID, aStep);
        } // end if postvolume = DipoleVac2PV
     } // end if DipolStatus
    if (versionType=="Cal" || versionType == "PolCal"){
       auto VacStep3PV=fDetector->GetVacStep3PV();
       auto VacStep4PV=fDetector->GetVacStep4PV();
       auto AluwrapPV =fDetector->GetAluwrapPV();

      //  if (cal2stat==1 && aStep->GetTotalEnergyDeposit()>0. && aTrack->GetDefinition() == G4OpticalPhoton::OpticalPhotonDefinition()){
       if ( cal2stat==1 && postvolume == VacStep3PV && prevolume !=VacStep3PV && aTrack->GetDefinition() == G4OpticalPhoton::OpticalPhotonDefinition()) {
           if (versionType=="Cal" && dipolStatus=="Off"){
             tupleID = 0;
           }
           else if (versionType=="Cal" && dipolStatus=="On"){
             tupleID = dipole1stat + dipole2stat+dipole3stat;
           }
           else if (versionType=="PolCal" && dipolStatus=="Off"){
             tupleID = core1stat + core2stat+ cal1stat;
           }
           else {
             tupleID = core1stat + core2stat + dipole1stat + dipole2stat+dipole3stat;
           }

           WriteSingleCalEntry(tupleID, aStep);

           }

       if( cal1stat==1 && postvolume == VacStep4PV && prevolume !=VacStep4PV && prevolume != AluwrapPV && aTrack->GetParticleDefinition()->GetPDGEncoding() == 22) {

         if (versionType=="Cal" && dipolStatus=="Off"){
           tupleID = cal1stat;
         }
         else if (versionType=="Cal" && dipolStatus=="On"){
           tupleID = dipole1stat + dipole2stat +dipole3stat +cal1stat;
         }
         else if (versionType=="PolCal" && dipolStatus=="Off"){
           tupleID = core1stat + core2stat;
         }
         else {
           tupleID = core1stat + core2stat + dipole1stat + dipole2stat +dipole3stat+ cal1stat;
         }
         WriteSingleCalEntry(tupleID, aStep);
       }
       /*
       if(postvolume == VacStep4PV && prevolume !=VacStep4PV && prevolume !=AluwrapPV && (aTrack->GetParticleDefinition()->GetPDGEncoding() == 11 || aTrack->GetParticleDefinition()->GetPDGEncoding()==-11)) {
                   aTrack->SetTrackStatus(fStopAndKill);
                 }
 */
       }  // end if version cal|polcal
    } // end if outType=single

  else if (outputType == "shower"){
    if (versionType=="Cal" || versionType == "PolCal"){
      auto edep = aStep->GetTotalEnergyDeposit();
      auto VirtCalo=fDetector->GetVirtCaloPV()->GetLogicalVolume();
      if ( edep>0 && VirtCalo->IsAncestor(postvolume)) {
        //G4cout << "An entry should be written now" << G4endl;
        //G4cout << "poststepvolume  "<< postvolume->GetName()<<G4endl;
        WriteShowerDevEntry(0, aStep);
      }
    }
    else{
      G4cout << "### Warning!  You want to look at the shower development inside the calorimeter, but no calorimeter is placed !###" << G4endl;
      // should throw a real error and stop the run at some point
    }

  } // end if outType=shower
} // end UserSteppingAction


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
