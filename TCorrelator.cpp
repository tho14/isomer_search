#include <iostream>
#include <cfloat>
#include <cmath>

#include "TParameters.h"
#include "TVariables.h"
#include "TCorrelator.h"

// TO e16032

/****************************************
 * Correlation flags:                   *
 * ==================================== *
 * 0  - unset flag                      *
 * 4  - invalid pspmt implant pixel     *
 * 8  - invalid pspmt decay pixel       *
 * 12 - invalid pspmt corr pixel        *
 * 16 - ion implantation                *
 * 20 - decay                           *
 * 24 - invalid implant time            *
 * 28 - invalid decay time              *
 * 32 - good correlation                *
 * 36 - exceeds correlation window      *
 * 40 - back-to-back implants           *
 * 44 - short implant dt                *
 * 48 - light ion only                  *
 * 52 - PSPMT reject (not useful pulse) *
 * 56 - SeGA only(Clover array- e17011) *
 * 60 - LaBr3 only                      *
 * 64 - Gamma only                      *
 * // TO Pin03 removed completely       *
 * 68 - Pin03 only                      *
 * 72 - Light ion, no PSPMT             *
 * 76 - PSPMT ovfl, no Pins             *
 * 92 - Invalid event (no hits)         *
 * 96 - unknown event                   *
 * ==================================== *
 * 99 - reset correlation aray          * 
 ****************************************/

// Implant reset function
void TImplant::Reset()
{
  implanted = false;
  time = 0;        // Time of event
  dE1 = 0;         // dE from Pin01
  dE2 = 0;         // dE from Pin02
  tofpin01i2s = 0; // Time of flight Pin01-I2S
  tofpin02i2s = 0; // Time of flight Pin02-I2S
  dyecal = 0;      // Dynode energy
  dt = 0;          // Time since last implant 
  numcorr = 0;        // Number of times this implant has been used in a correlation

  // Trace fit information
  dychisq_single = 0;
  dyE1real_single = 0;
  dyE1steepness_single = 0;
  dyE1decay_single = 0;
  dyT1_single = 0;
  dychisq_double = 0;
  dyE1real_double = 0;
  dyE1steepness_double = 0;
  dyE1decay_double = 0;
  dyT1_double = 0;
  dyE2real_double = 0;
  dyE2steepness_double = 0;
  dyE2decay_double = 0;
  dyT2_double = 0;
  dytdiff = 0; 
}

// Decay reset function
void TDecay::Reset()
{
  time = 0;        // Time of event
  dE1 = 0;         // dE from Pin01
  dE2 = 0;         // dE from Pin02
  tofpin01i2s = 0; // Time of flight Pin01-I2S
  tofpin02i2s = 0; // Time of flight Pin02-I2S
  dyecal = 0;      // Dynode energy
  dt = 0;          // Time since last correlated implant 
  dnumcorr = 0;    // Number of decays correlated to ion    

  // Trace fit information
  dychisq_single = 0;
  dyE1real_single = 0;
  dyE1steepness_single = 0;
  dyE1decay_single = 0;
  dyT1_single = 0;
  dychisq_double = 0;
  dyE1real_double = 0;
  dyE1steepness_double = 0;
  dyE1decay_double = 0;
  dyT1_double = 0;
  dyE2real_double = 0;
  dyE2steepness_double = 0;
  dyE2decay_double = 0;
  dyT2_double = 0;
  dytdiff = 0; 
}

// Lets make sure we are making the correlator only once
TCorrelator::TCorrelator()
{
  std::cout << "Starting correlator..." << std::endl;

 //Use 32Na only
  // inroot = new TFile("/projects/e16032/e16032_Analysis_THO/fixed_correlator_analysis_packet/selectors/proof_selectors_all_runs/pid/diagnosing_double_bands_pin01/e16032_pid_cut.root");
  // if(!inroot) {
  //   printf("CANNOT FIND THE REQUESTED ROOT FILE! EXITING...\n");
  //   exit(EXIT_FAILURE); 
  // }
  // else {
  //   for(int i = 0; i < 30; i++) {
  //     if(i == 15){//32Na
  // 	cut[i] = (TCutG*)inroot->Get(Form("blob%d",i));
  // 	if(!cut[i]) {
  // 	  printf(Form("CANNOT GET THE TCutG OBJECT @ %d! EXITING...\n",
  // 		      i));
  // 	  exit(EXIT_FAILURE); 
  // 	}
  //     }
  //   }
  // }
  // inroot->Close();
  

  //************************ TO 06/21/22 ***************************//
  //Rootfile containing cuts on pin01 energy v i2pos spectrum to identify valid implants to correlate to
  //I only use pin01 parameters to populate the pid spectrum so I'm going forward with only pin01 conditions in the correlator.

  tighter_de_i2pos_cuts = new TFile("/projects/e16032/e16032_Analysis_THO/fixed_correlator_analysis_packet/selectors/proof_selectors_all_runs/pid/diagnosing_double_bands_pin01/cutter/i2_pos_correction_de_cuts_tightest_along_elemental_bands.root");

  if(!tighter_de_i2pos_cuts) {
    printf("CANNOT FIND THE REQUESTED ROOT FILE! EXITING...\n");
    exit(EXIT_FAILURE); 
  }
  else {
    for(int i = 0; i < 10; i++) {
      //if(i == 0){//Main distribution
      tighter_de_i2pos_cut[i] = (TCutG*)tighter_de_i2pos_cuts->Get(Form("cut%d",i));
      if(!tighter_de_i2pos_cut[i]) {
  	printf(Form("CANNOT GET THE TCutG OBJECT @ %d! EXITING...\n",
  		    i));
  	exit(EXIT_FAILURE); 
      }
    }
    //}
  }
  tighter_de_i2pos_cuts->Close();

  el_bands = new TFile("/projects/e16032/e16032_Analysis_THO/fixed_correlator_analysis_packet/selectors/proof_selectors_all_runs/pid/diagnosing_double_bands_pin01/cutter/elemental_cuts_on_dE_i2pos_gated_pid.root");

  if(!el_bands) {
    printf("CANNOT FIND THE REQUESTED ROOT FILE! EXITING...\n");
    exit(EXIT_FAILURE); 
  }
  else {
    for(int i = 0; i < 10; i++) {
      // if(i == 0){//Main distribution
      ele_bands[i] = (TCutG*)el_bands->Get(Form("cut%d",i));
      if(!ele_bands[i]) {
  	printf(Form("CANNOT GET THE TCutG OBJECT @ %d! EXITING...\n",
  		    i));
  	exit(EXIT_FAILURE); 
      }
    }
    //}
  }
  el_bands->Close(); 
  
}

// Reset
//
// Function:
//   - reset correlation arrays
//
// Params:
//   - none
//
// Return:
//   - none
//
void TCorrelator::Reset()
{
  std::cout << "Resetting correlator array..." << std::endl;
  for(int i=0; i<npspmt; i++) 
    {
      for(int j=0; j<npspmt; j++) 
	{
	  fImplant[i][j].Reset();
	  fDecay[i][j].Reset();
	}
    }
}

// Correlate
//
// Function:
//   - correlate decays with implanted ions by examining the relative position
//     and time of the implant and decay events.
//
// Params:
//   - beta decay parameters (i.e. detector classes)
//   - beta decay variables (i.e. calibration parameters, thresholds, etc.)
//
// Return:
//   - 0 if successful (valid correlation flag)
//   - 1 otherwise
//
int TCorrelator::Correlate(TBetaDecayParams &bdecay, TBetaDecayVariables &bdecayv)
{
  using std::cout;
  using std::endl;
  
  // Reset correlator
  int condition = 96; // inital condition is unknown event (no hits)
  if(bdecayv.corr.reset == 1)
    {
      TCorrelator::Reset(); 
      bdecay.corr.flag = 99; // Set the reset flag
      bdecayv.corr.reset = 0; 
    }

  bool implanted = false;
    
  // Get time
  double current_time = 0;
  if(bdecay.clock.initial > 0)
    {
      current_time = bdecay.clock.initial / 1.e6; // Time in ms
    }
  else
    {
      cout << "Invalid time: " << bdecay.clock.initial << " something is very wrong!" << endl;
      return 1; // bad
    }

  // Signal flags
  bool hasPin = false;
  bool hasPin01Imp = false;
  bool hasPin02Imp = false;
  bool hasPin01LI = false;
  bool hasPin02LI = false;
  //bool hasPin03LI = false;
  bool hasPinLI = false;
  bool hasPSPMT = false;
  bool hasSeGA = false;
  //bool hasClover = false;
  bool hasLaBr3 = false;
  bool overflowPSPMT = false;
   

  // Event identification flags
  bool isImplant = false;
  bool isUsefulPulse = false; 
  //bool isPSPMTReject = false;
  bool isDecay = false;
  bool isLightIon = false;
  bool isSeGAOnly = false;
  //bool isCloverOnly = false;
  bool isLaBr3Only = false;
  bool isGammaOnly = false;
  //bool isPin03Only = false;
  bool isLightIonNoPSPMT = false;
  bool isDynOvflNoPins = false;
  bool isInvalidEvent = false;

  // PSPMT
  int xPos = -1;
  int yPos = -1;

  /*********************************
   *** Event Type Identification ***
   *********************************/

  // Hit flags are set in TUnpacker.cpp
  //
  // TO
  // if(bdecayv.hit.pin01 == 1 || bdecayv.hit.pin02 == 1 || bdecayv.hit.pin03 == 1){
  if(bdecayv.hit.pin01 == 1 || bdecayv.hit.pin02 == 1){
    hasPin = true;
  }// Any Pin hit
  
  // Pin01
  if(bdecayv.hit.pin01 == 1) {
    if(bdecay.pin01.energy > 6000) {//TO From pid plot using pin01 entries
      hasPin01Imp = true;//TO This distinguishes relatively higher Z ions of interest from others detected in pin01 only
    }
    else {
      hasPin01LI = true;//TO Other entries detected in pin01 under the cut off
    }
  }
  
  // Pin02
  if(bdecayv.hit.pin02 == 1) {//TO See pin01 comments
    if(bdecay.pin02.energy > 2000) {
      hasPin02Imp = true;
    }
    else {
      hasPin02LI = true;
    }
  }
  
  // TO Pin03
  // This is for other light ions that did not leave a signal in either 
  // pin01 or pin02 (due to being lower than the thresholds set for 
  // those detectors) but end up making it to the cebr3.
  // It will be tough to discriminate such from actual decays that should 
  // satisfy this condition, except we have pin03 as a veto specifically set 
  // to identify these light ions. So the threshold and gain will be specifically 
  // set for that. We did not use pin03 for this purpose in e16032 so this condition 
  // will not be useful for us here.
  // So we are taking pin03 out of the correlator completely.
  
  // if(bdecayv.hit.pin03 == 1) {
  //   hasPin03LI = true;
  // }
  
  // Has any Pin light ion hit
  // if(hasPin01LI || hasPin02LI || hasPin03LI){
  if(hasPin01LI || hasPin02LI){
    hasPinLI = true;
  }

  if(bdecayv.hit.pspmt == 1){
    hasPSPMT = true;
  }

  //if(bdecayv.hit.clover == 1) hasClover = true;
  if(bdecayv.hit.sega == 1){
    hasSeGA = true;
  }
  
  if(bdecayv.hit.labr3 == 1){
    hasLaBr3 = true;
  }
  
  if(bdecay.pspmt.dyoverflow == 1){
    overflowPSPMT = true;
  }


  // Advanced type identification
  //
  // We don't want to make any attempt to classify pulses here, only to ensure that
  // double pulses where the individual energies are under the light ion limit but the
  // single pulse energy is over the light ion limit can still be identified as decays.
  // Any attempt at pulse classification happens during the data analysis.
  //
  // Logic is: if(goodSingleE) then keep the event;
  //           else if (goodDoubleE1 && goodDoubleE2) then keep the event.
  // This should only look for good double pulse energies if the single pulse is not good.
  //
  // ---> WARNING: Very permissive!
  //
  //
  // TO Adding energy checks from the corrected double pulse amplitudes (energies)
  // have proven to be redundant, so we stick with the single pulse.
  // Goes without saying that we don't need the good single/double pulse
  // discriminator in terms of chisq ratio here anymore.
  
  if(bdecay.pspmt.dyE1real_single > 0 && bdecay.pspmt.dyE1real_single < 6200)
    {
      isUsefulPulse = true;
    }
  
  // else if(bdecay.pspmt.dyE1real_double > 0 && bdecay.pspmt.dyE1real_double < 25000 &&
  // 	  bdecay.pspmt.dyE2real_double > 0 && bdecay.pspmt.dyE2real_double < 25000)
  //   {
  //     isUsefulPulse = true;
  //   }

  //Light Ions
  if(hasPinLI && hasPSPMT)
    {
      isLightIon = true;
      bdecay.corr.islightion = 1; // ID'd as a light ion
    }

  // //Implants    
  // if(hasPin01Imp && hasPin02Imp && hasPSPMT)
  //   {
  //     isImplant = true;
  //     bdecay.corr.isimplant = 1;
  //   }

  //********** TO 06/21/22 **************//
  //Including additional condition for pin01_energy i2pos gate to identify implants!
 
  for(int i = 0; i < 10; i++){
    //if(i == 0){\ 
    if(tighter_de_i2pos_cut[i]->IsInside(bdecay.tac.i2ni2s, bdecay.pin01.energy)){
      if(ele_bands[i]->IsInside(bdecay.tac.pin01i2s, bdecay.pin01.energy)){
	if(hasPin01Imp && hasPin02Imp && hasPSPMT)
	  {
	    isImplant = true;
	    bdecay.corr.isimplant = 1;
	  }
      }
      //}
    }
  }
    
  // Decays do not overflow PSPMT, do not come with the beam (no Pins). Explicitly exclude LIs.
  if(!hasPin && hasPSPMT && isUsefulPulse && !isLightIon)
    {
      isDecay = true;
      bdecay.corr.isdecay = 1;
    }
  
  // Gamma-ray detectors only

  // if(!hasPin && !hasPSPMT && hasClover && !hasLaBr3)
  // {
  //  isCloverOnly = true; 
  // }
  
  if(!hasPin && !hasPSPMT && hasLaBr3)
    {
      isLaBr3Only = true;
    }
  
  if(!hasPin && !hasPSPMT && (hasSeGA || hasLaBr3))
    {
      isGammaOnly = true;
    }

  // if(!hasPin && !hasPSPMT && hasSeGA && hasLaBr3)
  //   {
  //     isGammaOnly = true;
  //   }

  if(!hasPin && !hasPSPMT && hasSeGA)
    {
      isSeGAOnly = true;
    }

  
  //if(!hasPin01Imp && !hasPin01LI && !hasPin02Imp && !hasPin02LI && hasPin03LI && !hasPSPMT && !hasClover && !hasLaBr3)
  // if(!hasPin01Imp && !hasPin01LI && !hasPin02Imp && !hasPin02LI && hasPin03LI && !hasPSPMT && !hasSeGA && !hasLaBr3)
  //   {
  //     isPin03Only = true;
  //   }
  
  // PIN light ions with no PSPMT, can have gammas and many do (?!?)
  // if(hasPinLI && !hasPSPMT)
  //   {
  //     isLightIonNoPSPMT = true;
  //   }
  // Dynode overflow, no PINs, explicity exclude light ions
  if(!hasPin && hasPSPMT && overflowPSPMT && !isLightIon)
    {
      isDynOvflNoPins = true;
      bdecay.corr.isdynovflnopins = 1;
    }
  
  // Invalid events -- no required detector has a good hit as defined in the Unpacker
  // if(bdecayv.hit.pin01 == 0 && bdecayv.hit.pin02 == 0 && bdecayv.hit.pin03 == 0 && bdecayv.hit.pspmt == 0 && bdecayv.hit.clover == 0 && bdecayv.hit.labr3 == 0)
  // if(bdecayv.hit.pin01 == 0 && bdecayv.hit.pin02 == 0 && bdecayv.hit.pin03 == 0 && bdecayv.hit.pspmt == 0 && bdecayv.hit.sega == 0 && bdecayv.hit.labr3 == 0)
  if(bdecayv.hit.pin01 == 0 && bdecayv.hit.pin02 == 0 && bdecayv.hit.pspmt == 0 && bdecayv.hit.sega == 0 && bdecayv.hit.labr3 == 0)
    {
      isInvalidEvent = true;
    }

  // // Event tracking
  // bool hasGoodEnergy = false;
  // int pos = -1;
  // double ecalab = 0;
  // if(hasClover && hasPSPMT)
  //   {
  //     for(int i=0; i<nclovers; i++)
  // 	{
  // 	  if(bdecay.clover.ecalab[i] > 2204 && bdecay.clover.ecalab[i] < 2208)
  // 	    {
  // 	      hasGoodEnergy = true;
  // 	      pos = i;
  // 	      ecalab = bdecay.clover.ecalab[i];
  // 	      break;
  // 	    }
  // 	}
  //     if(hasGoodEnergy == true)
  // 	{
  // 	  // here we probably want to cout some event flags to see what is going on...
  // 	  cout << "hit.pin01  " << bdecayv.hit.pin01 << " ecal " << bdecay.pin01.ecal << " time " << bdecay.pin01.time << endl;
  // 	  cout << "hit.pin02  " << bdecayv.hit.pin02 << " ecal " << bdecay.pin02.ecal << " time " << bdecay.pin02.time << endl;
  // 	  cout << "hit.pin03  " << bdecayv.hit.pin03 << " ecal " << bdecay.pin03.ecal << " time " << bdecay.pin03.time << endl;
  // 	  cout << "hit.dynode " << bdecayv.hit.dynode << " ecal " << bdecay.pspmt.dyecal << " fit " << bdecay.pspmt.dyE1real_single << " time " << bdecay.pspmt.dytime << endl;
  // 	  cout << "hit.pspmt  " << bdecayv.hit.pspmt << endl;
  // 	  cout << "anode mult " << bdecay.pspmt.amult << endl;
  // 	  cout << "hit.clover " << bdecayv.hit.clover << " pos " << pos << " ecalab " << ecalab << endl;
  // 	  cout << "hit.labr3  " << bdecayv.hit.labr3 << endl;

  // 	  cout << "hasPin " << hasPin << endl;
  // 	  cout << "hasPin01Imp " << hasPin01Imp << endl;
  // 	  cout << "hasPin02Imp "<< hasPin02Imp << endl;
  // 	  cout << "hasPin01LI " << hasPin01LI << endl;
  // 	  cout << "hasPin02LI " << hasPin02LI << endl;
  // 	  cout << "hasPin03LI " << hasPin03LI << endl;
  // 	  cout << "hasPinLI " << hasPinLI << endl;
  // 	  cout << "hasPSPMT " << hasPSPMT << endl;
  // 	  cout << "hasClover " << hasClover << endl;
  // 	  cout << "hasLaBr3 " << hasLaBr3 << endl;
  // 	  cout << "overflowPSPMT " << overflowPSPMT << endl;
      
  // 	  cout << "isImplant " << isImplant << endl;
  // 	  // cout << "isUsefulPulse " << isUsefulPulse << endl; 
  // 	  cout << "isDecay " << isDecay << endl; 
  // 	  cout << "isLightIon " << isLightIon << endl; 
  // 	  cout << "isCloverOnly " << isCloverOnly << endl; 
  // 	  cout << "isLaBr3Only " << isLaBr3Only << endl; 
  // 	  cout << "isGammaOnly " << isGammaOnly << endl;
  // 	  cout << "isPin03Only " << isPin03Only << endl;
  // 	  cout << "isInvalidEvent " << isInvalidEvent << endl;

  // 	  getc(stdin);
  // 	}
  //   }

  /*********************************
   ***    Build correlations     ***
   *********************************/
  
  bool goodPos = false;
  if(isImplant)
    {
      // Get position info from the anode energy distribution fit
      xPos = static_cast<int>(bdecay.pspmt.lxpos);
      yPos = static_cast<int>(bdecay.pspmt.lypos);

      // Sanity check on position
      // Note that the 16x16 anode is 0 indexed on both axes!
      if(xPos >= 0 && xPos < npspmt)
	if(yPos >= 0 && yPos < npspmt)
	  {
	    goodPos = true;
	    // bdecay.corr.isimplant = 1; // ID'd as implant with good position
	    condition = 16;
	  }
	  
      if(!goodPos)
	{
	  condition = 4; // Bad implant position
	}
      else
	{ // We have a good position
	  // Check existing implant
	  double timeDiffIon = -1;
	  if(fImplant[xPos][yPos].time >= 0)
	    {
	      // Note: need abs(timeDiffIon) for reverse correlation!
	      timeDiffIon = std::abs(current_time - fImplant[xPos][yPos].time);

	      if(timeDiffIon > 0)
		{	    
		  // Check for existing implant in this pixel
		  if(fImplant[xPos][yPos].implanted)
		    {
		      condition = 40;
		      fImplant[xPos][yPos].dt = timeDiffIon;
		    }
		  // If the time and position are good, count this spot as implanted
		  // if it hasn't already been
		  else
		    {
		      fImplant[xPos][yPos].implanted = true;
		    }

		  // Assign values -- this overwrites the previous implant
		  fImplant[xPos][yPos].time = current_time;
		  fImplant[xPos][yPos].dE1 = bdecay.pid.de1;
		  fImplant[xPos][yPos].tofpin01i2s = bdecay.pid.pin01i2s;
		  fImplant[xPos][yPos].dE2 = bdecay.pid.de2;
		  fImplant[xPos][yPos].tofpin02i2s = bdecay.pid.pin02i2s;
		  fImplant[xPos][yPos].dyecal = bdecay.pspmt.dyecal;
		  fImplant[xPos][yPos].numcorr = 0;
		  // Store the fit information
		  fImplant[xPos][yPos].dychisq_single = bdecay.pspmt.dychisq_single;
		  fImplant[xPos][yPos].dyE1real_single = bdecay.pspmt.dyE1real_single;
		  fImplant[xPos][yPos].dyE1steepness_single = bdecay.pspmt.dyE1steepness_single;
		  fImplant[xPos][yPos].dyE1decay_single = bdecay.pspmt.dyE1decay_single;
		  fImplant[xPos][yPos].dyT1_single = bdecay.pspmt.dyT1_single;
		  fImplant[xPos][yPos].dychisq_double = bdecay.pspmt.dychisq_double;
		  fImplant[xPos][yPos].dyE1real_double = bdecay.pspmt.dyE1real_double;
		  fImplant[xPos][yPos].dyE1steepness_double = bdecay.pspmt.dyE1steepness_double;
		  fImplant[xPos][yPos].dyE1decay_double = bdecay.pspmt.dyE1decay_double;
		  fImplant[xPos][yPos].dyT1_double = bdecay.pspmt.dyT1_double;
		  fImplant[xPos][yPos].dyE2real_double = bdecay.pspmt.dyE2real_double;
		  fImplant[xPos][yPos].dyE2steepness_double = bdecay.pspmt.dyE2steepness_double;
		  fImplant[xPos][yPos].dyE2decay_double = bdecay.pspmt.dyE2decay_double;
		  fImplant[xPos][yPos].dyT2_double = bdecay.pspmt.dyT2_double;
		  fImplant[xPos][yPos].dytdiff = bdecay.pspmt.dytdiffE1E2;

		  // Correlator implants
		  bdecay.corr.itime = fImplant[xPos][yPos].time;
		  bdecay.corr.ide1 = fImplant[xPos][yPos].dE1;
		  bdecay.corr.itofpin01i2s = fImplant[xPos][yPos].tofpin01i2s;
		  bdecay.corr.ide2 = fImplant[xPos][yPos].dE2;
		  bdecay.corr.itofpin02i2s = fImplant[xPos][yPos].tofpin02i2s;
		  bdecay.corr.idyecal = fImplant[xPos][yPos].dyecal;
		  bdecay.corr.ixpos = xPos;
		  bdecay.corr.iypos = yPos;
		} // End good implant time difference check
	      else
		{
		  condition = 24; // Bad implant time
		}
	    } // End good implant time check
	  else
	    {
	      timeDiffIon = -1; // Reset time difference
	    }
	} // End implant position check      
    } // End implant analysis
  else if(isDecay)
    {
      // Get position info from the anode energy distribution fit
      xPos = static_cast<int>(bdecay.pspmt.lxpos);
      yPos = static_cast<int>(bdecay.pspmt.lypos);

      // Sanity check on position
      // Note that the 16x16 anode is 0 indexed on both axes!
      if(xPos >=0 && xPos < npspmt)
	if(yPos >= 0 && yPos < npspmt)
	  {
	    goodPos = true;
	    bdecay.corr.dxpos = xPos;
	    bdecay.corr.dypos = yPos;
	    // bdecay.corr.isdecay = 1; // ID'd as decay with good position
	    condition = 20;
	  }
      
      if(!goodPos)
	{
	  condition = 8; // Bad implant position
	  // if(hasGoodEnergy) {
	  //   cout << "Is decay with bad position " << xPos << " " << yPos << endl;
	  //   getc(stdin);
	  // }
	}
      else
	{ // We have a good position

	  // if(hasGoodEnergy) {
	  //   cout << "Is decay with position " << xPos << " " << yPos << endl;
	  //   getc(stdin);
	  // }
	      
	  // Search around the current decay position for correlation. One can only look for
	  // ions which came before decays, so from the decay position search a fCorrWinxfCorrWin
	  // grid around the current position for the ion which came closest in time to the decay.
	  // Decays are associated with the ion which came with the smallest time difference. The
	  // implant position corresponding to the minimum time difference is assigned to the
	  // decay (overwrites the previous decay position!) because the decay has been associated
	  // with that implant.
	  double decay_pixel[fCorrWin][fCorrWin]; // Array of time differences
	  double mintdiff = DBL_MAX; // Minimum time difference, initialized to large number
      
	  // Reset the time difference array with very large times
	  for(int i=0; i<fCorrWin; i++)
	    {
	      for(int j=0; j<fCorrWin; j++)
		{
		  decay_pixel[i][j] = DBL_MAX;
		  // if(hasGoodEnergy) {
		  //   cout << "Initial tdiff array " << i << " " << j << " " << decay_pixel[i][j] << endl;
		  // }
		}
	    } // Time difference array reset
	  // if(hasGoodEnergy)
	  //   getc(stdin);

	  // Search the correlation grid for implants and set the time differences
	  // Note: time differences must be abs(tdiff) for reverse correlation!
	  int corr_limit = (fCorrWin - 1)/2; // for fCorrWin = 3, corr_limt = 1
	  int num_corr = 0;

	  // if(hasGoodEnergy)
	  //   cout << "current time " << current_time << endl;
	  
	  for(int i=(-corr_limit); i<(corr_limit+1); i++)
	    {
	      for(int j=(-corr_limit); j<(corr_limit+1); j++)
		{
		  if((xPos+i) >= 0 && (xPos+i) < npspmt &&
		     (yPos+j) >= 0 && (yPos+j) < npspmt)
		    { // Is it a valid pixel?
		      if(fImplant[xPos+i][yPos+j].implanted)
			{
			  double tdiff = std::abs(current_time-fImplant[xPos+i][yPos+j].time);
			  if(tdiff > 0)
			    {
			      decay_pixel[i+corr_limit][j+corr_limit] = tdiff;
			      // if(hasGoodEnergy)
			      // 	{
			      // 	  cout << "Tdiff array " << i << " " << j << " " << xPos+i << " " << yPos+j << " " << " itime " << fImplant[xPos+i][yPos+j].time << " " << decay_pixel[i+corr_limit][j+corr_limit] << endl;
			      // 	}
			    }
			  implanted = true; // There was at least one implant in the correlation field
			  num_corr++;
			}
		    }
		}
	    } // End search of the correlation grid for minimum tdiff between decay and ion

	  int xposmin = 500; // Implant x-position corresponding to minimum time difference
	  int yposmin = 500; // Implant y-position corresponding to minimum time difference

	  // Search the correlation grid for the minimum time difference and overwrite the decay position
	  // with the implant position corresponding to the implant with the minimum time difference.
	  if(implanted)
	    {
	      for(int i=(-corr_limit); i<(corr_limit+1); i++)
		{
		  for(int j=(-corr_limit); j<(corr_limit+1); j++)
		    {
		      if((xPos+i) >= 0 && (xPos+i) < npspmt &&
			 (yPos+j) >= 0 && (yPos+j) < npspmt)
			{ // Is it a valid pixel? 
			  if(decay_pixel[i+corr_limit][j+corr_limit] > 0 && decay_pixel[i+corr_limit][j+corr_limit] < mintdiff)
			    {
			      mintdiff = decay_pixel[i+corr_limit][j+corr_limit];
			      xposmin = xPos+i;
			      yposmin = yPos+j;
			    }
			}
		    }
		}      
	    } // End assignment of correlated position
	
	  bdecay.corr.mult = static_cast<int>(num_corr);      
	  num_corr = 0;

	  // Reset the position to the position of the implant with
	  // the minimum time difference.
	  xPos = xposmin;
	  yPos = yposmin;

	  // if(hasGoodEnergy)
	  //   cout << "mintdiff " << mintdiff << " " << xposmin << " " << yposmin << endl;

	  // Sanity check on position after we've found the minimum time difference
	  // for the implant position assoicated with the current decay. The associated
	  // position should always be good, so we should never see flag == 12!
	  // Note that the 16x16 anode is 0 indexed on both axes!
	  goodPos = false;
	  if(xPos >=0 && xPos < npspmt)
	    if(yPos >= 0 && yPos < npspmt)
	      {
		goodPos = true;

		// if(hasGoodEnergy)
		//   cout << "has good pos " << xPos << " " << yPos << endl;
	      }
	      
	  if(!goodPos)
	    {
	      condition = 12; // Bad implant position assoicated with the current decay
	      // cout << xPos << " " << yPos << " " << mintdiff << endl;
	    }
	  else		  
	    { // We have a good position
		  
	      // Correlation logic -- is the time difference between the decay and implant
	      // within the correlation window? Does the time difference between the last
	      // two implants in the pixel allow for "unambiguous" identification of the parent?
	      // Note: time differences must be abs(tdiff) for reverse correlation
	      double ionDecayTDiff = mintdiff;

	      // if(hasGoodEnergy)
	      // 	{
	      // 	  cout << "good time diff is " << mintdiff << " " << ionDecayTDiff << " at " << xPos << " " << yPos << endl;
	      // 	getc(stdin);
	      // 	}
	      
	      if(ionDecayTDiff <= 0)
		{
		  condition = 28; // Decay before associated implant. This should never happen! 
		}
	      // else if(ionDecayTDiff > bdecayv.corr.corrwindow)
	      // 	{
	      // 	  condition = 36; // Time difference exceeds correlation window
	      // 	}
	      /* 
		 Try to do this offline when we build histograms
	      */
	      // dt == 0 is the first implant
	      // else if(fImplant[xPos][yPos].dt > 0 && fImplant[xPos][yPos].dt < bdecayv.corr.minimplant)
	      // 	{
	      // 	  condition = 44; // Back-to-back implants in this pixel
	      // 	}
	      else
		{
		  condition = 32; // Good correlated event
		  fDecay[xPos][yPos].time = current_time;
		  fDecay[xPos][yPos].dE1 = fImplant[xPos][yPos].dE1;
		  fDecay[xPos][yPos].tofpin01i2s = fImplant[xPos][yPos].tofpin01i2s;
		  fDecay[xPos][yPos].dE2 = fImplant[xPos][yPos].dE2;
		  fDecay[xPos][yPos].tofpin02i2s = fImplant[xPos][yPos].tofpin02i2s;
		  fDecay[xPos][yPos].dyecal = bdecay.pspmt.dyecal;
		  fDecay[xPos][yPos].dt = ionDecayTDiff;
		  fDecay[xPos][yPos].dnumcorr++;
		  // Store the fit information
		  fDecay[xPos][yPos].dychisq_single = bdecay.pspmt.dychisq_single;
		  fDecay[xPos][yPos].dyE1real_single = bdecay.pspmt.dyE1real_single;
		  fDecay[xPos][yPos].dyE1steepness_single = bdecay.pspmt.dyE1steepness_single;
		  fDecay[xPos][yPos].dyE1decay_single = bdecay.pspmt.dyE1decay_single;
		  fDecay[xPos][yPos].dyT1_single = bdecay.pspmt.dyT1_single;
		  fDecay[xPos][yPos].dychisq_double = bdecay.pspmt.dychisq_double;
		  fDecay[xPos][yPos].dyE1real_double = bdecay.pspmt.dyE1real_double;
		  fDecay[xPos][yPos].dyE1steepness_double = bdecay.pspmt.dyE1steepness_double;
		  fDecay[xPos][yPos].dyE1decay_double = bdecay.pspmt.dyE1decay_double;
		  fDecay[xPos][yPos].dyT1_double = bdecay.pspmt.dyT1_double;
		  fDecay[xPos][yPos].dyE2real_double = bdecay.pspmt.dyE2real_double;
		  fDecay[xPos][yPos].dyE2steepness_double = bdecay.pspmt.dyE2steepness_double;
		  fDecay[xPos][yPos].dyE2decay_double = bdecay.pspmt.dyE2decay_double;
		  fDecay[xPos][yPos].dyT2_double = bdecay.pspmt.dyT2_double;
		  fDecay[xPos][yPos].dytdiff = bdecay.pspmt.dytdiffE1E2;

		  // Correlated parameters
		  bdecay.corr.dtimplant = fImplant[xPos][yPos].dt; // Implant dt from same pixel
		  bdecay.corr.dtimecal = fDecay[xPos][yPos].dt*bdecayv.clock.calib;
		  bdecay.corr.dtime = fDecay[xPos][yPos].time;
		  bdecay.corr.dde1  = fDecay[xPos][yPos].dE1;
		  bdecay.corr.dtofpin01i2s = fDecay[xPos][yPos].tofpin01i2s;
		  bdecay.corr.dde2 = fDecay[xPos][yPos].dE2;
		  bdecay.corr.dtofpin02i2s = fDecay[xPos][yPos].tofpin02i2s;
		  bdecay.corr.ddyecal = fDecay[xPos][yPos].dyecal;
		  bdecay.corr.didyecal = fImplant[xPos][yPos].dyecal;
		  bdecay.corr.dnumcorr = fDecay[xPos][yPos].dnumcorr;
		  bdecay.corr.dixpos = xPos;
		  bdecay.corr.diypos = yPos;

		  // if(hasGoodEnergy) {
		  //   cout << "this is a good correlated event " << ionDecayTDiff << " < " << bdecayv.corr.corrwindow << " and implant " << fImplant[xPos][yPos].dt << " > " << bdecayv.corr.minimplant << " PID " << bdecay.corr.dtofpin02i2s << " " << bdecay.corr.dde2 << " Hit enter to continue..." << endl;
		  //   getc(stdin);
		  // }
		  
		  // Store some information from the fits
		  // ONLY written for good correlations!
		  // --ASC 6/25/2019
		  // Correlator: implant fits
		  bdecay.corr.didychisq_single = fImplant[xPos][yPos].dychisq_single;
		  bdecay.corr.didyE1real_single = fImplant[xPos][yPos].dyE1real_single;
		  bdecay.corr.didyE1steepness_single = fImplant[xPos][yPos].dyE1steepness_single;
		  bdecay.corr.didyE1decay_single = fImplant[xPos][yPos].dyE1decay_single;
		  bdecay.corr.didyT1_single = fImplant[xPos][yPos].dyT1_single;
		  bdecay.corr.didychisq_double = fImplant[xPos][yPos].dychisq_double;
		  bdecay.corr.didyE1real_double = fImplant[xPos][yPos].dyE1real_double;
		  bdecay.corr.didyE1steepness_double = fImplant[xPos][yPos].dyE1steepness_double;
		  bdecay.corr.didyE1decay_double = fImplant[xPos][yPos].dyE1decay_double;
		  bdecay.corr.didyT1_double = fImplant[xPos][yPos].dyT1_double;
		  bdecay.corr.didyE2real_double = fImplant[xPos][yPos].dyE2real_double;
		  bdecay.corr.didyE2steepness_double = fImplant[xPos][yPos].dyE2steepness_double;
		  bdecay.corr.didyE2decay_double = fImplant[xPos][yPos].dyE2decay_double;
		  bdecay.corr.didyT2_double = fImplant[xPos][yPos].dyT2_double;
		  bdecay.corr.didytdiff = fImplant[xPos][yPos].dytdiff;
		  // Correlator: decay fits
		  bdecay.corr.ddychisq_single = fDecay[xPos][yPos].dychisq_single;
		  bdecay.corr.ddyE1real_single = fDecay[xPos][yPos].dyE1real_single;
		  bdecay.corr.ddyE1steepness_single = fDecay[xPos][yPos].dyE1steepness_single;
		  bdecay.corr.ddyE1decay_single = fDecay[xPos][yPos].dyE1decay_single;
		  bdecay.corr.ddyT1_single = fDecay[xPos][yPos].dyT1_single;
		  bdecay.corr.ddychisq_double = fDecay[xPos][yPos].dychisq_double;
		  bdecay.corr.ddyE1real_double = fDecay[xPos][yPos].dyE1real_double;
		  bdecay.corr.ddyE1steepness_double = fDecay[xPos][yPos].dyE1steepness_double;
		  bdecay.corr.ddyE1decay_double = fDecay[xPos][yPos].dyE1decay_double;
		  bdecay.corr.ddyT1_double = fDecay[xPos][yPos].dyT1_double;
		  bdecay.corr.ddyE2real_double = fDecay[xPos][yPos].dyE2real_double;
		  bdecay.corr.ddyE2steepness_double = fDecay[xPos][yPos].dyE2steepness_double;
		  bdecay.corr.ddyE2decay_double = fDecay[xPos][yPos].dyE2decay_double;
		  bdecay.corr.ddyT2_double = fDecay[xPos][yPos].dyT2_double;
		  bdecay.corr.ddytdiff = fDecay[xPos][yPos].dytdiff;
		} // End good correlated event
		  
	    } // End implant position assoicated with the current decay
	      
	} // End decay position check
	  
    } // End decay analysis      
  else if(isLightIon)
    {
      condition = 48;
    }
  // else if(isPSPMTReject)
  //   {
  //     condition = 52;
  //   }
  // TO
  //else if(isCloverOnly)
  else if(isSeGAOnly)
    { // Can do some isomer hunting in here
      condition = 56;
    }
  else if(isLaBr3Only)
    { // Can do some isomer hunting in here
      condition = 60;
    }
  else if(isGammaOnly)
    { // Simply to make sure we understand what we're looking at
      condition = 64;
    }
  // else if(isPin03Only)
  //   {
  //     condition = 68;
  //   }
  else if(isLightIonNoPSPMT)
    {
      condition = 72;
    }
  else if(isDynOvflNoPins)
    {
      condition = 76;
    }
  else if(isInvalidEvent)
    {
      condition = 90;
    }
  else
    {
      condition = 96; // Unknown event type, probably redundant
    }

  bool printunknown = false;
  if(condition == 76 && printunknown)
    {
      // here we probably want to cout some event flags to see what is going on...
     
      cout << "condition  " << condition << endl;
      cout << "hit.pin01  " << bdecayv.hit.pin01 << " energy " << bdecay.pin01.energy << " ecal " << bdecay.pin01.ecal << " time " << bdecay.pin01.time << endl;
      cout << "hit.pin02  " << bdecayv.hit.pin02 << " energy " << bdecay.pin02.energy << " ecal " << bdecay.pin02.ecal << " time " << bdecay.pin02.time << endl;
      // cout << "hit.pin03  " << bdecayv.hit.pin03 << " energy " << bdecay.pin03.energy << " ecal " << bdecay.pin03.ecal << " time " << bdecay.pin03.time << endl;
      cout << "hit.dynode " << bdecayv.hit.dynode << " energy " << bdecay.pspmt.dyenergy << " ecal " << bdecay.pspmt.dyecal << " time " << std::setprecision(12) << bdecay.pspmt.dytime << endl;
      cout << "hit.pspmt  " << bdecayv.hit.pspmt << endl;
      cout << "anode mult " << bdecay.pspmt.amult << endl;

      cout << std::setprecision(6);
      // TO
      // cout << "hit.clover " << bdecayv.hit.clover << endl;
      // cout << "clover mult " << bdecay.clover.mult << endl;
      // if(bdecayv.hit.clover) {
      // 	cout << "--> clover energies:" << endl;
      // 	for(auto i=0; i<totcrystals; i++) {
      // 	  if(bdecay.clover.ecal[i] > 0)
      // 	    cout << "---->  pos " << (i/4)+1 << " cry " << i%4 << " ecal " << bdecay.clover.ecal[i] << endl;
      // 	}
      // }
      cout << "hit.labr3  " << bdecayv.hit.labr3 << endl;
      cout << "labr3 mult " << bdecay.labr3.mult << endl;
      if(bdecayv.hit.labr3) {
	cout << "--> labr3 energies:" << endl;
	for(auto i=0; i<nlabr3; i++) {
	  if(bdecay.labr3.ecal[i] > 0)
	    cout << "----> pos " << i+1 << " ecal " << bdecay.labr3.ecal[i] << endl;
	}
      }
      // TO e16032
      cout << "hit.sega " << bdecayv.hit.sega << endl;
      cout << "sega mult " << bdecay.sega.mult << endl;
      if(bdecayv.hit.sega) {
	cout << "--> sega energies:" << endl;
	for(auto i=0; i<nsega; i++) {
	  if(bdecay.sega.ecal[i] > 0)
	    cout << "---->  pos " << i+1 << " ecal " << bdecay.sega.ecal[i] << endl;
	}
      }
    
      cout << "hasPin " << hasPin << endl;
      cout << "hasPin01Imp " << hasPin01Imp << endl;
      cout << "hasPin02Imp "<< hasPin02Imp << endl;
      cout << "hasPin01LI " << hasPin01LI << endl;
      cout << "hasPin02LI " << hasPin02LI << endl;
      // cout << "hasPin03LI " << hasPin03LI << endl;
      cout << "hasPinLI " << hasPinLI << endl;
      cout << "hasPSPMT " << hasPSPMT << endl;
      //
      //cout << "hasClover " << hasClover << endl;
      cout << "hasLaBr3 " << hasLaBr3 << endl;
      cout << "overflowPSPMT " << overflowPSPMT << endl;
      //TO e16032
      cout << "hasSeGA " << hasSeGA << endl;
      
      cout << "isImplant " << isImplant << endl;
      // cout << "isUsefulPulse " << isUsefulPulse << endl; 
      cout << "isDecay " << isDecay << endl; 
      cout << "isLightIon " << isLightIon << endl; 
      //
      //cout << "isCloverOnly " << isCloverOnly << endl; 
      cout << "isLaBr3Only " << isLaBr3Only << endl; 
      cout << "isGammaOnly " << isGammaOnly << endl;
      // cout << "isPin03Only " << isPin03Only << endl;
      cout << "isLightIonNoPSPMT " << isLightIonNoPSPMT << endl;
      cout << "isDynOvflNoPins " << isDynOvflNoPins << endl;
      // TO e16032
      cout << "isSeGAOnly " << isSeGAOnly << endl; 
      cout << "isInvalidEvent " << isInvalidEvent << endl;

      getc(stdin);
    }

  // End correlation
  bdecay.corr.flag = condition; // Set the correlation flag
  
  return 0;
}
