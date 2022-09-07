#define isomer_cxx
// The class definition in isomer.h has been generated automatically
// by the ROOT utility TTree::MakeSelector(). This class is derived
// from the ROOT class TSelector. For more information on the TSelector
// framework see $ROOTSYS/README/README.SELECTOR or the ROOT User Manual.


// The following methods are defined in this file:
//    Begin():        called every time a loop on the tree starts,
//                    a convenient place to create your histograms.
//    SlaveBegin():   called after Begin(), when on PROOF called only on the
//                    slave servers.
//    Process():      called for each event, in this function you decide what
//                    to read and fill your histograms.
//    SlaveTerminate: called at the end of the loop on the tree, when on PROOF
//                    called only on the slave servers.
//    Terminate():    called at the end of the loop on the tree,
//                    a convenient place to draw/fit your histograms.
//
// To use this file, try the following session on your Tree T:
//
// root> T->Process("isomer.C")
// root> T->Process("isomer.C","some options")
// root> T->Process("isomer.C+")
//


#include "isomer.h"
#include <TH2.h>
#include <TStyle.h>

void isomer::Begin(TTree * /*tree*/)
{
   // The Begin() function is called at the start of the query.
   // When running with PROOF Begin() is only called on the client.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();
}

void isomer::SlaveBegin(TTree * /*tree*/)
{
  // The SlaveBegin() function is called after the Begin() function.
  // When running with PROOF SlaveBegin() is called on each slave server.
  // The tree argument is deprecated (on PROOF 0 is passed).

  TString option = GetOption();

  for(int i = 0; i < 30; i++) {
    cut[i] = new TCutG();
    cut[i] = (TCutG*)iongates->Get(Form("blob%d", i));
  }

  hPID1 = new TH2D("PID1", "PID1",
		  1000, 6000, 24000, 1000,6000,24000);
  hPID1->GetXaxis()->SetTitle("TOF");
  hPID1->GetYaxis()->SetTitle("dE1");
  GetOutputList()->Add(hPID1);

  hPID2 = new TH2D("PID2", "PID2",
		   1000, 6000, 24000, 1000,6000,24000);
  hPID2->GetXaxis()->SetTitle("TOF");
  hPID2->GetYaxis()->SetTitle("dE1");
  GetOutputList()->Add(hPID2);


  //for(int i=0; i<nblob; i++){
    for(int i=14; i<17; i++){
    hsegaecal_v_tdiff_gamma_only[i] = new TH2D(Form("Sega_Ecal_v_Tdiff_gamma_only%d",i),
					       Form("Sega_Ecal_v_Tdiff_gamma_only%d",i),
					       1000, -1000, 99000, 1200,0,2400);
    //100, -100, 990, 1200,0,2400);
    //1000, -500, 500, 1200,0,2400);
    hsegaecal_v_tdiff_gamma_only[i]->GetXaxis()->SetTitle("Sega_Time-Dynode_corr_itime / 100 ns");
    //hsegaecal_v_tdiff_gamma_only[i]->GetXaxis()->SetTitle("Sega_Time-Dynode_corr_itime / ns");
    hsegaecal_v_tdiff_gamma_only[i]->GetYaxis()->SetTitle("Sega_Ecal / 2 keV");
    GetOutputList()->Add(hsegaecal_v_tdiff_gamma_only[i]);

    hlabr3ecal_v_tdiff_gamma_only[i] = new TH2D(Form("LaBr3_Ecal_v_Tdiff_gamma_only%d",i),
						Form("LaBr3_Ecal_v_Tdiff_gamma_only%d",i),
						1000, -1000, 99000, 400,0,2400);
    //1000, -500, 500, 400,0,2400);
    hlabr3ecal_v_tdiff_gamma_only[i]->GetXaxis()->SetTitle("LaBr3_TimeCal-Dynode_Time / 100 ns");
    //hlabr3ecal_v_tdiff_gamma_only[i]->GetXaxis()->SetTitle("LaBr3_TimeCal-Dynode_Time / ns");
    hlabr3ecal_v_tdiff_gamma_only[i]->GetYaxis()->SetTitle("LaBr3_Ecal / 6 keV");
    GetOutputList()->Add(hlabr3ecal_v_tdiff_gamma_only[i]);
  }

}

Bool_t isomer::Process(Long64_t entry)
{
  // The Process() function is called for each entry in the tree (or possibly
  // keyed object in the case of PROOF) to be processed. The entry argument
  // specifies which entry in the currently loaded tree is to be processed.
  // When processing keyed objects with PROOF, the object is already loaded
  // and is available via the fObject pointer.
  //
  // This function should contain the \"body\" of the analysis. It can contain
  // simple or elaborate selection criteria, run algorithms on the data
  // of the event and typically fill histograms.
  //
  // The processing can be stopped by calling Abort().
  //
  // Use fStatus to set the return value of TTree::Process().
  //
  // The return value is currently not used.

  fReader.SetEntry(entry);

  /*Trying to populate the same histogram with entries that satisfy the condition that we have an implant event; as well as events where we have only gammas!
    Probably not working because we implicitly reject implant events when we want detectors only*/
  
  double maxtdiff = 200000;//200 us max for now

  //PID
  for(int j = 0; j < nsega; j++){
    if(sega_ecal[j] > 10){
      for(int k = 0; k < npspmt; k++){
	for(int l = 0; l < npspmt; l++){
	  if(*corr_isimplant == 1 || *corr_flag == 56){ 
	    hPID1->Fill(corr_sega_implant_itofpin01i2s[((npspmt*npspmt*j)+(npspmt*k))+l], corr_sega_implant_idE1[((npspmt*npspmt*j)+(npspmt*k))+l]);
	  }
	}
      }
    }
  }

  for(int j = 0; j < nlabr3; j++){
    if(labr3_ecal[j] > 10){
      for(int k = 0; k < npspmt; k++){
	for(int l = 0; l < npspmt; l++){
	  if(*corr_isimplant == 1 || *corr_flag == 60){ 
	    hPID2->Fill(corr_labr3_implant_itofpin01i2s[((npspmt*npspmt*j)+(npspmt*k))+l], corr_labr3_implant_idE1[((npspmt*npspmt*j)+(npspmt*k))+l]);
	  }
	}
      }
    }
  }
  
  //SeGAs
  for(int j = 0; j < nsega; j++){
    if(sega_ecal[j] > 10){
      for(int k = 0; k <  npspmt; k++){
	for(int l = 0; l < npspmt; l++){     
	  //if(corr_sega_implant_tdiff[j][k][l] > 0 && corr_sega_implant_tdiff[j][k][l] < maxtdiff) {
	  //for(int i=0; i<nblob; i++){
	    for(int i=14; i<17; i++){
	    if(cut[i] && cut[i]->IsInside(corr_sega_implant_itofpin01i2s[((npspmt*npspmt*j)+(npspmt*k))+l], corr_sega_implant_idE1[((npspmt*npspmt*j)+(npspmt*k))+l])){
	      if(*corr_isimplant == 1 || *corr_flag == 56){ 
		//hsegaecal_v_tdiff_gamma_only[i]->Fill(corr_sega_implant_tdiff[j][k][l], sega_ecal[j]);
		hsegaecal_v_tdiff_gamma_only[i]->Fill(corr_sega_implant_tdiff[((npspmt*npspmt*j)+(npspmt*k))+l], sega_ecal[j]);
	      }
	    }
	  }
	  //}
	}
      }
    }
  }

  //LaBr3s
  for(int j = 0; j < nlabr3; j++){
    if(labr3_ecal[j] > 10){
      for(int k = 0; k < npspmt; k++){
	for(int l = 0; l < npspmt; l++){     
	  //if(corr_labr3_implant_tdiff[j][k][l] > 0 && corr_labr3_implant_tdiff[j][k][l] < maxtdiff) {
	  //for(int i=0; i<nblob; i++){
	    for(int i=14; i<17; i++){
	    if(cut[i] && cut[i]->IsInside(corr_labr3_implant_itofpin01i2s[((npspmt*npspmt*j)+(npspmt*k))+l], corr_labr3_implant_idE1[((npspmt*npspmt*j)+(npspmt*k))+l])){
	      if(*corr_isimplant == 1 || *corr_flag == 60){
		//hlabr3ecal_v_tdiff_gamma_only[i]->Fill(corr_labr3_implant_tdiff[j][k][l], labr3_ecal[j]);
		hlabr3ecal_v_tdiff_gamma_only[i]->Fill(corr_labr3_implant_tdiff[((npspmt*npspmt*j)+(npspmt*k))+l], labr3_ecal[j]);
	      }
	    }
	  }
	  //}
	}
      }
    }
  }


  return kTRUE;
}

void isomer::SlaveTerminate()
{
   // The SlaveTerminate() function is called after all entries or objects
   // have been processed. When running with PROOF SlaveTerminate() is called
   // on each slave server.

}

void isomer::Terminate()
{
  // The Terminate() function is the last function to be called during
  // a query. It always runs on the client, it can be used to present
  // the results graphically or save the results to file.

  TFile *outroot = new TFile("./output/e16032_isomer_search_run_199_all.root","RECREATE");

  outroot->cd();
  OutputObject("PID1");
  OutputObject("PID2");
  
  TDirectory* dirSeGAOnly = outroot->mkdir("SeGAOnly/");
  dirSeGAOnly->cd();

  //for(int i=0; i<nblob; i++){
    for(int i=14; i<17; i++){
    OutputObject(Form("Sega_Ecal_v_Tdiff_gamma_only%d",i));
  }

  TDirectory* dirLaBr3Only = outroot->mkdir("LaBr3Only/");
  dirLaBr3Only->cd();

  //for(int i=0; i<nblob; i++){
    for(int i=14; i<17; i++){
    OutputObject(Form("LaBr3_Ecal_v_Tdiff_gamma_only%d",i));
  }


}
