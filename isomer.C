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

  //for(int i=0; i<30; i++){
  for(int i=14; i<17; i++){
    hsegaecal_v_tdiff_gamma_only[i] = new TH2D(Form("Sega_Ecal_v_Tdiff_gamma_only%d",i),
					       Form("Sega_Ecal_v_Tdiff_gamma_only%d",i),
					       //190, 985, 1080, 400,0,2400);
					       100, 990, 100000, 1200,0,2400);
    //hsegaecal_v_tdiff_gamma_only[i]->GetXaxis()->SetTitle("Sega_TimeCal-Dynode_Time / 0.5 ns");
    hsegaecal_v_tdiff_gamma_only[i]->GetXaxis()->SetTitle("Sega_Time-Dynode_Time / 1000 ns");
    hsegaecal_v_tdiff_gamma_only[i]->GetYaxis()->SetTitle("Sega_Ecal / 2 keV");
    GetOutputList()->Add(hsegaecal_v_tdiff_gamma_only[i]);

    hlabr3ecal_v_tdiff_gamma_only[i] = new TH2D(Form("LaBr3_Ecal_v_Tdiff_gamma_only%d",i),
						Form("LaBr3_Ecal_v_Tdiff_gamma_only%d",i),
						//190, 985, 1080, 400,0,2400);
						100, 990, 100000, 400,0,2400);
    //hlabr3ecal_v_tdiff_gamma_only[i]->GetXaxis()->SetTitle("LaBr3_TimeCal-Dynode_Time / 0.5 ns");
    hlabr3ecal_v_tdiff_gamma_only[i]->GetXaxis()->SetTitle("LaBr3_TimeCal-Dynode_Time / 1000 ns");
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
  

  //SeGAs
  double maxtdiff = 200000;//200 us max for now
  
  for(int i=14; i<17; i++){//Looking at Na isotopes
    if(cut[i] && cut[i]->IsInside(*tac_pin01i2s, *pin01_energy)){
      for(int j = 0; j < 16; j++){
  	if(sega_ecal[j] > 10){
  	  Double_t tdiff_dynode_sega = sega_time[j] - *pspmt_dytime + 1000;//Offset by 1000
  	  if(tdiff_dynode_sega > 0 && tdiff_dynode_sega < maxtdiff) { 
  	    if(*corr_isimplant == 1){
  	      //Fill same histo before including SeGA only
  	      hsegaecal_v_tdiff_gamma_only[i]->Fill(tdiff_dynode_sega, sega_ecal[j]);
  	    }
  	    //if(*corr_flag == 64){//Gamma only; Isn't this quite redundant?
  	    // if(*corr_flag == 56){//There is a SeGA hit
  	    //   hsegaecal_v_tdiff_gamma_only[i]->Fill(tdiff_dynode_sega, sega_ecal[j]);
  	    // }
  	    //}
  	  }
  	}
      }
    }
  }

  //LaBr3s
  for(int i=14; i<17; i++){//Looking at Na isotopes
    if(cut[i] && cut[i]->IsInside(*tac_pin01i2s, *pin01_energy)){
      for(int j = 0; j < 15; j++){
  	if(labr3_ecal[j] > 10){
  	  Double_t tdiff_dynode_labr3 = labr3_timecal[j] - *pspmt_dytime + 1000; //offset by 1000
  	  if(tdiff_dynode_labr3 > 0 && tdiff_dynode_labr3 < maxtdiff) { 
  	    // if(*corr_isimplant == 1){
  	    //   //Fill same histo before including LaBr3 only
  	    //   hlabr3ecal_v_tdiff_gamma_only[i]->Fill(tdiff_dynode_labr3, labr3_ecal[j]);
  	    // }
  	    //if(*corr_flag == 64){//Gamma only; Isn't this quite redundant?
  	      if(*corr_flag == 60){//There is a LaBr3 hit
  	    	hlabr3ecal_v_tdiff_gamma_only[i]->Fill(tdiff_dynode_labr3, labr3_ecal[j]);
  	      }
  	      //}
  	  }
  	}
      }
    }
  }

  // // /*Try including detector only condition before the blob identification condition*/
  // if(*corr_flag == 56){
  //   for(int j = 0; j < 16; j++){
  //     if(sega_ecal[j] > 10){
  // 	Double_t tdiff_dynode_sega = sega_time[j] - *pspmt_dytime + 1000;//Offset by 1000
  // 	if(tdiff_dynode_sega > 0 && tdiff_dynode_sega < maxtdiff) {
  // 	  for(int i=14; i<17; i++){//Looking at Na isotopes
  // 	  //if(cut[i] && cut[i]->IsInside(*tac_pin01i2s, *pin01_energy)){
  // 	      hsegaecal_v_tdiff_gamma_only[i]->Fill(tdiff_dynode_sega, sega_ecal[j]);
  // 	      //}
  // 	      }
  // 	}
  //     }
  //   }
  // }

  // if(*corr_flag == 60){
  //   for(int j = 0; j < 15; j++){
  //     if(labr3_ecal[j] > 10){
  // 	Double_t tdiff_dynode_labr3 = labr3_timecal[j] - *pspmt_dytime + 1000; //offset by 1000
  // 	if(tdiff_dynode_labr3 > 0 && tdiff_dynode_labr3 < maxtdiff) {
  // 	  for(int i=14; i<17; i++){//Looking at Na isotopes
  // 	  //if(cut[i] && cut[i]->IsInside(*tac_pin01i2s, *pin01_energy)){
  // 	      hlabr3ecal_v_tdiff_gamma_only[i]->Fill(tdiff_dynode_labr3, labr3_ecal[j]);
  // 	      //}
  // 	      }
  // 	}
  //     }
  //   }
  // }
	    

    // //Try without sega only first?
    // if(*corr_flag == 64){//Gamma only; This is quite redundant.
    //   if(*corr_flag == 56){//There is a SeGA hit
    // 	for(int i = 0; i < 16; i++){
    // 	  if(sega_ecal[i] > 10){
    // 	    //Double_t tdiff_dynode_labr3_cal = labr3_timecal[i] - *pspmt_dytime;
    // 	    Double_t tdiff_dynode_sega = sega_time[i] - *pspmt_dytime;
    // 	    if(tdiff_dynode_sega > 0 && tdiff_dynode_sega < maxtdiff) {
    // 	      for(int j=0; j<30; j++){
    // 		if(cut[j] && cut[j]->IsInside(*tac_pin01i2s, *pin01_energy)){
    // 		  hsegaecal_v_tdiff_gamma_only[j]->Fill(tdiff_dynode_sega, sega_ecal[i]);
    // 		}
    // 	      }
    // 	    }
    // 	    //gamma gamma here??
    // 	  }
    // 	}
    //   }

  
  //     if(*corr_flag == 60){//There is a LaBr3 hit
  // 	for(int i = 0; i < 15; i++){
  // 	  if(labr3_ecal[i] > 10){
  // 	    Double_t tdiff_dynode_labr3_cal = labr3_timecal[i] - *pspmt_dytime;
  // 	    //Double_t tdiff_dynode_sega = sega_time[i] - *pspmt_dytime;
  // 	    if(tdiff_dynode_labr3_cal > 0 && tdiff_dynode_labr3_cal < maxtdiff){
  // 	      for(int j=0; j<30; j++){
  // 		if(cut[j] && cut[j]->IsInside(*tac_pin01i2s, *pin01_energy)){
  // 		  hlabr3ecal_v_tdiff_gamma_only[j]->Fill(tdiff_dynode_labr3_cal, labr3_ecal[i]);
  // 		}
  // 	      }
  // 	    }
  // 	  }
  // 	}
  //     }
  //   }
  // }

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

  TFile *outroot = new TFile("./output2/e16032_isomer_search_32Na_run_199.root","RECREATE");

  outroot->cd();

  TDirectory* dirSeGAOnly = outroot->mkdir("SeGAOnly/");
  dirSeGAOnly->cd();
  
  for(int i=14; i<17; i++){
    OutputObject(Form("Sega_Ecal_v_Tdiff_gamma_only%d",i));
  }

  TDirectory* dirLaBr3Only = outroot->mkdir("LaBr3Only/");
  dirLaBr3Only->cd();
  
  for(int i=14; i<17; i++){
    OutputObject(Form("LaBr3_Ecal_v_Tdiff_gamma_only%d",i));
  }


}
