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

#include "G4OpBoundaryProcess.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction(G4String version)
: G4VUserDetectorConstruction(),
  PhysicalWorld(0), PhysicalCore(0), fConvMaterial(0), fWorldMaterial(0), fCaloMaterial(0)
{
  versionType=version;
  allMaterials = new Materials();
  allMaterials->DefineMaterials();

  fSizeXY = 50*mm;
  fCoreThick = 75*mm;
  fConvThick = 1.75*mm;
  fWorldSize = 4.1*m;
  CrystalNumber= "one";

  SetConvMaterial("G4_W");
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
  G4double vacthick = 1.0*mm;
  G4double  maggap1 = 48.5*mm;
  G4double  maggap2 = 12.5*mm;
  G4double magthick = 2.*(maggap1+maggap2+fConvThick)+fCoreThick;

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
    G4LogicalVolume* LogicalSolenoid = ConstructSolenoid(magthick, maggap2, vacthick);

    new G4PVPlacement(0,	//rotation
                     G4ThreeVector(0.0*mm, 0.0*mm, 0.0*mm),// translation position
                     LogicalSolenoid,      //its logical volume
                       "PhysicalMagnet",   //its name  (2nd constructor)
                       LogicalWorld,         //its mother volume
                       false,              //no boolean operation
                       0);                 //copy number

  } //end if-statement polarimeter


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
    G4double spacePolCal = 50. *mm; //5cm between pol and cal
    G4double motherLength = detthick+alairgapthick+aluwrapthick+2.*vacthick;
    G4double caloZposition = (magthick+motherLength)/2+spacePolCal;

    G4LogicalVolume* fVirtCaloLV = ConstructCalorimeter(detthick, detxy,
        alairgapthick, aluwrapthick, vacthick);

    fVirtCaloPV = new G4PVPlacement(0,                   //no rotation
                           G4ThreeVector(0.,0.,caloZposition),    //its position
                                   fVirtCaloLV,            //its logical volume
                                   "virtualCalorimeter",                 //its name
                                  LogicalWorld,               //its mother
                                   false,                     //no boolean operat
                                   0);                        //copy number

  } //end if-statement for the calorimeter geometry

  PrintParameters();

  //always return the root volume
  //
  return PhysicalWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Construct the Solenoid , returns LV of motherVolume
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4LogicalVolume* DetectorConstruction::ConstructSolenoid(G4double magthick,
  G4double maggap2, G4double vacthick){
    //
    // Geometry Parameters
    //

    G4double  absrad  = fSizeXY/2.;
    G4double shieldrad=75.0*mm;
    G4double corethick = fCoreThick;
    G4double convthick = fConvThick;
    G4double  coilthick = corethick + 25.0*mm;
    G4double  shieldthick = corethick - 25.0*mm;
    G4double  conedist = corethick/2. + maggap2;

    //
    //Get materials
    //
    G4Material* absMat    = allMaterials->GetMat("G4_W");
    G4Material* magMat    = allMaterials->GetMat("G4_Fe");
    G4Material* coilMat   = allMaterials->GetMat("G4_Cu");
    G4Material* shieldMat = allMaterials->GetMat("G4_Pb");

    // Housing of Magnet
    //
    G4double DzArrayMagnet   [] = {-magthick/2.  , -conedist , -corethick/2. , corethick/2.,  conedist, magthick/2.    };
    G4double RminArrayMagnet [] = {36.84308*mm,  absrad,  absrad , absrad,  absrad,  36.84308*mm};
    G4double RmaxArrayMagnet [] = {196.0*mm   ,  196.0*mm, 196.0*mm ,196.0*mm, 196.0*mm, 196.0*mm    };

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
                                    170.0*mm,  // outer radius
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


    // Conversion Target
    //
    G4Tubs *solidConversion = new G4Tubs("solidConversion", // name
                                        0.0*mm, // inner radius
                                        absrad, // outer radius
                                        convthick/2., // half length in z
                                        0.0*deg, // starting angle
                                        360.0*deg ); // total angle

    G4LogicalVolume *LogicalConversion = new G4LogicalVolume(solidConversion, 	 //its solid
                absMat,          //its material
                "ConversionTarget" ,	 //its name
                0,0,0);

    new G4PVPlacement(0,	//rotation
                G4ThreeVector(0.0*mm, 0.0*mm, -maggap2-convthick/2-corethick/2),
              LogicalConversion,         //its logical volume
              "PhysicalConversion",   //its name  (2nd constructor)
              LogicalSolenoid,              //its mother volume
              false,                 //no boolean operation
              0);                       //copy number


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
                         G4ThreeVector(0.,0., - corethick/2 -maggap2 +vacthick/2 +1.0*mm),    //its position
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

      G4VisAttributes * ConversionTargetVis= new G4VisAttributes( G4Colour(105/255. ,105/255. ,105/255. ));
      ConversionTargetVis->SetVisibility(true);
      ConversionTargetVis->SetLineWidth(2);
      ConversionTargetVis->SetForceSolid(true);
      LogicalConversion->SetVisAttributes(ConversionTargetVis);

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
G4LogicalVolume* DetectorConstruction::ConstructCalorimeter( G4double detthick, G4double detxy,
    G4double alairgapthick, G4double aluwrapthick, G4double vacthick){
    //..........................................................................
    // Define some geometry parameters .........................................
    G4double alairgapxy = detxy + 2*alairgapthick;
    G4double alairgaplength = detthick + alairgapthick;
    G4double aluwrapxy = alairgapxy + 2*aluwrapthick;
    G4double aluwraplength = alairgaplength + aluwrapthick + vacthick;

    //defining the size of the Calorimetercell and the virtual calorimeter (mother volume of the calorimetercells)
    //G4int NbofCalor = 9; //here later free paramter to select numer of crystals
    G4double calorcellxy = aluwrapxy;
    G4double calorcelllength = aluwraplength + vacthick;
    G4double virtcalorlength = calorcelllength;

    G4double vac3xy = alairgapxy;// this version is to place the vacstep in the aluwrapping

    G4double virtcalorxy;
    if (CrystalNumber == "one"){
      virtcalorxy = calorcellxy;
    }
    else if (CrystalNumber == "nine"){
      virtcalorxy = 3*calorcellxy;
    }
    else {
      virtcalorxy = calorcellxy;
      SetCrystalnumber("one");
      G4cout << "NO VALID IMPUT FOR CRYSTALNUMBER: Crystalnumber set to one!" << fCaloMaterial->GetName() << G4endl;
    }
    //..........................................................................
    // Get Materials  .........................................
    G4Material* Air       = allMaterials->GetMat("Air");
    G4Material* Al        = allMaterials->GetMat("Aluminium");
    G4Material* Vacuum    = allMaterials->GetMat("Galactic");

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

    if(CrystalNumber=="nine"){
    //the array for the placement of the 9 calorimetercells in the virtual calorimeter
    G4double CalorRX[9]={0,0,calorcellxy,calorcellxy,calorcellxy,0,-calorcellxy,-calorcellxy,-calorcellxy};
    G4double CalorRY[9]={0,calorcellxy,calorcellxy,0,-calorcellxy,-calorcellxy,-calorcellxy,0,calorcellxy};

    for (G4int i=0;i<=8;i++){
    fCaloCellPV = new G4PVPlacement(0,		       //no rotation
                 G4ThreeVector(CalorRX[i],CalorRY[i],0),  //its position
                 fCaloCellLV,            //its logical volume
                "physicalcalorimeter",    //its name
                 fVirtCaloLV,               //its mother
                 false,                     //no boolean operat
                 i);                        //copy number       //copy number
    }
    }

    else if (CrystalNumber == "one"){
    fCaloCellPV = new G4PVPlacement(0,		       //no rotation
                 G4ThreeVector(0,0,0),  //its position
                 fCaloCellLV,            //its logical volume
                "physicalcalorimeter",    //its name
                 fVirtCaloLV,               //its mother
                 false,                     //no boolean operat
                 0);                        //copy number       //copy number
    }


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

   /*
   //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   // Optical boundary surfaces
   //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   // Al <-> Air optical surface copied from QuaSim
   //
   G4MaterialPropertiesTable *AlWrapProperty = new G4MaterialPropertiesTable();
   const G4int n_AlAir=8;
   // G4double PE_AlWrap[n_AlAir] = {1.38*eV, 1.90*eV, 2.38*eV, 2.48*eV, 5.17*eV, 5.64*eV, 6.20*eV, 7.77*eV };
   // G4double RE_AlWrap[n_AlAir] = {0.82,    0.83,    0.84,    0.85,    0.86,    0.84,    0.81,    0.74 };
   G4double PE_AlWrap[n_AlAir] = {1.38*eV, 1.90*eV, 2.38*eV, 2.48*eV, 5.17*eV, 5.64*eV, 6.20*eV, 6.7*eV };
   G4double RE_AlWrap[n_AlAir] = {0.82,    0.83,    0.84,    0.85,    0.86,    0.84,    0.81,    0.79 };
   AlWrapProperty -> AddProperty("REFLECTIVITY", PE_AlWrap, RE_AlWrap, n_AlAir);

   // optical and logical surface
   G4OpticalSurface* OpAlWrapSurface = new G4OpticalSurface("AirAluSurface");
   OpAlWrapSurface -> SetType(dielectric_metal);
   OpAlWrapSurface -> SetFinish(ground);
   G4double AlPolish = 0; // 1 = smooth, 0 = maximum roughness
   OpAlWrapSurface -> SetPolish(AlPolish);
   OpAlWrapSurface -> SetModel(glisur);
   OpAlWrapSurface -> SetMaterialPropertiesTable(AlWrapProperty);
   // need to attach them to all the physical volumes for the alu
   G4LogicalBorderSurface* AlWrapSurface =
            new G4LogicalBorderSurface("AirAluSurface",fAluwrapPV , fAlAirGapPV, OpAlWrapSurface);
   G4LogicalBorderSurface* AlWrapSurface2 =
            new G4LogicalBorderSurface("AirAluSurface", fAlAirGapPV, fAluwrapPV, OpAlWrapSurface);


    //
    /// - Quartz <-> Air: Unified model (taken from QuaSi) /have to check if TF1 <-> Air is different

    // properties table
    G4MaterialPropertiesTable* AirQSurfProp = new G4MaterialPropertiesTable();
    const G4int n_AirQ=2;
    G4double OpAirSpecularlobe[n_AirQ] = {1.0, 1.0};
    G4double OpAirSpecularspike[n_AirQ] = {0.0, 0.0};
    G4double OpAirBackscatter[n_AirQ] = {0.0, 0.0};

    G4double PE_OpAir[n_AirQ] = {1.38*eV, 6.70*eV};
    G4double RI_OpAir[n_AirQ] = {1.00029, 1.00029};
    AirQSurfProp -> AddProperty("RINDEX", PE_OpAir, RI_OpAir, n_AirQ);
    AirQSurfProp -> AddProperty("SPECULARLOBECONSTANT", PE_OpAir, OpAirSpecularlobe, n_AirQ);
    AirQSurfProp -> AddProperty("SPECULARSPIKECONSTANT", PE_OpAir, OpAirSpecularspike, n_AirQ);
    AirQSurfProp -> AddProperty("BACKSCATTERCONSTANT", PE_OpAir, OpAirBackscatter, n_AirQ);

    // optical and logical surface
    G4OpticalSurface* OpAirSurface = new G4OpticalSurface("QAirSurface");
    OpAirSurface -> SetType(dielectric_dielectric);
    OpAirSurface -> SetModel(unified);

    // from Janacek, Morses, Simulating Scintillator Light ..., IEEE 2010:
    // polished 1.3 degree, etched 3.8 degree, ground 12 degree
    //          0.023              0.066              0.21
    // degree   1      2      3      4      5      6      7      8      9      10     11     12
    // radians  0.017  0.035  0.052  0.070  0.087  0.105  0.122  0.140  0.157  0.175  0.192  0.209
    // degree   13     14     15
    // radians  0.227  0.244  0.262

    OpAirSurface -> SetFinish(ground);
    //  G4double sigma_alpha = 0.023;
    //  G4double sigma_alpha = 0.066;
    G4double sigma_alpha = 0.209;
    OpAirSurface -> SetSigmaAlpha(sigma_alpha);
    OpAirSurface -> SetMaterialPropertiesTable(AirQSurfProp);
    G4LogicalBorderSurface* AirSurface =
    	new G4LogicalBorderSurface("QAirSurface", fDetectorPV, fAlAirGapPV, OpAirSurface);
    G4LogicalBorderSurface* AirSurface2 =
    	new G4LogicalBorderSurface("QAirSurface", fAlAirGapPV, fDetectorPV, OpAirSurface);
    */
return fVirtCaloLV;

}


void DetectorConstruction::PrintParameters()
{ if(versionType=="Pol" || versionType=="PolCal"){
  G4cout << "\n The ConverterTarget is made of " << fConvMaterial->GetName()
          << " , " << G4BestUnit(fSizeXY,"Length")<<  "in diameter and "
         <<  G4BestUnit(fConvThick,"Length") << " thick"
           << G4endl;
 G4cout << "\n The IronCore is"
        <<  G4BestUnit(fCoreThick,"Length") << " thick"
          << G4endl;
  }

  if(versionType=="Cal" || versionType=="PolCal"){
    G4cout << "\n The Calorimeter is made of " << fCaloMaterial->GetName() << G4endl;
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
      UpdateGeometry();
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
      UpdateGeometry();
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
      UpdateGeometry();
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
  UpdateGeometry();
}

void DetectorConstruction::SetCoreThick(G4double value)
{
  fCoreThick = value;
  if (fWorldSize<fCoreThick) fWorldSize = 10*fCoreThick;
  UpdateGeometry();
}

void DetectorConstruction::SetConvThick(G4double value)
{
  fConvThick = value;
  UpdateGeometry();
}

void DetectorConstruction::SetCrystalnumber(G4String value)
{
  CrystalNumber=value;
  UpdateGeometry();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "G4RunManager.hh"

void DetectorConstruction::UpdateGeometry()
{
  if (PhysicalWorld)
    G4RunManager::GetRunManager()->DefineWorldVolume(Construct());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
