{
  // Makes MySelector.C and MySelector.h from test.root
  // Because I always forget how to do this -- ASC 7/29/20

  //Modified for e16032; Update Filename -- TO
 
  gROOT->ProcessLine(".L libTParameters.so");
  //TFile *f = TFile::Open("/projects/e16032/e16032_Analysis_THO/analysis_packet/test.root");
  TFile *f = TFile::Open("/mnt/analysis/e16032/tho/run-0199-675-e16032_analysis_tho.root");
  TTree *t = (TTree *) f->Get("data");
  t->MakeSelector("MySelector");
  gROOT->ProcessLine(".!ls MySelector*");  
}
