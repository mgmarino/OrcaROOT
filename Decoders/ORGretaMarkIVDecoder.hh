// ORGretaDecoder.hh

#ifndef _ORGretaMarkIVDecoder_hh_
#define _ORGretaMarkIVDecoder_hh_

#include "ORGretaDecoder.hh"
/* This card is very similar to the original Greta card, but has some important
   differences.  For example, it is possible that a record has a Master record
   accompanying it and so this is important to check when reading data from a 
   record.  Also, the raw data points (i.e. waveform data) are given in sign
   extended format, meaning that the points will occupy the entire 16-bit word.
   Currently, the decoder assumes that these are just unsigned numbers and so
   a cast should be done while processing to correctly decode these numbers.  
   The reason this is not done in the decoder is to remain consistent with the
   old card.  This can be re-evaluated in the future.  

   The old interface was changed to allow CFD Point One/Two to occupy 32 bits.

   New Greta card additions:
   - Master header (optional, must check for existence using HasMasterHeader())
     - Trigger_TS
     - HM Number
     - Low Resolution Energy
   - Slave header
     - FB_LED
     - HM Number
   - IsTTCLTimeout() -> signifies data generated by a timeout signal during TTCL mode.
   - CFD point 1/2 -> now occupies full 32 bits.


    It is also important to note that currently the record does not support 
    running in TTCL (Trigger Timing and Control Link) mode which occurs when
    a card is master over other cards (slaves).  
   */
   

class ORGretaMarkIVDecoder: public ORGretaDecoder
{
  public:
    ORGretaMarkIVDecoder();
    virtual ~ORGretaMarkIVDecoder() {}
    enum EGretaMarkIVConsts {kORCAHeadLen    = 2,
                             kMasterHeadLen  = 4, 
                             kSlaveHeaderLen = 4, 
                             kMarkIVBufHeadLen     = 7};
    
    virtual std::string GetDataObjectPath() { return "ORGretina4Model:Gretina4"; }  
    virtual std::string GetDictionaryObjectPath() { return "ORGretina4Model"; }  
    
    virtual bool SetDataRecord(UInt_t* record);
       
    virtual inline UShort_t GetEnergyNormalization();
    virtual inline UInt_t GetEnergy();
    virtual inline UShort_t GetEnergyHi();
    virtual inline UShort_t GetChannelNum();
    virtual inline Bool_t EnergyIsNegative();
    virtual inline Int_t GetSignedEnergy();
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
    virtual inline Bool_t HasSlaverHeader() {return fHasSlaverHeader;}
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

    /* Functions satisfying the ORVDigitizerDecoder interface. */
    virtual inline UShort_t GetBitResolution() { return 14; }
    virtual UInt_t GetEventWaveformPoint( size_t /*event*/, 
                                          size_t waveformPoint );
      /* All other functions are correctly inherited from the parent class. */

   
  protected:
    virtual inline size_t GetRecordOffset() {return kORCAHeadLen + fOffset;}
    /* GetRecordOffset() returns how many words the record is offset from the 
       beginning.  This is useful when additional headers are added. */
    bool fHasMasterHeader;
    bool fHasSlaverHeader;
    size_t fOffset;
};

inline UShort_t ORGretaMarkIVDecoder::GetEnergyNormalization()
{
  return (UShort_t) GetIntegrationTime(); 
}

inline UInt_t ORGretaMarkIVDecoder::GetEnergy()
{
  UInt_t energy = ORGretaDecoder::GetEnergy();
  if (energy & 0x1000000) energy = (~energy & 0x1ffffff) + 1;
  return energy;
}

inline UShort_t ORGretaMarkIVDecoder::GetEnergyHi()
{
  return (UShort_t) (fDataRecord[GetRecordOffset()+3] & 0x1ff); 
}

inline Bool_t ORGretaMarkIVDecoder::EnergyIsNegative()
{
  return (Bool_t) (GetEnergyHi() & 0x100); 
}

inline Int_t ORGretaMarkIVDecoder::GetSignedEnergy()
{
  if(EnergyIsNegative()) return -1 * (Int_t) GetEnergy();
  else return GetEnergy();
}


inline UShort_t ORGretaMarkIVDecoder::GetChannelNum()
{
  return (UShort_t) (fDataRecord[GetRecordOffset()] & 0xF);
}


inline UShort_t ORGretaMarkIVDecoder::GetMasterBoardID() 
{
  return (UShort_t) (fDataRecord[kORCAHeadLen] & 0xffff);   
}

inline UShort_t ORGretaMarkIVDecoder::GetMasterHeaderLength() 
{
  return (UShort_t) ((fDataRecord[kORCAHeadLen] & 0x7FF0000) >> 16);   
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

