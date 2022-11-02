#ifndef TupleFunctions_h
#define TupleFunctions_h

#include "globals.hh"
#include "G4Step.hh"

void WriteSingleEntry(int tupleID, const G4Step* aStep);

void WriteSingleCalEntry(int tupleID, const G4Step* aStep);

#endif
