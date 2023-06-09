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
/// \file polarisation/Pol01/include/EventAction.hh
/// \brief Definition of the EventAction class
//
// $Id: EventAction.hh 98772 2016-08-09 14:25:31Z gcosmo $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef EventAction_h
#define EventAction_h 1

#include "RunAction.hh"
#include "G4UserEventAction.hh"
#include "globals.hh"

class RunAction;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class EventAction : public G4UserEventAction
{
public:
  EventAction(RunAction*, G4String outType, G4String version);
  virtual ~EventAction();

  virtual void BeginOfEventAction(const G4Event*);
  virtual void EndOfEventAction(const G4Event*);

  void AddVals(G4double Eval, G4double Npart);
  void AddGammaVals(G4double EGammapol, G4double Ngamma);
  void AddeVals(G4double Ee,G4double Ne);
  void AddEnergyCalo(G4double ECalo, G4int crystNo);
  void AddPhotonEnergy(G4double EPhoton);
  void AddGammaEnergy(G4double EGamma);

  std::vector<G4double>& GetCalEdep() { return fEnergyCalo; }

private:
  RunAction* fRunAction;

  G4double fEnergySum;
  G4double fNP;
  G4double fGammaEnergySum;
  G4double fNGamma;
  G4double fElectronEnergySum;
  G4double fNElectron;
  std::vector<G4double> fEnergyCalo;
  G4double fPhotonEnergySum;
  G4double fGammaEnergyIn;
  G4String outputType;
  G4String versionType;

  int tupleID;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// inline functions
inline void EventAction::AddVals(G4double Eval, G4double Npart) {
  fEnergySum += Eval;
  fNP += Npart;
}
inline void EventAction::AddGammaVals(G4double EGammapol, G4double Ngamma) {
  fGammaEnergySum += EGammapol;
  fNGamma += Ngamma;
}
inline void EventAction::AddeVals(G4double Ee, G4double Ne) {
  fElectronEnergySum += Ee;
  fNElectron += Ne;
}
inline void EventAction::AddEnergyCalo(G4double ECalo, G4int crystNo) { //deposited energy in the crstals
  fEnergyCalo[crystNo] += ECalo;
}

inline void EventAction::AddPhotonEnergy(G4double EPhoton) { //Photon Energy which comes out of the crystal
  fPhotonEnergySum += EPhoton;
}

inline void EventAction::AddGammaEnergy(G4double EGamma) { //Gamma Energy on the entrace of the crystals
  fGammaEnergyIn+= EGamma;
}
#endif
