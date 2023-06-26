/// @file QuaSiMaterials.hh \copydoc QuaSiMaterials

#ifndef Materials_h
#define Materials_h 1

#include <string>
#include "globals.hh"
#include "G4Material.hh"

/**
* Defining all necessary materials
*
* Elements, materials, optical properties, ...
* Could set / get things in here.
*/

class Materials{

public:
  /** Defines materials. Called by DetectorContruction */
  Materials(); //
  /** The destructor. */
  ~Materials();

  void DefineMaterials(); ///< Definition of all used elements and materials and their properties.
  G4Material* GetMat(G4String material); ///< Returns pointer to the G4Material matching the string.


private:

  G4Material* Galactic; // Vacuum
  G4Material* Quartz; // Quartz (fused silica) for cherenkov detector
  // G4Material* CsI;  // CesiumIodide
  G4Material* TF1; // LeadGlas
  G4Material* TF101; //LeadGlas radiation hard
  G4Material* Air;
  G4Material* Iron;
  G4Material* Tungsten;
  G4Material* Lead;
  G4Material* Copper;
  G4Material* Aluminium;
  G4Material* PEEK;

  // G4Material* Grease; ///< Optical grease between PMT window and quartz stick


};


#endif /*Materials_h*/
