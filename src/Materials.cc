/// @file QuaSiMaterials.cc \copydoc QuaSiMaterials

#include "Materials.hh"

#include "G4Material.hh"
#include "G4MaterialTable.hh"
#include "G4Element.hh"
#include "G4ElementTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
//#include "G4ThreeVector.hh"
#include "G4NistManager.hh"

Materials::Materials()
{

}


Materials::~Materials()
{;}


void Materials::DefineMaterials()
{
  G4double a;  // mass of a mole;
  G4double z;  // z=mean number of protons;
  G4double density;
  G4int ncomponents, natoms;
  G4String name, symbol;
  G4double fractionmass;

  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // define elements
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // G4Element* I       = new G4Element(name="Iodine"  ,symbol="I"   , z= 53., a = 126.904*g/mole);
  // G4Element* Cs      = new G4Element(name="Cesium"  ,symbol="Cs"  , z= 55., a = 132.905*g/mole);
  G4Element* O       = new G4Element(name="Oxygen"  ,symbol="O"   , z= 8  , a = 15.099*g/mole);
  G4Element* C       = new G4Element(name="Carbon"  ,symbol="C"   , z= 6  , a = 12.011*g/mole);
  G4Element* H       = new G4Element(name="Hydrogen",symbol="H"   , z= 1  , a = 1.008*g/mole);
  G4Element* As      = new G4Element(name="Arsenic" ,symbol="As"  , z= 33 , a = 74.922*g/mole);
  G4Element* Al      = new G4Element(name="Aluminum",symbol="Al"  , z= 13 , a = 26.982*g/mole);
  G4Element* Si      = new G4Element(name="Silicon" ,symbol="Si"  , z= 14 , a = 28.086*g/mole);
  G4Element* N       = new G4Element(name="Nitrogen",symbol="N"   , z= 7  , a = 14.007*g/mole);
  G4Element* Ar      = new G4Element(name="Argon"   ,symbol="Ar"  , z=18  , a = 39.948*g/mole);
  G4Element* Ce      = new G4Element(name="Cer"     ,symbol="Ce"  , z=58  , a = 140.116*g/mole);
  G4Element* Pb      = new G4Element(name="Blei"    ,symbol="Pb"  , z=82  , a = 207.2*g/mole);
  G4Element* Na      = new G4Element(name="Natrium" ,symbol="Na"  , z=11  , a = 22.9898*g/mole);


  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // define materials
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  density = 3.74*g/cm3;
  G4Material* As2O3 = new G4Material(name="Arsenictrioxide", density, ncomponents=2);
  As2O3->AddElement(As, natoms=2);
  As2O3->AddElement(O , natoms=3);

  density = 8.3*g/cm3; //wikipedia
  G4Material* Pb3O4 = new G4Material(name="RedLead", density, ncomponents=2);
  Pb3O4->AddElement(Pb, natoms=3);
  Pb3O4->AddElement(O , natoms=4);

  density = 2.27*g/cm3; //wikipedia
  G4Material* Na2O = new G4Material(name="Sodiumoxide", density, ncomponents=2);
  Na2O->AddElement(Na, natoms=2);
  Na2O->AddElement(O , natoms=1);

  density = 6.773*g/cm3; //wikipedia
  G4Material* Cer = new G4Material(name="Cer", density, ncomponents=1);
  Cer->AddElement(Ce , natoms=1);

  //material defined using NIST Manager
  auto nistManager = G4NistManager::Instance();

  Iron = nistManager->FindOrBuildMaterial("G4_Fe");
  Tungsten = nistManager->FindOrBuildMaterial("G4_W");
  Copper = nistManager->FindOrBuildMaterial("G4_Cu");
  Lead = nistManager->FindOrBuildMaterial("G4_Pb");
  Steel = nistManager->FindOrBuildMaterial("G4_STAINLESS-STEEL");
  Lanex = nistManager->FindOrBuildMaterial("G4_GADOLINIUM_OXYSULFIDE");

  G4Material* PbO = nistManager->FindOrBuildMaterial("G4_LEAD_OXIDE");
  G4Material* SiO2 = nistManager->FindOrBuildMaterial("G4_SILICON_DIOXIDE");
  G4Material* K2O = nistManager->FindOrBuildMaterial("G4_POTASSIUM_OXIDE");

  /*
  // CsI (Cesiumiodide for the scintillator. Optical properties still need to be defined !!! )
  density = 4.53*g/cm3;
  G4Material* CsI = new G4Material(name="CesiumIodide", density, ncomponents=2);
  CsI->AddElement(Cs, natoms=5);
  CsI->AddElement(I, natoms=5);
 */

  // Lead Glass TF1 with the chemical composition Maryna gave me
  TF1 = new G4Material("TF1", density= 3.860*g/cm3, ncomponents=5);
  TF1->AddMaterial(PbO   , fractionmass=0.512);
  TF1->AddMaterial(SiO2  , fractionmass=0.413);
  TF1->AddMaterial(K2O   , fractionmass=0.035);
  TF1->AddMaterial(Na2O  , fractionmass=0.035);
  TF1->AddMaterial(As2O3 , fractionmass=0.005);

  // Lead Glass TF101 with the chemical composition Maryna gave me
  TF101 = new G4Material("TF101", density= 3.860*g/cm3, ncomponents=4);
  TF101->AddMaterial(Pb3O4 , fractionmass=0.5123);
  TF101->AddMaterial(SiO2  , fractionmass=0.4157); //here 0.4153 but it do not add up to 1 therefore I just use 0.4157
  TF101->AddMaterial(K2O   , fractionmass=0.07);
  TF101->AddMaterial(Cer   , fractionmass=0.002);

  // PEEK: polyether ether ketone, radiation hard polymer
  PEEK = new G4Material("PEEK", density=1320*kg/m3, ncomponents=3);
  PEEK->AddElement(C, natoms=19);
  PEEK->AddElement(H, natoms=14);
  PEEK->AddElement(O, natoms=3);


  // Aluminium
  Aluminium = new G4Material ("Aluminium", density=2.70*g/cm3, ncomponents=1);
  Aluminium->AddElement (Al, 1.);

  // Quartz / fused silica
  Quartz = new G4Material ("Quartz", density=2.20*g/cm3, ncomponents=2);
  Quartz->AddElement (O,  2./3.);
  Quartz->AddElement (Si, 1./3.);

  // Air
  Air = new G4Material("Air" , density = 1.290*mg/cm3, ncomponents = 3);
  Air->AddElement(N, 78*perCent);
  Air->AddElement(O, 21*perCent);
  Air->AddElement(Ar, 1*perCent);

  // Vacuum
  Galactic = new G4Material("Galactic", z=1., a=1.01*g/mole,density= universe_mean_density,
                  kStateGas, 2.73*kelvin, 3.e-18*pascal);


  //*************************************************************************************
  // This part below is to define the optical properties
  //*************************************************************************************

  //-------------------------------------------------------------------------------------
  // Quartz (taken from Quartz Cherenkov Simulation)
  //-------------------------------------------------------------------------------------
  const G4int nSpectRI=25;
  const G4int nSpectAbs=9;

  G4double EnergySpectrosil[nSpectRI]={1.38 *eV, 1.455*eV, 1.755*eV, 1.889*eV,
                                       1.926*eV, 1.959*eV, 2.104*eV, 2.110*eV,
                                       2.270*eV, 2.550*eV, 2.845*eV, 3.064*eV,
                                       3.397*eV, 3.709*eV, 3.965*eV,  4.886*eV,
                                       4.993*eV, 4.999*eV, 5.417*eV,  5.780*eV,
                                       6.011*eV, 6.383*eV, 6.411*eV, 6.424*eV, 6.7*eV};

  G4double RISpectrosil[nSpectRI]={1.45181, 1.45247, 1.45515, 1.45637, 1.4567 ,
                                   1.45702, 1.4584 , 1.45846, 1.46008, 1.46313,
                                   1.46669, 1.46962, 1.47454, 1.47975, 1.48447,
                                   1.50547, 1.50838, 1.50855, 1.52109, 1.53365,
                                   1.54259, 1.55884, 1.56014, 1.56077, 1.57495};

  G4double ESpectrosil[nSpectAbs] ={1.38*eV , 3.1*eV ,  3.35*eV , 4.0*eV , 4.43*eV ,
                                   4.96*eV , 5.64*eV , 6.53*eV , 6.7*eV};

  G4double ABSpectrosil[nSpectAbs]={134.8*mm, 131.2*mm, 130.1*mm, 124.8*mm, 119.9*mm,
                                    112.6*mm, 106.0*mm, 94.9*mm, 1.6*mm};

  G4MaterialPropertiesTable* MPT_Quartz = new G4MaterialPropertiesTable();

  MPT_Quartz->AddProperty ("RINDEX",  EnergySpectrosil, RISpectrosil, nSpectRI);
  MPT_Quartz->AddProperty ("ABSLENGTH", ESpectrosil, ABSpectrosil, nSpectAbs);


  Quartz->SetMaterialPropertiesTable(MPT_Quartz);

  //-------------------------------------------------------------------------------------
  // TF1 / TF101  Lead glass
  //-------------------------------------------------------------------------------------
  const int n_LeadGlass=13;
  const int n_LeadGlass2=20;

  G4double PE_LeadGlass[n_LeadGlass] = {1.39*eV, 1.46*eV, 1.61*eV, 1.76*eV,
                                        1.89*eV, 1.93*eV, 2.10*eV, 2.11*eV,
                                        2.27*eV, 2.55*eV, 2.58*eV, 2.85*eV,
                                        3.06*eV }; //3.40*eV};

  G4double RI_LeadGlass[n_LeadGlass] = {1.63172, 1.63289, 1.63602, 1.63901,
                                        1.642076, 1.64295, 1.6475, 1.647665,
                                        1.652188, 1.661196, 1.662347, 1.672451,
                                        1.68229}; //1.70022};

  G4double PE_LeadGlass2[n_LeadGlass2]= {1.38*eV, 1.55*eV, 1.65*eV, 1.77*eV,
                                         1.82*eV, 1.88*eV, 1.94*eV, 2.00*eV,
                                         2.07*eV, 2.14*eV, 2.21*eV, 2.30*eV,
                                         2.38*eV, 2.48*eV, 2.58*eV, 2.70*eV,
                                         2.82*eV, 2.95*eV, 3.10*eV, 3.26*eV };


  G4double abslength_LeadGlass[n_LeadGlass2]= {3731.2*mm, 4982.5*mm,3731.2*mm, 1858.1*mm, // absorption length calculated from data sheet
                                               1858.1*mm,1649.2*mm,1649.2*mm,  1649.2*mm,
                                               1858.1*mm, 2482.5*mm, 2482.5*mm, 2482.5*mm,
                                               1858.1*mm, 1482.0*mm,1135.9*mm, 864.2*mm,
                                               582.3*mm, 436.5*mm, 245.1*mm, 57.5*mm};


  G4MaterialPropertiesTable* MPT_LeadGlass = new G4MaterialPropertiesTable();
  MPT_LeadGlass->AddProperty ("RINDEX", PE_LeadGlass, RI_LeadGlass, n_LeadGlass);
  MPT_LeadGlass->AddProperty ("ABSLENGTH", PE_LeadGlass2, abslength_LeadGlass, n_LeadGlass2);

  TF1->SetMaterialPropertiesTable (MPT_LeadGlass);
  TF101->SetMaterialPropertiesTable (MPT_LeadGlass);

  //-------------------------------------------------------------------------------------
  // Vacuum
  //-------------------------------------------------------------------------------------
  G4double PhoEn[nSpectRI]={1.38 *eV, 1.455*eV, 1.755*eV, 1.889*eV,
                            1.926*eV, 1.959*eV, 2.104*eV, 2.110*eV,
                            2.270*eV, 2.550*eV, 2.845*eV, 3.064*eV,
                            3.397*eV, 3.709*eV, 3.965*eV,  4.886*eV,
                            4.993*eV, 4.999*eV, 5.417*eV,  5.780*eV,
                            6.011*eV, 6.383*eV, 6.411*eV, 6.424*eV, 6.7*eV};

  G4double RIVac[nSpectRI]={1.0, 1.0, 1.0, 1.0, 1.0 ,
                            1.0, 1.0 , 1.0, 1.0, 1.0,
                            1.0, 1.0, 1.0, 1.0, 1.0,
                            1.0, 1.0, 1.0, 1.0, 1.0,
                            1.0, 1.0, 1.0, 1.0, 1.0};

  G4MaterialPropertiesTable* MPT_Vacuum = new G4MaterialPropertiesTable();
  MPT_Vacuum->AddProperty ("RINDEX",  PhoEn, RIVac, nSpectRI);
  Galactic->SetMaterialPropertiesTable (MPT_Vacuum);

  //-------------------------------------------------------------------------------------
  // Air (values taken from QuaSi)
  //-------------------------------------------------------------------------------------
  const int n_air=2;

  G4double PE_Air[n_air] = {1.38*eV, 6.7*eV};
  G4double RI_Air[n_air] = {1.00029, 1.00029};

  G4MaterialPropertiesTable* MPT_Air = new G4MaterialPropertiesTable();
  MPT_Air->AddProperty ("RINDEX", PE_Air, RI_Air, n_air);
  Air->SetMaterialPropertiesTable (MPT_Air);

  //-------------------------------------------------------------------------------------
  // Aluminum (values taken from QuaSi)
  //-------------------------------------------------------------------------------------
  G4double PE_Al[8] = {1.38*eV, 1.90*eV, 2.38*eV, 2.48*eV, 5.17*eV, 5.64*eV, 6.20*eV, 6.7*eV };
  G4double RE_Al[8] = {0.82,    0.83,    0.84,    0.85,    0.86,    0.84,    0.81,    0.79 };

  G4MaterialPropertiesTable *MPT_Al = new G4MaterialPropertiesTable();
  MPT_Al->AddProperty ("REFLECTIVITY", PE_Al, RE_Al, 8);
  Aluminium->SetMaterialPropertiesTable(MPT_Al);

  // Print materials
  G4cout << *(G4Material::GetMaterialTable()) << G4endl;


}


G4Material* Materials::GetMat(G4String material)
{
  // Returns a material
  G4Material* pttoMaterial = G4Material::GetMaterial(material);
  return pttoMaterial;
}
