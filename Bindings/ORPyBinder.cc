#include "ORPyBinder.hh"
#include "PyROOT.h"
#include "MemoryRegulator.h"
#include "MethodProxy.h"

ClassImp(ORPyBinder)

//______________________________________________________________________________
ORPyBinder::~ORPyBinder()
{
  Py_XDECREF( fSelf );
}

//______________________________________________________________________________
void ORPyBinder::SetupSelf() const
{
    if (fSelf != NULL) return;
    // Get the python object I'm referring to
    fSelf = 
      PyROOT::TMemoryRegulator::RetrieveObject(
        const_cast<ORPyBinder*>(this), IsA());

    if (fSelf == NULL) {
      Py_INCREF( Py_None );
      fSelf = Py_None;
    }
}

//______________________________________________________________________________
PyObject* ORPyBinder::CallFunc( const char* funcname, PyObject* args ) const 
{
   SetupSelf();

   PyObject* result = 0;
   PyObject* pymethod = 
     PyObject_GetAttrString( fSelf, const_cast<char*>(funcname)); 

   if ( ! PyROOT::MethodProxy_CheckExact( pymethod ) ) {
     result = PyObject_CallFunctionObjArgs( pymethod, args, 0 );
   } else {
      Py_INCREF( Py_None );
      result = Py_None;
   }
   Py_XDECREF( pymethod );

   return result;
}


