// OR1DHistoHistogramsDecoder.hh

#ifndef _OR1DHistoHistogramsDecoder_hh_
#define _OR1DHistoHistogramsDecoder_hh_

#include <string>
#include "ORVDataDecoder.hh"

class OR1DHistoHistogramsDecoder : public ORVDataDecoder
{
  public:
    OR1DHistoHistogramsDecoder() {}
    virtual ~OR1DHistoHistogramsDecoder() {}
 
    
    virtual void Swap(UInt_t* dataRecord);
    /* Overloading Swap for 1Dhisto. */

    virtual inline UInt_t NKeyWordsOf(UInt_t* record) { return record[1]; }
    virtual inline UInt_t NKeyCharactersOf(UInt_t* record) { return NKeyWordsOf(record)*4; }
    virtual std::string KeysOf(UInt_t* record);
    virtual size_t NKeysOf(UInt_t* record);
    virtual std::string IthKeyOf(UInt_t* record, size_t iKey);
    virtual inline UInt_t NBinsOf(UInt_t* record) { return record[2 + NKeyWordsOf(record)]; }
    virtual inline UInt_t* HistogramDataOf(UInt_t* record) { return record + 3 + NKeyWordsOf(record); }
    virtual inline UInt_t IthBinOf(UInt_t* record, size_t iBin) { return HistogramDataOf(record)[iBin]; }
      
    virtual std::string GetDataObjectPath() { return "1DHisto:Histograms"; }
};

#endif
