// ORVXMDecoder.hh
/*
Record description from ORCA.
unsigned long data[5];
data[0] = dataId | 5;
data[1] = ut_time;
data[2] = (running?1:0)<<16 | ([self uniqueIdNumber]&0x0000fffff);

//encode the position 
union {
    long asLong;
    float asFloat;
}thePosition;
thePosition.asFloat = [self xyPosition].x;
data[3] = thePosition.asLong;

thePosition.asFloat = [self xyPosition].y;
data[4] = thePosition.asLong;
*/
#ifndef _ORVXMDecoder_hh_
#define _ORVXMDecoder_hh_

#include "ORVDataDecoder.hh"
#include <string>

class ORVXMDecoder: public ORVDataDecoder
{
  public:
		ORVXMDecoder() {}
    virtual ~ORVXMDecoder() {}
    enum EORVXMConsts {kNumMotors = 2};
    
    virtual std::string GetDataObjectPath()			{ return "ORMCA927Model:Spectrum"; }  // FIXME
    virtual std::string GetDictionaryObjectPath()	{ return "ORMCA927Model"; }  		    // FIXME

		// ============================================
		// = The number of motors in this VXM module. =
		// ============================================
	  virtual inline UInt_t GetNumberOfMotors() {return (UInt_t)kNumMotors;}
	
		// ================
		// = Return time. =
		// ================
		virtual inline Long64_t GetTime(UInt_t* record) {return (Long64_t)record[1];} 
	
		// =============================================
		// = Return whether or not the VXM is running. =
		// =============================================
		virtual inline UShort_t GetIsRunning(UInt_t* record) {return (UShort_t)(record[2] >> 16);} 
	
		// ================================================
		// = Return X position, this is in STEPS, not mm. =
		// ================================================
		virtual inline Double_t GetXPosition(UInt_t* record) {
			union {
			    long asLong;
			    double asDouble;
			}thePosition;
			thePosition.asLong = record[3];
			return thePosition.asDouble; }
	
		// ================================================
		// = Return Y position, this is in STEPS, not mm. =
		// ================================================
		virtual inline Double_t GetYPosition(UInt_t* record) {
			union {
			    long asLong;
			    double asDouble;
			}thePosition;
			thePosition.asLong = record[4];
			return thePosition.asDouble; }

		// ================
		// = Debugging... =
		// ================
	  void Dump(UInt_t* dataRecord);
    
};

#endif
