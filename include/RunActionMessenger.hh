#ifndef RunActionMessenger_h
#define RunActionMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class RunAction;
class G4UIdirectory;
class G4UIcmdWithABool;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class RunActionMessenger: public G4UImessenger
{
public:
  RunActionMessenger(RunAction*);
  virtual ~RunActionMessenger();

  virtual void SetNewValue(G4UIcommand*, G4String);

private:
  RunAction*            fRun;

  G4UIdirectory*        fRunDir;
  G4UIcmdWithABool* fCore1StatCmd;
  G4UIcmdWithABool* fCore2StatCmd;
  G4UIcmdWithABool* fDipole1StatCmd;
  G4UIcmdWithABool* fDipole2StatCmd;
  G4UIcmdWithABool* fDipole3StatCmd;
  G4UIcmdWithABool* fCal1StatCmd;
  G4UIcmdWithABool* fCal2StatCmd;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
