// ORVXMTreeWriter.hh

#ifndef _ORVXMTreeWriter_hh_
#define _ORVXMTreeWriter_hh_

#include "ORVTreeWriter.hh"
#include "ORVXMDecoder.hh"

class ORVXMTreeWriter : public ORVTreeWriter
{
  public:
    ORVXMTreeWriter(std::string treeName = "VXM");
    virtual ~ORVXMTreeWriter();

    virtual EReturnCode ProcessMyDataRecord(UInt_t* record);
	virtual inline void Clear() { fNumberOfMotors = 0; fTime = 0; fMotorID = 0; fRawPosition = 0.0; fPositon_in_mm = 0.0, fConversion = 0.0;}

  protected:
    virtual EReturnCode InitializeBranches();

  protected:
    ORVXMDecoder* fVXMDecoder;
		UInt_t fNumberOfMotors;
		Long64_t fTime;
		UShort_t fMotorID;
		Float_t fRawPosition;
		Float_t fPositon_in_mm;
		Float_t fConversion;
};
#endif

