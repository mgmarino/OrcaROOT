// ORCaen5720Decoder.hh
// Basic information taken from ORCaen1720Decoder.hh provided by Jarek Kaspar.  
// Updated by Laura Bodine
// Zpack2.5 and zerolengthencoding are not currently handled here...
// if your data is formatted that way then you must add functionality to this decoder

#ifndef _ORCaen5720Decoder_hh_
#define _ORCaen5720Decoder_hh_

#include "ORVDataDecoder.hh"

class ORCaen5720Decoder : public ORVDataDecoder
{
  public:
    ORCaen5720Decoder() {}

    virtual ~ORCaen5720Decoder() {}

  enum ECaen5720Consts { kEventDataLen = 1024, kEventHeaderLen = 4, kWFLen = 2018 };


	 virtual inline UInt_t CardOf(UInt_t* /*record*/)
	 	{ return 0; }

         virtual inline UInt_t UnitNumberOf(UInt_t* record)
                { return record[1]>>16&0xf;}

         virtual inline UInt_t EventSize(UInt_t* record) //number of 32-bit words in an event
	 	{ return record[2] & 0x0fffffff; }

         virtual inline UInt_t ChannelMask(UInt_t* record)
		{ return record[3] & 0xff; }


  	 virtual inline UInt_t Pattern(UInt_t* record)
  	 	{ return (record[3] >> 8) & 0xffff; }

	 virtual inline UInt_t GetNEventsOf(UInt_t* record)
                { return (LengthOf(record)-2)/EventSize(record); }

         virtual inline UInt_t EventCount(UInt_t* record)
                { UInt_t * test = GetEventPointer(record);
                  return test[2] & 0xffffff; }

         virtual inline UInt_t Clock(UInt_t* record)
                { UInt_t * test = (GetEventPointer(record)); 
	           return test[3];}

         virtual inline bool Packed(UInt_t* record) 
                { return (record[1]>>0  & 0x1);}

       	 virtual UInt_t TraceLength(UInt_t* record);

  /**************************************************************************
   // Copy waveforms iteratively because hte packed2.5 data requires it
   *************************************************************************/
        virtual void CopyTrace(UInt_t* record, UInt_t *Waveform, UInt_t numSamples); //trace of first active channel

        virtual void CopyTraces(UInt_t* record, UInt_t *Waveform0, UInt_t *Waveform1,UInt_t *Waveform2,UInt_t *Waveform3, UInt_t numSamples); //trace of all channels (inactive read 0);

        virtual inline UInt_t* GetEventPointer(UInt_t* record) {return (record + 2);};

        virtual std::string GetDataObjectPath()
                { return "ORDT5720Model:waveform"; }

        virtual inline std::string GetDictionaryObjectPath() 
                { return "ORDT5720Model"; } 


};

#endif

