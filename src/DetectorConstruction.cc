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
/// \file polarisation/Pol01/src/DetectorConstruction.cc
/// \brief Implementation of the DetectorConstruction class
//
// $Id: DetectorConstruction.cc 98772 2016-08-09 14:25:31Z gcosmo $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "DetectorConstruction.hh"
#include "DetectorMessenger.hh"

#include "G4Material.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Polycone.hh"
#include "G4SubtractionSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
#include "G4MaterialPropertiesTable.hh"

#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4UnitsTable.hh"

#include "G4PolarizationManager.hh"
// #include "G4NistManager.hh"
#include "G4SystemOfUnits.hh"

// #include "G4OpBoundaryProcess.hh"
// #include "G4LogicalBorderSurface.hh"
// #include "G4LogicalSkinSurface.hh"

#include "G4UniformMagField.hh"
#include "G4FieldManager.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction(G4String version, G4String dipolState, G4String caloType, G4String beamLine)
: G4VUserDetectorConstruction(),
  PhysicalWorld(0), PhysicalCore(0), fConvMaterial(0), fWorldMaterial(0), fCaloMaterial(0), fLogicalDipol(0)
{
  versionType=version;
  dipolStatus = dipolState;
  caloTyp = caloType;
  BeamLine = beamLine;
  allMaterials = new Materials();
  allMaterials->DefineMaterials();

  fSizeXY = 50*mm;
  fCoreThick = 150*mm;
  fConvThick = 1.75*mm;
  fWorldSize = 7.1*m;
  CrystalNumber= 9;
  fDipoleB = 0.1 * tesla;
  fDipoleSize = G4ThreeVector( 50*mm, 25*mm, 100*mm);
  fZtoCalo = 50 * mm;

  SetConvMaterial("Galactic");
  SetWorldMaterial("Galactic");
  SetCaloMaterial("TF101");

  fMessenger = new DetectorMessenger(this);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::~DetectorConstruction()
{
  delete fMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  // Cleanup old geometry
  G4GeometryManager::GetInstance()->OpenGeometry();
  G4PhysicalVolumeStore::GetInstance()->Clean();
  G4LogicalVolumeStore::GetInstance()->Clean();
  G4SolidStore::GetInstance()->Clean();

  G4PolarizationManager::GetInstance()->Clean();

  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Geometry parameters
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  //
  // Solenoid
  //...........................
  vacthick = 1.0*mm; // thickness of vacuum steps , aka. idealized detectors
  coregap = 12.5*mm; // -b: distance between core and cone, else: dist between core and conv. target
  if (BeamLine == "On"){
    conelength = 50*mm; // length of opening cone 
    magthick = 2.*(conelength+coregap)+fCoreThick;
  }else{
    conelength = 48.5*mm;
    magthick = 2.*(conelength+coregap+fConvThick)+fCoreThick;
  }
  
  
  

  //
  //Dipol
  //....................
  G4double dipoleGap = 50*mm; // Gap between solenoid and dipole

  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // World
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  G4Box*
  SolidWorld = new G4Box("World",                            //name
                   fWorldSize/2,fWorldSize/2,fWorldSize/2); //dimensions

  G4LogicalVolume*
  LogicalWorld = new G4LogicalVolume(SolidWorld,                   //shape
                               fWorldMaterial,           //material
                              "World");                  //name

  PhysicalWorld = new G4PVPlacement(0,                          //no rotation
                             G4ThreeVector(),            //at (0,0,0)
                             LogicalWorld,                     //logical volume
                             "World",                    //name
                             0,                          //mother volume
                             false,                      //no boolean operation
                             0);                         //copy number


  LogicalWorld->SetVisAttributes (G4VisAttributes::GetInvisible());

  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Polarimeter geometry
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  if (versionType == "Pol" || versionType == "PolCal"){

    //virtual mother volume containing the rest of the polarimeter
    G4LogicalVolume* LogicalSolenoid = ConstructSolenoid(magthick, coregap, vacthick, BeamLine);

    new G4PVPlacement(0,	//rotation
                     G4ThreeVector(0.0*mm, 0.0*mm, magthick/2.),// translation position
                     LogicalSolenoid,      //its logical volume
                       "PhysicalMagnet",   //its name  (2nd constructor)
                       LogicalWorld,         //its mother volume
                       false,              //no boolean operation
                       0);                 //copy number

  } //end if-statement polarimeter
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Dipol geometry
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  if (dipolStatus == "On"){
    G4LogicalVolume* LogicalDipol = ConstructDipol();
    G4double dipolZpos;
    if (versionType == "Pol" || versionType == "PolCal"){
         dipolZpos = magthick+dipoleGap+fDipoleSize[2]/2.;
      } // end if version
    else if (versionType == "Cal"){
         dipolZpos = fDipoleSize[2]/2.;
    }
    new G4PVPlacement(0,	//rotation
                     G4ThreeVector(0.0*mm, 0.0*mm, dipolZpos),// translation position
                     LogicalDipol,      //its logical volume
                       "PhysicalDipol",   //its name  (2nd constructor)
                       LogicalWorld,         //its mother volume
                       false,              //no boolean operation
                       0);

    auto solidBigVac = new G4Box("BigVacSolid",  //Name
                                  75.*cm,   // x size
                                  75.*cm,     // y size
                                  1.*mm/2.); // z size

    auto BigVacLV = new G4LogicalVolume(solidBigVac,    //its solid
                                          fWorldMaterial,    //its material
                                          "BigVac");       //its name

    fBigVacPV = new G4PVPlacement(0,                   //no rotation
                          G4ThreeVector(0.0*mm, 0.0*mm, dipolZpos-fDipoleSize[2]/2-1.0*mm),    //its position
                                  BigVacLV,            //its logical volume
                                  "BigVacPV",                 //its name
                                  LogicalWorld,               //its mother
                                  false,                     //no boolean operat
                                  0);                       //copy number

    auto BigVacLV2 = new G4LogicalVolume(solidBigVac,    //its solid
                                          fWorldMaterial,    //its material
                                          "BigVac2");       //its name

    fBigVacPV2 = new G4PVPlacement(0,                   //no rotation
                          G4ThreeVector(0.0*mm, 0.0*mm, dipolZpos+fDipoleSize[2]/2+1.0*mm),    //its position
                                  BigVacLV2,            //its logical volume
                                  "BigVacPV2",                 //its name
                                  LogicalWorld,               //its mother
                                  false,                     //no boolean operat
                                  0);                       //copy number

    auto solidDipVac3 = new G4Box("BigVacSolid",  //Name
                                  1.*mm/2,   // x size
                                  360.*mm/2,     // y size
                                  (dipoleGap+fDipoleSize[2]+fZtoCalo)/2); // z size

    auto dipVac3LV = new G4LogicalVolume(solidDipVac3,    //its solid
                                          fWorldMaterial,    //its material
                                          "dipVac3");       //its name

    fDipVac3PV = new G4PVPlacement(0,                   //no rotation
                          G4ThreeVector(fDipoleSize[0]+1.*mm, 0.0*mm, dipolZpos),    //its position
                                  dipVac3LV,            //its logical volume
                                  "BigVacPV",                 //its name
                                  LogicalWorld,               //its mother
                                  false,                     //no boolean operat
                                  0);                       //copy number


  } // endif dipol State

  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Calorimeter geometry
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  if (versionType == "Cal" || versionType == "PolCal"){
    G4double detthick = 45.*cm; // length of the lead glass crystals
    G4double detxy = 3.8*cm; // width in x/y of crystals

    G4double alairgapthick = 0.001 *mm;   // thickness of the air gap between
                                          //the aluwrapping and the crystal
    G4double aluwrapthick = 0.01  *mm;   // wikipedia: alu foil thickness
                                         //between 0.004 and 0.02 mm

    G4double caloLength = detthick+alairgapthick+aluwrapthick+2.*vacthick;
    //G4double caloZposition;

    // statements to get the correct Z-Position of the calorimeter
    if (dipolStatus == "Off"){

        if(versionType == "Cal"){
          caloZposition = caloLength/2.;
        }
        else if(versionType == "PolCal"){
          caloZposition = magthick + fZtoCalo + caloLength/2.;
        }

    }
    else if (dipolStatus == "On"){
      if(versionType == "Cal"){
          caloZposition = fDipoleSize[2]+fZtoCalo+caloLength/2.;
        }
        else if(versionType == "PolCal"){
          caloZposition = magthick + dipoleGap + fDipoleSize[2] + fZtoCalo + caloLength/2.;
        }
    }

    G4LogicalVolume* fVirtCaloLV = ConstructCalorimeter(caloTyp, detthick, detxy,
        alairgapthick, aluwrapthick, vacthick);

    fVirtCaloPV = new G4PVPlacement(0,                   //no rotation
                           G4ThreeVector(0.,0.,caloZposition),    //its position
                                   fVirtCaloLV,            //its logical volume
                                   "virtualCalorimeter",                 //its name
                                  LogicalWorld,               //its mother
                                   false,                     //no boolean operat
                                   0);                        //copy number

  } //end if-statement for the calorimeter geometry

  if (BeamLine == "On") {
    ConstructBeamLine(LogicalWorld,magthick);

  }
  PrintParameters();

  //always return the root volume
  //
  return PhysicalWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Construct the Solenoid , returns LV of motherVolume
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4LogicalVolume* DetectorConstruction::ConstructSolenoid(G4double magthick,
  G4double coregap, G4double vacthick, G4String beamLine){
    //
    // Geometry Parameters
    //
    absrad = fSizeXY/2.;
    shieldrad = 75.0*mm;
    conelength = 50.0*mm;
    corethick = fCoreThick;
    coilthick = corethick + 25.0*mm;
    shieldthick = corethick - 25.0*mm;

    if (beamLine == "On"){
      Rmax = 162.0*mm;
      Ropen = 30.0*mm;
      Routercoil = 161*mm;
      conedist = corethick/2. + coregap;
    } else{
      Rmax = 196.0*mm;
      Ropen = 36.84308*mm;
      Routercoil = 170*mm;
      convthick = fConvThick;
      conedist = corethick/2. + coregap + convthick;
    }

    //
    //Get materials
    //
    //G4Material* absMat    = allMaterials->GetMat("G4_W");
    G4Material* magMat    = allMaterials->GetMat("G4_Fe");
    G4Material* coilMat   = allMaterials->GetMat("G4_Cu");
    G4Material* shieldMat = allMaterials->GetMat("G4_Pb");

    // Housing of Magnet
    //
    G4double DzArrayMagnet   [] = {-magthick/2., -conedist, -corethick/2., corethick/2.,  conedist, magthick/2.};
    G4double RminArrayMagnet [] = {Ropen,  absrad,  absrad , absrad,  absrad,  Ropen};
    G4double RmaxArrayMagnet [] = {Rmax, Rmax, Rmax, Rmax, Rmax, Rmax};

    G4Tubs *solidSolenoid = new G4Tubs("solidSolenoid",
                                        0.0*mm, // inner radius
                                        197.0*mm ,  // outer radius
                                        magthick/2., // half length in z
                                        0.0*deg,  // starting angle
                                        360.0*deg ); // total angle

    G4LogicalVolume * LogicalSolenoid = new G4LogicalVolume(solidSolenoid, //its solid
                    fWorldMaterial, 	 //its material
                     "MotherSolenoid" ,		 //its name
                     0,0,0);

    G4Polycone *solidMagnet = new G4Polycone("solidMagnet", 	 //its name
              0.0*deg, 		 //its start angle
              360.0*deg,		 //its opening angle
              6, 		         //its nZ
              DzArrayMagnet, 	 //z value
              RminArrayMagnet, 	 //rmin
              RmaxArrayMagnet ); 	 //rmax

    G4LogicalVolume * LogicalMagnet = new G4LogicalVolume(solidMagnet, //its solid
                    magMat, 	 //its material
                     "Magnet" ,		 //its name
                     0,0,0);

    new G4PVPlacement(0,	//rotation
                    G4ThreeVector(0.0*mm, 0.0*mm, 0.0*mm),// translation position
                    LogicalMagnet,      //its logical volume
                      "PhysicalMagnet",   //its name  (2nd constructor)
                      LogicalSolenoid,         //its mother volume
                      false,              //no boolean operation
                      0);                 //copy number

    //Copper Coils
    //
    G4Tubs *solidCuTube= new G4Tubs("solidCuTube", //name
                                    shieldrad, // inner radius
                                    Routercoil,  // outer radius
                                    coilthick/2., // half length in z
                                    0.0*deg,  // starting angle
                                    360.0*deg ); // total angle

    G4LogicalVolume * LogicalCuTube = new G4LogicalVolume(solidCuTube, //its solid
               coilMat,              //its material
               "CuTube" ,		         //its name
               0,0,0);

     new G4PVPlacement(0,	//rotation
           G4ThreeVector(0.0*mm, 0.0*mm, 0.0*mm), // translation position
             LogicalCuTube,      // its logical volume
             "PhysicalCuTube",   // its name  (2nd constructor)
             LogicalMagnet,     // its mother volume
             false,              // no boolean operation
             0);


    // Lead Tube
    //
    G4Tubs *solidPbtube= new G4Tubs("solidPbtube", // name
                                    absrad, // inner radius
                                    shieldrad, // outer radius
                                    shieldthick/2., // half length in z
                                    0.0*deg, // start angle
                                    360.0*deg ); // total angle

    G4LogicalVolume * LogicalPbtube = new G4LogicalVolume(solidPbtube, 	 //its solid
    						shieldMat, 		 //its material
    						"Pbtube" ,		 //its name
    						0,0,0);

    new G4PVPlacement(0,	//rotation
    				G4ThreeVector(0.0*mm, 0.0*mm, 0.0*mm),
    				LogicalPbtube,      //its logical volume
    			    "PhysicalPbTube",   //its name  (2nd constructor)
    			    LogicalMagnet,     //its mother volume
    			    false,              //no boolean operation
    			    0);                 //copy number

    if (beamLine != "On"){
      // Conversion Target
      //
      G4Tubs *solidConversion = new G4Tubs("solidConversion", // name
                                          0.0*mm, // inner radius
                                          absrad, // outer radius
                                          convthick/2., // half length in z
                                          0.0*deg, // starting angle
                                          360.0*deg ); // total angle

      G4LogicalVolume *LogicalConversion = new G4LogicalVolume(solidConversion, 	 //its solid
                  fConvMaterial,          //its material
                  "ConversionTarget" ,	 //its name
                  0,0,0);

      new G4PVPlacement(0,	//rotation
                  G4ThreeVector(0.0*mm, 0.0*mm, -coregap-convthick/2-corethick/2),
                LogicalConversion,         //its logical volume
                "PhysicalConversion",   //its name  (2nd constructor)
                LogicalSolenoid,              //its mother volume
                false,                 //no boolean operation
                0);                       //copy number

      G4VisAttributes * ConversionTargetVis= new G4VisAttributes( G4Colour(105/255. ,105/255. ,105/255. ));
      ConversionTargetVis->SetVisibility(true);
      ConversionTargetVis->SetLineWidth(2);
      ConversionTargetVis->SetForceSolid(true);
      LogicalConversion->SetVisAttributes(ConversionTargetVis);

      }
    

    // Iron Core
    //
    auto *solidCore = new G4Tubs ("Container",                           //its name
                     0.0*mm, absrad*mm, corethick/2, 0.0*deg, 360.0*deg );//its dimensions

    G4LogicalVolume*
    LogicalCore = new G4LogicalVolume(solidCore,                        //its shape
                               magMat,             //its material
                               "IronCore");                   //its name

    PhysicalCore = new G4PVPlacement(0,                             //no rotation
                             G4ThreeVector(),               //at (0,0,0)
                             LogicalCore,                          //its logical volume
                             "IronCorePV",    //its name
                             LogicalSolenoid,                        //its mother  volume
                             false,                         //no boolean operation
                             0);                            //copy number

    // register logical Volume in PolarizationManager with polarization
    G4PolarizationManager * polMgr = G4PolarizationManager::GetInstance();
    polMgr->SetVolumePolarization(LogicalCore,G4ThreeVector(0.,0.,1.));


    //
    //vacuum step 1
    //
    auto *VacStepS1 = new G4Tubs("VacStep1",  //Name
                                0.,         // inner radius
                                absrad,     // outer radius
                                vacthick/2., // half length in z
                                0.0*deg,    // starting phi angle
                                360.0*deg); // angle of the segment


    auto  *VacStepLV1 = new G4LogicalVolume(VacStepS1,    //its solid
                                           fWorldMaterial,    //its material
                                           "VacStep1");  //its name

    fVacStepPV1 = new G4PVPlacement(0,                 //no rotation
                         G4ThreeVector(0.,0., - corethick/2 -coregap +vacthick/2 +1.0*mm),    //its position
                                 VacStepLV1,            //its logical volume
                                 "VacStep1",                 //its name
                                 LogicalSolenoid,               //its mother
                                 false,                     //no boolean operat
                                 0);                        //copy number



    // vacuum step 2
    //
    auto *VacStepS2 = new G4Tubs("VacStep2",  //Name
                                 0.,         // inner radius
                                 absrad,     // outer radius
                                 vacthick/2., // half length in z
                                 0.0*deg,    // starting phi angle
                                 360.0*deg); // angle of the segment

    auto *VacStepLV2 = new G4LogicalVolume(VacStepS2,    //its solid
                                          fWorldMaterial,    //its material
                                          "VacStep1");       //its name

    fVacStepPV2 = new G4PVPlacement(0,                   //no rotation
                          G4ThreeVector(0.,0.,corethick/2 + vacthick/2 + 10.0*mm),    //its position
                                  VacStepLV2,            //its logical volume
                                  "VacStep2",                 //its name
                                  LogicalSolenoid,               //its mother
                                  false,                     //no boolean operat
                                  0);                       //copy number



    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //VisAttributes
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    //Polarimeter
    //

      LogicalSolenoid->SetVisAttributes(G4VisAttributes::GetInvisible());

      G4VisAttributes * MagnetVis= new G4VisAttributes( G4Colour(255/255. ,102/255. ,102/255. ));
      MagnetVis->SetVisibility(true);
      MagnetVis->SetLineWidth(1);
      LogicalMagnet->SetVisAttributes(MagnetVis);

      G4VisAttributes * CopperCoilVis= new G4VisAttributes( G4Colour(255/255. ,0/255. ,255/255. ));
      CopperCoilVis->SetVisibility(true);
      CopperCoilVis->SetLineWidth(1);
      LogicalCuTube->SetVisAttributes(CopperCoilVis);

      G4VisAttributes * LeadTubeVis= new G4VisAttributes( G4Colour(0/255. ,102/255. ,204/255. ));
      LeadTubeVis->SetVisibility(true);
      LeadTubeVis->SetLineWidth(1);
      LogicalPbtube->SetVisAttributes(LeadTubeVis);

      G4VisAttributes * IronCoreVis= new G4VisAttributes( G4Colour(51/255. ,51/255. ,255/255. ));
      IronCoreVis->SetVisibility(true);
      IronCoreVis->SetLineWidth(2);
      IronCoreVis->SetForceSolid(true);
      LogicalCore->SetVisAttributes(IronCoreVis);

      G4VisAttributes * VacStepVis= new G4VisAttributes( G4Colour(255/255. ,165/255. ,0/255. ));
      VacStepVis->SetVisibility(true);
      VacStepVis->SetLineWidth(1);
      VacStepVis->SetForceSolid(true);
      VacStepLV1->SetVisAttributes(VacStepVis);
      VacStepLV2->SetVisAttributes(VacStepVis);

    return LogicalSolenoid;
  }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Construct the Calorimeter , returns LV of motherVolume
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4LogicalVolume* DetectorConstruction::ConstructCalorimeter( G4String caloType, G4double detthick, G4double detxy,
    G4double alairgapthick, G4double aluwrapthick, G4double vacthick){
    //..........................................................................
    // Define some geometry parameters .........................................
    G4double alairgapxy = detxy + 2*alairgapthick;
    G4double alairgaplength = detthick + alairgapthick;
    G4double aluwrapxy = alairgapxy + 2*aluwrapthick;
    G4double aluwraplength = alairgaplength + aluwrapthick + vacthick;

    //defining the size of the Calorimetercell and the virtual calorimeter (mother volume of the calorimetercells)
    //G4int NbofCalor = 9; //here later free parameter to select number of crystals
    G4double calorcellxy = aluwrapxy;
    G4double calorcelllength = aluwraplength + vacthick;
    G4double virtcalorlength;

    G4double vac3xy = alairgapxy;// this version is to place the vacstep in the aluwrapping

    G4double virtcalorxy;
    G4double frontPlateZ, frontPlateY,frontPlateX, tbPlateZ;

    if (caloType == "full"){
      SetCrystalnumber(9);
      frontPlateZ=10*mm;
      frontPlateX=160*mm;
      frontPlateY=130*mm;
      tbPlateZ=693*mm;
      virtcalorlength=tbPlateZ;
      virtcalorxy = frontPlateX;
    }
    else{
      virtcalorlength = calorcelllength;
      if (CrystalNumber == 1){
        virtcalorxy = calorcellxy;
      }
      else if (CrystalNumber == 9){
        virtcalorxy = 3*calorcellxy;
      }
      else {
        virtcalorxy = calorcellxy;
        SetCrystalnumber(1);
        G4cout << "NO VALID IMPUT FOR CRYSTALNUMBER: Crystalnumber set to one!" << fCaloMaterial->GetName() << G4endl;
      }
    }

    //..........................................................................
    // Get Materials  .........................................
    G4Material* Air       = allMaterials->GetMat("Air");
    G4Material* Al        = allMaterials->GetMat("Aluminium");
    G4Material* Vacuum    = allMaterials->GetMat("Galactic");
    G4Material* PEEK = allMaterials->GetMat("PEEK");



    //................................................
    //Define Geometry ................................................
    // Virtuel calorimeter (mother volume for the whole calorimeter/detector)
    auto fVirtCaloS= new G4Box("virtualCalorimeter",  //Name
                                  virtcalorxy/2.,   // x size
                                  virtcalorxy/2.,     // y size
                                  virtcalorlength/2.); // z size


    auto fVirtCaloLV = new G4LogicalVolume(fVirtCaloS,    //its solid
                                           fWorldMaterial,    //its material
                                           "virtualCalorimeter");       //its name

    //
    // Calorimeter cells (placing (for now) 9 calorimeter cells in the virtual calorimeter)
    //
    auto fCaloCellS= new G4Box("physicalcalorimeter",  //Name
                                 calorcellxy/2.,   // x size
                                 calorcellxy/2.,     // y size
                                 calorcelllength/2.); // z size

    auto fCaloCellLV = new G4LogicalVolume(fCaloCellS,    //its solid
                                          fWorldMaterial,    //its material
                                          "physicalcalorimeter");       //its name

    G4double calocellZpos;
    if(caloType=="full"){calocellZpos=-tbPlateZ/2+frontPlateZ/2+9*mm+calorcelllength/2;}
    else{calocellZpos=0;}

    if(CrystalNumber==9){
    //the array for the placement of the 9 calorimetercells in the virtual calorimeter
    G4double CalorRX[9]={-calorcellxy, 0, calorcellxy,-calorcellxy, 0, calorcellxy, -calorcellxy, 0, calorcellxy};
    G4double CalorRY[9]={calorcellxy,calorcellxy,calorcellxy, 0,0,0,-calorcellxy,-calorcellxy,-calorcellxy};

    for (G4int i=0;i<=8;i++){
    fCaloCellPV = new G4PVPlacement(0,		       //no rotation
                 G4ThreeVector(CalorRX[i],CalorRY[i],calocellZpos),  //its position
                 fCaloCellLV,            //its logical volume
                "physicalcalorimeter",    //its name
                 fVirtCaloLV,               //its mother
                 false,                     //no boolean operat
                 i);                        //copy number       //copy number
    }
    }

    else if (CrystalNumber == 1){
    fCaloCellPV = new G4PVPlacement(0,		       //no rotation
                 G4ThreeVector(0,0,0),  //its position
                 fCaloCellLV,            //its logical volume
                "physicalcalorimeter",    //its name
                 fVirtCaloLV,               //its mother
                 false,                     //no boolean operat
                 0);                        //copy number       //copy number
    }

    //
    // Alu-wrapping
    //
    auto fAluWrapS= new G4Box("AluWrapping",  //Name
                                  aluwrapxy/2.,   // x size
                                  aluwrapxy/2.,     // y size
                                  aluwraplength/2.); // z size


    auto fAluwrapLV = new G4LogicalVolume(fAluWrapS,    //its solid
                                           Al,    //its material
                                           "AluWrapping");       //its name

    fAluwrapPV = new G4PVPlacement(0,                   //no rotation
                           G4ThreeVector(0.,0.,vacthick/2),    //its position // old 0.,0.,-vacthick/2
                                   fAluwrapLV,            //its logical volume
                                   "AluWrapping",                 //its name
                                   fCaloCellLV,               //its mother
                                   false,                     //no boolean operat
                                   0);                        //copy number

    //
    // AirGap
    //
    auto fAlAirGapS= new G4Box("AlAirGap",  //Name
                                 alairgapxy/2.,   // x size
                                 alairgapxy/2.,     // y size
                                 alairgaplength/2.); // z size


    auto fAlAirGapLV = new G4LogicalVolume(fAlAirGapS,    //its solid
                                          Air,    //its material
                                          "AlAirGap");       //its name

    fAlAirGapPV = new G4PVPlacement(0,                   //no rotation
                          G4ThreeVector(0.,0.,-(vacthick-aluwrapthick)/2),    //its position // old 0.,0.,aluwrapthick/2
                                  fAlAirGapLV,            //its logical volume
                                  "AlAirGap",                 //its name
                                  fAluwrapLV,               //its mother
                                  false,                     //no boolean operat
                                  0);                        //copy number

    //
    // Detector(in this case a crystal)
    //
    //Detector Box shape as in E166
    auto fDetectorS= new G4Box("Detector",  //Name
                                 detxy/2.,   // x size
                                 detxy/2.,     // y size
                                 detthick/2.); // z size


    fDetectorLV = new G4LogicalVolume(fDetectorS,    //its solid
                                          fCaloMaterial,    //its material
                                          "Detector");       //its name

    fDetectorPV = new G4PVPlacement(0,                   //no rotation
                          G4ThreeVector(0.,0.,alairgapthick/2),    //its position
                                  fDetectorLV,            //its logical volume
                                  "Detector",                 //its name
                                  fAlAirGapLV,               //its mother
                                  false,                     //no boolean operat
                                  0);                        //copy number

    //
    //vacuum step 3
    //
    auto fVacStepS3 = new G4Box("VacStep3",  //Name
                                 vac3xy/2.,
                                 vac3xy/2,
                                 vacthick/2.);

    auto fVacStepLV3 = new G4LogicalVolume(fVacStepS3,    //its solid
                                          Vacuum,    //its material
                                          "VacStep3");       //its name

    fVacStepPV3 = new G4PVPlacement(0,                   //no rotation
                          G4ThreeVector(0.,0.,(aluwrapthick+alairgaplength)/2),    //its position //old 0.,0.,aluwraplength/2
                                  fVacStepLV3,            //its logical volume
                                  "VacStep3",                 //its name
                                  fAluwrapLV,               //its mother //old fCaloCellLV
                                  false,                     //no boolean operat
                                  0);                        //copy number

    //
    // vacuum step 4
    // all in front of the crystall

    auto fVacStepS4 = new G4Box("VacStep4",  //Name
                                 calorcellxy/2.,
                                 calorcellxy/2,
                                 vacthick/2.);

    auto fVacStepLV4 = new G4LogicalVolume(fVacStepS4,    //its solid
                                          Vacuum,    //its material
                                          "VacStep4");       //its name

    fVacStepPV4 = new G4PVPlacement(0,                   //no rotation
                          G4ThreeVector(0.,0.,-aluwraplength/2),    //its position //old 0.,0.,aluwraplength/2
                                  fVacStepLV4,            //its logical volume
                                  "VacStep4",                 //its name
                                  fCaloCellLV,               //its mother //old fCaloCellLV
                                  false,                     //no boolean operat
                                  0);                        //copy number

    if (caloType == "full"){
      //
      // Plastic front plate with holes in it -----------------------------------------------------------
      //

      auto FrontPlateBox = new G4Box("FrontPlateBox",  //Name
                                  frontPlateX/2.,
                                  frontPlateY/2,
                                  frontPlateZ/2.);

      auto FrontPlateLV = new G4LogicalVolume(FrontPlateBox,    //its solid
                                          PEEK,    //its material
                                          "FrontPlateLV");       //its name

      G4double FrontPlateZPos = -tbPlateZ/2+frontPlateZ/2+9*mm;

      new G4PVPlacement(0,                   //no rotation
                        G4ThreeVector(0.,0.,FrontPlateZPos),    //its position //old 0.,0.,aluwraplength/2
                                FrontPlateLV,            //its logical volume
                                "FrontPlate",                 //its name
                                fVirtCaloLV,               //its mother
                                false,                     //no boolean operat
                                0);                        //copy number

      G4VisAttributes* FrontPlateVis = new G4VisAttributes(G4Colour(0,128/255,1));
      FrontPlateVis->SetForceSolid(true);
      FrontPlateLV->SetVisAttributes(FrontPlateVis);

      auto FrontPlaneHoleTub = new G4Tubs("FrontPlaneHoleTub",
                                        0.0*mm, // inner radius
                                        18.5*mm ,  // outer radius
                                        frontPlateZ/2., // half length in z
                                        0.0*deg,  // starting angle
                                        360.0*deg ); // total angle

      auto FrontPlaneHoleLV = new G4LogicalVolume(FrontPlaneHoleTub,    //its solid
                                          fWorldMaterial,    //its material
                                          "FrontPlateHoleLV");       //its name

      G4double FrontHolDist = 39.0*mm ;
      G4double FrontHolPosX[9]={-FrontHolDist, 0, FrontHolDist,-FrontHolDist, 0, FrontHolDist, -FrontHolDist, 0, FrontHolDist};
      G4double FrontHolPosY[9]={FrontHolDist,FrontHolDist,FrontHolDist, 0,0,0,-FrontHolDist,-FrontHolDist,-FrontHolDist};

      for (G4int i=0;i<=8;i++){
        new G4PVPlacement(0,		       //no rotation
                  G4ThreeVector(FrontHolPosX[i],FrontHolPosY[i],0),  //its position
                  FrontPlaneHoleLV,            //its logical volume
                  "FrontPlaneHole",    //its name
                  FrontPlateLV,               //its mother
                  false,                     //no boolean operat
                  i);                        //copy number       //copy number
      }

      //
      // Top and botto aluminium plates -----------------------------------------------------------
      //
      G4double tbPlateY= 20*mm;
      auto TBPlateBox = new G4Box("TBPlateBox",  //Name
                                  frontPlateX/2.,
                                  tbPlateY/2,
                                  tbPlateZ/2.);

      auto FrontPlateCut = new G4Box("TBPlateBox",  //Name
                                  frontPlateX+2/2.,
                                  3*mm,
                                  frontPlateZ/2.);

      G4ThreeVector FPCutTrans(0, -tbPlateY/2+2*mm, FrontPlateZPos);

      G4SubtractionSolid* TBPlateSolid = new G4SubtractionSolid("TBPlateSolid", // Name
                                                                 TBPlateBox, // 1. Volume
                                                                 FrontPlateCut, // 2. Volume
                                                                 0, // rotation
                                                                 FPCutTrans // translation
                                                                 );

      auto TBPlateLV = new G4LogicalVolume(TBPlateSolid,    //its solid
                                          Al,    //its material
                                          "TBPlateLV");       //its name

      new G4PVPlacement(0,                   //no rotation
                        G4ThreeVector(0.,frontPlateX/2-tbPlateY/2.,0.),    //its position //
                                TBPlateLV,            //its logical volume
                                "TopPlate",                 //its name
                                fVirtCaloLV,               //its mother
                                false,                     //no boolean operat
                                0);                        //copy number

      G4RotationMatrix* zRot = new G4RotationMatrix;  // Rotates X and y axes only
      zRot->rotateZ(M_PI*rad);

      new G4PVPlacement(zRot,                   //no rotation
                        G4ThreeVector(0.,-frontPlateX/2+tbPlateY/2.,0.),    //its position //
                                TBPlateLV,            //its logical volume
                                "TopPlate",                 //its name
                                fVirtCaloLV,               //its mother
                                false,                     //no boolean operat
                                1);                        //copy number
    }




    //------------------------------------------------------------------
    //set visual VisAttributes
    //------------------------------------------------------------------
    //make virtual mother volumes invisible
    fVirtCaloLV->SetVisAttributes(G4VisAttributes::GetInvisible());
    fCaloCellLV->SetVisAttributes(G4VisAttributes::GetInvisible());

    /*
    G4VisAttributes * AirVis= new G4VisAttributes( G4Colour(119/255. ,136/255. ,153/255. ));
    AirVis->SetVisibility(true);
    AirVis->SetLineWidth(2);
    AirVis->SetForceWireframe( true );
    fAlAirGapLV->SetVisAttributes(AirVis);
    */
    fAlAirGapLV->SetVisAttributes(G4VisAttributes::GetInvisible());

    G4VisAttributes * AluVis= new G4VisAttributes( G4Colour(119/255. ,136/255. ,153/255. ));
    AluVis->SetVisibility(true);
    AluVis->SetLineWidth(2);
    AluVis->SetForceSolid(false);
    fAluwrapLV->SetVisAttributes(AluVis);

    G4VisAttributes * CrystalVis= new G4VisAttributes( G4Colour(224/255. ,255/255. ,255/255. ));
    CrystalVis->SetVisibility(true);
    CrystalVis->SetLineWidth(2);
    CrystalVis->SetForceSolid(true);
    fDetectorLV->SetVisAttributes(CrystalVis);

    G4VisAttributes * CaloVacStepVis= new G4VisAttributes( G4Colour(255/255. ,165/255. ,0/255. ));
    CaloVacStepVis->SetVisibility(true);
    CaloVacStepVis->SetLineWidth(1);
    CaloVacStepVis->SetForceSolid(true);
    fVacStepLV3->SetVisAttributes(CaloVacStepVis);
    fVacStepLV4->SetVisAttributes(CaloVacStepVis);

return fVirtCaloLV;

}

G4LogicalVolume* DetectorConstruction::ConstructDipol(){

  //................................................
  //Define Geometry ................................................
  // Virtuel calorimeter (mother volume for the whole calorimeter/detector)
  auto solidDipole = new G4Box("DipoleSolid",  //Name
                                fDipoleSize[0]/2.,   // x size
                                fDipoleSize[1]/2.,     // y size
                                fDipoleSize[2]/2.); // z size


  fLogicalDipol = new G4LogicalVolume(solidDipole,    //its solid
                                         fWorldMaterial,    //its material
                                         "DipoleFieldVolume");       //its name

  return fLogicalDipol;
}

void DetectorConstruction::ConstructBeamLine(G4LogicalVolume* LogicalWorld, G4double magthick){

  //...............................
  // Define all parameters
  //................................

  // BeamLineTube
  G4double BLLength = 2600*mm;
  G4double BLInnerR = 18.5*mm;
  G4double BLOuterR = 19.25*mm;

  // Exit window of vacuum chamber
  G4double WindowThick = 0.3*mm;

  // Copper Collimator
  G4double CollimatorLength = 200*mm;
  G4double CollimatorWidth = 100*mm;
  G4double CollimatorRadius = 9.4*mm;

  // Lanex Screen
  G4double LanexRad = 76.2*mm / 2.; // Diameter of Lanex Screen = 3 inches
  G4double LanexThick = 0.5*mm; // thickness

  //solenoid holder
  G4double holderBoxWidth = 24.5*cm;
  G4double holderBoxHeight = 10*cm;

  //Distances between parts
  G4double DistTubColl = 100*mm;
  G4double DistCollSol = 200*mm;

  // position of particle gun (at centre of vacuum chamber, here beginning of tube)
  G4double IPz = BLLength + WindowThick + DistTubColl + CollimatorLength + DistCollSol;

  //.................................
  // Get the Materials
  //.................................
  G4Material* Steel = allMaterials->GetMat("G4_STAINLESS-STEEL");
  G4Material* Vacuum = allMaterials->GetMat("Galactic");
  G4Material* Alu = allMaterials->GetMat("Aluminium");
  G4Material* Copper = allMaterials->GetMat("G4_Cu");
  G4Material* Lanex = allMaterials->GetMat("G4_GADOLINIUM_OXYSULFIDE");

  //...............................
  // beam line steel tube
  //................................
  auto SolidBL = new G4Tubs("BLSteelTube", //name
                              0, // inner radius
                              BLOuterR,  // outer radius
                              BLLength/2., // half length in z
                              0.0*deg,  // starting angle
                              360.0*deg ); // total angle

  auto LogicBL = new G4LogicalVolume(SolidBL, //its solid
                                      Steel, // its material
                                      "BLSteelTube" ); // its name

  new G4PVPlacement(0,                   //no rotation
                    G4ThreeVector(0.,0.,-IPz+BLLength/2),    //its position
                            LogicBL,            //its logical volume
                            "BLSteelTube",                 //its name
                            LogicalWorld,               //its mother
                            false,                     //no boolean operat
                            0);                        //copy number

  //...............................
  // beam line inner vacuum
  //................................

  auto SolidBLVac = new G4Tubs("BLVacTube", //name
                              0, // inner radius
                              BLInnerR,  // outer radius
                              BLLength/2., // half length in z
                              0.0*deg,  // starting angle
                              360.0*deg ); // total angle

  auto LogicBLVac = new G4LogicalVolume(SolidBLVac, //its solid
                                      Vacuum, // its material
                                      "BLVacTube" ); // its name

  new G4PVPlacement(0,                   //no rotation
                    G4ThreeVector(0.,0.,0.),    //its position
                            LogicBLVac,            //its logical volume
                            "BLVacTube",                 //its name
                            LogicBL,               //its mother
                            false,                     //no boolean operat
                            0);                        //copy number

  //...............................
  // exit window of vacuum pipe
  //................................
  auto SolidWindow = new G4Tubs("WindowTube", //name
                              0, // inner radius
                              BLOuterR,  // outer radius
                              WindowThick/2., // half length in z
                              0.0*deg,  // starting angle
                              360.0*deg ); // total angle

  auto LogicWindow = new G4LogicalVolume(SolidWindow, //its solid
                                      Alu, // its material
                                      "Window" ); // its name

  new G4PVPlacement(0,                   //no rotation
                    G4ThreeVector(0.,0.,-IPz+BLLength+WindowThick/2.),    //its position
                            LogicWindow,            //its logical volume
                            "Window",                 //its name
                            LogicalWorld,               //its mother
                            false,                     //no boolean operat
                            0);                        //copy number

  //...............................
  // Copper Collimator
  //................................
  auto SolidCollimator = new G4Box("CollimatorSolid",  //Name
                                CollimatorWidth/2.,   // x size
                                CollimatorWidth/2.,     // y size
                                CollimatorLength/2.); // z size

  auto LogicCollimator = new G4LogicalVolume(SolidCollimator, //its solid
                                      Copper, // its material
                                      "Collimator" ); // its name

  new G4PVPlacement(0,                   //no rotation
                    G4ThreeVector(0.,0.,-(CollimatorLength/2. + DistCollSol)),    //its position
                            LogicCollimator,            //its logical volume
                            "Collimator",                 //its name
                            LogicalWorld,               //its mother
                            false,                     //no boolean operat
                            0);                        //copy number

  //...............................
  // Copper Collimator  Hole
  //................................
  auto SolidCollHole = new G4Tubs("CollHoleSolid", //name
                              0, // inner radius
                              CollimatorRadius,  // outer radius
                              CollimatorLength/2., // half length in z
                              0.0*deg,  // starting angle
                              360.0*deg ); // total angle

  auto LogicCollHole = new G4LogicalVolume(SolidCollHole, //its solid
                                      fWorldMaterial, // its material
                                      "CollimatorHole" ); // its name

  new G4PVPlacement(0,                   //no rotation
                    G4ThreeVector(0.,0.,0.),    //its position
                            LogicCollHole,            //its logical volume
                            "CollimatorHole",                 //its name
                            LogicCollimator,               //its mother
                            false,                     //no boolean operat
                            0);                        //copy number

  // .........................................
  // Lanex Screens
  //..........................................
  auto SolidLanex = new G4Tubs("LanexSolid", //Name
                               0, // inner radius
                               LanexRad, // outer radius
                               LanexThick/2, // half length in Z
                               0.0*deg, // starting angle
                               360.0*deg // total angle
                             );

  auto LogicLanex = new G4LogicalVolume(SolidLanex, // it's solid
                                        Lanex, // its material
                                        "LanexScreen"
                                      );

  new G4PVPlacement(0,                   //no rotation
                    G4ThreeVector(0.,0.,-LanexThick),    //its position
                            LogicLanex,            //its logical volume
                            "LanexScreen1",                 //its name
                            LogicalWorld,               //its mother
                            false,                     //no boolean operat
                            0);

  new G4PVPlacement(0,                   //no rotation
                    G4ThreeVector(0.,0.,magthick+LanexThick),    //its position
                            LogicLanex,            //its logical volume
                            "LanexScreen1",                 //its name
                            LogicalWorld,               //its mother
                            false,                     //no boolean operat
                            0);
//..................................
  //Set all the visual Parameters
  //..................................

  G4VisAttributes * BLVis= new G4VisAttributes( G4Colour(191/255. ,191/255. ,191/255. ));
      BLVis->SetVisibility(true);
      BLVis->SetLineWidth(2);
      LogicBL->SetVisAttributes(BLVis);

  G4VisAttributes * BLVacVis= new G4VisAttributes( G4Colour(127/255. ,233/255. ,255/255. ));
      BLVacVis->SetVisibility(true);
      BLVacVis->SetLineWidth(2);
      BLVacVis->SetForceSolid(true);
      LogicBLVac->SetVisAttributes(BLVacVis);

  G4VisAttributes * WindowVis= new G4VisAttributes( G4Colour(252/255. ,252/255. ,12/255. ));
      WindowVis->SetVisibility(true);
      WindowVis->SetLineWidth(2);
      WindowVis->SetForceSolid(true);
      LogicWindow->SetVisAttributes(WindowVis);

  G4VisAttributes * CollVis= new G4VisAttributes( G4Colour(125/255. ,50/255. ,255/255. ));
      CollVis->SetVisibility(true);
      CollVis->SetLineWidth(2);
      LogicCollimator->SetVisAttributes(CollVis);

  G4VisAttributes * CollHoleVis= new G4VisAttributes( G4Colour(252/255. ,255/255. ,255/255. ));
      CollHoleVis->SetVisibility(true);
      CollHoleVis->SetLineWidth(2);
      CollHoleVis->SetForceSolid(true);
      LogicCollHole->SetVisAttributes(CollHoleVis);

}


void DetectorConstruction::PrintParameters()
{
  if(versionType=="Pol" || versionType=="PolCal"){
  G4cout << "\n The ConverterTarget is made of " << fConvMaterial->GetName()
          << " , " << G4BestUnit(fSizeXY,"Length")<<  "in diameter and "
         <<  G4BestUnit(fConvThick,"Length") << " thick"
           << G4endl;
 G4cout << "\n The IronCore is"
        <<  G4BestUnit(fCoreThick,"Length") << " thick"
          << G4endl;
  }

  if(versionType=="Cal" || versionType=="PolCal"){
    G4cout << "\n The Calorimeter is made of " << fCaloMaterial->GetName() <<". \n"<< G4endl;
    G4cout <<"You are simulating the "<< caloTyp << "calorimeter." <<"\n"<< G4endl;
    G4cout <<"It's composed of " << CrystalNumber << "crystals."<<"\n"<< G4endl;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Instantiate the magnetic field
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  void DetectorConstruction::ConstructSDandField()
  {
    if (dipolStatus == "On"){
      G4ThreeVector fieldValue = G4ThreeVector (0.,  fDipoleB,  0.);
      G4MagneticField* magField = new G4UniformMagField(fieldValue);

      auto localFieldManager = new G4FieldManager(magField);
      fLogicalDipol -> SetFieldManager(localFieldManager, true);
    }
  }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void DetectorConstruction::SetConvMaterial(G4String materialChoice)
{
  // search the material by its name
  // G4Material* mat =
  //   G4NistManager::Instance()->FindOrBuildMaterial(materialChoice);
  G4Material* mat = allMaterials->GetMat(materialChoice);
  if (mat != fConvMaterial) {
    if(mat) {
      fConvMaterial = mat;
    } else {
      G4cout << "### Warning!  Converter Target material: <"
           << materialChoice << "> not found" << G4endl;
    }
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetWorldMaterial(G4String materialChoice)
{
  // search the material by its name
  // G4Material* mat =
  //   G4NistManager::Instance()->FindOrBuildMaterial(materialChoice);
  G4Material* mat = allMaterials->GetMat(materialChoice);
  if (mat != fWorldMaterial) {
    if(mat) {
      fWorldMaterial = mat;
    } else {
      G4cout << "### Warning! World material: <"
           << materialChoice << "> not found" << G4endl;
    }
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetCaloMaterial(G4String materialChoice)
{
  // search the material by its name
  // G4Material* mat =
  //   G4NistManager::Instance()->FindOrBuildMaterial(materialChoice);
  G4Material* mat = allMaterials->GetMat(materialChoice);
  if (mat != fCaloMaterial) {
    if(mat) {
      fCaloMaterial = mat;
    } else {
      G4cout << "### Warning! Calorimeter material: <"
           << materialChoice << "> not found" << G4endl;
    }
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetSizeXY(G4double value)
{
  fSizeXY = value;
  if (fWorldSize<fSizeXY) fWorldSize = 10*fSizeXY;
}

void DetectorConstruction::SetCoreThick(G4double value)
{
  fCoreThick = value;
  if (fWorldSize<fCoreThick) fWorldSize = 10*fCoreThick;
}

void DetectorConstruction::SetConvThick(G4double value)
{
  fConvThick = value;
}

void DetectorConstruction::SetCrystalnumber(G4int value)
{
  CrystalNumber = value;
}

void DetectorConstruction::SetDipoleB(G4double value)
{
  fDipoleB = value;
  ConstructSDandField();
}

void DetectorConstruction::SetDipoleSize(G4ThreeVector value)
{
  fDipoleSize = value;
}

void DetectorConstruction::SetZtoCalo(G4double value)
{
  fZtoCalo = value;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "G4RunManager.hh"

void DetectorConstruction::UpdateGeometry()
{
  if (PhysicalWorld)
    G4RunManager::GetRunManager()->DefineWorldVolume(Construct());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
