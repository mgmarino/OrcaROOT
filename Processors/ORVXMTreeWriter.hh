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
	virtual inline void Clear() { fNumberOfMotors = 0; fTime = 0; fIsRunning = 0; fX = 0.0; fY = 0.0;}

  protected:
    virtual EReturnCode InitializeBranches();

  protected:
    ORVXMDecoder* fVXMDecoder;
		UInt_t fNumberOfMotors;
		Long64_t fTime;
		UShort_t fIsRunning;
		Float_t fX;
		Float_t fY;
};
#endif

