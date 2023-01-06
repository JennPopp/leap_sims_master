#ifndef SteppingActionMessenger_h
#define SteppingActionMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class SteppingAction;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWith3Vector;
class G4UIcmdWithAnInteger;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class SteppingActionMessenger: public G4UImessenger
{
public:
  SteppingActionMessenger(SteppingAction*);
  virtual ~SteppingActionMessenger();

  virtual void SetNewValue(G4UIcommand*, G4String);

private:
  SteppingAction*            fStep;

  G4UIdirectory*        fStepDir;
  // Here add all the GUI commands like e.g. :
  // G4UIcmdWithADoubleAndUnit* fConvZCmd;
};
