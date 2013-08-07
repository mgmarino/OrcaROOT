// ORRunTimesCalculator.hh

#ifndef _ORRunTimesCalculator_hh_
#define _ORRunTimesCalculator_hh_

#include "ORUtilityProcessor.hh"

class ORRunTimesCalculator : public ORUtilityProcessor
{
  public:
    virtual ~ORRunTimesCalculator() {}

    virtual EReturnCode EndRun();

    virtual size_t GetNRuns() { return fRunStartTimes.size(); }
    virtual Int_t GetRunNumber(size_t index);
    virtual Int_t GetRunStartTime(Int_t runNumber) 
      { return fRunStartTimes[fRunNumberIndex[runNumber]]; }
    virtual Int_t GetRunStopTime(Int_t runNumber) 
      { return fRunStopTimes[fRunNumberIndex[runNumber]]; }
    virtual Int_t GetRunDuration(Int_t runNumber) 
      { return GetRunStopTime(runNumber) - GetRunStartTime(runNumber); }

  protected:
    std::map<Int_t,Int_t> fRunNumberIndex;
    std::vector<Int_t> fRunStartTimes;
    std::vector<Int_t> fRunStopTimes;
};

#endif
