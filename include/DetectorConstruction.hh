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
/// \file polarisation/Pol01/include/DetectorConstruction.hh
/// \brief Definition of the DetectorConstruction class
//
// $Id: DetectorConstruction.hh 98772 2016-08-09 14:25:31Z gcosmo $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include "Materials.hh"

class G4LogicalVolume;
class G4Material;
class DetectorMessenger;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class DetectorConstruction : public G4VUserDetectorConstruction
{
public:

  DetectorConstruction(G4String version);
  ~DetectorConstruction();

public:

  G4VPhysicalVolume* Construct();

  void SetSizeXY   (G4double);
  void SetCoreThick   (G4double);
  void SetConvThick   (G4double);
  void SetConvMaterial (G4String);
  void SetWorldMaterial (G4String);
  void SetCaloMaterial (G4String);
  void SetCrystalnumber (G4String);

  void UpdateGeometry();

public:

  const G4VPhysicalVolume* GetWorld()      {return PhysicalWorld;};
//  const G4VPhysicalVolume* GetBox()        {return fBox;};
  const G4VPhysicalVolume* GetVacStep1PV() const;
  const G4VPhysicalVolume* GetVacStep2PV() const;
  const G4VPhysicalVolume* GetVacStep3PV() const;
  const G4VPhysicalVolume* GetVacStep4PV() const;
  const G4VPhysicalVolume* GetDetectorPV() const;
  const G4VPhysicalVolume* GetAluwrapPV() const;


  G4double           GetWorldSize()  {return fWorldSize;};
  G4double           GetSizeXY()  {return fSizeXY;};
  G4double           GetCoreZ()   {return fCoreThick;};
  G4double           GetConvZ()   {return fConvThick;};
  G4Material*        GetMaterial()   {return fConvMaterial;};
  G4String           GetCrystalNumber(){return CrystalNumber;};

  void               PrintParameters();

private:

  Materials *allMaterials; // Object giving access to all elements and materials defined by Materials.
  G4VPhysicalVolume*    PhysicalWorld;
  G4VPhysicalVolume*    PhysicalCore;
  G4VPhysicalVolume*   fVacStepPV1;
  G4VPhysicalVolume*   fVacStepPV2;
  G4VPhysicalVolume*   fVacStepPV3;
  G4VPhysicalVolume*   fVacStepPV4;
  G4VPhysicalVolume*   fDetectorPV;
  G4LogicalVolume*     fDetectorLV;
  G4VPhysicalVolume*   fAluwrapPV;
  G4VPhysicalVolume*   fAlAirGapPV;
  G4VPhysicalVolume*   fVirtCaloPV;
  G4VPhysicalVolume*   fCaloCellPV;


  G4double              fSizeXY;
  G4double              fCoreThick;
  G4double              fConvThick;
  G4double              fWorldSize;
  G4Material*           fConvMaterial;
  G4Material*           fWorldMaterial;
  G4Material*           fCaloMaterial;

  DetectorMessenger* fMessenger;

  G4String versionType;
  G4String CrystalNumber;

};

inline const G4VPhysicalVolume* DetectorConstruction::GetVacStep1PV() const {
  return fVacStepPV1;
}
inline const G4VPhysicalVolume* DetectorConstruction::GetVacStep2PV() const {
  return fVacStepPV2;
}
inline const G4VPhysicalVolume* DetectorConstruction::GetVacStep3PV() const {
  return fVacStepPV3;
}
inline const G4VPhysicalVolume* DetectorConstruction::GetVacStep4PV() const {
  return fVacStepPV4;
}
inline const G4VPhysicalVolume* DetectorConstruction::GetDetectorPV() const {
  return fDetectorPV;
}
inline const G4VPhysicalVolume* DetectorConstruction::GetAluwrapPV() const {
  return fAluwrapPV;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
