#include <iostream>
//#include <math.h>
#include <cmath> //see std::abs comment

#include "TUnpacker.h"
#include "TLinearSolver.h"
#include "Constants.h"

#include "TRandom3.h"
#include "TH1.h"
#include "TH2.h"
#include "TF1.h"
#include "TF2.h"
#include "TFitResult.h"
#include "TCanvas.h"

/***************************************************************************/
//TO e16032 Channel map
//Create an ID that will be used to identify each channel
//First crate (crate 0) has 13 modules with 16 channels each, so channels
//in first crate run from 0 to 207
//Channel ID
//0 = dynode
//1 - 15 = LaBr3
//16 = Pin01
//17 = Pin02
//18 = Pin03
//19 = TAC1 : Pin01 - I2nTOF
//20 = TAC2 : Pin01 - I2sTOF
//21 = TAC3 : Pin02 - I2nTOF
//22 = TAC4 : Pin02 - I2sTOF
//23 = TAC5 : I2N-I2S 
//24 = TAC6 : Pin01-RF
//30 = I2N
//31 = I2S
//32 = CLYC1 (UMass Lowell)
//33 = CLYC2 (UMass Lowell)
//48 - 63 = SeGA
//64 - 319 = anodes (in rows from top to bottom)
/***************************************************************************/

// ROOT-friendly fitting functions
//
/***********************************************************/
// Five-parameter generalized 2D Lorentzian lineshape. FWHM
// along each axis is given by 2*gamma.
Double_t f2DLorentz(Double_t *x, Double_t *par) {

  // Fit parameters
  Double_t scale   = par[0]; // Amplitude
  Double_t gammax  = par[1]; // Width x
  Double_t gammay  = par[2]; // Width y
  Double_t posx    = par[3]; // Position x
  Double_t posy    = par[4]; // Position y

  // Some quantities
  Double_t xx = x[0]-posx;
  Double_t yy = x[1]-posy;
  Double_t gammax2 = gammax*gammax;
  Double_t gammay2 = gammay*gammay;

  // The denominators of the x and y portions of the 2D fit
  Double_t denomx = 1.0 / (xx*xx + gammax2);
  Double_t denomy = 1.0 / (yy*yy + gammay2);

  // Final fit function
  Double_t val = scale*gammax2*gammay2*denomx*denomy;
  
  return val;
}

/***********************************************************/
// Three-parameter generalized 1D Lorentzian lineshape. FWHM
// along each axis is given by 2*gamma.
Double_t f1DLorentz(Double_t *x, Double_t *par) {

  // Fit parameters
  Double_t scale   = par[0]; // Amplitude
  Double_t gammax  = par[1]; // Width x
  Double_t posx    = par[2]; // Position x

  // Some quantities
  Double_t xx = x[0]-posx;
  Double_t gammax2 = gammax*gammax;

  // Final fit function
  Double_t val = (scale*gammax2)/(xx*xx + gammax2);
  
  return val;
}

// Lets make sure we make the unpacker
TUnpacker::TUnpacker()
{
  std::cout << "Creating channel mapper..." << std::endl;
}

// MapDynode
//
// Function:
//   - map DDAS channel address for the PSPMT dynode
//   - calibrate and threshold/uld check dynode
//
// Params:
//   - crate, slot, and channel number (0 indexed)
//   - overall ADC number in the setup (0 indexed)
//   - beta decay parameters (i.e. detector classes)
//   - beta decay variables (i.e. calibration parameters, thresholds, etc.)
//
// Return:
//   - 0 on success
//   - 1 on fail
//
int TUnpacker::MapDynode(int crateid, int slotid, int channum, int adcnumber, std::vector<UShort_t> trace, TBetaDecayParams &bdecay, TBetaDecayVariables &bdecayv, TModule module[])
{
  using std::cout;
  using std::endl;

  // Fill parameters, calibrate and threshold check the dynode

  TRandom3 *random3 = new TRandom3(0);
 
  // Determine id of the channel
  int id = totchan*crateid + nchannels*(slotid-2) + channum;
  // cout << id << endl;

  bdecay.ddasdiagnostics.chanhit[id]++; // Increment channel hitpattern


  if(id != 0) //Only checking for Dynode id
    {
      cout << "ERROR: Problems with id value " << id << " from crate " << crateid << " slot " << slotid << " chan " << channum << endl;
      exit(1);
    }

  //Dynode id specified in analyzer, so prev check is id dependent
  // Trace analysis
  //Getting the overflow here
  int overflow = 0;
  if(trace.size() > 0)
    {
      // module[adcnumber].utils.CheckOverflowUnderflow(trace,4,module[adcnumber].GetMaxVal());
      // overflow = module[adcnumber].utils.GetOverflow();
      overflow = CheckTraceOverflow(trace,module[adcnumber].GetMaxVal());
    }
  
  // PSPMT dynode
  bdecay.pspmt.dyoverflow = overflow; // Set overflow flag
    
  // Set initial dynode parameters
  // int eraw = bdecay.adc[adcnumber].channel[channum];
  // if(eraw > 0 && eraw < 1)
  //   cout << eraw << endl;
  
  //bdecay.pspmt.dyenergy = bdecay.adc[adcnumber].channel[channum] + random3->Rndm(); // Pixie energy
  double eraw = bdecay.adc[adcnumber].channel[channum] + random3->Rndm();// Pixie energy
  bdecay.pspmt.dyenergy = eraw;
  double ecal = bdecayv.pspmt.dyslope*eraw + bdecayv.pspmt.dyoffset;

  // cout<<"dyslope: " <<bdecayv.pspmt.dyslope<<" intercept: "<<bdecayv.pspmt.dyoffset<<endl;

  //TO Analysis first pass, extract cfd info, especially cfdfailbit to use as flag for applying timewalk correction
  // (events that have valid zero cfd times and those that did not overcome cfd threshold.)
  bdecay.pspmt.dytime = bdecay.time[adcnumber].timefull[channum]; // time
  bdecay.pspmt.dytimecfd = bdecay.time[adcnumber].timecfd[channum];
  bdecay.pspmt.dycfdfailbit = bdecay.time[adcnumber].cfdfail[channum];
  //bdecay.pspmt.dycfdfailbit = bdecay.time[adcnumber].cfdfailbit[channum];//class TDDASTime’ has no member named ‘cfdfailbit’
  
  if(ecal > bdecayv.pspmt.dythresh && ecal < bdecayv.pspmt.dyuld)
    {
      bdecay.pspmt.dyecal = ecal;
      bdecayv.hit.dynode = 1;
    }
  
  // Reset overflows to have a high energy
  if(bdecay.pspmt.dyoverflow == 1)
    {
      bdecay.pspmt.dyenergy = 99999;
      bdecay.pspmt.dyecal = 99999;
      bdecayv.hit.dynode = 1;
    }

  //Sanity check for dynode variables again, please.
  //cout<<" dyslope: "<<bdecayv.pspmt.dyslope<<" dyintercept: "<<bdecayv.pspmt.dyoffset<<" dythresh: "<<bdecayv.pspmt.dythresh<<" dyuld: " << bdecayv.pspmt.dyuld<<" dyenergy: "<<bdecay.pspmt.dyenergy<<" dyecal: "<<bdecay.pspmt.dyecal<<" dycfdfailbit: "<<bdecay.pspmt.dycfdfailbit<<" dyhit: "<<bdecayv.hit.dynode<<endl;

  delete random3; // cleanup

  return 0;
  
}

// MapChannels
//
// Function:
//   - map DDAS channel addresses to detector classes
//   - calibrate and threshold/uld check detectors
//
// Params:
//   - crate, slot, and channel number (0 indexed)
//   - overall ADC number in the setup (0 indexed)
//   - beta decay parameters (i.e. detector classes)
//   - beta decay variables (i.e. calibration parameters, thresholds, etc.)
//
// Return:
//   - 0 on success
//   - 1 on fail
//
int TUnpacker::MapChannels(int crateid, int slotid, int channum, int adcnumber, std::vector<UShort_t> trace, TBetaDecayParams &bdecay, TBetaDecayVariables &bdecayv, TModule module[])
{
  using std::cout;
  using std::endl;
  
  // Depending on what is hit, fill parameters, calibrate and threshold check

  TRandom3 *random3 = new TRandom3(0);
 
  // Determine id of the channel
  int id = totchan*crateid + nchannels*(slotid-2) + channum;
  // cout << id << endl;

  bdecay.ddasdiagnostics.chanhit[id]++; // Increment channel hitpattern

  // cout << crateid << " " << slotid << " " << channum << " " << adcnumber << " " << id << " " << bdecay.adc[adcnumber].channel[channum]  << endl;
  // getc(stdin);
  
  //TO e16032
  if(id < 0 || id > 319)
    {
      cout << "ERROR: Problems with id value " << id << " from crate " << crateid << " slot " << slotid << " chan " << channum << endl;
      exit(1);
    }

  // LaBr3
  if(id >= 1 && id < 16)
    {    
      int det = id - 1; // Count from 0
    
      // raw energy
      double eraw = bdecay.adc[adcnumber].channel[channum] + random3->Rndm();
      bdecay.labr3.energy[det] =  eraw;
      double ecal = bdecayv.labr3.square[det]*eraw*eraw + bdecayv.labr3.slope[det]*eraw + bdecayv.labr3.intercept[det];

      // calibrated energy
      if(ecal > bdecayv.labr3.thresh[det] && ecal < bdecayv.labr3.uld[det])
	{
	  if(!bdecayv.hit.labr3) {
	    bdecayv.hit.labr3 = 1;
	  }
	  bdecay.labr3.ecal[det] = ecal;
	  bdecay.labr3.mult++;
	  // cout << "LABR3 id: " << id << " pos: " << det+1 << " eraw: " << eraw << " ecal: " << ecal << " hit: " << bdecayv.hit.labr3 << endl;
	}

      if(bdecay.pspmt.dycfdfailbit == 1){
	// TOAligning leading edge response to cfd response
	// TO Same value for all detector pairs since we are correcting for the dynode timewalk! 
	bdecay.labr3.time[det] = bdecay.time[adcnumber].timefull[channum] + bdecayv.pspmt.dyhighcentroid[det] - 906.5;
	// TO Individual corrections - doesn't count as they are all the same!
	//bdecay.labr3.time[det] = bdecay.time[adcnumber].timefull[channum] + bdecayv.pspmt.dyhighcentroid[det] - bdecayv.pspmt.dylowcentroid[det];
      }
      else{
       	bdecay.labr3.time[det] = bdecay.time[adcnumber].timefull[channum];
      }
      // TO calibrated time - dylabr3tdiff
      bdecay.labr3.timecal[det] = bdecay.labr3.time[det] - bdecayv.labr3.tdiffdynode[det];
      // TO cfdfailbit
      bdecay.labr3.timecfd[det] = bdecay.time[adcnumber].timecfd[channum];
      bdecay.labr3.cfdfailbit[det] = bdecay.time[adcnumber].cfdfail[channum];     
    } // End LaBr3

  // Pin01
  if(id == 16)
    {
      bdecay.pin01.energy = bdecay.adc[adcnumber].channel[channum] + random3->Rndm();
      double ecal = bdecayv.pin01.slope*bdecay.pin01.energy + bdecayv.pin01.intercept;
      // TO ct 2021 check
      // cout <<"pin01uld: " << bdecayv.pin01.uld <<" thresh: "<< bdecayv.pin01.thresh<< " slope: "<< bdecayv.pin01.slope <<" intercept: "<< bdecayv.pin01.intercept <<endl;
      bdecay.pin01.time = bdecay.time[adcnumber].timefull[channum]; 
      if(ecal > bdecayv.pin01.thresh && ecal < bdecayv.pin01.uld)
  	{
	  bdecay.pin01.ecal = ecal;
  	  bdecayv.hit.pin01 = 1;
  	}
    }

  // Pin02
  if(id == 17)
    {
      bdecay.pin02.energy = bdecay.adc[adcnumber].channel[channum] + random3->Rndm();
      double ecal = bdecayv.pin02.slope*bdecay.pin02.energy + bdecayv.pin02.intercept;
      // TO Oct 2021 check
      // cout <<"pin02uld: " << bdecayv.pin02.uld <<" thresh: "<< bdecayv.pin02.thresh<< " slope: "<< bdecayv.pin02.slope <<" intercept: "<< bdecayv.pin02.intercept <<endl;
      bdecay.pin02.time = bdecay.time[adcnumber].timefull[channum]; 
      if(ecal > bdecayv.pin02.thresh && ecal < bdecayv.pin02.uld)
  	{
	  bdecay.pin02.ecal = ecal;
  	  bdecayv.hit.pin02 = 1;
  	}
    }

  // Pin03
  if(id == 18)
    {
      bdecay.pin03.energy = bdecay.adc[adcnumber].channel[channum] + random3->Rndm();
      double ecal = bdecayv.pin03.slope*bdecay.pin03.energy + bdecayv.pin03.intercept;
      //TO Oct 2021 check
      // cout <<"pin03uld: " << bdecayv.pin03.uld <<" thresh: "<< bdecayv.pin03.thresh<< " slope: "<< bdecayv.pin03.slope <<" intercept: "<< bdecayv.pin03.intercept <<endl;
      bdecay.pin03.time = bdecay.time[adcnumber].timefull[channum]; 
      if(ecal > bdecayv.pin03.thresh && ecal < bdecayv.pin03.uld)
  	{
	  bdecay.pin03.ecal = ecal;
  	  bdecayv.hit.pin03 = 1;
  	}
    }

  // TO e16032
  if(id == 19)
    {
      double tacraw = bdecay.adc[adcnumber].channel[channum] + random3->Rndm();
      if(tacraw > bdecayv.tac.thresh && tacraw < bdecayv.tac.uld) {
	bdecay.tac.pin01i2n = tacraw;
      } else {
	bdecay.tac.pin01i2n = 99999;
      }

      bdecay.tac.pin01i2ntime = bdecay.time[adcnumber].timefull[channum];
    }

  //TAC2:  Pin01 - I2S
  if(id == 20)
    {
      double tacraw = bdecay.adc[adcnumber].channel[channum] + random3->Rndm();
      if(tacraw > bdecayv.tac.thresh && tacraw < bdecayv.tac.uld) {
	bdecay.tac.pin01i2s = tacraw;
      } else {
	bdecay.tac.pin01i2s = 99999;
      }
      
      bdecay.tac.pin01i2stime = bdecay.time[adcnumber].timefull[channum];
    }
  //calculating I2 corrections from PIN02
  // TAC3: Pin02 - I2N
  if(id == 21)
    {
      double tacraw = bdecay.adc[adcnumber].channel[channum] + random3->Rndm();
      if(tacraw > bdecayv.tac.thresh && tacraw < bdecayv.tac.uld) {
	bdecay.tac.pin02i2n = tacraw;
      }{
	bdecay.tac.pin02i2n = 99999;
      }
      
      bdecay.tac.pin02i2ntime = bdecay.time[adcnumber].timefull[channum];
    }

  // TAC4: Pin02 - I2S
  if(id == 22)
    {
      double tacraw = bdecay.adc[adcnumber].channel[channum] + random3->Rndm();
      if(tacraw > bdecayv.tac.thresh && tacraw < bdecayv.tac.uld) {
	bdecay.tac.pin02i2s = tacraw;
      } else {
	bdecay.tac.pin02i2s = 99999;
      }      
      
      bdecay.tac.pin02i2stime = bdecay.time[adcnumber].timefull[channum];
    }

  // TAC5: I2N - I2S
  if(id == 23)
    {
      double tacraw = bdecay.adc[adcnumber].channel[channum] + random3->Rndm();
      if(tacraw > bdecayv.tac.thresh && tacraw < bdecayv.tac.uld) {
	bdecay.tac.i2ni2s = tacraw;
      } else {
	bdecay.tac.i2ni2s = 99999;
      }      
      
      bdecay.tac.i2ni2stime = bdecay.time[adcnumber].timefull[channum];
    }
  
  // TO SeGAs - valid only for e16032
  if(id >= 48 && id < 64) {
    
    int det = id - 48; // Count from 0 - no need for additional 1
    int pos = (static_cast<int>(id - 48) / 4); // position counting from 0
    
    // raw energy
    double eraw = bdecay.adc[adcnumber].channel[channum] + random3->Rndm();
    bdecay.sega.energy[det] = eraw;
    double ecal = bdecayv.sega.square[det]*eraw*eraw + bdecayv.sega.slope[det]*eraw + bdecayv.sega.intercept[det];    

    // calibrated energy
    if(ecal > bdecayv.sega.thresh[det] && ecal < bdecayv.sega.uld[det]) {
      if(!bdecayv.hit.sega) {
	bdecayv.hit.sega = 1;
      }
      bdecay.sega.ecal[det] = ecal;
      //bdecay.sega.ecalab[pos] += ecal; //addbacks apply to clovers
      bdecay.sega.mult++;
    }

    // time
    bdecay.sega.time[det] = bdecay.time[adcnumber].timefull[channum];
    // calibrated time
    //bdecay.sega.timecal[det] = bdecay.sega.time[det] - bdecayv.sega.tdiffdynode[det]; //No dynode tdiff

  } // End SeGA

  
  // PSPMT anodes
   
  // TO e16032
  if( (id >= 64 && id < 208) || (id >= 208 && id < 320) )
    {
      //e17011
      //int rownum = adcnumber - 1;// Anodes start in the 2nd ADC (counting from 0)
      //e16032
      int rownum = adcnumber - 4; //Anodes start in the 5th ADC (!!!!Counting from 0!!!! - computer) 
      
      // x and y locations on 16x16 anode, 0 indexed
      int xpix = channum; 
      int ypix = 15 - rownum;

      // Pixel number, 0 indexed
      //e16032
      int pix = ypix*npspmt + xpix;//so pix counts from 0 to 255

      // if(id == 285)
      // 	cout << pix << endl;

      // Sanity check
      if(pix > 255)//add 1 in pix so it counts 1 to 256; remove 1 to count from 0 to 255
	{
	  cout << "Pixel > 255 -- Problem with pixel number in PSPMT unpacker" << endl;
	}
      if(pix < 0)
	{
	  cout << "Pixel < 0 -- Problem with pixel number in PSPMT unpacker" << endl;
	}

      // Trace analysis
      int overflow = 0;
      if(trace.size() > 0)
	{
	  // module[adcnumber].utils.CheckOverflowUnderflow(trace,4,module[adcnumber].GetMaxVal());
	  // overflow = module[adcnumber].utils.GetOverflow();
	  overflow = CheckTraceOverflow(trace,module[adcnumber].GetMaxVal());
	  if(overflow == 1)
	    {
	      bdecay.pspmt.aoverflowcount++;
	    }
	}
      
      bdecay.pspmt.aoverflow[pix] = overflow;

      bdecay.pspmt.amultraw++;

      // Time
      double time = bdecay.time[adcnumber].timefull[channum];
      // Calibrated anode time
      double timecal = time - bdecayv.pspmt.tdiffdynode[pix];

      // Due to some odd events where the anodes have times but no energies, lets make sure that the
      // anode energy is reasonable before incremementing the mult. The conditon for a good anode event
      // is |t_anode-t_dynode| < 100 AND athresh < e_anode < auld.
      // --ASC 3/19/2020
      double eraw = bdecay.adc[adcnumber].channel[channum] + random3->Rndm();
      bdecay.pspmt.aenergy[pix] = eraw;
      double ecal = bdecayv.pspmt.aslope[pix]*eraw + bdecayv.pspmt.aoffset[pix];

      //This is currently returning integer values which could be hugely consequential
      //if(abs(timecal - bdecay.pspmt.dytime) < 100)
      //Fixing with std::
      if(std::abs(timecal - bdecay.pspmt.dytime) < 100)
	//This time check is quite important so, we don't have ridiculous time diff btw anode and dynode
	{
	  if(ecal > bdecayv.pspmt.athresh[pix] && ecal < bdecayv.pspmt.auld[pix])
	    {      
	      if(!bdecay.pspmt.ahit[pix]) // Save only the first anode hit info
		{
		  bdecay.pspmt.ahit[pix] = 1;
		  bdecay.pspmt.aecal[pix] = ecal;
		  bdecay.pspmt.asum += ecal;
		  bdecay.pspmt.atime[pix] = time;
		  bdecay.pspmt.atimecal[pix] = timecal;
		  bdecay.pspmt.amult++;
		}

	      bdecay.pspmt.pixelmult[pix]++; // But do increment pixel mult
	    } // End threshold and uld check

	  // Reset overflows to have a high energy
	  // Don't produce a hit because ecal < thresh
	  if(!bdecay.pspmt.ahit[pix]) // Save only the first anode hit info
	    {
	      if(bdecay.pspmt.aoverflow[pix] == 1)
		{
		  bdecay.pspmt.ahit[pix] = 1;
		  bdecay.pspmt.aecal[pix] = 99999;
		  bdecay.pspmt.asum += 99999;
		  bdecay.pspmt.atime[pix] = time;
		  bdecay.pspmt.atimecal[pix] = timecal;
		  bdecay.pspmt.amult++;
		}
	      
	      bdecay.pspmt.pixelmult[pix]++; // But do increment pixel mult
	    } // End overflow check
	  
	} // End of time check with dynode
      
      // Sanity check for anode variables again!!!!
      // for(int i = 0; i<256; i++){
      // 	if(bdecayv.pspmt.aslope[i]!= 0){
      // 	  //cout<<"anode " <<i<<" time dev: "<<bdecayv.pspmt.tdiffdynode[i]<<" slope: "<<bdecayv.pspmt.aslope[i]<<" intercept: "<<bdecayv.pspmt.aoffset[i]<<" thresh: "<<bdecayv.pspmt.athresh[i]<<" uld: " << bdecayv.pspmt.auld[i]<<" aenergy: "<<bdecay.pspmt.aenergy[i]<<" aecal: "<<bdecay.pspmt.aecal[i]<<" ahit: "<<bdecay.pspmt.ahit[i]<<" amult: "<<bdecay.pspmt.amult<<endl;

      // 	  cout<<"anode " <<i<<" time dev: "<<bdecayv.pspmt.tdiffdynode[i]<<" slope: "<<bdecayv.pspmt.aslope[i]<<" intercept: "<<bdecayv.pspmt.aoffset[i]<<" thresh: "<<bdecayv.pspmt.athresh[i]<<" uld: " << bdecayv.pspmt.auld[i]<<" aenergy: "<<bdecay.pspmt.aenergy[i]<<" aecal: "<<bdecay.pspmt.aecal[i]<<endl;
      // 	}
      // }
      
      
    } // End anodes

  delete random3; // cleanup

  return 0;

}

// UnpackDynodeFit
//
// Function:
//   - unpack PSPMT dynode fit results into bdecay parameters
//
// Params:
//   - beta decay parameters (i.e. detector classes)
//   - vector of fit results
//
// Return:
//   - none
//
void TUnpacker::UnpackDynodeFit(TBetaDecayParams &bdecay, std::vector<RootHitExtension> fits)
{
  for(unsigned i=0; i<fits.size(); i++)
    {
      if (fits[i].haveExtension){
	RootFit1Info fit1 = fits[i].onePulseFit;
	RootFit2Info fit2 = fits[i].twoPulseFit;

	// Assign single pulse fit results
	bdecay.pspmt.dyE1_single = fit1.pulse.amplitude;
	bdecay.pspmt.dyT1_single = fit1.pulse.position;

	// Comment out for template fitting
	bdecay.pspmt.dyE1steepness_single = fit1.pulse.steepness;
	bdecay.pspmt.dyE1decay_single = fit1.pulse.decayTime;
      
	bdecay.pspmt.dyoffset_single = fit1.offset;
	bdecay.pspmt.dychisq_single = fit1.chiSquare;

	// Check organization for defining E2 to be the second pulse
	if(fit2.pulses[0].position < fit2.pulses[1].position)
	  {
	    bdecay.pspmt.dyE1_double = fit2.pulses[0].amplitude;
	    bdecay.pspmt.dyT1_double = fit2.pulses[0].position;

	    // Comment out for template fitting
	    bdecay.pspmt.dyE1steepness_double = fit2.pulses[0].steepness;
	    bdecay.pspmt.dyE1decay_double = fit2.pulses[0].decayTime;
	
	    bdecay.pspmt.dyE2_double = fit2.pulses[1].amplitude;
	    bdecay.pspmt.dyT2_double = fit2.pulses[1].position;

	    // Comment out for template fitting
	    bdecay.pspmt.dyE2steepness_double = fit2.pulses[1].steepness;
	    bdecay.pspmt.dyE2decay_double = fit2.pulses[1].decayTime;
	
	  }
	else
	  {	
	    bdecay.pspmt.dyE1_double = fit2.pulses[1].amplitude;
	    bdecay.pspmt.dyT1_double = fit2.pulses[1].position;

	    // Comment out for template fitting
	    bdecay.pspmt.dyE1steepness_double = fit2.pulses[1].steepness;
	    bdecay.pspmt.dyE1decay_double = fit2.pulses[1].decayTime;
	
	    bdecay.pspmt.dyE2_double = fit2.pulses[0].amplitude;
	    bdecay.pspmt.dyT2_double = fit2.pulses[0].position;

	    // Comment out for template fitting
	    bdecay.pspmt.dyE2steepness_double = fit2.pulses[0].steepness;
	    bdecay.pspmt.dyE2decay_double = fit2.pulses[0].decayTime;
	  }
      
	bdecay.pspmt.dychisq_double = fit2.chiSquare;
	bdecay.pspmt.dyoffset_double = fit2.offset;
	bdecay.pspmt.dytdiffE1E2 = bdecay.pspmt.dyT2_double - bdecay.pspmt.dyT1_double;

	// Calculate real amplitudes
	bdecay.pspmt.dyE1real_single = DDAS::pulseAmplitude(bdecay.pspmt.dyE1_single,bdecay.pspmt.dyE1steepness_single,bdecay.pspmt.dyE1decay_single,bdecay.pspmt.dyT1_single);
	bdecay.pspmt.dyE1real_double = DDAS::pulseAmplitude(bdecay.pspmt.dyE1_double,bdecay.pspmt.dyE1steepness_double,bdecay.pspmt.dyE1decay_double,bdecay.pspmt.dyT1_double);
	bdecay.pspmt.dyE2real_double = DDAS::pulseAmplitude(bdecay.pspmt.dyE2_double,bdecay.pspmt.dyE2steepness_double,bdecay.pspmt.dyE2decay_double,bdecay.pspmt.dyT2_double);
      }

    }
}

// UnpackAnodePosition
//
// Function:
//   - determine the anode interaction position from the 2D energy map
//   - unnpack the fit position into bdecay parameters
//
// Params:
//   - beta decay parameters (i.e. detector classes)
//
// Return:
//   - none
//
void TUnpacker::UnpackAnodePosition(TBetaDecayParams &bdecay)
{
  
  using std::cout;
  using std::endl;
  
  TLinearSolver solver;
  double anodexproj[npspmt]; // x projection of 2D energy plot
  double anodeyproj[npspmt]; // y projection of 2D energy plot

  // Make sure things are cleared
  solver.Reset();
  for(int i=0; i<npspmt; i++)
    {
      anodexproj[i] = 0.;
      anodeyproj[i] = 0.;
    }

  // Fill the projection histograms
  for(int i=0; i<nanodes; i++)
    {
      if(bdecay.pspmt.aecal[i] > 0)
	{
	  int xpix = i % npspmt;
	  int ypix = i / npspmt;
	  if(ypix > 0 && ypix < npspmt-1) {
	    anodexproj[xpix] += bdecay.pspmt.aecal[i];
	    anodeyproj[ypix] += bdecay.pspmt.aecal[i];
	  }
	} // End anode energy check
    } // End loop over anodes
  /************************************************************************************
   * Anode position determination using the linear fitter. Positions
   * in x and y are found by fitting a Lorentzian lineshape to projections
   * along the x and y axes. Note that the reciprocal of the Lorentzian,
   *    1/L(x) = ((x-x0)^2 + sigma)/(A*sigma)
   * (where for convienience sigma = gamma^2) has the form
   *    1/L(x) = ax^2 + bx + c,
   * where
   *    a = 1/(A*sigma)
   *    b = -2*x0/(A*sigma)
   *    c = (x0^2 + sigma)/(A*sigma)
   * This function is linear in the parameters a, b, and c! For a fit function
   * f(x), the chisq is given by
   *    chisq = sum[ wi(yi - f(xi))^2 ]
   * where w is the weight, y is the observed data. The partial derivatives reduce 
   * to a matrix equation Mu = v where u are the fit parameters and v depends on the
   * observed data. The parameters can be determined by solving the matrix eqn.
   * In this case, the data is the reciprocal of the projection along the axis 
   * of interest, the weight is chosen to be yi^3 to account for both the 
   * data and the transformation and the fit function f(x) is the quadratic
   * given above.
   *
   * Fitting procedure:
   *   1. Take projection along the x- or y-axis
   *   2. Define matrix and vectors M, u, v
   *   3. Solve matrix equation
   *   4. Transform fit parameters back into Lorentzian parameters:
   *       x0 = -b/(2*a)
   *       sigma = (c - a*x0^2)/a
   *       A = 1/(a*sigma)
   *
   * Note: amplitudes are for the projections not the overall 2D distribution
   * though the x0 and sigma parameters are consistent with the 2D fit. One
   * can similarly linearize the 2D fit if the amplitude is important, but 
   * this is a much more tedious algebraic system to solve if all you're 
   * interested in is the position.
   ************************************************************************************/
  // Fits
  const int dim = 3;
  // Solutions from the solver (for plotting)
  double solverx[dim] = {0};
  double solvery[dim] = {0};
  // Lorentz solutions from the solver
  double Ampx = 0;
  double x0 = -1;
  double sigmax = 0;
  double Ampy = 0;
  double y0 = -1;
  double sigmay = 0;

  bool displaycondition = false;
  if(bdecay.pspmt.dyE1real_double > 30000 && bdecay.pspmt.dyE2real_double > 10000)
    displaycondition = true;
  
  // X part  
  // Calculate matrix elements and result vector from the data
  solver.dim = dim; // Number of fit parameters
  for(Int_t i=0; i<npspmt; i++)
    { // Data bins only
      if(anodexproj[i] > 0)
	{
	  Double_t x = i + 0.5;
	  Double_t d = anodexproj[i];
	  Double_t y = 1.0/d;
	  Double_t w = d*d*d; // Weight choice to account for data and transformation
	  solver.chisq = w*y*y;
	  solver.matrix[0][0] += w;
	  solver.matrix[1][0] += w*x;
	  solver.matrix[2][0] += w*x*x;
	  solver.matrix[2][1] += w*x*x*x;
	  solver.matrix[2][2] += w*x*x*x*x;
	  solver.vector[0] += w*y;
	  solver.vector[1] += w*y*x;
	  solver.vector[2] += w*y*x*x;
	}
    }
  solver.matrix[0][1] = solver.matrix[1][0];
  solver.matrix[1][1] = solver.matrix[2][0];
  solver.matrix[0][2] = solver.matrix[2][0];
  solver.matrix[1][2] = solver.matrix[2][1];  
  
  if(solver.SolveLinEq() != 0)
    {
      // cout << "X: Matrix is not invertable! The system has no solution." << endl;
      bdecay.pspmt.lxamp = ERR_MINV;
      bdecay.pspmt.lxpos = ERR_MINV;
      bdecay.pspmt.lxgamma = ERR_MINV;
    }
  else
    {
      for(int i=0; i<solver.dim; i++)
	{
	  solver.chisq -= solver.solution[i]*solver.vector[i];
	  solverx[i] = solver.solution[i];
	}
      
      // Transform back to Lorentzian parameters
      x0 = -solver.solution[1]/(2.0*solver.solution[2]);
      sigmax = (solver.solution[0]-solver.solution[2]*x0*x0)/solver.solution[2];
      Ampx = 1.0/(solver.solution[2]*sigmax);
      sigmax = sqrt(sigmax);
      
      // Result is outside anode pixel range or the quadratic fit has no minimum
      if(x0 < 0) {
	bdecay.pspmt.lxamp = ERR_BADPOS;
	bdecay.pspmt.lxpos = ERR_BADPOS;
	bdecay.pspmt.lxgamma = ERR_BADPOS;
      } else if(x0 > npspmt) {
	bdecay.pspmt.lxamp = ERR_BADPOS;
	bdecay.pspmt.lxpos = ERR_BADPOS;
	bdecay.pspmt.lxgamma = ERR_BADPOS;
      } else if(solver.solution[2] < 0) {
	bdecay.pspmt.lxamp = ERR_CONCDOWN;
	bdecay.pspmt.lxpos = ERR_CONCDOWN;
	bdecay.pspmt.lxgamma = ERR_CONCDOWN;
      } else {
	bdecay.pspmt.lxamp = Ampx;
	bdecay.pspmt.lxpos = x0;
	bdecay.pspmt.lxgamma = sigmax;
      }
       
      if(displaylinfitinfo && displaycondition)
	{
	  cout << "lineqsolver chisq: " << solver.chisq << endl;
	  cout << "lineqsolver fit par: " << solver.solution[2] << " b: " << solver.solution[1] << " c: " << solver.solution[0] << endl;
	  cout << "lineqsolver lor par: " << bdecay.pspmt.lxamp << " " << bdecay.pspmt.lxpos << " " << bdecay.pspmt.lxgamma << endl;
	  cout << "lxpos        : " << bdecay.pspmt.lxpos << endl;
	}
    }

  solver.Reset();
    
  // Y part
  // Calculate matrix elements and result vector from the data
  solver.dim = dim;
  for(Int_t i=0; i<npspmt; i++)
    {
      if(anodeyproj[i] > 0)
	{
	  Double_t x = i + 0.5;
	  Double_t d = anodeyproj[i];
	  Double_t y = 1.0/d;
	  Double_t w = d*d*d;
	  solver.chisq = w*y*y;
	  solver.matrix[0][0] += w;
	  solver.matrix[1][0] += w*x;
	  solver.matrix[2][0] += w*x*x;
	  solver.matrix[2][1] += w*x*x*x;
	  solver.matrix[2][2] += w*x*x*x*x;
	  solver.vector[0] += w*y;
	  solver.vector[1] += w*y*x;
	  solver.vector[2] += w*y*x*x;
	}
    }
  solver.matrix[0][1] = solver.matrix[1][0];
  solver.matrix[1][1] = solver.matrix[2][0];
  solver.matrix[0][2] = solver.matrix[2][0];
  solver.matrix[1][2] = solver.matrix[2][1];  
  
  if(solver.SolveLinEq() != 0)
    {
      // cout << "Y: Matrix is not invertable! The system has no solution." << endl;
      bdecay.pspmt.lyamp = ERR_MINV;
      bdecay.pspmt.lypos = ERR_MINV;
      bdecay.pspmt.lygamma = ERR_MINV;
    }
  else
    {      
      for(int i = 0; i < solver.dim; i++)
	{
	  solver.chisq -= solver.solution[i]*solver.vector[i];
	  solvery[i] = solver.solution[i];
	}
      
      // Transform back to Lorentzian parameters
      y0 = -solver.solution[1]/(2.0*solver.solution[2]);
      sigmay = (solver.solution[0]-solver.solution[2]*y0*y0)/solver.solution[2];
      Ampy = 1.0/(solver.solution[2]*sigmay);
      sigmay = sqrt(sigmay);

      // Result is outside anode pixel range or the quadratic fit has no minimum
      if(y0 < 0) {
	bdecay.pspmt.lyamp = ERR_BADPOS;
	bdecay.pspmt.lypos = ERR_BADPOS;
	bdecay.pspmt.lygamma = ERR_BADPOS;
      } else if(y0 > npspmt) {
	bdecay.pspmt.lyamp = ERR_BADPOS;
	bdecay.pspmt.lypos = ERR_BADPOS;
	bdecay.pspmt.lygamma = ERR_BADPOS;
      } else if(solver.solution[2] < 0) {
	bdecay.pspmt.lyamp = ERR_CONCDOWN;
	bdecay.pspmt.lypos = ERR_CONCDOWN;
	bdecay.pspmt.lygamma = ERR_CONCDOWN;
      } else {
	bdecay.pspmt.lyamp = Ampy;
	bdecay.pspmt.lypos = y0;
	bdecay.pspmt.lygamma = sigmay;
      }
       
      if(displaylinfitinfo && displaycondition)
	{
	  cout << "lineqsolver chisq: " << solver.chisq << endl;
	  cout << "lineqsolver fit par: " << solver.solution[2] << " b: " << solver.solution[1] << " c: " << solver.solution[0] << endl;
	  cout << "lineqsolver lor par: " << bdecay.pspmt.lyamp << " " << bdecay.pspmt.lypos << " " << bdecay.pspmt.lygamma << endl;
	  cout << "lypos        : " << bdecay.pspmt.lypos << endl;
	} // End display fit info  
    }

  /*******************************************************************************
   *
   * Display information for the fitting routines
   *
   * Would be best to put this into a function, need to include fit parameters
   * for linearized method in bdecay class... can rescope fit results, include 
   * condition in the draw function (still hardcoded...)... Task for another time
   *
   *******************************************************************************/
  // Gaussian approximation
  // See Fisher and Naidu, "A comparison of algorithms for subpixel peak detection"
  if(drawfits && displaycondition)
    {
      double max = -1;
      int maxx = -1;
      for(auto i=1; i<npspmt-1; i++)
	{
	  if(anodexproj[i] > max)
	    {
	      max = anodexproj[i];
	      maxx = i;
	    }
	}
      cout << "proj max " << max << " x " << maxx << endl;
      double lxm = log(anodexproj[maxx-1]);
      double tlx = 2.0*log(anodexproj[maxx]);
      double lxp = log(anodexproj[maxx+1]);
      double deltax = 0.5*(lxm-lxp)/(lxm-tlx+lxp);

      max = -1;
      int maxy = -1;
      for(auto i=2; i<npspmt-2; i++)
	{
	  if(anodeyproj[i] > max)
	    {
	      max = anodeyproj[i];
	      maxy = i;
	    }
	}
      cout << "proj max " << max << " y " << maxy << endl;
      double lym = log(anodeyproj[maxy-1]);
      double tly = 2.0*log(anodeyproj[maxy]);
      double lyp = log(anodeyproj[maxy+1]);
      double deltay = 0.5*(lym-lyp)/(lym-tly+lyp);

      cout << "GAUSSIAN: " << maxx+deltax << " " << maxy+deltay << endl;
    }
  
  // Draw the 2D fits (if its an ion)
  if(drawfits && displaycondition)
    {
      cout << "--> Pin01 ECal: " << bdecay.pin01.ecal << " Pin02 ECal: " << bdecay.pin02.ecal << " Pin03 Ecal: " << bdecay.pin03.ecal << " DyECal: " << bdecay.pspmt.dyecal << " DyE1real: " << bdecay.pspmt.dyE1real_single << endl;
      
      if(theApp==NULL) theApp = new TApplication("theApp",0,0);
      TCanvas c("c","c",800,500);
      c.cd();
      TH2D hanodeecal("Anode_ECal","Anode_ECal",npspmt,0,npspmt,npspmt,0,npspmt);

      // Fill the projection histograms
      Double_t max = -1;
      Double_t maxx = -1;
      Double_t maxy = -1;
      for(int i=0; i<nanodes; i++)
	{
	  if(bdecay.pspmt.aecal[i] > 0)
	    {
	      int xpix = i % npspmt;
	      int ypix = i / npspmt;
	      
	      if(ypix > 0 && ypix < npspmt-1) {
		hanodeecal.Fill(xpix,ypix,bdecay.pspmt.aecal[i]);

		if(bdecay.pspmt.aecal[i] > max)
		  {
		    max = bdecay.pspmt.aecal[i];
		    maxx = xpix;
		    maxy = ypix;
		  }
	      }
	      
	    } // End anode energy check
	} // End loop over anodes
      
      // ROOT iterative fit function
      TF2 fit2dlorentz("Fit_2D_Lorentz",f2DLorentz,0,npspmt,1,npspmt-1,5);
      fit2dlorentz.SetParameter(0,max);
      fit2dlorentz.SetParameter(1,14);
      fit2dlorentz.SetParameter(2,10);
      fit2dlorentz.SetParameter(3,maxx+0.5);
      fit2dlorentz.SetParLimits(3,fmax(maxx-3.,0.001),fmin(maxx+3.,15.999));
      fit2dlorentz.SetParameter(4,maxy+0.5);
      fit2dlorentz.SetParLimits(4,fmax(maxy-3.,0.001),fmin(maxy+3.,15.999));
      fit2dlorentz.SetLineColor(kRed);
      fit2dlorentz.SetLineWidth(2);
      hanodeecal.Fit("Fit_2D_Lorentz","QNEMRL");
      for(auto i=0; i<5; i++)
	{
	  cout << "Parameter " << i << ": " << fit2dlorentz.GetParameter(i) << endl;
	}
      cout << "2D fit position: " << fit2dlorentz.GetParameter(3) << ", " << fit2dlorentz.GetParameter(4) << endl;

      // Linearized projection method as 2D
      TF2 fit2dproj("Fit_2D_Proj",f2DLorentz,0,npspmt,0,npspmt,5);
      fit2dproj.SetParameter(0,1.0); // Doesn't matter
      fit2dproj.SetParameter(1,sigmax);
      fit2dproj.SetParameter(2,sigmay);
      fit2dproj.SetParameter(3,x0);
      fit2dproj.SetParameter(4,y0);
      fit2dproj.SetLineColor(kBlack);
      fit2dproj.SetLineWidth(2);
      
      hanodeecal.Draw("colz");
      fit2dlorentz.Draw("same");
      fit2dproj.Draw("same");
      c.Update();

      // Draw the projections
      // X
      TCanvas c2("c2","c2",800,500);
      c2.Divide(2,1);
      TH1D* hpx = hanodeecal.ProjectionX();
      TH1D* hpxinv = (TH1D*)hpx->Clone("hpxinv");
      Int_t bins = hpx->GetNbinsX();
      for(auto i=1; i<=bins; i++)
	{
	  if(hpxinv->GetBinContent(i) > 0)
	    {
	      Double_t tmp = 1.0/hpxinv->GetBinContent(i);
	      hpxinv->SetBinContent(i,tmp);
	    }
	}
      TF1 projfitx("Projection_Fit_X",f1DLorentz,0,npspmt,3);
      projfitx.SetParameter(0,Ampx);
      projfitx.SetParameter(1,sigmax);
      projfitx.SetParameter(2,x0);

      TF1 quadfitx("Quad_Fit_X","[0]+[1]*x+[2]*x*x",0,npspmt);		   
      quadfitx.SetParameter(0,solverx[0]);
      quadfitx.SetParameter(1,solverx[1]);
      quadfitx.SetParameter(2,solverx[2]);
      
      c2.cd(1);
      hpx->Draw("hist");
      projfitx.Draw("same");
      c2.cd(2);
      hpxinv->Draw("hist");
      quadfitx.Draw("same");

      // Y
      TCanvas c3("c3","c3",800,500);
      c3.Divide(2,1);
      TH1D* hpy = hanodeecal.ProjectionY();
      TH1D* hpyinv = (TH1D*)hpy->Clone("hpyinv");
      bins = hpy->GetNbinsX();
      for(auto i=1; i<=bins; i++)
	{
	  if(hpyinv->GetBinContent(i) > 0)
	    {
	      Double_t tmp = 1.0/hpyinv->GetBinContent(i);
	      hpyinv->SetBinContent(i,tmp);
	    }
	}
      TF1 projfity("Projection_Fit_Y",f1DLorentz,0,npspmt,3);
      projfity.SetParameter(0,Ampy);
      projfity.SetParameter(1,sigmay);
      projfity.SetParameter(2,y0);

      TF1 quadfity("Quad_Fit_Y","[0]+[1]*x+[2]*x*x",0,npspmt);		   
      quadfity.SetParameter(0,solvery[0]);
      quadfity.SetParameter(1,solvery[1]);
      quadfity.SetParameter(2,solvery[2]);
      
      c3.cd(1);
      hpy->Draw("hist");
      projfity.Draw("same");
      c3.cd(2);
      hpyinv->Draw("hist");
      quadfity.Draw("same");

      // Draw here
      theApp->Run(kTRUE);
    } // End draw fits

}

// UnpackAnodePositionGaus
//
// Function:
//   - determine the anode interaction position from the 2D energy map using simple gaussian approx
//   - unnpack the fit position into bdecay parameters
//
// Params:
//   - beta decay parameters (i.e. detector classes)
//
// Return:
//   - none
//
void TUnpacker::UnpackAnodePositionGaus(TBetaDecayParams &bdecay)
{

  using std::cout;
  using std::endl;
  
  double anodexproj[npspmt]; // x projection of 2D energy plot
  double anodeyproj[npspmt]; // y projection of 2D energy plot

  // Make sure things are cleared
  for(int i=0; i<npspmt; i++)
    {
      anodexproj[i] = 0.;
      anodeyproj[i] = 0.;
    }

  // Fill the projection histograms
  for(int i=0; i<nanodes; i++)
    {
      if(bdecay.pspmt.aecal[i] > 0)
	{
	  int xpix = i % npspmt;
	  int ypix = i / npspmt;
	  if(ypix > 0 && ypix < npspmt-1) {
	    anodexproj[xpix] += bdecay.pspmt.aecal[i];
	    anodeyproj[ypix] += bdecay.pspmt.aecal[i];
	  }
	} // End anode energy check
    } // End loop over anodes

  // Gaussian approximation
  // See Fisher and Naidu, "A comparison of algorithms for subpixel peak detection"
  double max = -1;
  int maxx = -1;
  for(auto i=1; i<npspmt-1; i++)
    {
      if(anodexproj[i] > max)
	{
	  max = anodexproj[i];
	  maxx = i;
	}
    }
  double lxm = log(anodexproj[maxx-1]);
  double tlx = 2.0*log(anodexproj[maxx]);
  double lxp = log(anodexproj[maxx+1]);
  double deltax = 0.5*(lxm-lxp)/(lxm-tlx+lxp);
  double x0 = maxx+deltax;

  // Result is outside anode pixel range
  if(x0 < 0) x0 = ERR_BADPOS;
  if(x0 > npspmt) x0 = ERR_BADPOS;
      
  bdecay.pspmt.lxpos = x0;

  max = -1;
  int maxy = -1;
  for(auto i=2; i<npspmt-2; i++)
    {
      if(anodeyproj[i] > max)
	{
	  max = anodeyproj[i];
	  maxy = i;
	}
    }
  double lym = log(anodeyproj[maxy-1]);
  double tly = 2.0*log(anodeyproj[maxy]);
  double lyp = log(anodeyproj[maxy+1]);
  double deltay = 0.5*(lym-lyp)/(lym-tly+lyp);
  double y0 = maxy+deltay;

  // Result is outside anode pixel range
  if(y0 < 0) y0 = ERR_BADPOS;
  if(y0 > npspmt) y0 = ERR_BADPOS;
      
  bdecay.pspmt.lypos = y0;   

}

//Create new overflow function
// new ASC 1/6/22
// CheckTraceOverflow
//
// Function:
//   - check if trace is an overflow (saturates ADC)
//
// Params:
//   - trace (vector of shorts)
//   - upper limit to check (depends on ADC type)
//
// Return:
//   - overflow flag (1 if overflowed, 0 if not, -1 if trace length is 0)
//
int TUnpacker::CheckTraceOverflow(std::vector<UShort_t> trace, int lim)
{
  int flag = 0;

  if(trace.size() > 0) {
    for(UInt_t i=0; i<trace.size(); i++) {
      if(trace[i] >= lim) {
	flag = 1;
      }
    }
  } else {
    flag = -1;
  }
  return flag;
}
