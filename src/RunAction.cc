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
/// \file polarisation/Pol01/src/RunAction.cc
/// \brief Implementation of the RunAction class
//
// $Id: RunAction.cc 98772 2016-08-09 14:25:31Z gcosmo $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "RunAction.hh"
#include "Analysis.hh"

#include "DetectorConstruction.hh"
#include "PrimaryGeneratorAction.hh"
#include "G4ParticleDefinition.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4EmCalculator.hh"

#include "G4Gamma.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"

#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include <iomanip>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::RunAction(DetectorConstruction* det, PrimaryGeneratorAction* prim,
  G4String outFile, G4String outType, G4String version)
: G4UserRunAction(),
   fDetector(det), fPrimary(prim),fProcCounter(0), fAnalysisManager(0)
{
  outFileName=outFile;
  outputType=outType;
  versionType=version;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::~RunAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::BeginOfRunAction(const G4Run* aRun)
{
  // Always creating analysis manager
  fAnalysisManager = G4AnalysisManager::Instance();
  fAnalysisManager->SetActivation(true);
  fAnalysisManager->SetVerboseLevel(1);

  std :: ostringstream oss;
  //oss << "Result_Run_" << aRun->GetRunID();
oss << "run"<< aRun->GetRunID()<< "_"<< outFileName ;

  G4String fileName=oss.str();


  // Open file histogram file
  fAnalysisManager->OpenFile(fileName);

    BookHisto();

  G4cout << "### Run " << aRun->GetRunID() << " start." << G4endl;

  // save Rndm status
  //  G4RunManager::GetRunManager()->SetRandomNumberStore(false);
  //  CLHEP::HepRandom::showEngineStatus();

  if (fProcCounter) delete fProcCounter;
  fProcCounter = new ProcessesCount;
  fTotalEventCount = 0;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::BookHisto()
{
  if (outputType == "bunch"){
    // Creating ntuple
    //
     fAnalysisManager->CreateNtuple("bremssim2", "vacstep2");
      fAnalysisManager->CreateNtupleDColumn("Esum");
      fAnalysisManager->CreateNtupleIColumn("NP");
      fAnalysisManager->FinishNtuple();}
  else if (outputType == "single"){
    // Creating ntuple vacstep1 , id=0
    //
    fAnalysisManager->CreateNtuple("bremssim1", "vacstep1");
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

    // Creating ntuple vacstep2 , id=1
    //
    fAnalysisManager->CreateNtuple("bremssim2", "vacstep2");
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
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::CountProcesses(G4String procName)
{
  // is the process already counted ?
  // *AS* change to std::map?!
  size_t nbProc = fProcCounter->size();
  size_t i = 0;
  while ((i<nbProc)&&((*fProcCounter)[i]->GetName()!=procName)) i++;
  if (i == nbProc) fProcCounter->push_back( new ProcessCount(procName));

  (*fProcCounter)[i]->Count();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::EndOfRunAction(const G4Run* aRun)
{
  G4int NbOfEvents = aRun->GetNumberOfEvent();
  if (NbOfEvents == 0) return;

  G4int  prec = G4cout.precision(5);

  G4Material* material = fDetector->GetMaterial();
  G4double density = material->GetDensity();
/*
  G4ParticleDefinition* particle =
                            fPrimary->GetParticleGun()->GetParticleDefinition();
  G4String Particle = particle->GetParticleName();
  G4double energy = fPrimary->GetParticleGun()->GetParticleEnergy();
  G4cout << "\n The run consists of " << NbOfEvents << " "<< Particle << " of "
         << G4BestUnit(energy,"Energy") << " through a Converter Target of"
         << G4BestUnit(fDetector->GetConvZ(),"Length") << " of "
         << material->GetName() << " (density: "
         << G4BestUnit(density,"Volumic Mass") << ")"
         << " and a"<< G4BestUnit(fDetector->GetCoreZ(),"Length")
         <<" iron core "<< G4endl;
*/
  //frequency of processes
  G4cout << "\n Process calls frequency --->\n";
  for (size_t i=0; i< fProcCounter->size();i++) {
     G4String procName = (*fProcCounter)[i]->GetName();
     G4int    count    = (*fProcCounter)[i]->GetCounter();
     G4cout << "\t" << procName << " = " << count<<"\n";
  }

  //cross check from G4EmCalculator
  //  G4cout << "\n Verification from G4EmCalculator. \n";
  //  G4EmCalculator emCal;

  //restore default format
  G4cout.precision(prec);


  // show Rndm status
  CLHEP::HepRandom::showEngineStatus();


  fAnalysisManager = G4AnalysisManager::Instance();
  fAnalysisManager->Write();
  fAnalysisManager->CloseFile();


  G4cout << "### Run " << aRun->GetRunID() << " Ended." << G4endl;
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
