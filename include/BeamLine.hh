// JP 29.01.2024
// BeamLine.hh

#ifndef BEAM_LINE_HH
#define BEAM_LINE_HH

//include necessary headers 
#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include "Materials.hh"
#include "G4ThreeVector.hh"

class G4LogicalVolume;
class G4Material;

class BeamLine{
public: 
    BeamLine();
    ~BeamLine();

    // Add member functions to construct geometries for this subsystem
    void ConstructBeamLine(G4LogicalVolume* LogicalWorld, G4double magthick); 

private:
    G4String BeamLine;

};
#endif