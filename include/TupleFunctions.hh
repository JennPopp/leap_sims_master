#ifndef TupleFunctions_h
#define TupleFunctions_h

#include "globals.hh"
#include "G4Step.hh"

void WriteSingleEntry(int tupleID, const G4Step* aStep);

void WriteSingleCalEntry(int tupleID, const G4Step* aStep);

void WriteShowerDevEntry(int tupleID, const G4Step* aStep);

void BookSingleTuple(G4String name, G4String title);

void BookSingleCalTuple(G4String name, G4String title);

void BookBunchTuple(G4String name, G4String title);

void BookBunchCalTuple(G4String name, G4String title);

void BookShowerDevTuple(G4String name, G4String title);

#endif
