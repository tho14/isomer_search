#include <cmath>

#include "TParameters.h"

using std::fill;
using std::begin;
using std::end;

ClassImp(TClock);
ClassImp(TTrace);
ClassImp(TDDASTime);
ClassImp(TADC);
ClassImp(TTAC);
ClassImp(TPIN);
ClassImp(TDSSD);
ClassImp(TSSSD);
ClassImp(TPSPMT);
ClassImp(TPID);
ClassImp(TCorrelatorData);
ClassImp(TClover);
ClassImp(TSeGA);
ClassImp(TLaBr3);
ClassImp(TDDASDiagnostics);
ClassImp(TBetaDecayParams);
ClassImp(TROOTDataOut);

// Clock
void TClock::Reset()
{
  time = 0.;
  current = 0.;
  initial = 0.;
}

// Trace
void TTrace::Reset()
{
  trace.clear();
}

// TAC
void TTAC::Reset()
{
   pin01rf = 0.;
   pin01xfp = 0.;
   pin02xfp = 0.;
   pin01i2n = 0.;
   pin01i2s = 0.;
   pin02i2n = 0.;
   pin02i2s = 0.;
   i2ni2s = 0.;
   pin01xfptime = 0.;
   pin02xfptime = 0.;
   pin01i2ntime = 0.;
   pin01i2stime = 0.;
   pin02i2ntime = 0.;
   pin02i2stime = 0.;
   i2ni2stime = 0.;
   pin01rftime = 0.;
}

// ADC
void TADC::Reset()
{
  fill(begin(channel),end(channel),0);
  for(int i=0; i<nchannels; i++)
    {
      chantrace[i].Reset();
    }
}

// DDAS time
void TDDASTime::Reset()
{
  fill(begin(timefull),end(timefull),0);
  fill(begin(timecfd),end(timecfd),0);

  fill(begin(cfdfail),end(cfdfail),0);
}

// PIN
void TPIN::Reset()
{
  energy = 0.;
  time = 0.;
  ecal = 0.;
}

// DSSD
void TDSSD::Reset()
{
  fill(begin(hitime),end(hitime),0.);
  fill(begin(hienergy),end(hienergy),0.);
  fill(begin(hiecal),end(hiecal),0.);
  fill(begin(lotime),end(lotime),0.);
  fill(begin(loenergy),end(loenergy),0.);
  fill(begin(loecal),end(loecal),0.);

  imax = 0.;
  imaxtime = 0.;
  dmax = 0.;
  dmaxtime = 0.;
  imaxch = 100;
  dmaxch = 100;
  imult = 0;
  dmult = 0; 
}

// SSSD
void TSSSD::Reset()
{
  fill(begin(hitime),end(hitime),0.);
  fill(begin(hienergy),end(hienergy),0.);
  fill(begin(hiecal),end(hiecal),0.);
  fill(begin(lotime),end(lotime),0.);
  fill(begin(loenergy),end(loenergy),0.);
  fill(begin(loecal),end(loecal),0.);

  himax = 0.;
  himaxtime = 0.;
  lomax = 0.;
  lomaxtime = 0.;
  himaxch = 100;
  lomaxch = 100;
  himult = 0;
  lomult = 0; 
}

void TPSPMT::Reset()
{
  // Dynode
  dyenergy = 0.;
  fill(begin(dyenergies),end(dyenergies),0.);
  dyecal = 0.;
  dyoverflow = false;
  dymult = 0;
  dytime = 0.;
  fill(begin(dytimes),end(dytimes),0.);

  //TO Extracting CFD information to use as conditions for analysis
  dytimecfd = 0;
  dycfdfailbit = 0;
  
  // Fit information
  // Single pulse
  dychisq_single = 0.;
  dyE1_single = 0.;
  dyE1real_single = 0.;
  dyT1_single = 0.;
  dyoffset_single = 0.;
  dyE1steepness_single = 0.; 
  dyE1decay_single = 0.;
  // Double pulse
  dychisq_double = 0.;
  dyE1_double = 0.;
  dyE1real_double = 0.;
  dyT1_double = 0.;
  dyE1steepness_double = 0.;
  dyE1decay_double = 0.;
  dyE2_double = 0.;
  dyE2real_double = 0.;
  dyT2_double = 0.;
  dyE2steepness_double = 0.;
  dyE2decay_double = 0.;
  dyoffset_double = 0.;
  dytdiffE1E2 = 0.;

  // Anode
  fill(begin(ahit),end(ahit),0);
  fill(begin(aenergy),end(aenergy),0.);
  fill(begin(aecal),end(aecal),0.);
  asum = 0.;
  fill(begin(atime),end(atime),0);
  fill(begin(atimecal),end(atimecal),0);
  fill(begin(aoverflow),end(aoverflow),0);
  aoverflowcount = 0;  
  amultraw = 0;
  amult = 0;
  fill(begin(pixelmult),end(pixelmult),0);
  // Anode linearlized position fit parameters
  lxamp = 0.;   // Amplitude of 1D fit
  lxpos = -1.;  // x-position from 2D fit
  lxgamma = 0.; // Lorentzian sigma in x
  lyamp = 0.;   // Amplitude of 1D fit
  lypos = -1.;  // y-position from 2D fit
  lygamma = 0.; // Lorentzian sigma in y
}

// PID
void TPID::Reset()
{
  de1 = 0.;
  de2 = 0.;
  pin01i2n = 0.;
  pin01i2s = 0.;
  pin02i2n = 0.;
  pin02i2s = 0.;
  pin01xfp = 0.;
  pin02xfp = 0.;
  pin01rf = 0.;
}

// Correlator
void TCorrelatorData::Reset()
{
  dtimplant = 0.;
  //itime = 0.; //we actually don't care...
  ide1 = 0.;
  ide2 = 0.;
  itofpin01i2n = 0.; 
  itofpin01i2s = 0.;
  itofpin02i2n = 0.;
  itofpin02i2s = 0.;
  itofpin01xfp = 0.;
  itofpin02xfp = 0.;
  itofpin01rf = 0.;
  dtimecal = 0.;
  dtime = 0.;
  dfrontch = 100;
  dbackch = 100;
  ifrontch = 100;
  ibackch = 100;
  ddfrontch = 100;
  ddbackch = 100;
  dfrontmax = 0.;
  dbackmax = 0.;
  dde1 = 0.;
  dde2 = 0.;
  dtofpin01i2n = 0.; 
  dtofpin01i2s = 0.;
  dtofpin02i2n = 0.;
  dtofpin02i2s = 0.;
  dtofpin01xfp = 0.;
  dtofpin02xfp = 0.;
  dtofpin01rf = 0.;
  flag = 0; // unset flag
  mult = 0;
  dmult = 0;
  dnumcorr = 0;

  //Isomer hunting!!!!
  //Trying something new
  // sega_implant_tdiff = new double**[nsega];
  // for(int i = 0; i < nsega; i++){
  //   sega_implant_tdiff[i] = new double*[npspmt];
  //   for(int j = 0; j < npspmt; j++){
  //     sega_implant_tdiff[i][j] = new double[npspmt];
  //     for(int k = 0; k < npspmt; k++){
  // 	sega_implant_tdiff[i][j][k] = 0;
  //     }
  //   }
  // }

  // sega_implant_idE1 = new double**[nsega];
  // for(int i = 0; i < nsega; i++){
  //   sega_implant_idE1[i] = new double*[npspmt];
  //   for(int j = 0; j < npspmt; j++){
  //     sega_implant_idE1[i][j] = new double[npspmt];
  //     for(int k = 0; k < npspmt; k++){
  // 	sega_implant_idE1[i][j][k] = 0;
  //     }
  //   }
  // }

  // sega_implant_itofpin01i2s = new double**[nsega];
  // for(int i = 0; i < nsega; i++){
  //   sega_implant_itofpin01i2s[i] = new double*[npspmt];
  //   for(int j = 0; j < npspmt; j++){
  //     sega_implant_itofpin01i2s[i][j] = new double[npspmt];
  //     for(int k = 0; k < npspmt; k++){
  // 	sega_implant_itofpin01i2s[i][j][k] = 0;
  //     }
  //   }
  // }
  
  //SeGAs
  for(int i = 0; i < nsega; i++){
    for(int j = 0; j < npspmt; j++){
      for(int k = 0; k < npspmt; k++){
  	sega_implant_tdiff[i][j][k] = {0};
  	sega_implant_idE1[i][j][k] =  {0};
  	sega_implant_itofpin01i2s[i][j][k] =  {0};
      }
    }
  }
  //LaBr3s
  for(int i = 0; i < nlabr3; i++){
    for(int j = 0; j < npspmt; j++){
      for(int k = 0; k < npspmt; k++){
  	labr3_implant_tdiff[i][j][k] =  {0};
    	labr3_implant_idE1[i][j][k] =  {0};
  	labr3_implant_itofpin01i2s[i][j][k] =  {0};
      }
    }
  }

  // PSPMT specific stuff
  // -- ASC 3/20/2020
  //
  // Dynode
  idyecal = 0.;
  ddyecal = 0.;
  didyecal = 0.;
  // Anode implant positions
  // --ASC 9/6/2019
  ixpos = -1;
  iypos = -1;
  dxpos = -1;
  dypos = -1;
  dixpos = -1; // For correlated events
  diypos = -1; // For correlated events
  // Correlated ion dynode fit information
  didychisq_single = 0.;
  didyE1real_single = 0.;
  didyT1_single = 0.;
  didyE1steepness_single = 0.;
  didyE1decay_single = 0.;
  didychisq_double = 0.;
  didyE1real_double = 0.;
  didyE1steepness_double = 0.;
  didyE1decay_double = 0.;
  didyT1_double = 0.;
  didyE2real_double = 0.;
  didyE2steepness_double = 0.;
  didyE2decay_double = 0.;
  didyT2_double = 0.;
  didytdiff = 0.;
  // Correlated decay dynode fit information
  ddychisq_single = 0.;
  ddyE1real_single = 0.;
  ddyT1_single = 0.;
  ddyE1steepness_single = 0.;
  ddyE1decay_single = 0.;
  ddychisq_double = 0.;
  ddyE1real_double = 0.;
  ddyE1steepness_double = 0.;
  ddyE1decay_double = 0.;
  ddyT1_double = 0.;
  ddyE2real_double = 0.;
  ddyE2steepness_double = 0.;
  ddyE2decay_double = 0.;
  ddyT2_double = 0.;
  ddytdiff = 0.;

  // Event type idenfiers
  isimplant = 0;
  isdecay = 0;  
  islightion = 0;
  isdynovflnopins = 0;
  // issegaonly = 0;
  // islabr3only = 0;
  // isgammaonly = 0;

}

// Clover array
void TClover::Reset()
{
  fill(begin(time),end(time),0.);
  fill(begin(timecal),end(timecal),0.);
  fill(begin(energy),end(energy),0.);
  fill(begin(ecal),end(ecal),0.);
  fill(begin(ecalab),end(ecalab),0.);
  mult = 0;
}

// SeGA
void TSeGA::Reset()
{
  fill(begin(time),end(time),0.);
  fill(begin(energy),end(energy),0.);
  fill(begin(ecal),end(ecal),0.);
  mult = 0;
}

// LaBr3
void TLaBr3::Reset()
{
  fill(begin(time),end(time),0.);
  fill(begin(timecal),end(timecal),0);
  fill(begin(energy),end(energy),0.);
  fill(begin(ecal),end(ecal),0.);
  mult = 0;

  //TO Extracting CFD information to use as conditions for analysis
  fill(begin(timecfd),end(timecfd),0.);
  fill(begin(cfdfailbit),end(cfdfailbit),0);

  //TO Checking times following timewalk alignment (leading edge aligned with cfd responses)
  // fill(begin(timealigned),end(timealigned),0.);
  // fill(begin(timealignedcal),end(timealignedcal),0.);
 
}

// DDAS diagnostics
void TDDASDiagnostics::Reset()
{
  cmult = 0;
  eventlength = 0;
  tdiffevent = 0;
  fill(begin(adchit),end(adchit),0);
  fill(begin(overflow),end(overflow),false);
  fill(begin(finishcode),end(finishcode),0);
  fill(begin(chanhit),end(chanhit),0);
}

// The beta decay parameters
void TBetaDecayParams::Reset()
{
  clock.Reset();
  tac.Reset();
  for(int i=0; i<nadcs; i++)
    {
      adc[i].Reset();
      time[i].Reset();
    }
  pin01.Reset();
  pin02.Reset();
  pin03.Reset();
  front.Reset();
  back.Reset();
  sssd01.Reset();
  pid.Reset();
  corr.Reset();
  pspmt.Reset();
  clover.Reset();
  sega.Reset();
  labr3.Reset();
  ddasdiagnostics.Reset();
}

// ROOT output
void TROOTDataOut::Reset()
{
  clock.Reset();
  tac.Reset();
  pin01.Reset();
  pin02.Reset();
  pin03.Reset();
  pid.Reset();
  corr.Reset();
  pspmt.Reset();
  clover.Reset();
  sega.Reset();
  labr3.Reset();
  ddasdiagnostics.Reset();
}

void TROOTDataOut::SetOutputValues(TBetaDecayParams bdecay) {
  clock = bdecay.clock;
  tac = bdecay.tac;
  pin01 = bdecay.pin01;
  pin02 = bdecay.pin02;
  pin03 = bdecay.pin03;
  pid = bdecay.pid;
  corr = bdecay.corr;
  clover = bdecay.clover;
  sega = bdecay.sega;
  pspmt = bdecay.pspmt;
  labr3 = bdecay.labr3;
  ddasdiagnostics = bdecay.ddasdiagnostics;
}
