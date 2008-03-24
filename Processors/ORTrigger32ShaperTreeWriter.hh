// ORTrigger32TreeWriter.hh

#ifndef _ORTrigger32ShaperTreeWriter_hh_
#define _ORTrigger32ShaperTreeWriter_hh_

#include "ORTrigger32ClockTreeWriter.hh"
#include "ORTrigger32GTIDTreeWriter.hh"
#include "ORTrigger32LiveTimeDecoder.hh"
#include "ORShaperShaperTreeWriter.hh"

#include "ORBasicTreeWriter.hh"
#include "ORCompoundDataProcessor.hh"

class ORTrigger32ShaperTreeWriter : public ORCompoundDataProcessor
{
  public:
    ORTrigger32ShaperTreeWriter(std::string treeBaseName = "trigger");
    virtual ~ORTrigger32ShaperTreeWriter();

  protected:
    ORTrigger32GTID1Decoder fTriggerGTID1Decoder;
    ORTrigger32GTIDTreeWriter* fTriggerGTID1TreeWriter;
    ORTrigger32ClockTreeWriter* fTrigger1ClockTreeWriter;
    ORTrigger32GTID2Decoder fTriggerGTID2Decoder;
    ORTrigger32GTIDTreeWriter* fTriggerGTID2TreeWriter;
    ORTrigger32ClockTreeWriter* fTrigger2ClockTreeWriter;
    ORTrigger32LiveTimeDecoder fTriggerLiveTimeDecoder;
    ORBasicTreeWriter* fTriggerLiveTimeTreeWriter;
	ORShaperShaperDecoder fshaperShaperDecoder;
	ORShaperShaperTreeWriter* fshaperShaperTreeWriter;


};

#endif
