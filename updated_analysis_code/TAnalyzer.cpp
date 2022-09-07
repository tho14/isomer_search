#include <iostream>
#include <vector>
#include <sys/time.h>

#include "/projects/e16032/threadfit/DDASRootFitEvent.h"
#include "/projects/e16032/threadfit/DDASRootFitHit.h"
#include "/projects/e16032/threadfit/DDASFitHit.h"
#include "/projects/e16032/threadfit/functions.h"
#include "/projects/e16032/threadfit/lmfit.h"

#include "TAnalyzer.h"
#include "TUnpacker.h"
#include "TParameters.h"
#include "TVariables.h"
#include "TCorrelator.h"
#include "TModule.h"
#include "Constants.h"

#include "TFile.h"

// Lets make sure we make an analyzer
TAnalyzer::TAnalyzer()
{
  std::cout << "Creating analyzer..." << std::endl;
}

// ResetChannelList
//
// Function:
//   - resets event channel vector
//
// Params:
//   - the channel list, a vector of DDAS channels
//   - the channel list iterator
//
// Return:
//   - none
//
void TAnalyzer::ResetChannelList(std::vector<DDASRootFitHit*>  channellist, std::vector<DDASRootFitHit*>::iterator channellist_it) 
{
  channellist_it = channellist.begin();
  
  for (channellist_it = channellist.begin(); channellist_it < channellist.end(); channellist_it++) 
    {
      delete *channellist_it;
    }
  
  channellist.clear();
}

// PrintChannelList
//
// Function:
//   - prints contents of the event channel vector
//
// Params:
//   - the channel list, a vector of DDAS channels
//   - the channel list iterator
//
// Return:
//   - none
//
void TAnalyzer::PrintChannelList(std::vector<DDASRootFitHit*>  channellist, std::vector<DDASRootFitHit*>::iterator channellist_it) 
{
  channellist_it = channellist.begin();
  
  for (channellist_it = channellist.begin(); channellist_it < channellist.end(); channellist_it++) 
    {
      int crateid = (*channellist_it)->crateid;
      int slotid = (*channellist_it)->slotid;
      int channum = (*channellist_it)->chanid;
      double energy = (*channellist_it)->energy;
      double time = (*channellist_it)->time;
    
      int id = totchan*crateid + nchannels*(slotid-2) + channum;
      
      std::cout << (*channellist_it) << " " << crateid << " " << slotid << " " << channum << " id: " << id << " energy: " << energy << " time: " << time << std::endl;
    }
  std::cout << "----------------------------------------------" << std::endl;
}

// AnalyzeData
//
// Function:
//   - initialize parameters and variables
//   - map DDAS events to detectors
//   - read calibration parameters and perform calibraton
//   - correlate events
//   - fill output data structures
//
// Params:
//   - input tree of DDAS events
//   - output tree of detector events
//
// Return:
//   - none
//
void TAnalyzer::AnalyzeData(TTree* tree_in, TTree* tree_out, std::string runnumber) 
{
  using std::cout;
  using std::flush;
  using std::endl;
  using std::string;
  
  //Scaling factors included in TModule!
  int modfreq[nadcs] = {500, 500, 250, 100, 250, 250, 250, 250, 250, 500, 500, 500, 500, 500, 500, 250, 250, 250, 250, 250};//MSPS
  int moddepth[nadcs] = {14, 12, 14, 12, 14, 14, 14, 16, 16, 14, 14, 14, 14, 14, 14, 16, 16, 14, 14, 14};//Bit Depth
  
  TModule module[nadcs]; // All your ADCs
  for(int i=0; i<nadcs; i++)
    {
      module[i].SetupModule(modfreq[i],moddepth[i]);
      // cout << "Module " << i << " -- MSPS: " << module[i].utils.GetADCFrequency() << " Type: " << module[i].GetType() << " Scale: " << module[i].GetScale() << " Max: " << module[i].GetMaxVal() << endl;
    }
  
  // Event info and iterator
  std::vector<DDASRootFitHit*> channellist;
  std::vector<DDASRootFitHit*>::iterator channellist_it;

  // Setup variables, data sorting classes
  TBetaDecayParams bdecay;
  TBetaDecayVariables bdecayv;
  bdecay.Reset();
  bdecayv.Initialize();
  TUnpacker unpacker;
  TCorrelator corr;
  
  // Output for of all the results
  TROOTDataOut *rootout = new TROOTDataOut();
  rootout->Reset();

  // Read calibration files
  // Overwrites initialized parameters!
  string PSPMTCalFile = "/projects/e16032/e16032_Analysis_THO/fixed_correlator_analysis_packet/rootanalysis/cal/PSPMTInit_Anodes.txt";//1 for experiment
  string PSPMTTimeCalFile = "/projects/e16032/e16032_Analysis_THO/fixed_correlator_analysis_packet/rootanalysis/cal/Anode_TimeDevs_"+runnumber.substr(0,4)+".txt";//1 per run
  string SeGACalFile = "/projects/e16032/e16032_Analysis_THO/fixed_correlator_analysis_packet/rootanalysis/cal/SeGAInit_"+runnumber.substr(0,4)+".txt";
  string LaBr3CalFile = "/projects/e16032/e16032_Analysis_THO/fixed_correlator_analysis_packet/rootanalysis/cal/LaBr3Init_"+runnumber.substr(0,4)+".txt";//1 for experiment
  string OtherCalFile = "/projects/e16032/e16032_Analysis_THO/fixed_correlator_analysis_packet/rootanalysis/cal/OtherInit.txt";
  string DyTimeWalkFile = "/projects/e16032/e16032_Analysis_THO/fixed_correlator_analysis_packet/rootanalysis/cal/parameters_for_low_dyenergy_time_response_all_segments.txt";
  //string FullTimeAlignmentFile = "/projects/e16032/e16032_Analysis_THO/fixed_correlator_analysis_packet/rootanalysis/cal/full_time_response_alignment_all_segments.txt";

  bdecayv.ReadPSPMT(PSPMTCalFile);
  bdecayv.ReadPSPMTTime(PSPMTTimeCalFile);
  bdecayv.ReadLaBr3(LaBr3CalFile);
  bdecayv.ReadLaBr3Time(LaBr3CalFile);
  bdecayv.ReadSeGA(SeGACalFile);
  //bdecayv.ReadLaBr3Time(SeGATimeCalFile);
  bdecayv.ReadOther(OtherCalFile);
  bdecayv.ReadDyTimewalkCorrection(DyTimeWalkFile);
  //bdecayv.ReadFullAlignment(FullTimeAlignmentFile);
  // ...

  // Here's the run number as an int//unused at the moment
  int run = stoi(runnumber.substr(0,4));

  double starttime = 0.;
  double endtime = 0.;

  // Make the relevant output stuff in the tree
  tree_out->Branch("rootout", "TROOTDataOut", &rootout, 32000, 99);
  //99: Maximum object split-level depth for each branch

  // Find the number of events in a file
  auto nevents = tree_in->GetEntries();
  cout << "Number events in run segment: " << tree_in->GetEntries() << endl;

  // Where to find the fit information
  std::vector<RootHitExtension>* fitdata(0);
  tree_in->SetBranchAddress("HitFits", &fitdata);

  // Built event raw data
  DDASRootFitEvent *rawhits;

  // Progress tracking
  timeval tvelapsed_c, tvelapsed_o;
  gettimeofday (&tvelapsed_o, NULL);
  Long64_t eventcounter = 0;
  
  // Loop over the events
  /********* Forward correlation ********/
  for (auto evt=0; evt<nevents; evt++)
  //for (auto evt=74; evt<nevents; evt++)
    //for (auto evt=0; evt<0.1*nevents; evt++)
    {
      // cout << "Processing evt " << evt << endl;
  
      // Reset data structures and hit variables for the new event
      ResetChannelList(channellist,channellist_it);
      rootout->Reset();
      bdecay.Reset();
      bdecayv.hit.Initialize();
      
      // Progress bar -- forward direction
      if(evt > 0 && (evt % (int)(nevents * 0.01)) == 0) {
  	cout << "Events processed " << evt << " -- percent done " << (int)(evt/(int)(nevents*0.01)) << "%";

  	gettimeofday (&tvelapsed_c, NULL);
  	cout << " -- Current Rate: "<< eventcounter/(((tvelapsed_c.tv_sec+tvelapsed_c.tv_usec/1.e6)-(tvelapsed_o.tv_sec+tvelapsed_o.tv_usec/1.e6))) << " events per second\r" << flush;
      
  	gettimeofday (&tvelapsed_o, NULL);
  	eventcounter=0;
      }
      
      //Check with only 0.1% of the file
      // if(evt > 0 && (evt % (int)(nevents * 0.01)) == 0) {
      // 	break;
      // }
      
      // // Check with only 5% of the file
      // if(evt > 0 && (evt % (int)(nevents * 0.05)) == 0) {
      // 	break;
      // }
      // // Check with only 10% of the file
      // if(evt > 0 && (evt % (int)(nevents * 0.1)) == 0) {
      // 	break;
      // }  
      /**************************************/
  
      /******** Reverse correlation ********/
      // for (auto evt=nevents-1; evt>=0; evt--)
      //   {

      //     // Reset data structures and hit variables for the new event
      //     ResetChannelList(channellist,channellist_it);
      //     rootout->Reset();
      //     bdecay.Reset();
      //     bdecayv.hit.Initialize();
  
      //     // Progress bar -- reverse correlation
      //     if(evt > 0 && ((nevents-evt) % (int)(nevents * 0.01)) == 0) {
      //     	cout << "Events processed " << evt << " -- percent done " << 100-(int)(evt/(int)(nevents*0.01)) << "%";
  
      // 	gettimeofday (&tvelapsed_c, NULL);
      // 	cout << " -- Current Rate: "<< eventcounter/(((tvelapsed_c.tv_sec+tvelapsed_c.tv_usec/1.e6)-(tvelapsed_o.tv_sec+tvelapsed_o.tv_usec/1.e6))) << " events per second\r" << flush;
      
      // 	gettimeofday (&tvelapsed_o, NULL);
      // 	eventcounter=0;
      //     }
  
      //     // // Check with only 1% of the file
      //     // if(evt > 0 && ((nevents-evt) % (int)(nevents * 0.01)) == 0) {
      //     // 	break;
      //     // }
      
      //     // // Check with only 5% of the file
      //     // if(evt > 0 && ((nevents-evt) % (int)(nevents * 0.05)) == 0) {
      //     // 	break;
      //     // }
      /**************************************/

      // Make the event
      rawhits = new DDASRootFitEvent();

      // Built raw event location in input tree
      TBranch *addasevent = tree_in->GetBranch("RawHits");
      addasevent->SetAddress(&rawhits);
        
      // Get the event
      tree_in->GetEntry(evt);

      // Hits with fit information have an extension which indicates
      // that there is associated fit info present
      std::vector<RootHitExtension>& fits(*fitdata);      

      // Set the vector of channels hit in this event
      channellist = rawhits->GetData();
      // PrintChannelList(channellist,channellist_it);
      // getc(stdin);

      // Now that we have the vector of channels, we need to unpack it into
      // bdecay classes. Iterate over the channellist, extract some basic information,
      // and send it to the channel mapper.
      int eventsize = channellist.size();
  
      bdecay.ddasdiagnostics.cmult = eventsize;

      channellist_it = channellist.begin();
  
      for(int i=0; i<nadcs; i++) 
	{
	  bdecay.ddasdiagnostics.adchit[i] = 0;
	}

      double previous_time = 0;

      // First pass through for only the dynode
      //bool first = true;
      for (channellist_it = channellist.begin(); channellist_it < channellist.end(); channellist_it++) 
	{
	  int crateid = (*channellist_it)->crateid;
	  int slotid = (*channellist_it)->slotid;
	  int channum = (*channellist_it)->chanid;
	  double current_time = (*channellist_it)->time; // in ns

	  // // Output the first time in the event
	  // // Debugging with respect to trace plotting tools
	  // if(first) {
	  //   cout << std::setprecision(12) << "evt " << evt << " first time " << current_time << endl;
	  //   first = false;
	  // }

	  // Update the current time for each entry
	  bdecay.clock.current = current_time;

	  if(crateid == 0 && slotid == 2 && channum == 0)//dynode
	    {
	      int adcnumber = 0; // Assumes dynode is in adcnumber == 0.

	      // Fill arrays with dynode hits (similar to double pulses but slightly longer than traces)
	      if(bdecay.pspmt.dymult < 10)
		{	
		  bdecay.pspmt.dyenergies[bdecay.pspmt.dymult] = (*channellist_it)->energy;
		  bdecay.pspmt.dytimes[bdecay.pspmt.dymult] = (*channellist_it)->time;		
		  bdecay.pspmt.dymult++; // increment dymult *after* using it above	  
		}
	      else
		{
		  cout << "More than 10 dynode triggers in an event!!!" << endl;
		}
      
	      if(bdecayv.hit.dynode == 0) // Sets values and analyzes for first dynode hit only
		{
		  bdecay.ddasdiagnostics.adchit[adcnumber]++;
		  bdecay.adc[adcnumber].channel[channum] = (*channellist_it)->energy;
		  bdecay.time[adcnumber].timefull[channum] = (*channellist_it)->time;
		  bdecay.time[adcnumber].timecfd[channum] = (*channellist_it)->timecfd;
		  // TO checking cfdfailbit condition
		  //cout << "cfdfailbit: "<<(*channellist_it)->cfdfailbit;
		  // TO extracting cfdfailbit condition
		  bdecay.time[adcnumber].cfdfail[channum] = (*channellist_it)->cfdfailbit;
		  //bdecay.time[adcnumber].cfdfailbit[channum] = (*channellist_it)->cfdfailbit;
		  // TO ‘class TDDASTime’ has no member named ‘cfdfailbit’
		
		  // Check for trace and extract
		  if((*channellist_it)->tracelength != 0)
		    {
		      bdecay.adc[adcnumber].chantrace[channum].trace = (*channellist_it)->GetTrace();	
		    }
	    
		  // Map the dynode channel, calibrate and threshold check.
		  unpacker.MapDynode(crateid, slotid, channum, adcnumber, (*channellist_it)->GetTrace(), bdecay, bdecayv, module);
		  unpacker.UnpackDynodeFit(bdecay, fits);
		
		} // End processing of dynode hit
	    } // End dynode address check
	} // End first pass over channel list

      // // Lets look at some mult > 1 dynode hits
      // if(bdecay.pspmt.dymult > 1)
      //   {
      // 	cout << "evt " << evt << endl;
      // 	cout << "mult " << bdecay.pspmt.dymult << endl;
      // 	for(int i=0; i<bdecay.pspmt.dymult; i++)
      // 	  {
      // 	    cout << i << " " << bdecay.pspmt.dyenergies[i] << " " << setprecision(12) << bdecay.pspmt.dytimes[i] << endl;
      // 	  }
      // 	cout << "hit: " << bdecay.pspmt.dyecal << " " << setprecision(5) << bdecay.pspmt.dyE1real_single << " " << setprecision(12) << bdecay.pspmt.dytime << endl;
      // 	getc(stdin);
      //   }

      // cout << "Done analyzing dynode" << endl;
      // getc(stdin);
    
      // Second pass through for all other detectors
      for (channellist_it = channellist.begin(); channellist_it < channellist.end(); channellist_it++) 
	{
	  // cout << "Processing chhit " << channellist_it-channellist.begin() << endl;
	
	  int crateid = (*channellist_it)->crateid;
	  int slotid = (*channellist_it)->slotid;
	  int channum = (*channellist_it)->chanid;
	  double current_time = (*channellist_it)->time; // in ns
      
	  /* The time of an event will be taken as the time of the first 
	     channel in the event. */      
	  if(channellist_it == channellist.begin()) 
	    {
	      starttime = current_time;
	      previous_time = current_time;
	      bdecay.clock.initial = current_time;
	    }
	  if(channellist_it == channellist.end()-1)
	    {
	      endtime = current_time;
	    }

	  // Time difference between events in us
	  bdecay.ddasdiagnostics.tdiffevent = current_time-previous_time; // in ns
	  previous_time = current_time;
	
	  // Update the current time for each entry
	  bdecay.clock.current = current_time;

	  // If it is not a dynode event, continue working
	  if(!(crateid == 0 && slotid == 2 && channum == 0))
	    {	
	      // TO e16032 - 2 crates
	      int adcnumber = -1;
	      if(crateid == 0) {
		adcnumber = slotid - 2;
	      }
	      else {//(crateid == 1)
		adcnumber = slotid + 11;
	      }
	  
	      bdecay.ddasdiagnostics.adchit[adcnumber]++;
	      bdecay.adc[adcnumber].channel[channum] = (*channellist_it)->energy;
	      bdecay.time[adcnumber].timefull[channum] = current_time;
	      bdecay.time[adcnumber].timecfd[channum] = (*channellist_it)->GetCFDTime();
	      //bdecay.time[adcnumber].timecfd[channum] = (*channellist_it)->timecfd;
	      // TO Extract cfdfailbit
	      bdecay.time[adcnumber].cfdfail[channum] = (*channellist_it)->cfdfailbit;
	      bdecay.ddasdiagnostics.overflow[channum] = (*channellist_it)->GetOverflowCode();
	      bdecay.ddasdiagnostics.finishcode[channum] = (*channellist_it)->GetFinishCode();
	  
	      // Check for trace and exract
	      if((*channellist_it)->tracelength != 0)
		{
		  bdecay.adc[adcnumber].chantrace[channum].trace = (*channellist_it)->GetTrace();
		}

	      // Map the channel, calibrate and theshold check.
	      unpacker.MapChannels(crateid, slotid, channum, adcnumber, (*channellist_it)->GetTrace(), bdecay, bdecayv, module);
	    } // End !dynode address check
           
	} // End loop over event channel list (unpacking stage)

      // cout << "Done with second pass" << endl;
      // getc(stdin);

      bdecay.ddasdiagnostics.eventlength = endtime - starttime; // in ns
    
      // Event level processing goes here
      // PID, TOF timing, analysis before writing, application of correction factors, etc.

      //TO e17011
      // bdecay.pid.de1 = bdecay.pin01.ecal;
      // bdecay.pid.pin01i2s = bdecay.tac.pin01i2s;
      // bdecay.pid.de2 = bdecay.pin02.ecal;
      // if(bdecay.tac.i2ni2s > 0) // Lets make sure we have the i2 signal before we do the correction
      //   bdecay.pid.pin02i2s = bdecay.tac.pin02i2s + bdecayv.tac.pin02i2scorr*bdecay.tac.i2ni2s + bdecayv.tac.pin02i2soffset;

      //TO e16032 PID, TOF, etc writing out raw signals from individual pins and tacs into the pid class (i2 position corrections, etc incorporated for e17011 to take care of pid cut offs; irrelevant for e16032!)
      
      bdecay.pid.de1 = bdecay.pin01.energy;
      bdecay.pid.pin01i2s = bdecay.tac.pin01i2s;
      bdecay.pid.pin01i2n = bdecay.tac.pin01i2n;
      bdecay.pid.de2 = bdecay.pin02.energy;
      bdecay.pid.pin02i2s = bdecay.tac.pin02i2s;
      bdecay.pid.pin02i2n = bdecay.tac.pin02i2n;
      
      // if(bdecay.tac.i2ni2s > 0) // Lets make sure we have the i2 signal before we do the correction
      // 	bdecay.pid.pin02i2s = bdecay.tac.pin02i2s + bdecayv.tac.pin02i2scorr*bdecay.tac.i2ni2s + bdecayv.tac.pin02i2soffset;
    
      // If dynode hit and anode mult > 0 set PSPMT hit flag and fit anode distribution
      if(bdecayv.hit.dynode == 1 && bdecay.pspmt.amult > 0)
	{
	  bdecayv.hit.pspmt = 1;
	  // Get the anode position by fitting the 2D energy distribution
	  unpacker.UnpackAnodePosition(bdecay);	
	}

      // cout << "Done with event level processing" << endl;

    
      // Correlate this event.
      if(corr.Correlate(bdecay,bdecayv))
	{
	  cout << "Bad correlation event " << evt << endl;
	}
      
      // Put selected data into root structure for write.
      rootout->SetOutputValues(bdecay);

      // Conditions go here e.g. only events with PSPMT
      // if(bdecay.pspmt.amult > 0)
      // {

      tree_out->Fill();
      // }
      // cout << "Tree filled" << endl;

      eventcounter++;
      // cout << "End of event " << evt << endl; getc(stdin);
    } // End loop over the events

}
