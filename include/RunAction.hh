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
/// \file polarisation/Pol01/include/RunAction.hh
/// \brief Definition of the RunAction class
//
// $Id: RunAction.hh 98772 2016-08-09 14:25:31Z gcosmo $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef RunAction_h
#define RunAction_h 1

#include "G4UserRunAction.hh"
#include "ProcessesCount.hh"
#include "globals.hh"
#include "G4AnalysisManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


class DetectorConstruction;
class PrimaryGeneratorAction;
class RunActionMessenger;
class G4Run;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class RunAction : public G4UserRunAction
{

public:

  RunAction(DetectorConstruction* det, PrimaryGeneratorAction* prim, G4String outFile, G4String outType, G4String version, G4String dipolState);
  virtual ~RunAction();

  virtual void BeginOfRunAction(const G4Run*);
  virtual void   EndOfRunAction(const G4Run*);

  void CountProcesses(G4String);

  void SetCore1Stat(G4bool);
  void SetCore2Stat(G4bool);
  void SetDipole1Stat(G4bool);
  void SetDipole2Stat(G4bool);
  void SetDipole3Stat(G4bool);
  void SetCal1Stat(G4bool);
  void SetCal2Stat(G4bool);

  G4bool GetCore1Stat() const;
  G4bool GetCore2Stat() const;
  G4bool GetDipole1Stat() const;
  G4bool GetDipole2Stat() const;
  G4bool GetDipole3Stat() const;
  G4bool GetCal1Stat() const;
  G4bool GetCal2Stat() const;

private:

  // void BookHisto();
  // void SaveHisto(G4int nevents);

  G4String outFileName;
  G4String outputType;
  G4String versionType;
  G4String dipolStatus;

  DetectorConstruction*   fDetector;
  PrimaryGeneratorAction* fPrimary;
  ProcessesCount*         fProcCounter;
  RunActionMessenger*     fRunMessenger;
  G4AnalysisManager*      fAnalysisManager;

  G4int fTotalEventCount;

  G4bool fCore1Stat;
  G4bool fCore2Stat;
  G4bool fDipole1Stat;
  G4bool fDipole2Stat;
  G4bool fDipole3Stat;
  G4bool fCal1Stat;
  G4bool fCal2Stat;


};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
inline G4bool RunAction::GetCore1Stat() const {
  return fCore1Stat;
}
inline G4bool RunAction::GetCore2Stat() const {
  return fCore2Stat;
}
inline G4bool RunAction::GetDipole1Stat() const {
  return fDipole1Stat;
}
inline G4bool RunAction::GetDipole2Stat() const {
  return fDipole2Stat;
}
inline G4bool RunAction::GetDipole3Stat() const {
  return fDipole3Stat;
}
inline G4bool RunAction::GetCal1Stat() const {
  return fCal1Stat;
}
inline G4bool RunAction::GetCal2Stat() const {
  return fCal2Stat;
}


#endif
