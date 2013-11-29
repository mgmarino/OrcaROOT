// ORUtilityProcessor.hh

#ifndef _ORUtilityProcessor_hh_
#define _ORUtilityProcessor_hh_

#include "ORDataProcessor.hh"

// This class is a base class for utility processors: processors that don't
// actually handle data, and therefore do not need a dataid or a decoder.
// Examples are the ORFileWriter and the ORCompoundProcessor.

class ORUtilityProcessor : public ORDataProcessor 
{
  public:
    ORUtilityProcessor() : ORDataProcessor(NULL) {}
    virtual ~ORUtilityProcessor() {}

    // overload the functions that set/use the unneccessary members of
    // ORDataProcessor
    virtual EReturnCode ProcessDataRecord(UInt_t*) { return kSuccess; }
    virtual void SetDataId() {}
    virtual void SetDecoderDictionary() {}

  private:
    // fDataDecoder is NULL by definition here and should not be accessed 
    // by deriving classes (to eliminate seg faults): make it private. If a
    // deriving class suddenly needs a decoder, it should make its own.
    // We know this hack is terrible OO but this achieves what we need without
    // having to refactor the entire code structure for this small feature.
    using ORDataProcessor::fDataDecoder; 
    // for that matter, make fDataId private too! It doesn't make any sense
    // in this context.
    using ORDataProcessor::fDataId;
};

#endif
