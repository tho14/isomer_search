#include <iostream>

#include "TModule.h"

// Class constructor
TModule::TModule()
{
  fType = -2;
  fADCFreq = 0;
  fScale = 0;
  fClockCal = 0;
  fMaxVal = 0;
}

// SetupModule
//
// Function:
//   - sets up Pixie16 module and module utilities
//
// Params:
//   - the global ADC number
//   - the module frequency
//
// Return:
//   - none
//
void TModule::SetupModule(int adcfreq, int adcdepth)
{
  // Types of modules
  //Similar designations as in e16032
  // -2 = unknown
  // -1 = empty slot
  //  0 = 100-12 (100 MSPS, 12 bit)
  //  1 = 250-14 (250 MSPS, 14 bit)
  //  2 = 250-16 (250 MSPS, 16 bit)
  //  3 = 500-12 (500 MSPS, 12 bit)
  //  4 = 500-14 (500 MSPS, 14 bit)
  //
  // Module scaling factors assume 12 bits
  //
  if(adcfreq == 0 && adcdepth == 0)
    {
      fType = -1;
      fADCFreq = 0;
      fScale = 0.;
      fClockCal = 0;
      fMaxVal = 0;
    }
  if(adcfreq == 100 && adcdepth == 12)
    {
      fType = 0;
      fADCFreq = 100;
      fScale = 1.;
      fClockCal = 10.;
      fMaxVal = 4092;
    }
  if(adcfreq == 250 && adcdepth == 14)
    {
      fType = 1;
      fADCFreq = 250;
      fScale = 0.25;
      fClockCal = 4.;
      fMaxVal = 16380;
    }    
  if(adcfreq == 250 && adcdepth == 16)
    {
      fType = 2;
      fADCFreq = 250;
      fScale = 0.0625;
      fClockCal = 4.;
      fMaxVal = 65532;
    }      
  if(adcfreq == 500 && adcdepth == 12)
    {
      fType = 3;
      fADCFreq = 500;
      fScale = 1.;
      fClockCal = 2.;
      fMaxVal = 4092;
    }
  if(adcfreq == 500 && adcdepth == 14)
    {
      fType = 4;
      fADCFreq = 500;
      fScale = 0.25;
      fClockCal = 2.;
      fMaxVal = 16380;
    }
  
}
