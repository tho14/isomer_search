#ifndef __TMODULE_H
#define __TMODULE_H

//#include "/usr/opt/ddas/2.2/include"

class TModule
{
 private:
  int fType;
  int fADCFreq;
  double fScale;
  double fClockCal;
  int fMaxVal;

 public:
  TModule();
  void SetupModule(int adcfreq, int adcdepth);
  int GetType() {return fType;}
  int GetADCFreq() {return fADCFreq;}
  double GetScale() {return fScale;}
  double GetClockCal() {return fClockCal;}
  int GetMaxVal() {return fMaxVal;}
};

#endif
