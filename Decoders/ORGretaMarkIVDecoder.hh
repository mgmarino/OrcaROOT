// ORGretaDecoder.hh

#ifndef _ORGretaMarkIVDecoder_hh_
#define _ORGretaMarkIVDecoder_hh_

#include "ORGretaDecoder.hh"
using namespace std;

class ORGretaMarkIVDecoder: public ORGretaDecoder
{
  public:
    ORGretaMarkIVDecoder();
    virtual ~ORGretaMarkIVDecoder() {}
    enum EGretaMarkIVConsts {kORCAHeadLen    = 2,
                             kMasterHeadLen  = 4, 
                             kSlaveHeaderLen = 4, 
                             kMarkIVBufHeadLen     = 7};
    
    virtual std::string GetDataObjectPath() { return "ORGretinaMarkIVModel:Gretina"; }  
    
    virtual bool SetDataRecord(UInt_t* record);
       
    // Functions that return data from Master Header
    virtual inline Bool_t HasMasterHeader() {return fHasMasterHeader;}
    virtual inline UShort_t GetMasterBoardID();
    virtual inline UShort_t GetMasterHeaderLength();
    virtual inline UShort_t GetMasterTrigger_TSLow();
    virtual inline UShort_t GetMasterTrigger_TSMed();
    virtual inline UShort_t GetMasterTrigger_TSHigh();
    virtual inline ULong64_t GetMasterTrigger_TS();
    virtual inline UShort_t GetMasterHMNum();
    virtual inline UShort_t GetMasterLowResEnergy();

    // Functions that return data from Slave Header
    virtual inline UShort_t GetSlaveBoardID();
    virtual inline UShort_t GetSlaveHeaderLength();
    virtual inline UShort_t GetSlaveFB_LEDLow();
    virtual inline UShort_t GetSlaveFB_LEDMed();
    virtual inline UShort_t GetSlaveFB_LEDHigh();
    virtual inline ULong64_t GetSlaveFB_LED();
    virtual inline UShort_t GetSlaveHMNum();

    //Functions that return data from buffer header:
    virtual inline size_t GetBufHeadLen() {return (size_t) kMarkIVBufHeadLen;}
    virtual inline Bool_t IsTTCLTimeout();
    virtual inline UInt_t GetCFDPointOne();
    virtual inline UInt_t GetCFDPointTwo();
   
  protected:
    virtual inline size_t GetRecordOffset() {return kORCAHeadLen + fOffset;}
    /* GetRecordOffset() returns how many words the record is offset from the 
       beginning.  This is useful when additional headers are added. */
    bool fHasMasterHeader;
    size_t fOffset;
};

inline UShort_t ORGretaMarkIVDecoder::GetMasterBoardID() 
{
  return (UShort_t) (fDataRecord[kORCAHeadLen] & 0xffff);   
}

inline UShort_t ORGretaMarkIVDecoder::GetMasterHeaderLength() 
{
  return (UShort_t) (fDataRecord[kORCAHeadLen] >> 16);   
}

inline UShort_t ORGretaMarkIVDecoder::GetMasterTrigger_TSLow() 
{
  return (UShort_t) (fDataRecord[kORCAHeadLen + 1] & 0xffff);   
}

inline UShort_t ORGretaMarkIVDecoder::GetMasterTrigger_TSMed() 
{
  return (UShort_t) (fDataRecord[kORCAHeadLen + 1] >> 16);   
}

inline UShort_t ORGretaMarkIVDecoder::GetMasterTrigger_TSHigh() 
{
  return (UShort_t) (fDataRecord[kORCAHeadLen + 2] & 0xffff);   
}

inline ULong64_t ORGretaMarkIVDecoder::GetMasterTrigger_TS() 
{
  return BitConcat(GetMasterTrigger_TSLow(), GetMasterTrigger_TSMed(),
           GetMasterTrigger_TSHigh());;   
}

inline UShort_t ORGretaMarkIVDecoder::GetMasterHMNum() 
{
  return (UShort_t) (fDataRecord[kORCAHeadLen + 2] >> 16);   
}

inline UShort_t ORGretaMarkIVDecoder::GetMasterLowResEnergy() 
{
  return (UShort_t) (fDataRecord[kORCAHeadLen + 3] & 0xffff);   
}

inline UShort_t ORGretaMarkIVDecoder::GetSlaveBoardID() 
{
  return (UShort_t) (fDataRecord[kORCAHeadLen+kMasterHeadLen] & 0xffff);   
}

inline UShort_t ORGretaMarkIVDecoder::GetSlaveHeaderLength() 
{
  return (UShort_t) (fDataRecord[kORCAHeadLen+kMasterHeadLen] >> 16);   
}

inline UShort_t ORGretaMarkIVDecoder::GetSlaveFB_LEDLow() 
{
  return (UShort_t) (fDataRecord[kORCAHeadLen+kMasterHeadLen + 1] & 0xffff);   
}

inline UShort_t ORGretaMarkIVDecoder::GetSlaveFB_LEDMed() 
{
  return (UShort_t) (fDataRecord[kORCAHeadLen+kMasterHeadLen + 1] >> 16);   
}

inline UShort_t ORGretaMarkIVDecoder::GetSlaveFB_LEDHigh() 
{
  return (UShort_t) (fDataRecord[kORCAHeadLen+kMasterHeadLen + 2] & 0xffff);   
}

inline ULong64_t ORGretaMarkIVDecoder::GetSlaveFB_LED() 
{
  return BitConcat(GetSlaveFB_LEDLow(), GetSlaveFB_LEDMed(),
           GetSlaveFB_LEDHigh());;   
}

inline UShort_t ORGretaMarkIVDecoder::GetSlaveHMNum() 
{
  return (UShort_t) (fDataRecord[kORCAHeadLen+kMasterHeadLen + 2] >> 16);   
}

inline Bool_t ORGretaMarkIVDecoder::IsTTCLTimeout() 
{
  return (Bool_t) (fDataRecord[GetRecordOffset()+3] >> 11);   
}

inline UInt_t ORGretaMarkIVDecoder::GetCFDPointOne() 
{
  return (fDataRecord[GetRecordOffset()+5]);   
}

inline UInt_t ORGretaMarkIVDecoder::GetCFDPointTwo() 
{
  return (fDataRecord[GetRecordOffset()+6]);   
}


#endif

