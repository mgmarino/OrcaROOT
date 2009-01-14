// ORCTITempDecoder.hh
/*
*/
#ifndef _ORCTITempDecoder_hh_
#define _ORCTITempDecoder_hh_

#include "ORVBasicTreeDecoder.hh"

class ORCTITempDecoder: public ORVBasicTreeDecoder
{
  public:
    ORCTITempDecoder() {}
    virtual ~ORCTITempDecoder() {}
    enum EORCTITempConsts {kNumChannels = 1};
    
    virtual UShort_t GetDeviceID(UInt_t* record) { return record[1] & 0xFFFF; } 

    virtual Float_t GetTemperature(UInt_t* record) 
      { return (*((Float_t*) (record + 2))); }
    /* Temperature of a channel. */
    virtual UInt_t GetTime(UInt_t* record)
      { return record[3]; }
    /* Time in channel given in seconds since 1 Jan 1970. */

    virtual void Dump(UInt_t* record);
    /* Provides a debug dump of the record. */

    virtual std::string GetDataObjectPath() 
      { return "CTITempModel:Temperatures"; }  
 
    /* Satisfying the ORVBasicTreeDecoder interface. */
    virtual size_t GetNPars() { return 3; } 
    virtual std::string GetParName(size_t iPar);
    virtual UInt_t GetPar(UInt_t* record, size_t iPar, size_t iRow);
    
};

#endif
