//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Mon Aug 29 15:36:36 2022 by ROOT version 6.10/02
// from TTree data/data
// found on file: run-0199-502-e16032_analysis_tho.root
//////////////////////////////////////////////////////////

#ifndef isomer_h
#define isomer_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TSelector.h>
#include <TTreeReader.h>
#include <TTreeReaderValue.h>
#include <TTreeReaderArray.h>

// Headers needed by this particular selector
#include <TSystem.h>
#include <TH1.h>
#include <TH2.h>
#include <TStyle.h>
#include <TCutG.h>
#include <TMath.h>
#include <TDirectory.h>

#define runnum1 172
#define runnum2 186
#define runnum3 199

#define fcorrwin1 1
#define fcorrwin2 3

TFile *iongates = new TFile("/projects/e16032/e16032_Analysis_THO/fixed_correlator_analysis_packet/selectors/proof_selectors_all_runs/pid/diagnosing_double_bands_pin01/e16032_pid_cut.root");

class isomer : public TSelector {
 public :
  TTreeReader     fReader;  //!the tree reader
  TTree          *fChain = 0;   //!pointer to the analyzed TTree or TChain

  TCutG *cut[30] = {};

  double halflife[30];
  int nsega = 16;
  int nlabr3 = 15;
  int nblob = 30;
  int npspmt = 16;

  TH2D* hsegaecal_v_tdiff_gamma_only[30] = {nullptr};//first fill with implant events; add events with gamma only
  TH2D* hlabr3ecal_v_tdiff_gamma_only[30] = {nullptr};
  TH2D *hPID1 = nullptr;
  TH2D *hPID2 = nullptr;

  // Readers to access the data (delete the ones you do not need).
  TTreeReaderValue<unsigned int> fUniqueID = {fReader, "fUniqueID"};
  TTreeReaderValue<unsigned int> fBits = {fReader, "fBits"};
  TTreeReaderValue<unsigned int> clock_fUniqueID = {fReader, "clock.fUniqueID"};
  TTreeReaderValue<unsigned int> clock_fBits = {fReader, "clock.fBits"};
  TTreeReaderValue<Double_t> clock_time = {fReader, "clock.time"};
  TTreeReaderValue<Double_t> clock_current = {fReader, "clock.current"};
  TTreeReaderValue<Double_t> clock_initial = {fReader, "clock.initial"};
  TTreeReaderValue<unsigned int> tac_fUniqueID = {fReader, "tac.fUniqueID"};
  TTreeReaderValue<unsigned int> tac_fBits = {fReader, "tac.fBits"};
  TTreeReaderValue<Double_t> tac_pin01rf = {fReader, "tac.pin01rf"};
  TTreeReaderValue<Double_t> tac_pin01xfp = {fReader, "tac.pin01xfp"};
  TTreeReaderValue<Double_t> tac_pin02xfp = {fReader, "tac.pin02xfp"};
  TTreeReaderValue<Double_t> tac_pin01i2n = {fReader, "tac.pin01i2n"};
  TTreeReaderValue<Double_t> tac_pin01i2s = {fReader, "tac.pin01i2s"};
  TTreeReaderValue<Double_t> tac_pin02i2n = {fReader, "tac.pin02i2n"};
  TTreeReaderValue<Double_t> tac_pin02i2s = {fReader, "tac.pin02i2s"};
  TTreeReaderValue<Double_t> tac_i2ni2s = {fReader, "tac.i2ni2s"};
  TTreeReaderValue<Double_t> tac_pin01xfptime = {fReader, "tac.pin01xfptime"};
  TTreeReaderValue<Double_t> tac_pin02xfptime = {fReader, "tac.pin02xfptime"};
  TTreeReaderValue<Double_t> tac_pin01i2ntime = {fReader, "tac.pin01i2ntime"};
  TTreeReaderValue<Double_t> tac_pin01i2stime = {fReader, "tac.pin01i2stime"};
  TTreeReaderValue<Double_t> tac_pin02i2ntime = {fReader, "tac.pin02i2ntime"};
  TTreeReaderValue<Double_t> tac_pin02i2stime = {fReader, "tac.pin02i2stime"};
  TTreeReaderValue<Double_t> tac_i2ni2stime = {fReader, "tac.i2ni2stime"};
  TTreeReaderValue<Double_t> tac_pin01rftime = {fReader, "tac.pin01rftime"};
  TTreeReaderValue<unsigned int> pin01_fUniqueID = {fReader, "pin01.fUniqueID"};
  TTreeReaderValue<unsigned int> pin01_fBits = {fReader, "pin01.fBits"};
  TTreeReaderValue<Double_t> pin01_energy = {fReader, "pin01.energy"};
  TTreeReaderValue<Double_t> pin01_ecal = {fReader, "pin01.ecal"};
  TTreeReaderValue<Double_t> pin01_time = {fReader, "pin01.time"};
  TTreeReaderValue<unsigned int> pin02_fUniqueID = {fReader, "pin02.fUniqueID"};
  TTreeReaderValue<unsigned int> pin02_fBits = {fReader, "pin02.fBits"};
  TTreeReaderValue<Double_t> pin02_energy = {fReader, "pin02.energy"};
  TTreeReaderValue<Double_t> pin02_ecal = {fReader, "pin02.ecal"};
  TTreeReaderValue<Double_t> pin02_time = {fReader, "pin02.time"};
  TTreeReaderValue<unsigned int> pin03_fUniqueID = {fReader, "pin03.fUniqueID"};
  TTreeReaderValue<unsigned int> pin03_fBits = {fReader, "pin03.fBits"};
  TTreeReaderValue<Double_t> pin03_energy = {fReader, "pin03.energy"};
  TTreeReaderValue<Double_t> pin03_ecal = {fReader, "pin03.ecal"};
  TTreeReaderValue<Double_t> pin03_time = {fReader, "pin03.time"};
  TTreeReaderValue<unsigned int> pid_fUniqueID = {fReader, "pid.fUniqueID"};
  TTreeReaderValue<unsigned int> pid_fBits = {fReader, "pid.fBits"};
  TTreeReaderValue<Double_t> pid_de1 = {fReader, "pid.de1"};
  TTreeReaderValue<Double_t> pid_de2 = {fReader, "pid.de2"};
  TTreeReaderValue<Double_t> pid_pin01i2n = {fReader, "pid.pin01i2n"};
  TTreeReaderValue<Double_t> pid_pin01i2s = {fReader, "pid.pin01i2s"};
  TTreeReaderValue<Double_t> pid_pin02i2n = {fReader, "pid.pin02i2n"};
  TTreeReaderValue<Double_t> pid_pin02i2s = {fReader, "pid.pin02i2s"};
  TTreeReaderValue<Double_t> pid_pin01xfp = {fReader, "pid.pin01xfp"};
  TTreeReaderValue<Double_t> pid_pin02xfp = {fReader, "pid.pin02xfp"};
  TTreeReaderValue<Double_t> pid_pin01rf = {fReader, "pid.pin01rf"};
  TTreeReaderValue<unsigned int> corr_fUniqueID = {fReader, "corr.fUniqueID"};
  TTreeReaderValue<unsigned int> corr_fBits = {fReader, "corr.fBits"};
  TTreeReaderValue<Double_t> corr_dtimplant = {fReader, "corr.dtimplant"};
  TTreeReaderValue<Double_t> corr_itime = {fReader, "corr.itime"};
  TTreeReaderValue<Double_t> corr_ide1 = {fReader, "corr.ide1"};
  TTreeReaderValue<Double_t> corr_ide2 = {fReader, "corr.ide2"};
  TTreeReaderValue<Double_t> corr_itofpin01i2n = {fReader, "corr.itofpin01i2n"};
  TTreeReaderValue<Double_t> corr_itofpin01i2s = {fReader, "corr.itofpin01i2s"};
  TTreeReaderValue<Double_t> corr_itofpin02i2n = {fReader, "corr.itofpin02i2n"};
  TTreeReaderValue<Double_t> corr_itofpin02i2s = {fReader, "corr.itofpin02i2s"};
  TTreeReaderValue<Double_t> corr_itofpin01xfp = {fReader, "corr.itofpin01xfp"};
  TTreeReaderValue<Double_t> corr_itofpin02xfp = {fReader, "corr.itofpin02xfp"};
  TTreeReaderValue<Double_t> corr_itofpin01rf = {fReader, "corr.itofpin01rf"};
  TTreeReaderValue<Double_t> corr_dtimecal = {fReader, "corr.dtimecal"};
  TTreeReaderValue<Double_t> corr_dtime = {fReader, "corr.dtime"};
  TTreeReaderValue<Int_t> corr_ddfrontch = {fReader, "corr.ddfrontch"};
  TTreeReaderValue<Int_t> corr_ddbackch = {fReader, "corr.ddbackch"};
  TTreeReaderValue<Int_t> corr_ifrontch = {fReader, "corr.ifrontch"};
  TTreeReaderValue<Int_t> corr_ibackch = {fReader, "corr.ibackch"};
  TTreeReaderValue<Int_t> corr_dfrontch = {fReader, "corr.dfrontch"};
  TTreeReaderValue<Int_t> corr_dbackch = {fReader, "corr.dbackch"};
  TTreeReaderValue<Double_t> corr_dfrontmax = {fReader, "corr.dfrontmax"};
  TTreeReaderValue<Double_t> corr_dbackmax = {fReader, "corr.dbackmax"};
  TTreeReaderValue<Double_t> corr_dde1 = {fReader, "corr.dde1"};
  TTreeReaderValue<Double_t> corr_dde2 = {fReader, "corr.dde2"};
  TTreeReaderValue<Double_t> corr_dtofpin01i2n = {fReader, "corr.dtofpin01i2n"};
  TTreeReaderValue<Double_t> corr_dtofpin01i2s = {fReader, "corr.dtofpin01i2s"};
  TTreeReaderValue<Double_t> corr_dtofpin02i2n = {fReader, "corr.dtofpin02i2n"};
  TTreeReaderValue<Double_t> corr_dtofpin02i2s = {fReader, "corr.dtofpin02i2s"};
  TTreeReaderValue<Double_t> corr_dtofpin01xfp = {fReader, "corr.dtofpin01xfp"};
  TTreeReaderValue<Double_t> corr_dtofpin02xfp = {fReader, "corr.dtofpin02xfp"};
  TTreeReaderValue<Double_t> corr_dtofpin01rf = {fReader, "corr.dtofpin01rf"};
  TTreeReaderValue<Int_t> corr_flag = {fReader, "corr.flag"};
  TTreeReaderValue<Int_t> corr_mult = {fReader, "corr.mult"};
  TTreeReaderValue<Int_t> corr_dmult = {fReader, "corr.dmult"};
  TTreeReaderValue<Int_t> corr_dnumcorr = {fReader, "corr.dnumcorr"};
  TTreeReaderArray<Double_t> corr_labr3_implant_tdiff = {fReader, "corr.labr3_implant_tdiff[15][16][16]"};
   TTreeReaderArray<Double_t> corr_sega_implant_tdiff = {fReader, "corr.sega_implant_tdiff[16][16][16]"};
   TTreeReaderArray<Double_t> corr_labr3_implant_idE1 = {fReader, "corr.labr3_implant_idE1[15][16][16]"};
   TTreeReaderArray<Double_t> corr_labr3_implant_itofpin01i2s = {fReader, "corr.labr3_implant_itofpin01i2s[15][16][16]"};
   TTreeReaderArray<Double_t> corr_sega_implant_idE1 = {fReader, "corr.sega_implant_idE1[16][16][16]"};
   TTreeReaderArray<Double_t> corr_sega_implant_itofpin01i2s = {fReader, "corr.sega_implant_itofpin01i2s[16][16][16]"};
  TTreeReaderValue<Double_t> corr_idyecal = {fReader, "corr.idyecal"};
  TTreeReaderValue<Double_t> corr_ddyecal = {fReader, "corr.ddyecal"};
  TTreeReaderValue<Double_t> corr_didyecal = {fReader, "corr.didyecal"};
  TTreeReaderValue<Int_t> corr_ixpos = {fReader, "corr.ixpos"};
  TTreeReaderValue<Int_t> corr_iypos = {fReader, "corr.iypos"};
  TTreeReaderValue<Int_t> corr_dxpos = {fReader, "corr.dxpos"};
  TTreeReaderValue<Int_t> corr_dypos = {fReader, "corr.dypos"};
  TTreeReaderValue<Int_t> corr_dixpos = {fReader, "corr.dixpos"};
  TTreeReaderValue<Int_t> corr_diypos = {fReader, "corr.diypos"};
  TTreeReaderValue<Double_t> corr_didychisq_single = {fReader, "corr.didychisq_single"};
  TTreeReaderValue<Double_t> corr_didyE1real_single = {fReader, "corr.didyE1real_single"};
  TTreeReaderValue<Double_t> corr_didyT1_single = {fReader, "corr.didyT1_single"};
  TTreeReaderValue<Double_t> corr_didyE1steepness_single = {fReader, "corr.didyE1steepness_single"};
  TTreeReaderValue<Double_t> corr_didyE1decay_single = {fReader, "corr.didyE1decay_single"};
  TTreeReaderValue<Double_t> corr_didychisq_double = {fReader, "corr.didychisq_double"};
  TTreeReaderValue<Double_t> corr_didyE1real_double = {fReader, "corr.didyE1real_double"};
  TTreeReaderValue<Double_t> corr_didyE1steepness_double = {fReader, "corr.didyE1steepness_double"};
  TTreeReaderValue<Double_t> corr_didyE1decay_double = {fReader, "corr.didyE1decay_double"};
  TTreeReaderValue<Double_t> corr_didyT1_double = {fReader, "corr.didyT1_double"};
  TTreeReaderValue<Double_t> corr_didyE2real_double = {fReader, "corr.didyE2real_double"};
  TTreeReaderValue<Double_t> corr_didyE2steepness_double = {fReader, "corr.didyE2steepness_double"};
  TTreeReaderValue<Double_t> corr_didyE2decay_double = {fReader, "corr.didyE2decay_double"};
  TTreeReaderValue<Double_t> corr_didyT2_double = {fReader, "corr.didyT2_double"};
  TTreeReaderValue<Double_t> corr_didytdiff = {fReader, "corr.didytdiff"};
  TTreeReaderValue<Double_t> corr_ddychisq_single = {fReader, "corr.ddychisq_single"};
  TTreeReaderValue<Double_t> corr_ddyE1real_single = {fReader, "corr.ddyE1real_single"};
  TTreeReaderValue<Double_t> corr_ddyT1_single = {fReader, "corr.ddyT1_single"};
  TTreeReaderValue<Double_t> corr_ddyE1steepness_single = {fReader, "corr.ddyE1steepness_single"};
  TTreeReaderValue<Double_t> corr_ddyE1decay_single = {fReader, "corr.ddyE1decay_single"};
  TTreeReaderValue<Double_t> corr_ddychisq_double = {fReader, "corr.ddychisq_double"};
  TTreeReaderValue<Double_t> corr_ddyE1real_double = {fReader, "corr.ddyE1real_double"};
  TTreeReaderValue<Double_t> corr_ddyE1steepness_double = {fReader, "corr.ddyE1steepness_double"};
  TTreeReaderValue<Double_t> corr_ddyE1decay_double = {fReader, "corr.ddyE1decay_double"};
  TTreeReaderValue<Double_t> corr_ddyT1_double = {fReader, "corr.ddyT1_double"};
  TTreeReaderValue<Double_t> corr_ddyE2real_double = {fReader, "corr.ddyE2real_double"};
  TTreeReaderValue<Double_t> corr_ddyE2steepness_double = {fReader, "corr.ddyE2steepness_double"};
  TTreeReaderValue<Double_t> corr_ddyE2decay_double = {fReader, "corr.ddyE2decay_double"};
  TTreeReaderValue<Double_t> corr_ddyT2_double = {fReader, "corr.ddyT2_double"};
  TTreeReaderValue<Double_t> corr_ddytdiff = {fReader, "corr.ddytdiff"};
  TTreeReaderValue<Int_t> corr_isdecay = {fReader, "corr.isdecay"};
  TTreeReaderValue<Int_t> corr_isimplant = {fReader, "corr.isimplant"};
  TTreeReaderValue<Int_t> corr_islightion = {fReader, "corr.islightion"};
  TTreeReaderValue<Int_t> corr_isdynovflnopins = {fReader, "corr.isdynovflnopins"};
  TTreeReaderValue<unsigned int> pspmt_fUniqueID = {fReader, "pspmt.fUniqueID"};
  TTreeReaderValue<unsigned int> pspmt_fBits = {fReader, "pspmt.fBits"};
  TTreeReaderValue<Double_t> pspmt_dyenergy = {fReader, "pspmt.dyenergy"};
  TTreeReaderArray<Double_t> pspmt_dyenergies = {fReader, "pspmt.dyenergies[10]"};
  TTreeReaderValue<Double_t> pspmt_dyecal = {fReader, "pspmt.dyecal"};
  TTreeReaderValue<Bool_t> pspmt_dyoverflow = {fReader, "pspmt.dyoverflow"};
  TTreeReaderValue<Int_t> pspmt_dymult = {fReader, "pspmt.dymult"};
  TTreeReaderValue<Double_t> pspmt_dytime = {fReader, "pspmt.dytime"};
  TTreeReaderArray<Double_t> pspmt_dytimes = {fReader, "pspmt.dytimes[10]"};
  TTreeReaderValue<Double_t> pspmt_dytimecfd = {fReader, "pspmt.dytimecfd"};
  TTreeReaderValue<Int_t> pspmt_dycfdfailbit = {fReader, "pspmt.dycfdfailbit"};
  TTreeReaderValue<Double_t> pspmt_dychisq_single = {fReader, "pspmt.dychisq_single"};
  TTreeReaderValue<Double_t> pspmt_dyE1_single = {fReader, "pspmt.dyE1_single"};
  TTreeReaderValue<Double_t> pspmt_dyE1real_single = {fReader, "pspmt.dyE1real_single"};
  TTreeReaderValue<Double_t> pspmt_dyT1_single = {fReader, "pspmt.dyT1_single"};
  TTreeReaderValue<Double_t> pspmt_dyoffset_single = {fReader, "pspmt.dyoffset_single"};
  TTreeReaderValue<Double_t> pspmt_dyE1steepness_single = {fReader, "pspmt.dyE1steepness_single"};
  TTreeReaderValue<Double_t> pspmt_dyE1decay_single = {fReader, "pspmt.dyE1decay_single"};
  TTreeReaderValue<Double_t> pspmt_dychisq_double = {fReader, "pspmt.dychisq_double"};
  TTreeReaderValue<Double_t> pspmt_dyE1_double = {fReader, "pspmt.dyE1_double"};
  TTreeReaderValue<Double_t> pspmt_dyE1real_double = {fReader, "pspmt.dyE1real_double"};
  TTreeReaderValue<Double_t> pspmt_dyT1_double = {fReader, "pspmt.dyT1_double"};
  TTreeReaderValue<Double_t> pspmt_dyE1steepness_double = {fReader, "pspmt.dyE1steepness_double"};
  TTreeReaderValue<Double_t> pspmt_dyE1decay_double = {fReader, "pspmt.dyE1decay_double"};
  TTreeReaderValue<Double_t> pspmt_dyE2_double = {fReader, "pspmt.dyE2_double"};
  TTreeReaderValue<Double_t> pspmt_dyE2real_double = {fReader, "pspmt.dyE2real_double"};
  TTreeReaderValue<Double_t> pspmt_dyT2_double = {fReader, "pspmt.dyT2_double"};
  TTreeReaderValue<Double_t> pspmt_dyE2steepness_double = {fReader, "pspmt.dyE2steepness_double"};
  TTreeReaderValue<Double_t> pspmt_dyE2decay_double = {fReader, "pspmt.dyE2decay_double"};
  TTreeReaderValue<Double_t> pspmt_dyoffset_double = {fReader, "pspmt.dyoffset_double"};
  TTreeReaderValue<Double_t> pspmt_dytdiffE1E2 = {fReader, "pspmt.dytdiffE1E2"};
  TTreeReaderArray<Int_t> pspmt_ahit = {fReader, "pspmt.ahit[256]"};
  TTreeReaderArray<Double_t> pspmt_aenergy = {fReader, "pspmt.aenergy[256]"};
  TTreeReaderArray<Double_t> pspmt_aecal = {fReader, "pspmt.aecal[256]"};
  TTreeReaderValue<Double_t> pspmt_asum = {fReader, "pspmt.asum"};
  TTreeReaderArray<Double_t> pspmt_atime = {fReader, "pspmt.atime[256]"};
  TTreeReaderArray<Double_t> pspmt_atimecal = {fReader, "pspmt.atimecal[256]"};
  TTreeReaderArray<Int_t> pspmt_aoverflow = {fReader, "pspmt.aoverflow[256]"};
  TTreeReaderValue<Int_t> pspmt_aoverflowcount = {fReader, "pspmt.aoverflowcount"};
  TTreeReaderValue<Int_t> pspmt_amultraw = {fReader, "pspmt.amultraw"};
  TTreeReaderValue<Int_t> pspmt_amult = {fReader, "pspmt.amult"};
  TTreeReaderArray<Int_t> pspmt_pixelmult = {fReader, "pspmt.pixelmult[256]"};
  TTreeReaderValue<Double_t> pspmt_lxamp = {fReader, "pspmt.lxamp"};
  TTreeReaderValue<Double_t> pspmt_lxpos = {fReader, "pspmt.lxpos"};
  TTreeReaderValue<Double_t> pspmt_lxgamma = {fReader, "pspmt.lxgamma"};
  TTreeReaderValue<Double_t> pspmt_lyamp = {fReader, "pspmt.lyamp"};
  TTreeReaderValue<Double_t> pspmt_lypos = {fReader, "pspmt.lypos"};
  TTreeReaderValue<Double_t> pspmt_lygamma = {fReader, "pspmt.lygamma"};
  TTreeReaderValue<unsigned int> clover_fUniqueID = {fReader, "clover.fUniqueID"};
  TTreeReaderValue<unsigned int> clover_fBits = {fReader, "clover.fBits"};
  TTreeReaderArray<Double_t> clover_time = {fReader, "clover.time[64]"};
  TTreeReaderArray<Double_t> clover_timecal = {fReader, "clover.timecal[64]"};
  TTreeReaderArray<Double_t> clover_energy = {fReader, "clover.energy[64]"};
  TTreeReaderArray<Double_t> clover_ecal = {fReader, "clover.ecal[64]"};
  TTreeReaderArray<Double_t> clover_ecalab = {fReader, "clover.ecalab[16]"};
  TTreeReaderValue<Int_t> clover_mult = {fReader, "clover.mult"};
  TTreeReaderValue<unsigned int> sega_fUniqueID = {fReader, "sega.fUniqueID"};
  TTreeReaderValue<unsigned int> sega_fBits = {fReader, "sega.fBits"};
  TTreeReaderArray<Double_t> sega_time = {fReader, "sega.time[16]"};
  TTreeReaderArray<Double_t> sega_energy = {fReader, "sega.energy[16]"};
  TTreeReaderArray<Double_t> sega_ecal = {fReader, "sega.ecal[16]"};
  TTreeReaderValue<Int_t> sega_mult = {fReader, "sega.mult"};
  TTreeReaderValue<unsigned int> labr3_fUniqueID = {fReader, "labr3.fUniqueID"};
  TTreeReaderValue<unsigned int> labr3_fBits = {fReader, "labr3.fBits"};
  TTreeReaderArray<Double_t> labr3_energy = {fReader, "labr3.energy[15]"};
  TTreeReaderArray<Double_t> labr3_ecal = {fReader, "labr3.ecal[15]"};
  TTreeReaderArray<Double_t> labr3_time = {fReader, "labr3.time[15]"};
  TTreeReaderArray<Double_t> labr3_timecal = {fReader, "labr3.timecal[15]"};
  TTreeReaderValue<Int_t> labr3_mult = {fReader, "labr3.mult"};
  TTreeReaderArray<Double_t> labr3_timecfd = {fReader, "labr3.timecfd[15]"};
  TTreeReaderArray<Int_t> labr3_cfdfailbit = {fReader, "labr3.cfdfailbit[15]"};
  TTreeReaderValue<unsigned int> ddasdiagnostics_fUniqueID = {fReader, "ddasdiagnostics.fUniqueID"};
  TTreeReaderValue<unsigned int> ddasdiagnostics_fBits = {fReader, "ddasdiagnostics.fBits"};
  TTreeReaderValue<Int_t> ddasdiagnostics_cmult = {fReader, "ddasdiagnostics.cmult"};
  TTreeReaderValue<Int_t> ddasdiagnostics_eventlength = {fReader, "ddasdiagnostics.eventlength"};
  TTreeReaderValue<Int_t> ddasdiagnostics_tdiffevent = {fReader, "ddasdiagnostics.tdiffevent"};
  TTreeReaderArray<Int_t> ddasdiagnostics_adchit = {fReader, "ddasdiagnostics.adchit[20]"};
  TTreeReaderArray<Bool_t> ddasdiagnostics_overflow = {fReader, "ddasdiagnostics.overflow[16]"};
  TTreeReaderArray<Int_t> ddasdiagnostics_finishcode = {fReader, "ddasdiagnostics.finishcode[16]"};
  TTreeReaderArray<Int_t> ddasdiagnostics_chanhit = {fReader, "ddasdiagnostics.chanhit[416]"};


  isomer(TTree * /*tree*/ =0) { }
  virtual ~isomer() { }
  virtual Int_t   Version() const { return 2; }
  virtual void    Begin(TTree *tree);
  virtual void    SlaveBegin(TTree *tree);
  virtual void    Init(TTree *tree);
  virtual Bool_t  Notify();
  virtual Bool_t  Process(Long64_t entry);
  virtual Int_t   GetEntry(Long64_t entry, Int_t getall = 0) { return fChain ? fChain->GetTree()->GetEntry(entry, getall) : 0; }
  virtual void    SetOption(const char *option) { fOption = option; }
  virtual void    SetObject(TObject *obj) { fObject = obj; }
  virtual void    SetInputList(TList *input) { fInput = input; }
  virtual TList  *GetOutputList() const { return fOutput; }
  virtual void    SlaveTerminate();
  virtual void    Terminate();

  //Checking that histograms are actually written to file
  void OutputObject(const char *objname) { //
    if(fOutput->FindObject(objname)) {
      fOutput->FindObject(objname)->Write();
    } else {
      printf("CANNOT FIND THIS FILE! \n");
    }
  }

  ClassDef(isomer,0);

};

#endif

#ifdef isomer_cxx
void isomer::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the reader is initialized.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   fReader.SetTree(tree);
}

Bool_t isomer::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}


#endif // #ifdef isomer_cxx
