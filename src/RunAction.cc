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
#include "RunActionMessenger.hh"
#include "G4AnalysisManager.hh"

#include "DetectorConstruction.hh"
#include "PrimaryGeneratorAction.hh"
#include "TupleFunctions.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4EmCalculator.hh"
#include "G4ParticleDefinition.hh"

#include "G4Gamma.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"

#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include <iomanip>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::RunAction(DetectorConstruction* det, PrimaryGeneratorAction* prim,
  G4String outFile, G4String outType, G4String version, G4String dipolState)
: G4UserRunAction(),
   fDetector(det), fPrimary(prim),fProcCounter(0), fAnalysisManager(0),fCore1Stat(1),fCore2Stat(1),fDipole1Stat(1),fDipole2Stat(1),fCal1Stat(1),fCal2Stat(1)
{
  fRunMessenger = new RunActionMessenger(this);


  dipolStatus = dipolState;
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
  fAnalysisManager->SetDefaultFileType("root");
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
    if((versionType=="Pol" || versionType=="PolCal")&&fCore2Stat==1){
     // book ntuple with id=0
     // detector behind iron absorber
     BookBunchTuple("bremssim2", "vacstep2");
     }
    if((versionType=="Cal" || versionType=="PolCal")&&(fCal1Stat==1 || fCal2Stat==1)){
      // ntuple with id 0 if Cal else 1
     BookBunchCalTuple("calorimeter", "crystal_vacstep3");
     // histrogramm id=0
     fAnalysisManager->CreateH1("EPhotons","Cherekov Spectrum", 100, 1.3, 3.3);
    }
  }
  else if  (outputType == "single"){

    if(versionType=="Pol" || versionType=="PolCal"){
      // Creating ntuple vacstep1 , id=0
      //detector between converter and iron core
       if (fCore1Stat==1){BookSingleTuple("bremssim1","vacstep1");}
      // Creating ntuple vacstep2 , id=1
      // detector behind iron core
       if (fCore2Stat==1){BookSingleTuple("bremssim2","vacstep2");}
    }// end of if version

    if(versionType=="Cal" || versionType=="PolCal"){

       //id=0 if Cal else 2
       if(fCal2Stat==1){
         BookSingleCalTuple("calorimeter", "vacstep3");
       }

       //id=1 if Cal else 3
       if(fCal1Stat==1){BookSingleCalTuple("calorimeterIn", "vacstep4");}
    } // end of if version

    if(dipolStatus=="On"){
      //id=4 if PolCal else 2
      if(fDipole1Stat==1){BookSingleTuple("dipoleVac","BigVac");}
      //id=5 if PolCal else 3
      if(fDipole2Stat==1){BookSingleTuple("dipoleVac2","BigVac2");}
    } // end if dipolStatus
   } // end of if single
/*
   else if (outputType == "histo" && dipolStatus=="On"){
     //create H1s for Energy -> for dipole1/2 and gamma,e- and e+
      //fAnalysisManager->CreateH1("Ee-Dip1","Ee- in Dipole1Det;E / MeV; counts", 1000, 0.,Eprim*MeV);

    //create H3s for Polarization information -> for dipole1/2 and gamma,e- and e+
    auto nbinsPol=100;
    auto minPol=-1.;
    auto maxPol=1.;
    fAnalysisManager->CreateH3("Polgamma-Dip1", "Pol_gamma in Dipole1Det",nbinsPol,minPol,maxPol,nbinsPol,minPol,maxPol,nbinsPol,minPol,maxPol);
    fAnalysisManager->CreateH3("Pole--Dip1")

  }*/
  } // end of BookHisto

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::CountProcesses(G4String procName)
{
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

//  G4Material* material = fDetector->GetMaterial();
//  G4double density = material->GetDensity();
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

G4cout << "....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......" << G4endl;
G4cout << "Core1 vactep state: " <<  fCore1Stat << G4endl;
G4cout << "Core2 vactep state: " <<  fCore2Stat << G4endl;
G4cout << "Dipole1 vactep state: " <<  fDipole1Stat << G4endl;
G4cout << "Dipole2 vactep state: " <<  fDipole2Stat << G4endl;
G4cout << "Cal1 vactep state: " <<  fCal1Stat << G4endl;
G4cout << "Cal2 vactep state: " <<  fCal2Stat << G4endl;
G4cout << "....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......" << G4endl;

  G4cout << "### Run " << aRun->GetRunID() << " Ended." << G4endl;
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


void RunAction::SetCore1Stat(G4bool value)
{
  fCore1Stat = value;
}

void RunAction::SetCore2Stat(G4bool value)
{
  fCore2Stat = value;
}

void RunAction::SetDipole1Stat(G4bool value)
{
  fDipole1Stat = value;
}

void RunAction::SetDipole2Stat(G4bool value)
{
  fDipole2Stat = value;
}

void RunAction::SetCal1Stat(G4bool value)
{
  fCal1Stat = value;
}

void RunAction::SetCal2Stat(G4bool value)
{
  fCal2Stat = value;
}
