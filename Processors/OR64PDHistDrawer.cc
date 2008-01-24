// OR64PDHistDrawer.cc

#include "OR64PDHistDrawer.hh"

#include "TRandom3.h"
#include "ORLogger.hh"
#include <iostream>
#include <fstream>


using namespace std;

OR64PDHistDrawer::OR64PDHistDrawer(ORShaperShaperDecoder* histDecoder) :
ORDataProcessor(histDecoder)
{
  fShaperDecoder = histDecoder;
	
	app = new TApplication("OR64PDHistDrawer", NULL, NULL, NULL, 0);

	Tcanvas = new TCanvas("c1","ORCAroot Rate histogram");
	Ecanvas = new TCanvas("c2","ORCAroot Energy histogram");
	fEnergyHist = NULL;
	fRateHist = NULL;
	r3 = new TRandom3(0);
	gRandom = r3;

}
OR64PDHistDrawer::~OR64PDHistDrawer() 
{
	if (r3 != NULL ) delete r3;
	if (Ecanvas ) delete Ecanvas;
	if (Tcanvas ) delete Tcanvas;
	if ( app ) delete app;
}

ORDataProcessor::EReturnCode OR64PDHistDrawer::StartProcessing()
{
  if(fShaperDecoder == NULL) {
    ORLog(kWarning) << "StartProcessing(): fShaperDecoder was NULL, can't proceed" << endl;
    KillProcessor();
    return kFailure;
  }
  return kSuccess;
}

ORDataProcessor::EReturnCode OR64PDHistDrawer::StartRun()
{
	//read in hist settings
	ifstream histFile;
	histFile.open("Hist_Settings.txt");	//open a file
	
	if(!histFile.is_open()) {
		ORLog(kWarning) << "Hist_Settings.txt file couldn't open, using defaults" << endl;
	}
	maxE = 100;
	threshold = 10;
	nbinsE = 200;
	maxT = 1800;
	fRefreshTime = 10;
	reset = 0;

	int lineNum=0;
	char line[196];

	histFile.exceptions(ifstream::badbit);
	try {
		//first line is definition, then settings:
		histFile.getline(line, 196);
		lineNum++;
		histFile >> maxE >> threshold >> nbinsE >> maxT >> fRefreshTime;
		lineNum++;
	} catch ( ifstream::failure fail) {
		ORLog(kWarning) << "StartRun(): Problem reading Hist_Settings.txt at line " << lineNum << endl;
	}
	histFile.close();
	
	//read in energy calibration from file
	ifstream ecalFile;
	ecalFile.open("ecal_test.txt");	//open a file
	
	if(!ecalFile.is_open()) {
		ORLog(kWarning) << "StartRun(): Energy Cal file ecal_test.txt didn't open" << endl;
//		KillProcessor();
//		return kFailure;
		fUseEcal = false;
	}

	UInt_t ecal_ca,ecal_ch, waste;
	Double_t ecal_s,ecal_s_e,ecal_c,ecal_c_e;
	ecalFile.exceptions(ifstream::badbit);
	lineNum=0;
	try {
		//first line is definition, then parameters:
		ecalFile.getline(line, 196);
		fUseEcal = true;
		while ( !ecalFile.eof() ) {
			lineNum++;
			ecalFile >> waste >> ecal_ca >> ecal_ch >> ecal_s >> ecal_s_e >> ecal_c >> ecal_c_e;
			if ( ecal_ca >8 || ecal_ca < 1 || ecal_ch >7) {
				ORLog(kWarning) << "StartRun(): Energy Cal has wrong number of cards or channels" << endl;
				//KillProcessor();
				//return kFailure;
				fUseEcal = false;
				break;
			}
			ecal_slope[ecal_ca][ecal_ch]=ecal_s;
			ecal_slope_error[ecal_ca][ecal_ch]=ecal_s_e;
			ecal_constant[ecal_ca][ecal_ch]=ecal_c;
			ecal_constant_error[ecal_ca][ecal_ch]=ecal_c_e;
			}
		
		} catch ( ifstream::failure fail) {
			ORLog(kWarning) << "StartRun(): Problem reading ecal_test.txt at line " << lineNum << endl;
//			KillProcessor();
//			return kFailure;
			fUseEcal = false;
		
	}
	ecalFile.close();			//close it
	
			
//reset histograms, deleted when file closed	
	if ( fEnergyHist != NULL) {
		fEnergyHist = NULL;	
	}
	if ( fRateHist != NULL) {
		fRateHist = NULL;
	}
	fLastDrawTime = 0;
  return kSuccess;
}
ORDataProcessor::EReturnCode OR64PDHistDrawer::ProcessRecordTime(Double_t t_s)
{
	time = t_s;
	return kSuccess;
}

ORDataProcessor::EReturnCode OR64PDHistDrawer::ProcessMyDataRecord(UInt_t* record)
{
	if(fEnergyHist == NULL && fUseEcal) {
		fEnergyHist = new TH1D("Energy","Energy Spectrum of 61 Pixels", nbinsE, 0, maxE);
		fEnergyHist->SetYTitle("Counts");
		fEnergyHist->SetXTitle("Energy [keV]");
	} else if (fEnergyHist == NULL && !fUseEcal ) {
		fEnergyHist = new TH1D("ADC","ADC Spectrum of 61 Pixels", 2048, 0, 2048);
		fEnergyHist->SetYTitle("Counts");
		fEnergyHist->SetXTitle("ADC Bin");
		
	}
	if(fRateHist == NULL) {
		fRateHist = new TH1D("Rate","Total Rate in 61 Pixels", maxT, 0, maxT);
		fRateHist->SetYTitle("Counts/sec");
		fRateHist->SetXTitle("Time [s]");
	}
	
	adc =  fShaperDecoder->ADCValueOf(record);
	card = fShaperDecoder->CardOf(record);
	channel = fShaperDecoder->ChannelOf(record);

	//Draw histograms every 10 seconds
	if ( ( time - fLastDrawTime ) > fRefreshTime ) {
		fLastDrawTime = time;
		Ecanvas->cd();
		fEnergyHist->Draw();
		Ecanvas->Update();
		Tcanvas->cd();
		fRateHist->Draw();
		Tcanvas->Update();
		// app->Run();
	}
	
	//don't draw events from low-gain pixels
	if ( card ==6&&channel==4 ) return kSuccess;
	if ( card ==6&&channel==6) return kSuccess;
	if( card == 5&&channel==4) return kSuccess;

	if ( fUseEcal ) {
		//convert adc to energy for each event
		/* adc values are already binned, correct for this by throwing random number */
		energy = (adc-ecal_constant[card][channel])/ecal_slope[card][channel];	
		ELow = (adc-0.5-ecal_constant[card][channel])/ecal_slope[card][channel];
		EHigh = (adc+0.5-ecal_constant[card][channel])/ecal_slope[card][channel];
		if ( EHigh < maxE && ELow > threshold ) {
			//reset rate histogram when run time exceeds hist limit
			if ( time > maxT ) {
				if (reset < floor(time/maxT) ) {
					reset++;
					fRateHist->Reset();
				}
				time = time - (reset)*maxT;
			}
			
			upBin = fEnergyHist->FindBin(EHigh, 0, 0);
			lowBin = fEnergyHist->FindBin(ELow, 0, 0);
			if ( upBin == lowBin ) {
				//adc bin fully contained in an energy bin
				fEnergyHist->Fill(energy);
				fRateHist->Fill(time);
			} else {
				Double_t binEdge = fEnergyHist->GetBinLowEdge(upBin);
				Double_t Prob = (EHigh-binEdge)/(EHigh-ELow);
				Double_t RndNum = r3->Rndm();
				//determine which energy bin to incriment
				if( RndNum > Prob ) {
					fEnergyHist->Fill(EHigh);
					fRateHist->Fill(time);
				} else {
					fEnergyHist->Fill(ELow);
					fRateHist->Fill(time);
				}	
				
			}
		}
		
	} else {
		fEnergyHist->Fill(adc);
		fRateHist->Fill(time);
	}
	
	return kSuccess;
}

ORDataProcessor::EReturnCode OR64PDHistDrawer::EndRun()
{
	if (fRateHist != NULL ) {
		fRateHist->Write();
	}
	if (fEnergyHist != NULL ) {
		fEnergyHist->Write();
	}
	return kSuccess;
}

ORDataProcessor::EReturnCode OR64PDHistDrawer::EndProcessing()
{
	// if a TFile is open, the histos will be deleted by the file. If not,
	// gDirectory will be named "Rint", and the histos will remain in memory.
	
	
	if(string(gDirectory->GetName()) != "Rint") return kSuccess;
	if (fRateHist != NULL ) {
		delete fRateHist;
		fRateHist = NULL;
	}
	if (fEnergyHist != NULL ) {
		delete fEnergyHist;
		fEnergyHist = NULL;
	}
  return kSuccess;
}
