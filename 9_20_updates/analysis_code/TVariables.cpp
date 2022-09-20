#include <fstream>
#include <iostream>

#include "TVariables.h"

using std::cout;
using std::endl;
using std::ifstream;

// Clock
void TClockCalibrator::Initialize()
{
  calib        = 1.;
  scale        = 1.;
  max          = 1000.;
  max_daughter = 10;
}

// PIN
void TPINCalibrator::Initialize()
{
  slope     = 1.;
  intercept = 0.;
  thresh    = 0.;
  uld       = 30000.;
}

// TAC
void TTACCalibrator::Initialize()
{
  pin01i2ncorr = 1.;
  pin01i2noffset = 0.;
  pin01i2scorr = 1.;
  pin01i2soffset = 0.;  
  pin02i2ncorr = 1.;
  pin02i2noffset = 0.;
  pin02i2scorr = 1.;
  pin02i2soffset = 0.;
  pin01xfpcorr = 1.;
  pin02xfpcorr = 1.;
  pin01rfcorr = 1.;
  i2ni2scorr = 1.;
  thresh = 5.;
  //uld = 30000.;
  //TO 6/14/22
  uld = 60000.;
}

// DSSD
void TDSSDCalibrator::Initialize()
{
  for(int i=0; i<ndssd; i++)
    {
      hioffset[i] = 0.;
      hislope[i]  = 1.;
      looffset[i] = 0.;
      loslope[i]  = 1.;
      hithresh[i] = 0.;
      hiuld[i] = 30000.;
      lothresh[i] = 0.;
      lould[i] = 30000.;
    }
}

// SSSD
void TSSSDCalibrator::Initialize()
{
  for(int i=0; i<nsssd; i++)
    {
      hioffset[i] = 0.;
      hislope[i]  = 1.;
      looffset[i] = 0.;
      loslope[i]  = 1.;
      hithresh[i] = 0.;
      hiuld[i] = 30000.;
      lothresh[i] = 0.;
      lould[i] = 30000.;
    }
}

// PSPMT
void TPSPMTCalibrator::Initialize()
{
  // Anodes
  for(int i = 0; i<nanodes; i++)
    {
      aoffset[i] = 0.;
      aslope[i] = 1.;
      athresh[i] = 0.;//1 in calib file
      auld[i] = 60000.;//60000 in calib file
      tdiffdynode[i] = 0.;
    }
  // Dynode
  dyoffset = 0.;
  dyslope = 1.;
  dythresh = 0.;
  dyuld = 60000;

  for(int i = 0; i<nlabr3; i++){
    //TO Timewalk correction parameters due to low dynode energies for individual labr3-dynode pairs (For alignment to high energy time response centroids) - Parameters from fitting a the timewalk profile with a 4th order polynomial
    // dytwalkp0[i] = 0;
    // dytwalkp1[i] = 0;
    // dytwalkp2[i] = 0;
    // dytwalkp3[i] = 0;
    // dytwalkp4[i] = 0;

    //TO High energy time response centroids for individual labr3-dynode pairs
    dyhighcentroid[i] = 0;
    dylowcentroid[i] = 0;

    //Full time alignment
    // p0[i] = 0;
    // p1[i] = 0;
    // p2[i] = 0;
    // p3[i] = 0;
    // p4[i] = 0;
    // p5[i] = 0;
    // p6[i] = 0;
    // p7[i] = 0;
    // p8[i] = 0;
    // p9[i] = 0;
    // p10[i] = 0;
    // p11[i] = 0;
    // p12[i] = 0;
    // p13[i] = 0;
    
  }

}

// Hit
void THitFlag::Initialize()
{
  pin01 = 0;
  pin02 = 0;
  pin03 = 0;
  fronthi = 0;
  frontlo = 0;
  backhi = 0;
  backlo = 0;
  sssd01hi = 0;
  sssd01lo = 0;
  pspmt = 0;
  dynode = 0;
  clover = 0;
  sega = 0;
  labr3 = 0;

}  

// Correlator
void TCorrelatorCalibrator::Initialize()
{
  reset = 1; // Reset correlator on first pass
  resetclock = 0;
  minimplant = 50.;  // ms
  corrwindow = 100.; // ms
}

// PID
void TPIDCalibrator::Initialize()
{
  de1scale = 1.;
  de1offset = 0.;
  de1tofcorr = 0.;
  de1tofoffset = 0.;
  de2scale = 1.;
  de2offset = 0.;
  de2tofcorr = 0.;
  de2tofoffset  = 0.;
}

// Clover
void TCloverCalibrator::Initialize()
{
  for(int i=0; i<totcrystals; i++)
    {
      square[i] = 0;
      slope[i] = 1.;
      intercept[i] = 0.;
      thresh[i] = 0.;
      uld[i] = 30000.;
      tdiffdynode[i] = 0;
    }
}

// SeGA
void TSeGACalibrator::Initialize() {
  for(int i=0; i<nsega; i++)
    {
      square[i] = 0.;
      slope[i] = 1.;
      intercept[i] = 0.;
      thresh[i] = 0.;
      uld[i]  = 30000.;
    }
}

// LaBr3
void TLaBr3Calibrator::Initialize()
{
  for(int i=0; i<nlabr3;i++)
    {
      square[i] = 0;
      slope[i] = 1.;
      intercept[i] = 0.;
      thresh[i] = 0.;
      uld[i] = 30000.;
      tdiffdynode[i] = 0;

      //TO Dynode timewalk correction parameters for individual labr3-dynode pairs
      // dytwalkp0[i] = 0;
      // dytwalkp1[i] = 0;
      // dytwalkp2[i] = 0;
      // dytwalkp3[i] = 0;
      // dytwalkp4[i] = 0;
    }
}

// Full set of beta decay variables
void TBetaDecayVariables::Initialize()
{
  clock.Initialize();
  clockisomer.Initialize();
  pin01.Initialize();
  pin02.Initialize();
  pin03.Initialize();
  front.Initialize();
  back.Initialize();
  tac.Initialize();
  hit.Initialize();
  corr.Initialize();
  pid.Initialize();
  pspmt.Initialize();
  clover.Initialize();
  sega.Initialize();
  labr3.Initialize();
}

// Read calibration files
void TBetaDecayVariables::ReadDSSD(std::string name)
{
  cout << "Reading DSSD calibration from: " << name << endl;

  ifstream calfile(name.c_str());
  
  int linenum = 0;

  int location = -1;
  double value = 0;
  
  if(!calfile)
    {
      cout << "Unable to open file " << name << endl;
    }
  else
    {
      //read in cal file until end of file
      while(calfile)
  	{
  	  if(calfile && (isdigit(calfile.peek())))
  	    {
  	      linenum++;
  	      calfile >> location >> value;

	      // cout << linenum << " " << location << " " << value << endl;
	      
  	      if(linenum < 41) front.hioffset[location-1] = value;
  	      else if(linenum < 81) back.hioffset[location-1] = value;
  	      else if(linenum < 121) front.hislope[location-1] = value;
  	      else if(linenum < 161) back.hislope[location-1] = value;
  	      else if(linenum < 201) front.hithresh[location-1] = value;
  	      else if(linenum < 241) back.hithresh[location-1] = value;
  	      else if(linenum < 281) front.hiuld[location-1] = value;
  	      else if(linenum < 321) back.hiuld[location-1] = value;
  	      else if(linenum < 361) front.loslope[location-1] = value;
  	      else if(linenum < 401) back.loslope[location-1] = value;
	      else if(linenum < 441) front.lothresh[location-1] = value;
  	      else if(linenum < 481) back.lothresh[location-1] = value;
  	      else if(linenum < 521) front.lould[location-1] = value;
  	      else if(linenum < 561) back.lould[location-1] = value;
  	    } // line read
  	  else
  	    {
  	      //ignore line
  	      calfile.ignore(1000,'\n');
  	    }
  	} // end while read
    }
  
  calfile.close();
}

void TBetaDecayVariables::ReadSSSD(std::string name)
{
  cout << "Reading SSSD calibration from: " << name << endl;

  ifstream calfile(name.c_str());
  
  int linenum = 0;
  int location = -1;
  double value = 0;

  if(!calfile)
    {
      cout << "Unable to open file " << name << endl;
    }
  else
    {
      // Read in cal file until end of file
      while(calfile)
	{
	  if(calfile && (isdigit(calfile.peek())))
	    {
	      linenum++;
	      calfile >> location >> value;

	      // cout << linenum << " " << location << " " << value << endl;
	      
	      if(linenum < 17) sssd01.hioffset[location-1] = value;
	      else if(linenum < 33) sssd01.hislope[location-1] = value;
	      else if(linenum < 49) sssd01.loslope[location-1] = value;
	      else if(linenum < 65) sssd01.loslope[location-1] = value;
	      else if(linenum < 81) sssd01.hithresh[location-1] = value;
	      else if(linenum < 97) sssd01.hiuld[location-1] = value;
	      else if(linenum < 113) sssd01.lothresh[location-1] = value;
	      else if(linenum < 129) sssd01.lould[location-1] = value;
	    }
	  else
	    {
	      //ignore line
	      calfile.ignore(1000,'\n');
	    }
	}
    }
  
  calfile.close();
}

void TBetaDecayVariables::ReadPSPMT(std::string name)
{
  cout << "Reading PSPMT calibration from: " << name << endl;

  ifstream calfile(name.c_str());
  
  int linenum = 0;
  int location = -1;
  double value = 0;

  if(!calfile)
    {
      cout << "Unable to open file " << name << endl;
    }
  else
    {
      // Read in cal file until end of file
      //e16032 - PSPMT calib file only includes slope; all other variables set to predefined values
      while(calfile)
	{
	  if(calfile && (isdigit(calfile.peek())))
	    {
	      linenum++;
	    
	      //if(linenum < 1025)
	      //	{
		  calfile >> location >> value;

		  // cout << linenum << " " << location << " " << value << endl;
		 
		  if(linenum < 257) pspmt.aslope[location-1] = value;
		  //else if(linenum < 513) pspmt.aoffset[location-1] = value;
		  //else if(linenum < 769) pspmt.athresh[location-1] = value;
		  //else if(linenum < 1025) pspmt.auld[location-1] = value;
		  //	}
	      // else
	      // 	{
	      // 	  calfile >> value;

	      // 	  // cout << linenum << " " << value << endl;
		  //TO Include dynode energy calibrations
	       	  else if(linenum == 257) pspmt.dyslope = value;
	       	  else if(linenum == 258) pspmt.dyoffset = value;
	      // 	  if(linenum == 1027) pspmt.dythresh = value;
	      // 	  if(linenum == 1028) pspmt.dyuld = value;
	      // 	}
	    }
	  else
	    {
	      //ignore line
	      calfile.ignore(1000,'\n');
	    }
	}
    }
}

void TBetaDecayVariables::ReadPSPMTTime(std::string name)
//TO For pspmt time deviations
{
  cout << "Reading PSPMT anode-dynode time calibration from: " << name << endl;

  ifstream calfile(name.c_str());

  int linenum = 0;
  int location = -1;
  double value = 0;

  if(!calfile)
    {
      cout << "Unable to open file " << name << endl;
    }
  else
    {
      // Read in cal file until end of file
      while(calfile)
	{
	  if(calfile && (isdigit(calfile.peek())))
	    {
	      linenum++;
	      calfile >> location >> value;

	      // cout << location << " " << value << endl;

	      if(linenum < 257) pspmt.tdiffdynode[location-1] = value;
	    }
	  else
	    {
	      //ignore line
	      calfile.ignore(1000,'\n');
	    }
	}
    
    }

}

void TBetaDecayVariables::ReadClover(std::string name)
{
  cout << "Reading Clover calibration from: " << name << endl;

  ifstream calfile(name.c_str());
  
  int linenum = 0;
  int location = -1;
  double value = 0;

  if(!calfile)
    {
      cout << "Unable to open file " << name << endl;
    }
  else
    {
      // Read in cal file until end of file
      while(calfile) {
	if(calfile && (isdigit(calfile.peek())))
	  {
	    linenum++;
	    calfile >> location >> value;

	    // cout << linenum << " " << location << " " << value << endl;

	    if(linenum < 65) clover.square[location-1] = value;
	    else if(linenum < 129) clover.slope[location-1] = value;
	    else if(linenum < 193) clover.intercept[location-1] = value;
	    else if(linenum < 257) clover.thresh[location-1] = value;
	    else if(linenum < 321) clover.uld[location-1] = value;

	  }
	else
	  {
	    //ignore line
	    calfile.ignore(1000,'\n');
	  }
      }
    }
  
  calfile.close();
}

void TBetaDecayVariables::ReadCloverTime(std::string name)
{
  cout << "Reading Clover-dynode time calibration from: " << name << endl;

  ifstream calfile(name.c_str());

  int linenum = 0;
  int location = -1;
  double value = 0;

  if(!calfile)
    {
      cout << "Unable to open file " << name << endl;
    }
  else
    {
      // Read in cal file until end of file
      while(calfile)
	{
	  if(calfile && (isdigit(calfile.peek())))
	    {
	      linenum++;
	      calfile >> location >> value;

	      // cout << linenum << " " << location-1 << " " << location << " " << value << endl;

	      if(linenum < 65) clover.tdiffdynode[location-1] = value;
	    }
	  else
	    {
	      //ignore line
	      calfile.ignore(1000,'\n');
	    }
	}
    
    }
}

void TBetaDecayVariables::ReadSeGA(std::string name)
{
  cout << "Reading SeGA calibration from: " << name << endl;

  ifstream calfile(name.c_str());
  
  int linenum = 0;
  int location = -1;
  double value = 0;

  if(!calfile) {
    cout << "Unable to open file " << name << endl;
  } else {
    // Read in cal file until end of file
    while(calfile) {
      if(calfile && (isdigit(calfile.peek()))) {
	linenum++;
	calfile >> location >> value;

	// cout << linenum << " " << location << " " << value << endl;
	if(linenum < 17) sega.square[location-1] = value;
	else if(linenum < 33) sega.slope[location-1] = value;
	else if(linenum < 49) sega.intercept[location-1] = value;
	else if(linenum < 65) sega.thresh[location-1] = value;
	else if(linenum < 81) sega.uld[location-1] = value;

	// if(linenum > 14 && linenum < 31 ) sega.square[location-1] = value;
	// else if(linenum > 33 && linenum < 50) sega.slope[location-1] = value;
	// else if(linenum > 52 && linenum < 69) sega.intercept[location-1] = value;
	// else if(linenum > 73 && linenum < 90) sega.thresh[location-1] = value;
	// else if(linenum > 93 && linenum < 110) sega.uld[location-1] = value;
      }
      else
	{
	  //ignore line
	  calfile.ignore(1000,'\n');
	}
    }
  }
  
  calfile.close();
}

void TBetaDecayVariables::ReadLaBr3(std::string name)
{
  cout << "Reading LaBr3 calibration from: " << name << endl;

  ifstream calfile(name.c_str());
  
  int linenum = 0;
  int location = -1;
  double value = 0;

  if(!calfile)
    {
      cout << "Unable to open file " << name << endl;
    }
  else
    {
      // Read in cal file until end of file
      while(calfile)
	{
	  if(calfile && (isdigit(calfile.peek())))
	    {
	      linenum++;
	      calfile >> location >> value;

	      // cout << linenum << " " << location << " " << value << endl;

	      // if(linenum < 16) labr3.square[location-1] = value;
	      // else if(linenum < 31) labr3.slope[location-1] = value;
	      // else if(linenum < 46) labr3.intercept[location-1] = value;
	      // else if(linenum < 61) labr3.thresh[location-1] = value;
	      // else if(linenum < 76) labr3.uld[location-1] = value;
	      
	      //TO e16032
	      if(linenum < 17) labr3.square[location-1] = value;
	      else if(linenum < 33) labr3.slope[location-1] = value;
	      else if(linenum < 49) labr3.intercept[location-1] = value;
	      else if(linenum < 65) labr3.thresh[location-1] = value;
	      else if(linenum < 81) labr3.uld[location-1] = value;
	    }
	  else
	    {
	      //ignore line
	      calfile.ignore(1000,'\n');
	    }
	}
    }
  calfile.close();
}

void TBetaDecayVariables::ReadLaBr3Time(std::string name)
{
  cout << "Reading LaBr3-dynode time calibration from: " << name << endl;

  ifstream calfile(name.c_str());

  int linenum = 0;
  int location = -1;
  double value = 0;

  if(!calfile)
    {
      cout << "Unable to open file " << name << endl;
    }
  else
    {
      // Read in cal file until end of file
      while(calfile)
	{
	  if(calfile && (isdigit(calfile.peek())))
	    {
	      linenum++;
	      calfile >> location >> value;

	      // cout << linenum << " " << location-1 << " " << location << " " << value << endl;
	      //For now, e16032 labr3 time diff calib variables are included in the same file containing values for energy calibration
	      // if(linenum < 16) labr3.tdiffdynode[location-1] = value;
	      if(linenum >80 && linenum < 97) labr3.tdiffdynode[location-1] = value;
	    }
	  else
	    {
	      //ignore line
	      calfile.ignore(1000,'\n');
	    }
	}
    
    }

}

//TO Dynode Timewalk Corrections
void TBetaDecayVariables::ReadDyTimewalkCorrection(std::string name)
{
  cout << "Reading low dynode energy timewalk corrections & high energy response centroids for each labr3-dynode pair from: " << name << endl;

  ifstream calfile(name.c_str());
  
  int linenum = 0;
  int location = -1;
  double value = 0;

  if(!calfile)
    {
      cout << "Unable to open file " << name << endl;
    }
  else
    {
      // Read in cal file until end of file
      while(calfile)
	{
	  if(calfile && (isdigit(calfile.peek())))
	    {
	      linenum++;
	      calfile >> location >> value;

	      // cout << linenum << " " << location << " " << value << endl;

	      // if(linenum < 16) pspmt.dytwalkp0[location-1] = value;
	      // else if(linenum < 31) pspmt.dytwalkp1[location-1] = value;
	      // else if(linenum < 46) pspmt.dytwalkp2[location-1] = value;
	      // else if(linenum < 61) pspmt.dytwalkp3[location-1] = value;
	      // else if(linenum < 76) pspmt.dytwalkp4[location-1] = value;
	      //high dyenergy time response centroids
	      if(linenum < 16) pspmt.dyhighcentroid[location-1] = value;
	      else if(linenum < 31) pspmt.dylowcentroid[location-1] = value;
	      // if(linenum < 16) labr3.dytwalkp0[location-1] = value;
	      // else if(linenum < 31) labr3.dytwalkp1[location-1] = value;
	      // else if(linenum < 46) labr3.dytwalkp2[location-1] = value;
	      // else if(linenum < 61) labr3.dytwalkp3[location-1] = value;
	      // else if(linenum < 76) labr3.dytwalkp4[location-1] = value;
	    }
	  else
	    {
	      //ignore line
	      calfile.ignore(1000,'\n');
	    }
	}
    }
  calfile.close();
}

void TBetaDecayVariables::ReadOther(std::string name)
{
  cout << "Reading Other calibration from: " << name << endl;

  ifstream calfile(name.c_str());
  
  int linenum = 0;
  double value = 0;
 
  if(!calfile)
    {
      cout << "Unable to open file " << name << endl;
    }
  else
    {
      //read in cal file until end of file
      while(calfile)
  	{
  	  if(calfile && (isdigit(calfile.peek())))
	  {
  	      linenum++;
	      calfile >> value;
	      
	      // cout << linenum << " " << value << endl;
	      
	      if(linenum == 1) clock.scale = value;
	      if(linenum == 2) clock.calib = value;
	      if(linenum == 3) corr.corrwindow = value;
	      if(linenum == 4) corr.minimplant = value;
	      if(linenum == 5) clockisomer.max = value;
	      if(linenum == 6) clockisomer.calib = value;
	      if(linenum == 7) pin01.slope = value;
	      if(linenum == 8) pin01.intercept = value;
	      if(linenum == 9) pin01.thresh = value;
	      //skipped undefined pin01.ithresh
	      if(linenum == 11) pin01.uld = value;
	      if(linenum == 12) pin02.slope = value;
	      if(linenum == 13) pin02.intercept = value;
	      if(linenum == 14) pin02.thresh = value;
	      //pin02.uld unavailable in e16032 calib file; setting to pin01.uld
	      if(linenum == 11) pin02.uld = value;
	      if(linenum == 15) pin03.slope = value;
	      if(linenum == 16) pin03.intercept = value;
	      if(linenum == 17) pin03.thresh = value;
	      //pin03.uld unavailable in e16032 calib file
	      if(linenum == 11) pin03.uld = value;	      
	      if(linenum == 18) tac.pin01i2ncorr = value;
	      if(linenum == 19) tac.pin01i2noffset = value;
	      //e16032 unresolved
	      // if(linenum == 21) tac.pin01i2scorr = value;
	      // if(linenum == 22) tac.pin01i2soffset = value;
	      // if(linenum == 23) tac.pin02i2ncorr = value;
	      // if(linenum == 24) tac.pin02i2noffset = value;
	      // if(linenum == 25) tac.pin02i2scorr = value;
	      // if(linenum == 26) tac.pin02i2soffset = value;
	      // //
	      // if(linenum == 27) tac.i2ni2scorr = value;
	      // if(linenum == 28) tac.thresh = value;
	      // if(linenum == 29) tac.uld = value;
  	    } 
  	  else
  	    {
	      calfile.ignore(1000,'\n');
  	    }
  	} 
    }
  
  // getc(stdin);
  
  calfile.close();
}

