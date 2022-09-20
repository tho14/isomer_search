#include "TFile.h"
#include "TF1.h"
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "TCanvas.h"

#include "TGraph.h"
#include "TGraphErrors.h"
#include "TPad.h"
#include "TAxis.h"
#include "TROOT.h"

#include "TExec.h"
#include "TStyle.h"
#include "TMinuit.h"

#include <vector>
#include <iostream>
#include <fstream>

using namespace std;

void two_d_energy_projections(){
  
  gROOT->SetBatch(1);

  //Labr3s
  
  /*Obtain 3D Egamma2(Z)//Egamma1(Y)//Tdiff(X) histogram
    Binning information below:
    # of bins |   lower limit |    upper limit
    Tdiff     420,            -10 ns    200 us
    Egamma1   300 (100),          0,          600 ()
    Egamma2   300 (100),          0,          600 ()   */

  int blobnum = 14;
  int nblob = 30;
  //****INPUT*****//
  vector<Double_t> ecal_low = {0, 217, 397};//
  vector<Double_t> ecal_high = {1200, 225, 405};//

  //Get File
  TFile *filein1 = new TFile("/projects/e16032/e16032_Analysis_THO/fixed_correlator_analysis_packet/selectors/proof_selectors_all_runs_corrected_pid/32Na_isomer/us_range/detector_gamgam/output/e16032_isomer_search.root");
  
  //Segas
  //Get 3D histogram; make 2D projection
  TH3D *hsega_3D;
  TH2D *hsega_2D;
  vector<TH2D*> hsega_2Dclone;

  cout<<"Making 2D projections of time difference distribution (Segas) "<<endl;
  for(Int_t i = 0; i < ecal_low.size(); i++){
    hsega_3D = (TH3D*)filein1->Get(Form("SeGA/Sega_Ecal_TDIFF_%d",blobnum));
    hsega_3D->GetYaxis()->SetRangeUser(ecal_low[i],ecal_high[i]);
    hsega_2D = (TH2D*)hsega_3D->Project3D(Form("zx_%d",i));
    hsega_2D->SetNameTitle(Form("sega_v_tdiff_ecal_slice_%g_%g",ecal_low[i],ecal_high[i]),Form("sega_v_tdiff_ecal_slice_%g_%g",ecal_low[i],ecal_high[i]));
    hsega_2Dclone.push_back(hsega_2D);
  }

  //Segas
  cout<<"Writing 2D projections to file"<<endl;
  
  TFile *fileout1 = new TFile(Form("./output/2d_ecal_projections_blob_%d.root",blobnum),"RECREATE");
  for(Int_t i = 0; i < ecal_low.size(); i++){
    hsega_2Dclone[i]->Write();
  }


}
