// OR64PDHistDrawer.hh

#ifndef _OR64PDHistDrawer_hh_
#define _OR64PDHistDrawer_hh_

#include "TROOT.h"
#include "TApplication.h"
#include "TH1.h"
#include "TCanvas.h"
#include "ORDataProcessor.hh"
#include "ORShaperShaperDecoder.hh"

class TRandom3;


class OR64PDHistDrawer : public ORDataProcessor
{
  public:
    OR64PDHistDrawer(ORShaperShaperDecoder* histDecoder);
	virtual ~OR64PDHistDrawer();

    virtual EReturnCode StartProcessing();
    virtual EReturnCode StartRun();
    virtual EReturnCode ProcessMyDataRecord(UInt_t* record);
  	virtual EReturnCode ProcessRecordTime(Double_t t_s);
    virtual EReturnCode EndRun();
    virtual EReturnCode EndProcessing();

  protected:
	ORShaperShaperDecoder *fShaperDecoder;
	TH1* fEnergyHist;
	TH1* fRateHist;
	TCanvas* Tcanvas;
	TCanvas* Ecanvas;
	TApplication *app;
	TRandom3 *r3;
	
	UInt_t card;
	UInt_t channel;
	UInt_t adc;
	
	Double_t ecal_slope[9][8];		
	Double_t ecal_slope_error[9][8];
	Double_t ecal_constant[9][8];
	Double_t ecal_constant_error[9][8];
	Double_t maxE;
	Double_t threshold; 
	Double_t energy;
	Double_t	ELow;
	Double_t EHigh;
	size_t lowBin;
	size_t upBin;
	UInt_t nbinsE;
	Int_t maxT;
	Double_t time;
	Double_t fLastDrawTime;
	Double_t fRefreshTime;
	Int_t reset;
	bool fUseEcal;
	


};

#endif
