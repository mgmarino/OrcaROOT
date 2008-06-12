// ORVHistDecoder.hh

#ifndef _ORVHistDecoder_hh_
#define _ORVHistDecoder_hh_

#include <string>
#include "ORVDataDecoder.hh"

class ORVHistDecoder : virtual public ORVDataDecoder
{
  public:
    ORVHistDecoder() {}
    virtual ~ORVHistDecoder() {}

    // Functions for histogram booking. By default, the x-binning assumes
    // the x-values are integers running from 0 to NbinsX-1.
    virtual std::string GetHistName(int iHist) = 0;
    virtual std::string GetHistTitle(int iHist) = 0;
    virtual std::string GetXTitle() = 0;
    virtual size_t GetNbinsX() = 0;
    virtual double GetXLo() { return -0.5; }
    virtual double GetXHi() { return GetNbinsX() - 0.5; }

    // Functions for histogram filling
    virtual int GetHistIndex(UInt_t* record) = 0; // user must define an indexing scheme
    virtual size_t GetNEntries(UInt_t* /*record*/) { return 1; }
    virtual double GetX(UInt_t* record, size_t i) = 0; // the variable to be histogrammed
    virtual double GetWeight(UInt_t* /*record*/, size_t /*i*/) // optional: weight the entry
      { return 1.0; } 

    // Optional: multi-dimensional
    virtual inline size_t GetNDim() { return 1; }
    virtual std::string GetYTitle() { return "Y"; }
    virtual inline double GetY(UInt_t* /*record*/, size_t /*i*/) { return 0.0; }
    virtual inline size_t GetNbinsY() { return 1; }
    virtual inline double GetYLo() { return -0.5; }
    virtual inline double GetYHi() { return GetNbinsY() - 0.5; }
    virtual std::string GetZTitle() { return "Z"; }
    virtual inline double GetZ(UInt_t* /*record*/, size_t /*i*/) { return 0.0; }
    virtual inline size_t GetNbinsZ() { return 1; }
    virtual inline double GetZLo() { return -0.5; }
    virtual inline double GetZHi() { return GetNbinsZ() - 0.5; }
};

#endif
