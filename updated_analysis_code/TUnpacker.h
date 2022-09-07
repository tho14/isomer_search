#ifndef __TUNPACKER_H
#define __TUNPACKER_H

#include <vector>

#include "/projects/e16032/threadfit/DDASRootFitHit.h"


#include "TParameters.h"
#include "TVariables.h"
#include "TModule.h"

#include "TApplication.h"

class TUnpacker
{
 public:
  TUnpacker();
  int MapDynode(int crateid, int slotid, int channum, int adcnumber, std::vector<UShort_t> trace, TBetaDecayParams &bdecay, TBetaDecayVariables &bdecayv, TModule module[]);
  int MapChannels(int crateid, int slotid, int channum, int adcnumber, std::vector<UShort_t> trace, TBetaDecayParams &bdecay, TBetaDecayVariables &bdecayv, TModule module[]);
  void UnpackDynodeFit(TBetaDecayParams &bdecay, std::vector<RootHitExtension> fits);
  void UnpackAnodePosition(TBetaDecayParams &bdecay);
  void UnpackAnodePositionGaus(TBetaDecayParams &bdecay);
  int CheckTraceOverflow(std::vector<UShort_t> trace, int lim);
     

 private:
  TApplication* theApp;
};

#endif
