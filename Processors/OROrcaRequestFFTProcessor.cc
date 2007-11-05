// OROrcaRequestFFTProcessor.cc
#ifdef ORROOT_HAS_FFTW
#include "OROrcaRequestFFTProcessor.hh"
#include "TFFTRealComplex.h"
#include "TMath.h"

OROrcaRequestFFTProcessor::OROrcaRequestFFTProcessor() 
{
  /* Inputs required. Vector sizes can change*/
  SetInput("Waveform", &fWaveform, "Set of points to be fourier transformed.");
  SetInput("FFTOptions", &fFFTOptions, 
    "FFT options (ES(timate), M(easure), P(atient), EX(haustive)). Only the capita letter is needed.");

  /* Outputs sent back to Orca. */
  SetOutput("FFTReal", &fOutputReal, "Real FFT values");
  SetOutput("FFTComplex", &fOutputComplex, "Complex FFT values");
}

bool OROrcaRequestFFTProcessor::ExecuteProcess()
{
  /*  */
  fOutputReal.resize(fWaveform.size()/2 + 1);
  fOutputComplex.resize(fWaveform.size()/2 + 1);
  /* Since we're dealing with real data, only a certain number of values
   * are non-degenerate.  The rest is just the hermitian conjugate.     */  

  if (fWaveform.size() == 0 ) return true;
  /* Nothing to do! */

  /* Looking to see if we are supposed to do a Blackman or Hanning window. */
  std::string::size_type loc;
  if ((loc = fFFTOptions.find("WinBlack",0)) != std::string::npos ) {
    /* OK, we have to do Blackman windowing */
    for (size_t i=0;i<fWaveform.size();i++) {
      fWaveform[i] = fWaveform[i]*(0.42 - 0.5*TMath::Cos(i*TMath::TwoPi()/(fWaveform.size()-1))
        + TMath::Cos(2.*i*TMath::TwoPi()/(fWaveform.size()-1)));
    }
    fFFTOptions.erase(loc, 8);
  } else if ((loc = fFFTOptions.find("WinHamm")) != std::string::npos) {
    /* OK, we have to do Hamming windowing */
    for (size_t i=0;i<fWaveform.size();i++) {
      fWaveform[i] = fWaveform[i]*(0.54 - 0.46*TMath::Cos(i*TMath::TwoPi()/(fWaveform.size()-1)));
    }
    fFFTOptions.erase(loc, 7);
  }
  while ((loc = fFFTOptions.find(",")) != std::string::npos) {
    /* Getting rid of commas */
    fFFTOptions.erase(loc,1);
  } 

  TFFTRealComplex theFFTer(fWaveform.size(), kFALSE); 
  theFFTer.Init(fFFTOptions.c_str(), 0, 0);
  /* Setting up FFT, *not* in-place. 
   * Options correspond to "ES" (Estimate), "M" (measure), "P" (patient), 
   * "EX" (exhaustive).                                                   */
  
  theFFTer.SetPoints(&(fWaveform[0]));
  /* Setting the input points. */
  theFFTer.Transform();
  /* Run Transform */

  /*******************************************/
  theFFTer.GetPointsComplex(&(fOutputReal[0]),&(fOutputComplex[0]),kFALSE); 
  for (size_t i=0;i<fOutputComplex.size();i++) {
    fOutputReal[i] = fOutputReal[i]/TMath::Sqrt(fWaveform.size());
    fOutputComplex[i] = fOutputComplex[i]/TMath::Sqrt(fWaveform.size());
  }
  /* Setting output, rescaling, and we're done! */
  
  return true;
}

OROrcaRequestFFTProcessor::~OROrcaRequestFFTProcessor()
{
}

#endif


