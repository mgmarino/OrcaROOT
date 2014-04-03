#include "ORPyDataProcessor.hh"
#include "ORPyBindUtilities.hh"

ClassImp(ORPyDataProcessor)

IMPLEMENT_PYBOUND_RETCODE_FUNCTION(ORPyDataProcessor, StartProcessing)
IMPLEMENT_PYBOUND_RETCODE_FUNCTION(ORPyDataProcessor, StartRun)
IMPLEMENT_PYBOUND_RETCODE_FUNCTION(ORPyDataProcessor, EndRun)
IMPLEMENT_PYBOUND_RETCODE_FUNCTION(ORPyDataProcessor, EndProcessing)

ORPyDataProcessor::EReturnCode ORPyDataProcessor::ProcessMyDataRecord(UInt_t* record)
{
  assert(GetDecoder());
  PyObject* send_obj = ORPyBindUtils::ObjectFromBuffer(record, GetDecoder()->LengthOf(record)); 

  PyObject* res = CallFunc("ProcessMyDataRecord", send_obj);
  if (res == 0) return kAlarm;
  EReturnCode ret = (res == Py_None) ? kSuccess : (EReturnCode) PyInt_AsLong(res);
  Py_XDECREF( res );
  Py_XDECREF( send_obj );
  return ret;

}
