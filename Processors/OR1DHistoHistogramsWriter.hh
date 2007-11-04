// OR1DHistoHistogramsWriter.hh

#ifndef _OR1DHistoHistogramsWriter_hh_
#define _OR1DHistoHistogramsWriter_hh_

#include <string>
#include "ORDataProcessor.hh"
#include "OR1DHistoHistogramsDecoder.hh"


class OR1DHistoHistogramsWriter : public ORDataProcessor
{
  public:
    OR1DHistoHistogramsWriter(std::string histogramName = "ORCA1DHisto");
    virtual ~OR1DHistoHistogramsWriter() {}

    virtual EReturnCode ProcessMyDataRecord(UInt_t* record);

  protected:
    std::string fHistNameBase;
    OR1DHistoHistogramsDecoder* f1DHistoHistogramsDecoder;
};

#endif
