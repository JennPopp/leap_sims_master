#include "TTree.h"
#include "TFile.h"
#include <string>
#include <array>

void showEntries(){
//Enter the name of the .root file
TString name0 = "./run0_testresult.root";
//Enter the name of the ttree
TString ttname = "Spectrum";
// Enter the string of variable names you want to be shown, separated by :
TString varString ="pdg:E";

///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//pointer to file
  TFile *F0 = new TFile(name0);
  //pointer to ttree of sensor after magnet
  TTree *TT0=(TTree*)F0->Get(ttname);
// print the first few leaves of specified branches
  TT0->Scan(varString);
}
