// ORMCA927Decoder.hh

#ifndef _ORMCA927Decoder_hh_
#define _ORMCA927Decoder_hh_

#include "ORVMCADecoder.hh"
/*----------------------------------------------
 xxxx xxxx xxxx xxxx xxxx xxxx xxxx xxxx
 ^^^^ ^^^^ ^^^^ ^^-----------------------data id
                  ^^ ^^^^ ^^^^ ^^^^ ^^^^-length in longs
 xxxx xxxx xxxx xxxx xxxx xxxx xxxx xxxx
                       ^-----------------zdt (1=ZDT, 0=Normal)
                        ^----------------channel
						  ^^^^ ^^^^ ^^^^-device
 xxxx xxxx xxxx xxxx xxxx xxxx xxxx xxxx liveTime (20ms/bit, i.e. multiply by 0.02 to get seconds)
 xxxx xxxx xxxx xxxx xxxx xxxx xxxx xxxx realTime (20ms/bit, i.e. multiply by 0.02 to get seconds)
 xxxx xxxx xxxx xxxx xxxx xxxx xxxx xxxx 
                                 ^-------zdt Spectra enabled
                                  ^------zdt counting enabled
                                    ^^^^-zdt speed
 xxxx xxxx xxxx xxxx xxxx xxxx xxxx xxxx spare
 xxxx xxxx xxxx xxxx xxxx xxxx xxxx xxxx spare
 xxxx xxxx xxxx xxxx xxxx xxxx xxxx xxxx spare
 xxxx xxxx xxxx xxxx xxxx xxxx xxxx xxxx spare
 xxxx xxxx xxxx xxxx xxxx xxxx xxxx xxxx spare
 the spectrum follows and fills out the record (one spectrum/record)
 ------------------------------------------------*/
class ORMCA927Decoder: public ORVMCADecoder
{
  public:
    ORMCA927Decoder();
    virtual ~ORMCA927Decoder() {}
    enum EMCA927Consts {kBufHeadLen = 10};
                                           
    virtual std::string GetDataObjectPath()			{ return "ORMCA927Model:Spectrum"; }  
    //virtual std::string GetDictionaryObjectPath()	{ return "ORMCA927Model"; }  
    virtual bool SetDataRecord(UInt_t* record);
       
	//inline functions: ************************************************************************
	inline UShort_t GetType()				{ return ( fDataRecord[1] & 0x00002000 ) >> 13;}
	inline UShort_t GetChannelNum()			{ return ( fDataRecord[1] & 0x00001000 ) >> 12;}
	inline UShort_t GetDevice()				{ return ( fDataRecord[1] & 0x00000fff );}
	inline Double_t GetLiveTime()			{ return (fDataRecord[2] * 0.020);}
	inline Double_t GetRealTime()			{ return (fDataRecord[3] * 0.020);}
	inline Int_t    GetZDTMode()			{ return (fDataRecord[4]);}
	inline UInt_t*  GetSpectrumDataPointer(){ return (UInt_t*)(fDataRecord + kBufHeadLen);}
	
	virtual size_t  GetSpectrumLength();	
    virtual size_t	CopySpectrumData(UInt_t* spectrum, size_t len);
 	virtual UInt_t	GetEventSpectrumPoint( size_t /*event*/, size_t spectrumPoint );
	
    //Error checking:
    virtual bool IsValid();
    virtual void DumpBufferHeader();
   
    //debugging:
    void Dump(UInt_t* dataRecord);
};

#endif

