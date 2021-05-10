#include "TTree.h"
#include "TFile.h"
#include <string>
#include <array>

void showEntries(){
  string E[1]={"20"};
  string core[1]={"300"};
  string conv[1]={"1.75"};
  string NBunch = "100000";
//  TString name0 = string("./run0_")+"NBunch_"+NBunch+"_conv_"+conv[0]
            //      +"mm_core_"+core[0]+"mm_E_" + E[0] + "MeV.root";
TString name0 = "./submit_jobs/run0_testresult.root";
// get file of magnetic field parallel to polarization and propagation direction of incoming e-
  TFile *F0 = new TFile(name0);
  //pointer to ttree of sensor after magnet
  TTree *TTP0=(TTree*)F0->Get("bremssim2");

  TTP0->Scan("Esum:NP");
}
