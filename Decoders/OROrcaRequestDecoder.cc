// OROrcaRequestDecoder.cc

#include "OROrcaRequestDecoder.hh"
#include "ORLogger.hh"
#include "ORUtils.hh"
using namespace std;


//**************************************************************************************

OROrcaRequestDecoder::OROrcaRequestDecoder() { fDataRecord = NULL; }
	
bool OROrcaRequestDecoder::ParseDataRecord(UInt_t* dataRecord) 
{
  fDataRecord = dataRecord;

  ORLog(kDebug) << "ParseDataRecord(): Parsing the data record..." << endl;
  if (LengthOf(fDataRecord) - kBufferLength <=0) {
    ORLog(kWarning) << "Record length of 0." << endl;
    return false;
  }
  if (!fXMLRecord.LoadXmlPlist((char*) (fDataRecord + kBufferLength),
     sizeof(UInt_t)*(LengthOf(fDataRecord)-kBufferLength))) {
    ORLog(kError) << "Error parsing data record..." << endl;
    return false;
  }

  /* Getting XML Values */
  // Getting Tag Number
  ORLog(kDebug) << "ParseDataRecord(): Getting the Request Tag Number..." << endl;
  const ORDictValueI* reqTagPtr = 
    dynamic_cast<const ORDictValueI*>(fXMLRecord.LookUp("Request Tag Number"));
  if(!reqTagPtr) {
    ORLog(kError) << "Error getting Request Tag Number. " << endl;
    return false;
  }
  fRequestTagNumber = reqTagPtr->GetI();

  // Getting Request Type 
  ORLog(kDebug) << "ParseDataRecord(): Getting the Request Type..." << endl;
  const ORDictValueS* reqTypePtr = 
    dynamic_cast<const ORDictValueS*>(fXMLRecord.LookUp("Request Type"));
  if(!reqTypePtr) {
    ORLog(kError) << "Error getting Request Type. " << endl;
    return false;
  }
  fRequestType = reqTypePtr->GetS(); 

  // Getting Request Option
  
  ORLog(kDebug) << "ParseDataRecord(): Getting the Request Option..." << endl;
  const ORDictValueS* reqOptionPtr = 
    dynamic_cast<const ORDictValueS*>(fXMLRecord.LookUp("Request Option"));
  if(!reqOptionPtr) {
    ORLog(kError) << "Error getting Request Option. " << endl;
    return false;
  }
  fRequestOption = reqOptionPtr->GetS(); 


  // Getting Inputs
  ORLog(kDebug) << "ParseDataRecord(): Getting the Request Inputs..." << endl;
  fRequestInputs = dynamic_cast<const ORDictionary*>(fXMLRecord.LookUp("Request Inputs")); 
  if(!fRequestInputs) {
    ORLog(kError) << "Error getting Request Inputs. " << endl;
    return false;
  }

  ORLog(kDebug) << "ParseDataRecord(): Exiting" << endl;
  return true;
}


//debugging: *************************************************************************


void OROrcaRequestDecoder::Dump(UInt_t*) //debugging 
{
 //eventually something will be here 
}
