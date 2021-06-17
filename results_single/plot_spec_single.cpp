#include "TTree.h"
#include "TFile.h"
#include <string>
#include <array>

void plot_spec_single(){
  string E[1]={"60"};
  string core[1]={"75"};
  string conv[1]={"1.75"};
  string NBunch = "100000";
  TString name0 = string("./run0_")+"NBunch_"+NBunch+"_conv_"+conv[0]
                  +"mm_core_"+core[0]+"mm_E_" + E[0] + "MeV.root";
  TString name1 = string("./run1_")+"NBunch_"+NBunch+"_conv_"+conv[0]
                  +"mm_core_"+core[0]+"mm_E_" + E[0] + "MeV.root";
// get file of magnetic field parallel to polarization and propagation direction of incoming e-
  TFile *F0 = new TFile(name0);
  //pointer to ttree of sensor after magnet
  TTree *T0=(TTree*)F0->Get("bremssim2");

  TFile *F1 = new TFile(name1);
  //pointer to ttree of sensor after magnet
  TTree *T1=(TTree*)F1->Get("bremssim2");

auto C1 = new TCanvas("C1","plot_Esum",200,10,700,500);
  gStyle->SetTitleSize(0.05,"xyz");
  gStyle->SetTitleSize(0.09,"t");
  C1->SetRightMargin(0.09);
  C1->SetLeftMargin(0.15);
  C1->SetBottomMargin(0.15);

  int nbins = 100;
  double startval = 0.1;
  double endval = 60;
  double ebins[nbins+1];
  for (int i=0; i<=nbins; i++)
  {
    ebins[i]=exp(log(startval)+(log(endval)-log(startval))/nbins*double(i));
  }


  //Creates a 1D-histogram with floating numbers
    TH1F *h0 = new TH1F("h0", //Name
                            "E=60 MeV, core=75mm, conv=1.75mm, NBunch=10k;Photon Energy / MeV; Number of Transmitted Photons",// title; axes label
                            nbins, // number of bins X
                            ebins);// xbins

    T0->Draw("E>>h0", // varexp; "e1" produces TH1F, "e1:e2" unbinned 2D scatter plot
            "", // selection; if boolean expression is true, hist is filled with a weight = value
            "E");
  gStyle->SetOptStat(0);
  //Creates a 1D-histogram with floating numbers
    TH1F *h1 = new TH1F("h1", //Name
                            ";Photon Energy / MeV; Number of Transmitted Photons",// title; axes label
                            nbins, // number of bins X
                            ebins);// xbins

    T1->Draw("E>>h1", // varexp; "e1" produces TH1F, "e1:e2" unbinned 2D scatter plot
            "", // selection; if boolean expression is true, hist is filled with a weight = value
            "ESAME");
h0->SetMarkerColor(2);
h1->SetMarkerColor(4);
h0->SetMarkerStyle(20);
h1->SetMarkerStyle(20);
C1->SetLogx();

auto legend = new TLegend(0.5,0.75,0.9,0.9);
 //legend->SetHeader("The Legend Title","C"); // option "C" allows to center the header
 legend->AddEntry(h0,"Antiparallel Polarization","ep");
 legend->AddEntry(h1,"Parallel Polarization","ep");
 legend->Draw();
C1->Print("test.png");


}
