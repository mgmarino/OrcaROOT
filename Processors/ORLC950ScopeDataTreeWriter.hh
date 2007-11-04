// ORLC950ScopeDataTreeWriter.hh

#ifndef _ORLC950ScopeDataTreeWriter_hh_
#define _ORLC950ScopeDataTreeWriter_hh_

#include "ORTek754DScopeDataTreeWriter.hh"
#include "ORLC950ScopeDataDecoder.hh"

class ORLC950ScopeDataTreeWriter : public ORTek754DScopeDataTreeWriter 
{
  public:
    ORLC950ScopeDataTreeWriter(std::string treeName = "") : ORTek754DScopeDataTreeWriter(new ORLC950ScopeDataDecoder, treeName)
      { fLC950Decoder = dynamic_cast<ORLC950ScopeDataDecoder*>(fDataDecoder); }
    virtual ~ORLC950ScopeDataTreeWriter() { delete fLC950Decoder; }

  protected:
    ORLC950ScopeDataDecoder* fLC950Decoder;
};

#endif
