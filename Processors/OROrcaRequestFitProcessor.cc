// OROrcaRequestFitProcessor.cc

#include "OROrcaRequestFitProcessor.hh"

#include "TROOT.h"
#include "ORLogger.hh"
#include "TF1.h"
#include "TGraph.h"


using namespace std;

OROrcaRequestFitProcessor::OROrcaRequestFitProcessor() 
{
  /* Inputs required. Vector sizes can change*/
  SetInput("FitLowerBound", &fIntLower, "Lower bound of the fit");
  SetInput("FitUpperBound", &fIntUpper, "Upper bound of the fit");
  SetInput("FitFunction", &fFitFunc, 
    "Fit Function, either custom or predefined (see ROOT TFormula)");
  SetInput("FitParameters", &fInputParameters, 
    "Input parameters, not required for predefined functions");
  SetInput("FitOptions", &fFitOptions, "Fit Options");
  SetInput("FitYValues", &fYVector, "Y values to fit");

  /* Outputs sent back to Orca. */
  SetOutput("FitOutputYValues", &fOutputYVector, "Fitted Y values");
  SetOutput("FitOutputParameters", &fOutputParamVector, "Parameter list of fit");
  SetOutput("FitOutputParametersNames", &fOutputParamNamesVector, 
    "Parameter name list of fit");
  SetOutput("FitOutputErrorParameters", &fOutputParamErrorsVector, 
    "Error Parameter list of fit");
  SetOutput("FitChiSquare", &fOutputChiSquare, "Calculated chi-square of the fit");
  SetOutput("FitEquation", &fOutputEquation, "Output equation with fit values");
}

bool OROrcaRequestFitProcessor::ExecuteProcess()
{
  if (fYVector.size() == 0) {
    ORLog(kError) << "Size 0 for input y array. " << endl;
    return false;
  }
  TF1* f1 = new TF1("f1", fFitFunc.c_str());
  if (!f1) {
    ORLog(kError) << "Error allocating function for: " << fFitFunc <<  endl;
    return false;
  }
  if (f1->GetExpFormula().Sizeof() <= 1) {
    /* Good indication that it was unable to parse; fail here.*/
    ORLog(kError) << "Error parsing fit function.  Not well-formed." << endl;
    return false;
  }
  std::string histName = "temp";
  TGraph* graph = new TGraph(fYVector.size());
  if (!graph) {
    ORLog(kError) << "Error allocating graph." <<  endl;
    return false;
  }
  for (size_t i=0;i<fYVector.size();i++) {
    graph->SetPoint(i, i, fYVector[i]);
  }

  /***************************************************/
  /* Checking the parameters.  If none have come in, then we zero them out. */
  /***************************************************/
  if (fInputParameters.size() != 0) {
    if (fInputParameters.size() != (size_t)f1->GetNpar()) { 
      ORLog(kError) << "Incorrect number of parameters...trying to continue" 
        << endl;
    }
    fInputParameters.resize(f1->GetNpar(), 0.);
  } else {
    fInputParameters.clear();
    fInputParameters.resize(f1->GetNpar(), 0.);
  } 
  for (size_t i=0;i<fInputParameters.size();i++) {
    f1->SetParameter(i, fInputParameters[i]);
  }
  /***************************************************/
  /***************************************************/
  
  if (fIntUpper < fIntLower) {
    /* Making sure we're getting correct bounds. */
    ORLog(kWarning) << "Bound parameters incorrect, trying to continue." << endl; 
    int temp = fIntUpper;
    fIntUpper = fIntLower;
    fIntLower = temp;
  }
  if (fYVector.size() < (size_t)fIntUpper) fIntUpper = fYVector.size()-1;

  fFitOptions+="NQ"; // don't store, be quiet
  graph->Fit(f1, fFitOptions.c_str(), "", fIntLower, fIntUpper);
  /* Actually performing the fit.  (All this for one line of code!!!) */

  /* Now sorting out output parameters and loading them with correct
   * data.  */
  fOutputParamVector.resize(f1->GetNpar());
  fOutputParamErrorsVector.resize(f1->GetNpar());
  fOutputParamNamesVector.resize(f1->GetNpar());

  fOutputParamVector.assign(f1->GetParameters(), 
    f1->GetParameters()+f1->GetNpar());
  fOutputParamErrorsVector.assign(f1->GetParErrors(),
    f1->GetParErrors()+f1->GetNpar());
  for (size_t i=0;i<fOutputParamNamesVector.size();i++) {
    fOutputParamNamesVector[i] = f1->GetParName(i);
  }
  fOutputYVector.resize(((size_t)fIntUpper<=fYVector.size()) ? fIntUpper : fYVector.size());
  for (size_t i=0;i<fOutputYVector.size();i++) {
    fOutputYVector[i] = (i<(size_t)fIntLower) ? 0. : f1->Eval(i);
  }
  fOutputEquation = f1->GetExpFormula();
  /* This is useful if a user-defined equation was input. */

  fOutputChiSquare = (f1->GetNDF()==0) ? 0 : f1->GetChisquare()/f1->GetNDF(); 
  
  fInputParameters.clear();
  delete graph; 
  delete f1;

  return true;
}

OROrcaRequestFitProcessor::~OROrcaRequestFitProcessor()
{
}



