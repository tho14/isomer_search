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

  //for(int i=0; i<nblob; i++){
  for(int i=14; i<17; i++){
    //All
    hsegaecal_tdiff[i] = new TH3D(Form("Sega_Ecal_TDIFF_%d",i),
				  Form("Sega_Ecal_TDIFF_%d",i),
    				  420,-10, 200, 300, 0, 600, 300, 0, 600);
    hsegaecal_tdiff[i]->GetXaxis()->SetTitle("Sega_Time-Dynode_Time (us)");
    hsegaecal_tdiff[i]->GetYaxis()->SetTitle("E_{#gamma} (keV)");
    hsegaecal_tdiff[i]->GetZaxis()->SetTitle("E_{#gamma} (keV)");
    GetOutputList()->Add(hsegaecal_tdiff[i]);

    hlabr3ecal_tdiff[i] = new TH3D(Form("Labr3_Ecal_TDIFF_%d",i),
				   Form("Labr3_Ecal_TDIFF_%d",i),
    				   420,-10, 200, 100, 0, 600, 100, 0, 600);
    hlabr3ecal_tdiff[i]->GetXaxis()->SetTitle("Labr3_Time-Dynode_Time (us)");
    hlabr3ecal_tdiff[i]->GetYaxis()->SetTitle("E_{#gamma} (keV)");
    hlabr3ecal_tdiff[i]->GetYaxis()->SetTitle("E_{#gamma} (keV)");
    GetOutputList()->Add(hlabr3ecal_tdiff[i]);
    
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

  //SeGAs
  for(int j = 0; j < nsega; j++){
    if(sega_ecal[j] > 10){
      for(int m = j+1; m < nsega; m++){
	if(sega_ecal[m] > 10){
	  for(int k = 0; k <  npspmt; k++){
	    for(int l = 0; l < npspmt; l++){
	      for(int i=14; i<17; i++){
		if(cut[i] && cut[i]->IsInside(corr_sega_dynode_itofpin01i2s[((npspmt*npspmt*j)+(npspmt*k))+l], corr_sega_dynode_idE1[((npspmt*npspmt*j)+(npspmt*k))+l])){
		  if(sega_cfdfailbit[j]!= 1 && *pspmt_dycfdfailbit != 1){
		    hsegaecal_tdiff[i]->Fill((corr_sega_dynode_tdiff[((npspmt*npspmt*j)+(npspmt*k))+l])/1.e3, sega_ecal[j], sega_ecal[m]);
		    hsegaecal_tdiff[i]->Fill((corr_sega_dynode_tdiff[((npspmt*npspmt*j)+(npspmt*k))+l])/1.e3, sega_ecal[m], sega_ecal[j]);
		  }
		}
	      }
	    }
	  }
	}
      }
    }
  }
  
  //Labr3s
  for(int j = 0; j < nlabr3; j++){
    if(labr3_ecal[j] > 10){
      for(int m = j+1; m < nlabr3; m++){
	if(labr3_ecal[m] > 10){
	  for(int k = 0; k <  npspmt; k++){
	    for(int l = 0; l < npspmt; l++){
	      for(int i=14; i<17; i++){
		if(cut[i] && cut[i]->IsInside(corr_labr3_dynode_itofpin01i2s[((npspmt*npspmt*j)+(npspmt*k))+l], corr_labr3_dynode_idE1[((npspmt*npspmt*j)+(npspmt*k))+l])){
		  if(labr3_cfdfailbit[j]!= 1 && *pspmt_dycfdfailbit != 1){
		    hlabr3ecal_tdiff[i]->Fill((corr_labr3_dynode_tdiff[((npspmt*npspmt*j)+(npspmt*k))+l])/1.e3, labr3_ecal[j], labr3_ecal[m]);
		    hlabr3ecal_tdiff[i]->Fill((corr_labr3_dynode_tdiff[((npspmt*npspmt*j)+(npspmt*k))+l])/1.e3, labr3_ecal[m], labr3_ecal[j]);
		  }
		}
	      }
	    }
	  }
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

  TFile *outroot = new TFile("./output/e16032_isomer_search.root","RECREATE");

  outroot->cd();
  
  TDirectory* dirSeGA = outroot->mkdir("SeGA/");
  dirSeGA->cd();

  //for(int i=0; i<nblob; i++){
  for(int i=14; i<17; i++){
    OutputObject(Form("Sega_Ecal_TDIFF_%d",i));
  }

  TDirectory* dirLabr3 = outroot->mkdir("Labr3/");
  dirLabr3->cd();

  //for(int i=0; i<nblob; i++){
  for(int i=14; i<17; i++){
    OutputObject(Form("Labr3_Ecal_TDIFF_%d",i));
  }

}
