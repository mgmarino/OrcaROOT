// ORRunTimesCalculator.cc

#include "ORRunTimesCalculator.hh"
#include "ORRunContext.hh"
#include "ORLogger.hh"

using namespace std;

ORDataProcessor::EReturnCode ORRunTimesCalculator::EndRun()
{
  // stop time is not valid until EndRun, so store everything here.
  if(fRunNumberIndex.count(fRunContext->GetRunNumber()) != 0) {
    ORLog(kError) << "Already saw run " << fRunContext->GetRunNumber() << endl;
    return kSuccess;
  }
  fRunNumberIndex[fRunContext->GetRunNumber()] = fRunStartTimes.size();
  fRunStartTimes.push_back(fRunContext->GetStartTime());
  fRunStopTimes.push_back(fRunContext->GetStopTime());
  return kSuccess;
}

Int_t ORRunTimesCalculator::GetRunNumber(size_t index)
{
  map<Int_t, Int_t>::iterator it = fRunNumberIndex.begin();
  for(size_t i=0; i<index; i++) it++;
  return it->first;
}

