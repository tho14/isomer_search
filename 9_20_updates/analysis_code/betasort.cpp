//
// Analysis code for sorting NSCLDAQ data into Beta Group detector classes.
// Developed for analyzing data taken during the 2019-2020 clover campaign
// with the intention of being back-portable for (at least) e16032 analysis.
// The code assumes that there is fit information present in the input data.
//
// The structure of (some of) the code is shamelessly ripped off from GRSISort:
//   https://github.com/GRIFFINCollaboration/GRSISort
// developed by (among others) P. Bender, R. Dunlop and V. Bildstein.
// Structural elemnets have also been adopted from previous Beta Group
// analysis codes, specifically those by B. Lewis, A. Richard and B. Crider.
// 
// --A. Chester, 3/10/2020
//
//Repurposed for e16032
// TO 10/06/20

#include <iostream>
#include <string>

#include "TAnalyzer.h"

#include "TFile.h"
#include "TTree.h"

// Main
//
// Function:
//   - creates input/output data structures
//   - calls analyzer to build/fill detector classes
//   - writes output
//
// Params:
//   - none
//
// Return:
//   - 0 on success
//   - 1 on failure
//
int main(int argc, char **argv) 
{

  using std::cout;
  using std::endl;
  using std::string;
  
  // If we're happy how this is working, we can make a chain
  // for a single run number including all the segments and
  // write out one output, see FilterDDAS.cpp for an example.
  // The inputs and data processing have to be changed a little bit.
  // --ASC 2/21/20
  
  TFile *rootfile_in;
  TFile *rootfile_out;
  TTree *tree_in;
  TTree *tree_out;

  TAnalyzer analyzer; // for sorting data

  if(argc != 3)
    {
      cout << "Usage: betasort <infile> <outfile>" << endl;
      cout << "<infile>: File path for the dumped file to be analyzed" << endl;
      cout << "<outfile>: File path for the analyzed file to be saved" << endl;
      exit(1);
    }

  char *pFilein = argv[1];
  char *pFileout = argv[2];

  cout << "Input file: " << pFilein << endl;
  cout << "Output file: " << pFileout << endl;

  time_t start, stop;

  /*********************** IMPORTANT **************************
   *                                                          *
   * If the file name (or directory) changes, this might have *
   * to  be edited to reflect those changes!                  *
   *                                                          *
   ************************************************************/
  
  // Grab the four-digit run number from the file name
  
  // e.g. /mnt/analysis/e18018/dumped/run-0016-00-dumped.root
  // Local
  //e17011
  //cout << "Grabbing run number from the following chars: " << pFilein[48] << pFilein[49] << pFilein[50] << pFilein[51] << endl;
  
  // TO e16032 
  //e.g /mnt/analysis/e16032/rootfiles/run-0172-09.root
   cout << "Grabbing run number from the following chars: " << pFilein[35] << pFilein[36] << pFilein[37] << pFilein[38] << endl;
     string runnumber = string(&pFilein[35]);
     
  // TO e16032 133Ba Run84 - Different file path
  //e.g /mnt/analysis/e16032/tho/rootfiles/run-0084-09.root
  // cout << "Grabbing run number from the following chars: " << pFilein[39] << pFilein[40] << pFilein[41] << pFilein[42] << endl;
  // string runnumber = string(&pFilein[39]);

  time(&start);
    
  cout << "Check on run number from filename string: " << runnumber.substr(0,4).c_str() << endl;
      
  int havedata = 0;
  rootfile_in = new TFile(pFilein);
  if(!rootfile_in->IsZombie())
    {
      havedata = 1;
    }
  else
    {
      cout << "ERROR: Cannot open file!" << pFilein << endl;
      cout << "This file is a zombie. Was it closed properly?" << endl;
      cout << "Exiting..." << endl;
      exit(1);
    }

  // Output file and tree
  rootfile_out = new TFile(pFileout, "RECREATE");
  tree_out = new TTree("data","data");
  
  // Go through the data in the file
  if(havedata)
    {
      cout << "Processing data from " << pFilein << endl;
      
      // Get the data
      tree_in = (TTree*)rootfile_in->Get("DDASFit");
    
      // Call the analyzer to map channels and sort data
      analyzer.AnalyzeData(tree_in, tree_out, runnumber);
      
      // Close the rootfile input
      rootfile_in->Close();
      delete rootfile_in;
    } // End if(havedata)

  // Write Output
  rootfile_out->cd();
  tree_out->Write();
  rootfile_out->Close();
  
  time(&stop);
  float diff = difftime(stop,start);
  cout << endl << endl;
  cout << "Elapsed time = " << diff << " seconds" << endl;
    
  return 0;
}

