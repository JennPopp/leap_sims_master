#include "RunActionMessenger.hh"

#include "RunAction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithABool.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunActionMessenger::RunActionMessenger(RunAction* run)
:G4UImessenger(),fRun(run),
 fRunDir(0), fCore1StatCmd(0), fCore2StatCmd(0),fDipole1StatCmd(0),
 fDipole2StatCmd(0), fCal1StatCmd(0), fCal2StatCmd(0)

{
  fRunDir = new G4UIdirectory("/leap/run/");
  fRunDir->SetGuidance("run control");

  fCore1StatCmd = new G4UIcmdWithABool("/leap/run/SetCore1Stat",this);
  fCore1StatCmd->SetGuidance("set detector status for Core1 vacstep");
  fCore1StatCmd->SetParameterName("detector state",false);
  fCore1StatCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  fCore2StatCmd = new G4UIcmdWithABool("/leap/run/SetCore2Stat",this);
  fCore2StatCmd->SetGuidance("set detector status for Core2 vacstep");
  fCore2StatCmd->SetParameterName("detector state",false);
  fCore2StatCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  fDipole1StatCmd = new G4UIcmdWithABool("/leap/run/SetDipole1Stat",this);
  fDipole1StatCmd->SetGuidance("set detector status for Dipole1 vacstep");
  fDipole1StatCmd->SetParameterName("detector state",false);
  fDipole1StatCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  fDipole2StatCmd = new G4UIcmdWithABool("/leap/run/SetDipole2Stat",this);
  fDipole2StatCmd->SetGuidance("set detector status for Dipole2 vacstep");
  fDipole2StatCmd->SetParameterName("detector state",false);
  fDipole2StatCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  fCal1StatCmd = new G4UIcmdWithABool("/leap/run/SetCal1Stat",this);
  fCal1StatCmd->SetGuidance("set detector status for Cal1 vacstep");
  fCal1StatCmd->SetParameterName("detector state",false);
  fCal1StatCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  fCal2StatCmd = new G4UIcmdWithABool("/leap/run/SetDipole1Stat",this);
  fCal2StatCmd->SetGuidance("set detector status for Cal2 vacstep");
  fCal2StatCmd->SetParameterName("detector state",false);
  fCal2StatCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunActionMessenger::~RunActionMessenger()
{
  delete fCore1StatCmd;
  delete fCore2StatCmd;
  delete fDipole1StatCmd;
  delete fDipole2StatCmd;
  delete fCal1StatCmd;
  delete fCal2StatCmd;
  delete fRunDir;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunActionMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{
  if (command == fCore1StatCmd )
   { fRun->SetCore1Stat(fCore1StatCmd->GetNewBoolValue(newValue));}

  if (command == fCore2StatCmd )
   { fRun->SetCore2Stat(fCore2StatCmd->GetNewBoolValue(newValue));}

  if (command == fDipole1StatCmd )
   { fRun->SetDipole1Stat(fDipole1StatCmd->GetNewBoolValue(newValue));}

  if (command == fDipole2StatCmd )
    { fRun->SetDipole2Stat(fDipole2StatCmd->GetNewBoolValue(newValue));}

  if (command == fCal1StatCmd )
    { fRun->SetCal1Stat(fCal1StatCmd->GetNewBoolValue(newValue));}

  if (command == fCal2StatCmd )
    { fRun->SetCal2Stat(fCal2StatCmd->GetNewBoolValue(newValue));}

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
