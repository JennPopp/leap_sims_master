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
/// \file polarisation/Pol01/src/DetectorMessenger.cc
/// \brief Implementation of the DetectorMessenger class
//
// $Id: DetectorMessenger.cc 98772 2016-08-09 14:25:31Z gcosmo $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "DetectorMessenger.hh"

#include "DetectorConstruction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorMessenger::DetectorMessenger(DetectorConstruction * det)
:G4UImessenger(),
 fDetector(det)
{
  fLeapDir = new G4UIdirectory("/leap/");
  fLeapDir->SetGuidance("commands specific to this example");

  fDetDir = new G4UIdirectory("/leap/det/");
  fDetDir->SetGuidance("detector construction");

  fConvMaterCmd = new G4UIcmdWithAString("/leap/det/SetConvMaterial",this);
  fConvMaterCmd->SetGuidance("Select material of the Converter Target.");
  fConvMaterCmd->SetParameterName("choice",false);
  fConvMaterCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  fSizeXYCmd = new G4UIcmdWithADoubleAndUnit("/leap/det/SetSizeXY",this);
  fSizeXYCmd->SetGuidance("Set diameter of the converter target and iron core");
  fSizeXYCmd->SetParameterName("Size",false);
  fSizeXYCmd->SetRange("Size>0.");
  fSizeXYCmd->SetUnitCategory("Length");
  fSizeXYCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  fCoreZCmd = new G4UIcmdWithADoubleAndUnit("/leap/det/SetCoreThick",this);
  fCoreZCmd->SetGuidance("Set sizeZ of the iron core");
  fCoreZCmd->SetParameterName("CoreThick",false);
  fCoreZCmd->SetRange("CoreThick>0.");
  fCoreZCmd->SetUnitCategory("Length");
  fCoreZCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  fConvZCmd = new G4UIcmdWithADoubleAndUnit("/leap/det/SetConvThick",this);
  fConvZCmd->SetGuidance("Set sizeZ of the converter target");
  fConvZCmd->SetParameterName("ConvThick",false);
  fConvZCmd->SetRange("ConvThick>0.");
  fConvZCmd->SetUnitCategory("Length");
  fConvZCmd->AvailableForStates(G4State_PreInit,G4State_Idle);


  fUpdateCmd = new G4UIcmdWithoutParameter("/leap/det/update",this);
  fUpdateCmd->SetGuidance("Update calorimeter geometry.");
  fUpdateCmd->SetGuidance("This command MUST be applied before \"beamOn\" ");
  fUpdateCmd->SetGuidance("if you changed geometrical value(s).");
  fUpdateCmd->AvailableForStates(G4State_Idle);


  fCrystalnumberCmd = new G4UIcmdWithAString("/leap/det/Setnumberofcrystals",this);
  fCrystalnumberCmd->SetGuidance("Set the Number of the Crystals one or nine");
  fCrystalnumberCmd->SetParameterName("choice",false);
  fCrystalnumberCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorMessenger::~DetectorMessenger()
{
  delete fConvMaterCmd;
  delete fSizeXYCmd;
  delete fCoreZCmd;
  delete fConvZCmd;
  delete fUpdateCmd;
  delete fDetDir;
  delete fLeapDir;
  delete fCrystalnumberCmd;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
  if( command == fConvMaterCmd )
   { fDetector->SetConvMaterial(newValue);}

  if( command == fSizeXYCmd )
   { fDetector->SetSizeXY(fSizeXYCmd->GetNewDoubleValue(newValue));}

  if( command == fCoreZCmd )
   { fDetector->SetCoreThick(fCoreZCmd->GetNewDoubleValue(newValue));}

   if( command == fConvZCmd )
    { fDetector->SetConvThick(fConvZCmd->GetNewDoubleValue(newValue));}

  if( command == fUpdateCmd )
   { fDetector->UpdateGeometry(); }

 if( command == fCrystalnumberCmd )
   { fDetector->SetCrystalnumber(newValue);}


}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
