#include "RunActionMessenger.hh"

#include "RunAction.hh"
#include "G4UIdirectory.hh"
// #include "G4UIcmdWith3Vector.hh"
// #include "G4UIcmdWithAString.hh"
// #include "G4UIcmdWithAnInteger.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunActionMessenger::RunActionMessenger(RunAction* run)
:G4UImessenger(),fRun(run),
 fRunDir(0),
 // fAccCmd(0),
 // fVerbCmd(0)
{
  fRunDir = new G4UIdirectory("/leap/run/");
  fRunDir->SetGuidance("run control");

  // fAccCmd = new G4UIcmdWith3Vector("/testem/run/acceptance",this);
  // fAccCmd->SetGuidance("set Edep and RMS");
  // fAccCmd->SetGuidance("acceptance values for first layer");
  // fAccCmd->SetParameterName("edep","rms","limit",true);
  // fAccCmd->SetRange("edep>0 && edep<1 && rms>0");
  // fAccCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  //
  // fVerbCmd = new G4UIcmdWithAnInteger("/testem/run/verbose",this);
  // fVerbCmd->SetGuidance("set verbose level for runAction");
  // fVerbCmd->SetParameterName("verbose",false);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunActionMessenger::~RunActionMessenger()
{
  // delete fVerbCmd;
  // delete fAccCmd;
  delete fRunDir;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunActionMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{
  // if (command == fAccCmd )
  //  { fRun->SetEdepAndRMS(fAccCmd->GetNew3VectorValue(newValue));}
  //
  // if (command == fVerbCmd )
  //  { fRun->SetVerbose(fVerbCmd->GetNewIntValue(newValue));}
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
