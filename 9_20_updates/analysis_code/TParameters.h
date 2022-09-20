#ifndef __TPARAMETERS_H
#define __TPARAMETERS_H

#include <vector>

#include "TObject.h"

#include "Constants.h"

// Clock
class TClock: public TObject
{
 private:

 public:
  double  time;
  double  current;
  double  initial;

 public: 
  void Reset();

  ClassDef(TClock,2);
};

// Trace capture 
class TTrace : public TObject
{
 private:

 public:
  std::vector<UShort_t> trace;

 public:
  void Reset();
  
  ClassDef(TTrace,2);
};

// TACs 
class TTAC: public TObject
{
 private:
  
 public:
  double pin01rf;
  double pin01xfp;
  double pin02xfp;
  double pin01i2n;
  double pin01i2s;
  double pin02i2n;
  double pin02i2s;
  double i2ni2s;
  double pin01xfptime;
  double pin02xfptime;
  double pin01i2ntime;
  double pin01i2stime;
  double pin02i2ntime;
  double pin02i2stime;
  double i2ni2stime;
  double pin01rftime;

 public:
  void Reset();

  ClassDef(TTAC,2);
};

// ADCs 
class TADC: public TObject
{
 private:
  
 public:
  int channel[nchannels];
  TTrace chantrace[nchannels];  

 public:
  void Reset();
  
  ClassDef(TADC,2);
};

// Times 
class TDDASTime: public TObject
{
 private:

 public:
  double timefull[nchannels];
  double timecfd[nchannels];

  //cfdfailbit
  int cfdfail[nchannels];
  
  
 public:
  void Reset();

  ClassDef(TDDASTime,2);
};

// PIN 
class TPIN: public TObject
{
 private:
  
 public:
  double energy;
  double ecal;
  double time;
  
 public:
  void Reset();

  ClassDef(TPIN,2);
};

// DSSD 
class TDSSD: public TObject
{
 private:
  
 public:
  double hienergy[ndssd];
  double hiecal[ndssd];
  double hitime[ndssd];
  double loenergy[ndssd];
  double loecal[ndssd];
  double lotime[ndssd];
    
  double imax;
  double imaxtime;
  double dmax;
  double dmaxtime;
  int imaxch;
  int dmaxch;
  int imult;
  int dmult;

 public:
 void Reset();

  ClassDef(TDSSD,2);
};

// SSSD 
class TSSSD: public TObject
{
 private:
  
 public:
  double hienergy[nsssd];
  double hiecal[nsssd];
  double hitime[nsssd];
  double loenergy[nsssd];
  double loecal[nsssd];
  double lotime[nsssd];
  
  double himax;
  double himaxtime;
  double lomax;
  double lomaxtime;
  int himaxch;
  int lomaxch;
  int himult;
  int lomult;
  
 public:
  void Reset();

  ClassDef(TSSSD,2);
};

// PSPMT 
class TPSPMT: public TObject
{
 private:

 public:
  // Dynode
  double dyenergy;
  double dyenergies[10];
  double dyecal;
  bool dyoverflow;
  int dymult;
  double dytime;
  double dytimes[10];

  //TO Extracting CFD information to use as conditions for analysis
  double dytimecfd;
  int dycfdfailbit;
  
  // Fit information
  // Single pulse
  double dychisq_single;
  double dyE1_single;
  double dyE1real_single;
  double dyT1_single;
  double dyoffset_single;
  double dyE1steepness_single; 
  double dyE1decay_single;
  // Double pulse
  double dychisq_double;
  double dyE1_double;
  double dyE1real_double;
  double dyT1_double;
  double dyE1steepness_double;
  double dyE1decay_double;
  double dyE2_double;
  double dyE2real_double;
  double dyT2_double;
  double dyE2steepness_double;
  double dyE2decay_double;
  double dyoffset_double;
  double dytdiffE1E2;

  // Anode
  int ahit[nanodes];
  double aenergy[nanodes];
  double aecal[nanodes];
  double asum;
  double atime[nanodes];
  double atimecal[nanodes];
  int aoverflow[nanodes];
  int aoverflowcount;  
  int amultraw;
  int amult;
  int pixelmult[nanodes];
  // Anode linearlized position fit parameters
  double lxamp;   // Amplitude of 1D fit
  double lxpos;   // x-position from 2D fit
  double lxgamma; // Lorentzian gamma in x
  double lyamp;   // Amplitude of 1D fit
  double lypos;   // y-position from 2D fit
  double lygamma; // Lorentzian gamma in y
 
 public:
  void Reset();

  ClassDef(TPSPMT,2);
};

// PID 
class TPID: public TObject
{
 private:
  
 public:
  
  double de1;
  double de2;
  double pin01i2n;
  double pin01i2s;
  double pin02i2n;
  double pin02i2s;
  double pin01xfp;
  double pin02xfp;
  double pin01rf;
  
 public:
  void Reset();

  ClassDef(TPID,2);
};

// Correlator 
class TCorrelatorData: public TObject
{
 private:
  
 public:
  double dtimplant;
  double itime;
  //TO 09/12/22
  double gtime;
  double gde1;
  double gtofpin01i2s;
  //
  double ide1;
  double ide2;
  double itofpin01i2n; // tac tof from pin01i2n
  double itofpin01i2s; // tac tof from pin01i2s
  double itofpin02i2n; // tac tof from pin02i2n
  double itofpin02i2s; // tac tof from pin02i2s
  double itofpin01xfp; // tac tof from pin01xfp
  double itofpin02xfp; // tac tof from pin02xfp
  double itofpin01rf;  // tac tof from pin01rf
  double dtimecal;
  double dtime;
  int ddfrontch; // All decay positions
  int ddbackch;
  int ifrontch; // All implant positions
  int ibackch;
  int dfrontch; // Decays correlated with implant positions
  int dbackch;
  double dfrontmax; // Correlated front strip max ecal
  double dbackmax; // Correlated back strip max ecal
  double dde1; // corrected PIN1 dE of implant correlated to decay
  double dde2; // corrected PIN2 dE of implant correlated to decay
  double dtofpin01i2n; // tac tof from pin01i2n
  double dtofpin01i2s; // tac tof from pin01i2s
  double dtofpin02i2n; // tac tof from pin02i2n
  double dtofpin02i2s; // tac tof from pin02i2s
  double dtofpin01xfp; // tac tof from pin01xfp
  double dtofpin02xfp; // tac tof from pin02xfp
  double dtofpin01rf;  // tac tof from pin01rf
  int flag;
  int mult;
  int dmult;
  int dnumcorr;

  //Isomer hunting
  //SeGAs
  //Position dependent
  double sde1;
  double stofpin01i2s;
  double sega_implant_time;
  double sega_implant_dt;
  double sega_dynode_tdiff[nsega][npspmt][npspmt];
  double sega_dynode_idE1[nsega][npspmt][npspmt];
  double sega_dynode_itofpin01i2s[nsega][npspmt][npspmt];
  //Position independent
  /* double sega_implant_tdiff[nsega]; */
  /* double sega_implant_idE1[nsega]; */
  /* double sega_implant_itofpin01i2s[nsega]; */
  
  //LaBr3s
  //Position dependent
  double lde1;
  double ltofpin01i2s;
  double labr3_implant_time;
  double labr3_implant_dt;
  double labr3_dynode_tdiff[nlabr3][npspmt][npspmt];
  double labr3_dynode_idE1[nlabr3][npspmt][npspmt];
  double labr3_dynode_itofpin01i2s[nlabr3][npspmt][npspmt];
  // Position independent
  /* double labr3_implant_tdiff[nlabr3]; */
  /* double labr3_implant_idE1[nlabr3]; */
  /* double labr3_implant_itofpin01i2s[nlabr3]; */

  // PSPMT specific stuff
  // -- ASC 3/20/2020
  //
  // Dynode
  double idyecal;
  double ddyecal;
  double didyecal;
  // Anode implant positions
  // --ASC 9/6/2019
  int ixpos;
  int iypos;
  int dxpos;
  int dypos;
  int dixpos; // For correlated events
  int diypos; // For correlated events
  // Correlated ion dynode fit information
  double didychisq_single;
  double didyE1real_single;
  double didyT1_single;
  double didyE1steepness_single;
  double didyE1decay_single;
  double didychisq_double;
  double didyE1real_double;
  double didyE1steepness_double;
  double didyE1decay_double;
  double didyT1_double;
  double didyE2real_double;
  double didyE2steepness_double;
  double didyE2decay_double;
  double didyT2_double;
  double didytdiff;
  // Correlated decay dynode fit information
  double ddychisq_single;
  double ddyE1real_single;
  double ddyT1_single;
  double ddyE1steepness_single;
  double ddyE1decay_single;
  double ddychisq_double;
  double ddyE1real_double;
  double ddyE1steepness_double;
  double ddyE1decay_double;
  double ddyT1_double;
  double ddyE2real_double;
  double ddyE2steepness_double;
  double ddyE2decay_double;
  double ddyT2_double;
  double ddytdiff;
  
  // Event type identifiers
  int isdecay;
  int isimplant;
  int isimplantisomer;
  int islightion;
  int isdynovflnopins;
  /* int issegaonly; */
  /* int islabr3only; */
  /* int isgammaonly; */
  
 public:
  void Reset();

  ClassDef(TCorrelatorData,2);
};

// Clover 
class TClover: public TObject
{
 private:
  
 public:
  double time[totcrystals];
  double timecal[totcrystals];
  double energy[totcrystals];
  double ecal[totcrystals];
  double ecalab[nclovers]; // simple add back
  int mult;
  
 public:
  void Reset();

  ClassDef(TClover,2);
};

// SeGA 
class TSeGA: public TObject
{
 private:
  
 public:
  double time[nsega];
  double energy[nsega];
  double ecal[nsega];
  int mult;

  //TO Extracting CFD information to use as conditions for analysis
  double timecfd[nsega];
  int cfdfailbit[nsega];
  
 public:
  void Reset();

  ClassDef(TSeGA,2);
};

class TLaBr3: public TObject
{
 private:
  
 public:
  double energy[nlabr3];
  double ecal[nlabr3];
  double time[nlabr3];
  double timecal[nlabr3];
  int mult;

  //TO Extracting CFD information to use as conditions for analysis
  double timecfd[nlabr3];
  int cfdfailbit[nlabr3];

  //TO Corrected time to align time response over dynode range to 1000 (after first aligning low and high energy response)
  //Didn't use this in the end...
  //double timealigned[nlabr3];
  //double timealignedcal[nlabr3];
  
 public:
  void Reset();

  ClassDef(TLaBr3,2);
};

// DDAS diagnostics 
class TDDASDiagnostics: public TObject
{
 private:

 public:
  int cmult;
  int eventlength;
  int tdiffevent;
  int adchit[nadcs]; 
  bool overflow[nchannels];
  int finishcode[nchannels];
  int chanhit[ncrates*totchan]; // global channel hit pattern

   public:
  void Reset();
  
  ClassDef(TDDASDiagnostics,2);
};

// The entire beta-decay set-up 
class TBetaDecayParams: public TObject
{
 public:
  TClock clock;
  TDDASTime time[nadcs]; 
  TADC adc[nadcs];
  TTAC tac;
  TPIN pin01;
  TPIN pin02;
  TPIN pin03;
  TDSSD front;
  TDSSD back;
  TSSSD sssd01;
  TPID pid;
  TCorrelatorData corr;
  TPSPMT pspmt;
  TClover clover;
  TSeGA sega;
  TLaBr3 labr3;
  TDDASDiagnostics ddasdiagnostics;

 public:
  void Reset();

  ClassDef(TBetaDecayParams,2);
};

// Class data written to the output file
class TROOTDataOut : public TObject
{
 public:
  TClock clock;
  TTAC tac;
  TPIN pin01;
  TPIN pin02;
  TPIN pin03;
  TPID pid;
  TCorrelatorData corr;
  TPSPMT pspmt;
  TClover clover;
  TSeGA sega;
  TLaBr3 labr3;
  TDDASDiagnostics ddasdiagnostics;

  
 public:
  void Reset();
  void SetOutputValues(TBetaDecayParams bdecay);

  ClassDef(TROOTDataOut,2);
};
#endif
