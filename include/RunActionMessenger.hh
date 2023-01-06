#ifndef RunActionMessenger_h
#define RunActionMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class RunAction;
class G4UIdirectory;
/* add depending on Gui commands needed
class G4UIcmdWithAString;
class G4UIcmdWith3Vector;
class G4UIcmdWithAnInteger;
*/

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class RunActionMessenger: public G4UImessenger
{
public:
  RunActionMessenger(RunAction*);
  virtual ~RunActionMessenger();

//  virtual void SetNewValue(G4UIcommand*, G4String);

private:
  RunAction*            fRun;

  //G4UIdirectory*        fRunDir;
  //G4UIcmdWith3Vector*   fAccCmd;
  //G4UIcmdWithAnInteger* fVerbCmd;
};
