// ORTDC3377tdcTreeWriter.hh

#ifndef _ORTDC3377tdcTreeWriter_hh_
#define _ORTDC3377tdcTreeWriter_hh_

#include "ORBasicRDTreeWriter.hh"
#include "ORTDC3377tdcDecoder.hh"

class ORTDC3377tdcTreeWriter : public ORBasicRDTreeWriter
{
  public:
    ORTDC3377tdcTreeWriter(std::string treeName = "tdc3377Tree") : ORBasicRDTreeWriter(new ORTDC3377tdcDecoder, treeName)
      { fTDCDecoder = dynamic_cast<ORTDC3377tdcDecoder*>(fDataDecoder); }
    virtual ~ORTDC3377tdcTreeWriter() { delete fTDCDecoder; }

  protected:
    ORTDC3377tdcDecoder* fTDCDecoder;
};

#endif
