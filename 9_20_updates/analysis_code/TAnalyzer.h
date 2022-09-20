#ifndef __TANALYZER_H
#define __TANALYZER_H

#include <vector>
#include <string>

//e17011
//#include "DDASRootFitHit.h"

//TO e16032
#include "/projects/e16032/threadfit/DDASRootFitHit.h"

#include "TTree.h"

class TAnalyzer
{
 public:
  TAnalyzer();
  void ResetChannelList(std::vector<DDASRootFitHit*>  channellist, std::vector<DDASRootFitHit*>::iterator channellist_it);
  void PrintChannelList(std::vector<DDASRootFitHit*>  channellist, std::vector<DDASRootFitHit*>::iterator channellist_it);
  void AnalyzeData(TTree* tree_in, TTree* tree_out, std::string runnumber);
};

#endif
