#ifndef _ORPyBindUtilities_hh_
#define _ORPyBindUtilities_hh_
#ifndef __CINT__
#include "ORLogger.hh"
#include <limits> 
#include "PyROOT.h"

#define CHECK_PY_ERROR                                \
  if (PyErr_Occurred() != NULL) {                     \
    PyErr_Print();                                    \
    PyErr_Clear();                                    \
    ORLog(kFatal) << "Error " << __PRETTY_FUNCTION__ << std::endl; \
  }

#define IMPLEMENT_PYBOUND_FUNCTION(retVal, aclass,    \
    funcname, pyFunc, retIfError, retIfNoExist)       \
retVal aclass::funcname()                             \
{                                                     \
    PyObject* res = CallFunc(#funcname);              \
    if (res == 0) {                                   \
      CHECK_PY_ERROR                                  \
      return retIfError;                              \
    }                                                 \
    retVal ret = ( res == Py_None) ? retIfNoExist :   \
                  (retVal) pyFunc(res);               \
    Py_XDECREF( res );                                \
    return ret;                                       \
}

#define IMPLEMENT_PYBOUND_VOID_FUNCTION(aclass, funcname)  \
void aclass::funcname()                               \
{                                                     \
    PyObject* res = CallFunc(#funcname);              \
    if (res == 0) CHECK_PY_ERROR                      \
    Py_XDECREF( res );                                \
}


#define IMPLEMENT_PYBOUND_RETCODE_FUNCTION(aclass, funcname) \
  IMPLEMENT_PYBOUND_FUNCTION(aclass::EReturnCode, aclass,    \
    funcname, PyInt_AsLong, kAlarm, kSuccess)

namespace ORPyBindUtils {

	PyObject* ObjectFromBuffer(void* buf, size_t len, size_t stride_size, bool is_signed, bool is_int);

    template<typename P>
	PyObject* ObjectFromBuffer(const P* buf, size_t len)
	{
		return ObjectFromBuffer((void*)buf, len*sizeof(P), sizeof(P), 
				std::numeric_limits<P>::is_signed, std::numeric_limits<P>::is_integer); 
	}


}

#endif /* ndef __CINT__ */
#endif /* _ORPyBindUtilities_hh_ */
