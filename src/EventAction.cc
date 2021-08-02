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
/// \file polarisation/Pol01/src/EventAction.cc
/// \brief Implementation of the EventAction class
//
// $Id: EventAction.cc 98772 2016-08-09 14:25:31Z gcosmo $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "EventAction.hh"
#include "RunAction.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::EventAction(RunAction *ra, G4String outType, G4String version)
: G4UserEventAction(),
  fRunAction(ra)
{
  outputType=outType;
  versionType=version;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::~EventAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::BeginOfEventAction(const G4Event*)
{
  if (outputType == "bunch"){ // initialisation per event
    if(versionType == "Pol"){
      fEnergySum = 0.; // Sum of energy of particles behind magnet
      fNP=0; // Number of particles behind magnet
     }
     else if(versionType=="Cal"){
      fEnergyCalo =0.;
     }
     else if(versionType=="PolCal"){
      fEnergySum = 0.; // Sum of energy of particles behind magnet
      fNP=0; // Number of particles behind magnet
      fEnergyCalo =0.;
     }
   }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::EndOfEventAction(const G4Event*)
{
  if (outputType == "bunch"){
    // get analysis manager
    auto analysisManager = G4AnalysisManager::Instance();

    if(versionType=="Pol"){
    // fill ntuple id=0
     analysisManager->FillNtupleDColumn(0,0, fEnergySum);
     analysisManager->FillNtupleIColumn(0,1, fNP);
     analysisManager->AddNtupleRow(0);
    }
    else if(versionType=="Cal"){
     analysisManager->FillNtupleDColumn(0,0, fEnergyCalo);
     analysisManager->FillNtupleDColumn(0,1,fPhotonEnergySum);
     analysisManager->AddNtupleRow(0);
    }
    else if(versionType=="PolCal"){
     analysisManager->FillNtupleDColumn(0,0, fEnergySum);
     analysisManager->FillNtupleIColumn(0,1, fNP);
     analysisManager->AddNtupleRow(0);

     analysisManager->FillNtupleDColumn(1,0, fEnergyCalo);
     analysisManager->FillNtupleDColumn(0,1,fPhotonEnergySum);
     analysisManager->AddNtupleRow(1);
    }
  }

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
