/*
Author        :  Padraic Finnerty   
Contact       :  paddy@unc.edu
Date Created  :  3/12/2012

CLASS DECLARATION:  ORVXMDecoder.hh

DESCRIPTION: Class to decode VXM motor controller data.  See below for the
						 Orca shipMotorState method for the Orca details.

ORCA RECORD DESCRIPTION:
	- (void) shipMotorState:(int)motorIndex
	{
		if( [[ORGlobal sharedGlobal] runInProgress] && (motorIndex < [motors count])){
			ORVXMMotor* aMotor = [motors objectAtIndex:motorIndex];
			//get the time(UT!)
			time_t	ut_time;
			time(&ut_time);
				
			unsigned long data[5];
			data[0] = dataId | 5;
			data[1] = ut_time;
			data[2] = (motorIndex<<16) | ([self uniqueIdNumber]&0x0000fffff);
			//encode the position 
			union {
				long asLong;
				float asFloat;
			}thePosition;
			
			thePosition.asFloat = [aMotor motorPosition]; //steps
			data[3] = thePosition.asLong;
			
			thePosition.asFloat = [aMotor conversion]; //steps/mm
			data[4] = thePosition.asLong;
		
			[[NSNotificationCenter defaultCenter] postNotificationName:ORQueueRecordForShippingNotification 
																object:[NSData dataWithBytes:data length:sizeof(long)*5]];
		}
	}

REVISIONS:
	*
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
    enum EORVXMConsts {kNumMotors = 4};
    
    virtual std::string GetDataObjectPath()			{ return "VXMModel:Position"; }
    // virtual std::string GetDictionaryObjectPath()	{ return "VXMModel"; }

		// ============================================
		// = The number of motors in this VXM module. =
		// ============================================
	  virtual inline UInt_t GetNumberOfMotors() {return (UInt_t)kNumMotors;}
	
		// =====================
		// = Return UNIX time. =
		// =====================
		virtual inline Long64_t GetTime(UInt_t* record) {return (Long64_t)record[1];} 
	
		// =====================================
		// = Return the motor id (between 0-3) =
		// =====================================
		virtual inline UShort_t GetMotorID(UInt_t* record) {return (UShort_t)(record[2] >> 16);} 
	
		// ==================================================
		// = Return raw position, this is in STEPS, not mm. =
		// ==================================================
		virtual inline Float_t GetRawPosition(UInt_t* record) {
			union {
			    long asLong;
			    float asFloat;
			}thePosition;
			thePosition.asLong = record[3];
			return thePosition.asFloat; }
	
		// =========================================
		// = Return conversion factor in steps/mm. =
		// =========================================
		virtual inline Float_t GetConversionFactor(UInt_t* record) {
			union {
			    long asLong;
			    float asFloat;
			}thePosition;
			thePosition.asLong = record[4];
			return thePosition.asFloat; }
			
		// ====================================================
		// = Return converted position, this is mm, NOT steps =
		// ====================================================
		Double_t GetConvertedPosition(UInt_t* dataRecord);

		// ================
		// = Debugging... =
		// ================
	  void Dump(UInt_t* dataRecord);
    
};

#endif
