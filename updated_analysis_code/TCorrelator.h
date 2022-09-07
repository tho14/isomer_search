#ifndef __TCORRELATOR_H
#define __TCORREALTOR_H

#include "Constants.h"
#include "TModule.h"

#include "TCutG.h"
#include "TFile.h"

class TImplant
{
 public:
  bool implanted;
  double time;        // Time of event
  double dE1;         // dE from Pin01
  double dE2;         // dE from Pin02
  double tofpin01i2s; // Time of flight Pin01-I2S
  double tofpin02i2s; // Time of flight Pin02-I2S
  double dyecal;      // Dynode energy
  double dt;          // Time since last implant 
  int numcorr;        // Number of times this implant has been used in a correlation

  // Trace fit information
  double dychisq_single;
  double dyE1real_single;
  double dyE1steepness_single;
  double dyE1decay_single;
  double dyT1_single;
  double dychisq_double;
  double dyE1real_double;
  double dyE1steepness_double;
  double dyE1decay_double;
  double dyT1_double;
  double dyE2real_double;
  double dyE2steepness_double;
  double dyE2decay_double;
  double dyT2_double;
  double dytdiff; 

  void Reset();
};

class TDecay
{
 public:
  double time;        // Time of event
  double dE1;         // dE from Pin01
  double dE2;         // dE from Pin02
  double tofpin01i2s; // Time of flight Pin01-I2S
  double tofpin02i2s; // Time of flight Pin02-I2S
  double dyecal;      // Dynode energy
  double dt;          // Time since correlated implant 
  int dnumcorr;       // Number of decays correlated to ion

  // Trace fit information
  double dychisq_single;
  double dyE1real_single;
  double dyE1steepness_single;
  double dyE1decay_single;
  double dyT1_single;
  double dychisq_double;
  double dyE1real_double;
  double dyE1steepness_double;
  double dyE1decay_double;
  double dyT1_double;
  double dyE2real_double;
  double dyE2steepness_double;
  double dyE2decay_double;
  double dyT2_double;
  double dytdiff; 

  void Reset();
};

// Correlator setup
class TCorrelator
{
 public:
  TCorrelator();
  void Reset();
  int Correlate(TBetaDecayParams &bdecay, TBetaDecayVariables &bdecayv);

 private:  
  TImplant fImplant[npspmt][npspmt];
  TDecay fDecay[npspmt][npspmt];
  const int fCorrWin = 3; // TO Correlation pixel window size (has to be odd); 1 for e17011, considering 3 for e16032
  //const int fCorrWin = 1; // TO Seeing if this makes for better correlations

  // TO Using pid cuts
 public:
  /* TCutG *cut[30]; */
  /* TFile *inroot; */

  //************************ TO 06/21/22 ***************************//
  /*Rootfile containing cuts on pin01 energy v i2pos spectrum to identify valid implants to correlate to
  I only use pin01 parameters to populate the pid spectrum; I have verified that using the pin01 energy vs i2pos gate will
  also clean up the pid filled with pin02 parameters.*/

  //Tightest gates across elemental bands
  TFile* tighter_de_i2pos_cuts;
  TCutG* tighter_de_i2pos_cut[10]; //Using 10 cuts along isotopic chains; although the discrete energy cut off for event indentification in the correlator is still at 6k for pin01 and 2k for pin02.
  TFile* el_bands;
  TCutG* ele_bands[10];
};

#endif
