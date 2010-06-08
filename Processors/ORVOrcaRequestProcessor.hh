// ORVOrcaRequestProcessor.hh

#ifndef _ORVOrcaRequestProcessor_hh_
#define _ORVOrcaRequestProcessor_hh_

#include <string>
#include <map>
#include <vector>
#include "ORDictionary.hh"

//! OrcaRequest struct
/*!
* Since we are storing void pointers within the struct, it is important to
* keep track of what type is inside the ORVOrcaReqInputOutput struct.  The
* following list outlines this convention:
*
*           Character:  
*                       - kString    ->   std::string*
*                       - kStringVec ->   vector<std::string>*
*                       - kInt       ->   int* 
*                       - kIntVec    ->   vector<int>*
*                       - kReal      ->   double* 
*                       - kRealVec   ->   vector<double>*
*
* This is really just important for internal management.  Classes deriving
* from this one should not need to know about this.  Classes that read out
* the OutputMap do need to know about this.  
* The internal variables for arrays are std::vectors because this vastly
* simplifies memory management.  It is not required to submit vectors, though,
* only pointers to memory and the length of memory to be read in (0 for
* non-arrays).
*/



//! Class handling Requests from Orca.
/*!
   All processors that handle requests from Orca should derive from
   this class.

   The SetInput and SetOutput functions are used by derived classes to
       register the types of output for which the processor class is looking.  The
       prescription is the following:
      
          SetInput("Name of Variable", address to variable, "Description of
            variable")
      
       It is important to note that "Name of Variable" is used to match to
       the xml input from Orca.  The description of the variable is used to
       register what is actually being done with this variable.  Since the
       address of the variable *must* be passed in, variables used as
       inputs and outputs must be class variables since the address must
       remain the same.  Also, to receive and send array
       input/output one must use the std::vector container class.
         SetOutput works similarly to SetInput.
      
       SetInput/SetOutput should be run in the derived classes' constructors.

       The LoadInput function is how the Request Manager loads input into the
       processor.  The name of the variable is passed in, along with a
       pointer to the value to be loaded.  
       The size variable is used when an array is passed in.  
       LoadInput returns true on success, false otherwise.

 */
class ORVOrcaRequestProcessor 
{
  public:
    ORVOrcaRequestProcessor();
    virtual ~ORVOrcaRequestProcessor();

    enum EORVOrcaRequestProcessorConsts { kString, kStringVec, kInt,
                                          kIntVec, kReal, kRealVec};
    struct ORVOrcaReqInputOutput {
      ORVOrcaRequestProcessor::EORVOrcaRequestProcessorConsts type;
      void* varAddress;
      std::string description;
    };


    // The following two functions are called by the Request Manager to begin
    // procesing.  They must be defined by the derived class.
    virtual const std::string GetNameOfRequestProcessor() = 0; 
    virtual bool ExecuteProcess() = 0;

    bool LoadInput(const std::string&, const std::string*, size_t size = 0); 
    bool LoadInput(const std::string&, const int*, size_t size = 0); 
    bool LoadInput(const std::string&, const double*, size_t size = 0); 
    bool LoadInput(const std::string&, const ORDictValueA*); 
    const std::map< std::string, ORVOrcaReqInputOutput>* GetInputMap() {return &fInputMap;}
    const std::map< std::string, ORVOrcaReqInputOutput>* GetOutputMap() {return &fOutputMap;}

  protected:
    void SetInput(std::string, std::string*, std::string); 
    void SetInput(std::string, std::vector<std::string>*, std::string); 
    void SetInput(std::string, int*, std::string);
    void SetInput(std::string, std::vector<int>*, std::string);
    void SetInput(std::string, double*, std::string);
    void SetInput(std::string, std::vector<double>*, std::string);
    void SetOutput(std::string, std::string*, std::string); 
    void SetOutput(std::string, std::vector<std::string>*, std::string); 
    void SetOutput(std::string, int*, std::string);
    void SetOutput(std::string, std::vector<int>*, std::string);
    void SetOutput(std::string, double*, std::string);
    void SetOutput(std::string, std::vector<double>*, std::string);

  private:
    // These are kept private because we really don't want any derived class
    // messing with them except through the interface provided in the
    // protected functions.  There is no reason to access the maps directly.

    std::map< std::string, ORVOrcaReqInputOutput>::iterator fInputMapIter;
    std::map< std::string, ORVOrcaReqInputOutput> fInputMap;
    std::map< std::string, ORVOrcaReqInputOutput> fOutputMap;
};

#endif
