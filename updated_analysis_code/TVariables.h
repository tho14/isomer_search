#ifndef __TVARIABLES_H
#define __TVARIABLES_H

#include <string>

#include "Constants.h"

// Clock Calibrator
class TClockCalibrator
{  
 public:
  double calib;
  double scale;
  double max;
  double max_daughter;
  
 public:
  void Initialize();
};

// PIN Calibrator
class TPINCalibrator 
{
 public:
  double slope;
  double intercept;
  double thresh;
  double uld;

 public:
  void Initialize();
};

// TAC Calibrator
class TTACCalibrator
{
 public:
  double pin01i2ncorr;
  double pin01i2noffset;
  double pin01i2scorr;
  double pin01i2soffset;
  
  double pin02i2ncorr;
  double pin02i2noffset;
  double pin02i2scorr;
  double pin02i2soffset;
  
  double pin01xfpcorr;
  double pin02xfpcorr;
  
  double pin01rfcorr;
  
  double i2ni2scorr;
  double thresh;
  double uld;

 public:
  void Initialize();
};

// DSSD Calibrator
class TDSSDCalibrator
{
 public:
  double hioffset[ndssd];
  double hislope[ndssd];
  double looffset[ndssd];
  double loslope[ndssd];
  double hithresh[ndssd];
  double hiuld[ndssd];
  double lothresh[ndssd];
  double lould[ndssd];

 public:
  void Initialize();
};

// SSSD Calibrator
class TSSSDCalibrator
{
 public:
  double hioffset[nsssd];
  double hislope[nsssd];
  double looffset[nsssd];
  double loslope[nsssd];
  double hithresh[nsssd];
  double hiuld[nsssd];
  double lothresh[nsssd];
  double lould[nsssd];

 public:
  void Initialize();
};

// PSPMT Calibrator
class TPSPMTCalibrator
{
 public:
  // Anodes
  double aoffset[nanodes];
  double aslope[nanodes];
  double athresh[nanodes];
  double auld[nanodes];
  double tdiffdynode[nanodes];
  // Dynode
  double dyoffset;
  double dyslope;
  double dythresh;
  double dyuld;

  //TO Timewalk correction parameters due to low dynode energies for individual labr3-dynode pairs (For alignment to high energy time response centroids) - Parameters from fitting a the timewalk profile with a 4th order polynomial
  /* double dytwalkp0[nlabr3]; */
  /* double dytwalkp1[nlabr3]; */
  /* double dytwalkp2[nlabr3]; */
  /* double dytwalkp3[nlabr3]; */
  /* double dytwalkp4[nlabr3]; */
  //TO High energy time response centroids for individual labr3-dynode pairs
  double dyhighcentroid[nlabr3];
  double dylowcentroid[nlabr3];
  //Full Time Alignment - Still dependent on dynode energy
  /* double p0[nlabr3]; */
  /* double p1[nlabr3]; */
  /* double p2[nlabr3]; */
  /* double p3[nlabr3]; */
  /* double p4[nlabr3]; */
  /* double p5[nlabr3]; */
  /* double p6[nlabr3]; */
  /* double p7[nlabr3]; */
  /* double p8[nlabr3]; */
  /* double p9[nlabr3]; */
  /* double p10[nlabr3]; */
  /* double p11[nlabr3]; */
  /* double p12[nlabr3]; */
  /* double p13[nlabr3]; */

 public:
  void Initialize();
};

// Hit flags
class THitFlag 
{
 public:
  int pin01;
  int pin02;
  int pin03;
  int fronthi;
  int frontlo;
  int backhi;
  int backlo;
  int sssd01hi;
  int sssd01lo;
  int pspmt;
  int dynode;
  int clover;
  int sega;
  int labr3;

  //TO CLYC
  //int clyc;

 public:
  void Initialize();
};

// Correlation
class TCorrelatorCalibrator
{
 public:
  int reset;
  int resetclock; // Reset 50MHz clock
  double minimplant;
  double corrwindow;

 public:
  void Initialize();
};

// PID
class TPIDCalibrator
{
 public:
  double de1scale;
  double de1offset;
  double de1tofcorr;
  double de1tofoffset;
  double de2scale;
  double de2offset;
  double de2tofcorr;
  double de2tofoffset;

 public:
  void Initialize();
};

// Clover array crystals
class TCloverCalibrator
{
 public:
  double square[totcrystals];
  double slope[totcrystals];
  double intercept[totcrystals];
  double thresh[totcrystals];
  double uld[totcrystals];
  double tdiffdynode[nlabr3];

 public:
  void Initialize();
};

// SeGA
class TSeGACalibrator
{
 public:
  double square[nsega];
  double slope[nsega];
  double intercept[nsega];
  double thresh[nsega];
  double uld[nsega];

 public:
  void Initialize();
};

// LaBr3
class TLaBr3Calibrator
{
 public:
  double square[nlabr3];
  double slope[nlabr3];
  double intercept[nlabr3];
  double thresh[nlabr3];
  double uld[nlabr3];
  double tdiffdynode[nlabr3];

  //TO Dynode timewalk correction parameters for individual labr3-dynode pairs
  /* double dytwalkp0[nlabr3]; */
  /* double dytwalkp1[nlabr3]; */
  /* double dytwalkp2[nlabr3]; */
  /* double dytwalkp3[nlabr3]; */
  /* double dytwalkp4[nlabr3]; */

 public:
  void Initialize();
};

/* The entire beta decay setup */
class TBetaDecayVariables
{
 public:
  TClockCalibrator clock;
  TClockCalibrator clockisomer;
  TPINCalibrator pin01;
  TPINCalibrator pin02;
  TPINCalibrator pin03;
  TDSSDCalibrator front;
  TDSSDCalibrator back;
  TSSSDCalibrator sssd01;
  TTACCalibrator tac;
  THitFlag hit;
  TCorrelatorCalibrator corr;
  TPIDCalibrator pid;
  TPSPMTCalibrator pspmt;
  TCloverCalibrator clover;
  TSeGACalibrator sega;
  TLaBr3Calibrator labr3;


  
 public:
  void Initialize();
  void ReadDSSD(std::string name);
  void ReadSSSD(std::string name);
  void ReadPSPMTTime(std::string name);
  void ReadPSPMT(std::string name);
  void ReadClover(std::string name);
  void ReadCloverTime(std::string name);
  void ReadSeGA(std::string name);
  void ReadLaBr3(std::string name);
  void ReadLaBr3Time(std::string name);
  void ReadOther(std::string name);
  void ReadDyTimewalkCorrection(std::string name);//Correction parameters to shift low dyenergy time response with high dyenergy response for all labr3-dynode pairs
  //void ReadFullAlignment(std::string name);//Second pass; following low and high response alignment
  
};
#endif
