void test_asy(){
  TFile *F0 = new TFile("submit_jobs/run0_TestRun.root");
  TTree *T0=(TTree*)F0->Get("bremssim2");

  TFile *F1 = new TFile("submit_jobs/run1_TestRun.root");
  TTree *T1=(TTree*)F1->Get("bremssim2");

double nevt = 100000;

  T0->Draw("E>>Hist0", // varexp; "e1" produces TH1F, "e1:e2" unbinned 2D scatter plot
      "", // selection; if boolean expression is true, hist is filled with a weight = value
      "E"); // Drawing  Option
  TH1F *Hist0 = (TH1F*)gPad->GetPrimitive("Hist0");
  double N0=Hist0->GetEntries();
  double E0= Hist0->GetMean()*N0;

  T1->Draw("E>>Hist1", // varexp; "e1" produces TH1F, "e1:e2" unbinned 2D scatter plot
      "", // selection; if boolean expression is true, hist is filled with a weight = value
      "E"); // Drawing  Option
  TH1F *Hist1 = (TH1F*)gPad->GetPrimitive("Hist1");
  double N1=Hist1->GetEntries();
  double E1= Hist1->GetMean()*N1;

  double delta = (E1-E0)/(E1+E0);

   cout<<"asy is "<< delta*100<< "%"<<endl;

}
