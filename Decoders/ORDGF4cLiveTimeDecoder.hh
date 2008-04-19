
#ifndef _ORDGF4cLiveTimeDecoder_hh_
#define _ORDGF4cLiveTimeDecoder_hh_

#include "ORLogger.hh"
#include "ORUtils.hh"

#include <string>
#include "TMath.h" 
#include "ORVDataDecoder.hh"

class ORDGF4cLiveTimeDecoder : public ORVDataDecoder
{
  public:
    ORDGF4cLiveTimeDecoder() {}
    virtual ~ORDGF4cLiveTimeDecoder() {}
    virtual inline bool SetDataRecord(UInt_t* record);
    virtual inline UInt_t CrateOf();
    virtual inline UInt_t CardOf();
    virtual inline Double_t GetRealTime();
    virtual inline Double_t GetOldRealTime();
    virtual inline Double_t GetRunTime();
    virtual inline Double_t GetOldRunTime();
    virtual inline Double_t GetChanLiveTime(UInt_t);
    virtual inline Double_t GetOldChanLiveTime(UInt_t);
    virtual inline UInt_t GetChanNumEvents(UInt_t);
    virtual inline UInt_t GetOldChanNumEvents(UInt_t);
    virtual inline Double_t CalculateTime(UInt_t*);
    virtual inline Double_t UnpackDouble(UInt_t);
    virtual std::string GetDataObjectPath() { return "ORDGF4cModel:LiveTime"; }
    virtual std::string GetDictionaryObjectPath() { return "ORDGF4cModel"; }
    virtual inline Int_t GetRecordLength();

  protected:
    UInt_t* fDataRecord;
    UInt_t fDataRecordLength;
};
    
inline bool ORDGF4cLiveTimeDecoder::SetDataRecord(UInt_t* record)
{ 
  fDataRecord = record;
  if(fDataRecord == NULL) return false;
  fDataRecordLength = LengthOf(fDataRecord);
  Int_t length = GetRecordLength();
  if(length < 0) length = 0;
  if((fDataRecordLength != (UInt_t) length)&&(length > 0))
  {
    ORLog(kWarning) 
      << "Record length from xml header does not match record header" << endl
      << "xml header: " << length << ", header length: " 
      << fDataRecordLength << endl;
  }
  if(fDataRecordLength == 13)
  {
    ORLog(kWarning) 
    << "This data was taken with an outdated version of Orca.  " << endl
    << "DGF4c run times, live times, and real times have been truncated." << endl
    << "Live time data record length: " << fDataRecordLength << endl;
  }else if(fDataRecordLength != 19)
  {
    ORLog(kWarning) 
    << "SetDataRecord(): The length of the live time record is not an expected value: " 
    << fDataRecordLength << endl;
  }
  ORLog(kDebug) << "length " << fDataRecordLength << endl;
  return true;
}

inline UInt_t ORDGF4cLiveTimeDecoder::CrateOf() //returns crate # of XIA card
{ 
  return (fDataRecord[2] & 0x01e00000) >> 21; 
}

inline UInt_t ORDGF4cLiveTimeDecoder::CardOf()
{ 
  return (fDataRecord[2] & 0x001f0000) >> 16; 
}

/*
  The GetOld* versions of the following functions
  are used for data taken with an old version of 
  Orca.  
*/

inline Double_t ORDGF4cLiveTimeDecoder::GetRealTime()
{ 
  return CalculateTime(&fDataRecord[3]);
}

inline Double_t ORDGF4cLiveTimeDecoder::GetOldRealTime()
{ 
  return UnpackDouble(fDataRecord[3]);
}

inline Double_t ORDGF4cLiveTimeDecoder::GetRunTime()
{ 
  return CalculateTime(&fDataRecord[5]);
}

inline Double_t ORDGF4cLiveTimeDecoder::GetOldRunTime()
{ 
  return UnpackDouble(fDataRecord[4]);
}

inline Double_t ORDGF4cLiveTimeDecoder::GetChanLiveTime(UInt_t channelNum)
{
  return CalculateTime(&fDataRecord[7+channelNum*3]);
}

inline Double_t ORDGF4cLiveTimeDecoder::GetOldChanLiveTime(UInt_t channelNum)
{
  return UnpackDouble(fDataRecord[5+2*channelNum]);
}

inline Double_t ORDGF4cLiveTimeDecoder::CalculateTime(UInt_t* recordPtr)
{
  UInt_t a = recordPtr[0];
  UInt_t bc = recordPtr[1];
  UInt_t c = bc & 0xffff;
  UInt_t b = bc >> 16;
  // time in seconds:
  Double_t time = (a*TMath::Power(65536.0,2.0)+b*65536.0+c)*1.0e-6/40.;
  ORLog(kDebug) << "CalculateTime(): a = " << a << ", b = " << b << ", c = " << c<< endl;
  ORLog(kDebug) << "CalculateTime(): time = " << time << "s" << endl;
  return time;
}



inline Double_t ORDGF4cLiveTimeDecoder::UnpackDouble(UInt_t record)
{
  /*  Orca outputs some numbers in double format, but the second
  4 of 8 bytes are lost.  This method reads out the sign, 
  exponent, and (truncated) mantissa of the double, and 
  returns the value. */   

  UInt_t sign = (record & 0x80000000) >> 31;
  UInt_t expMask = 0x7ff00000; 
  UInt_t exponent = ((record & expMask) >> 20);
  if(exponent > 0) exponent -= 1023;
  Double_t mantissa = (Double_t) (record & ~expMask);
  if(mantissa > 0) mantissa += 0x100000;
  ORLog(kDebug) << "UnpackDouble() "  << endl;
  return (1-2*sign)*mantissa*TMathh::Power(2,exponent-20.0);    
}

inline UInt_t ORDGF4cLiveTimeDecoder::GetChanNumEvents(UInt_t channelNum)
{ 
  return fDataRecord[9+3*channelNum];
}

inline UInt_t ORDGF4cLiveTimeDecoder::GetOldChanNumEvents(UInt_t channelNum)
{ 
  return fDataRecord[6+2*channelNum];
}

inline Int_t ORDGF4cLiveTimeDecoder::GetRecordLength()
{
  return GetIntValueFromKey("LiveTime:length",CrateOf(),CardOf());
}
	
#endif
