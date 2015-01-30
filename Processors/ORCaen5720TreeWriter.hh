// ORCaen5720TreeWriter.hh

#ifndef _ORCaen5720TreeWriter_hh_
#define _ORCaen5720TreeWriter_hh_

#include "ORVTreeWriter.hh"
#include "ORCaen5720Decoder.hh"

class ORCaen5720TreeWriter : public ORVTreeWriter
{
  public:
    ORCaen5720TreeWriter(std::string treeName = "");
    virtual ~ORCaen5720TreeWriter();

    virtual EReturnCode ProcessMyDataRecord(UInt_t* record);
    virtual inline void Clear()
  { fChannelMap=0; fEventID=0; fEventFlags=0; fEventInfo=0; 
    fwaveform0[0]=0; fwaveform1[0]=0; fwaveform2[0]=0; fwaveform3[0]=0;}

  enum ECaen5720WFTreeWriter{ fmaxnumSamples = 10000};

  protected:
    virtual EReturnCode InitializeBranches();

  protected:
    ORCaen5720Decoder* fCaen5720Decoder;
  UInt_t fnumSamples;
  UInt_t fEventID, fEventFlags, fEventInfo;
  UInt_t fChannelMap;

  UInt_t fwaveform[fmaxnumSamples];
  UInt_t fwaveform0[fmaxnumSamples];
  UInt_t fwaveform1[fmaxnumSamples];
  UInt_t fwaveform2[fmaxnumSamples];
  UInt_t fwaveform3[fmaxnumSamples];

  UInt_t fClock, fEventSize, fEventCount, fChannelMask;

};

#endif

	 

   
	
