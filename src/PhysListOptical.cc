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
/// \file polarisation/Pol01/src/PhysListEmPolarized.cc
/// \brief Implementation of the PhysListEmPolarized class
//
//
// $Id: PhysListEmPolarized.cc 100257 2016-10-17 08:00:06Z gcosmo $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "PhysListOptical.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"//from QuaSi
#include "G4ParticleTable.hh"//from QuaSi
#include "G4ProcessManager.hh"

#include "G4Material.hh"//from QuaSi
#include "G4MaterialTable.hh"//from QuaSi

// #include "G4eMultipleScattering.hh"
#include "G4Cerenkov.hh"//from QuaSi
#include "G4Scintillation.hh"//from QuaSi
#include "G4OpAbsorption.hh"//from QuaSi
#include "G4OpRayleigh.hh"//from QuaSi
#include "G4OpBoundaryProcess.hh"//from QuaSi

// #include "G4PolarizedCompton.hh"
// #include "G4PolarizedGammaConversion.hh"
// #include "G4ePolarizedIonisation.hh"
// #include "G4ePolarizedBremsstrahlung.hh"
// #include "G4eplusPolarizedAnnihilation.hh"
// #include "G4PolarizedPhotoElectricEffect.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PhysListOptical::PhysListOptical(const G4String& name)
   :  G4VPhysicsConstructor(name)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PhysListOptical::~PhysListOptical()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysListOptical::ConstructProcess()
{
  //
  theCerenkovProcess           = new G4Cerenkov("Cerenkov");             /// - Cerenkov
  theScintillationProcess      = new G4Scintillation("Scintillation");   /// - Scintillation
  theAbsorptionProcess         = new G4OpAbsorption();		         /// - Bulk absorption
  theRayleighScatteringProcess = new G4OpRayleigh();		         /// - Rayleigh scattering
  theBoundaryProcess           = new G4OpBoundaryProcess();	         /// - Reflection / refraction at optical interfaces.
  //  theCerenkovProcess->DumpPhysicsTable();
  //  theScintillationProcess->DumpPhysicsTable();
  //  theAbsorptionProcess->DumpPhysicsTable();
  //  theRayleighScatteringProcess->DumpPhysicsTable();

  theCerenkovProcess->SetMaxNumPhotonsPerStep(300);
  /* large number of secondaries generated -> track secondaries first to avoid memory issues
   * on the other hand: this screws up counting of secondary electrons if they create
   * optical photons as well: they get suspended and then counted again...
   */
  theCerenkovProcess->SetTrackSecondariesFirst(false);

  //theScintillationProcess->SetScintillationYieldFactor(1.);
  // in G4 v.11 there are particle specific scintillation yields
  // and can be set using material properties, see example OpNovice2 

  theScintillationProcess->SetTrackSecondariesFirst(false);

  auto particleIterator=GetParticleIterator();
  particleIterator->reset();
  while( (*particleIterator)() ){
    G4ParticleDefinition* particle = particleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();
    G4String particleName = particle->GetParticleName();

    if (theCerenkovProcess->IsApplicable(*particle)) {
      pmanager->AddProcess(theCerenkovProcess);
      pmanager->SetProcessOrdering(theCerenkovProcess,idxPostStep);
      //     pmanager->AddContinuousProcess(theCerenkovProcess);
    }
    if (theScintillationProcess->IsApplicable(*particle)) {
      pmanager->AddProcess(theScintillationProcess);
      pmanager->SetProcessOrderingToLast(theScintillationProcess, idxAtRest);
      pmanager->SetProcessOrderingToLast(theScintillationProcess, idxPostStep);
    }
    if (particleName == "opticalphoton") {
      G4cout << " AddDiscreteProcess to OpticalPhoton " << G4endl;
      pmanager->AddDiscreteProcess(theAbsorptionProcess);
      pmanager->AddDiscreteProcess(theRayleighScatteringProcess);
      pmanager->AddDiscreteProcess(theBoundaryProcess);
    }
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
