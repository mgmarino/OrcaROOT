#include "ORPyTreeWriter.hh"
#include "ORPyBindUtilities.hh"

ClassImp(ORPyTreeWriter)

IMPLEMENT_PYBOUND_RETCODE_FUNCTION(ORPyTreeWriter, StartProcessing)
IMPLEMENT_PYBOUND_RETCODE_FUNCTION(ORPyTreeWriter, EndProcessing)
IMPLEMENT_PYBOUND_RETCODE_FUNCTION(ORPyTreeWriter, InitializeBranches)
IMPLEMENT_PYBOUND_VOID_FUNCTION(ORPyTreeWriter, Clear)

ORPyTreeWriter::EReturnCode ORPyTreeWriter::ProcessMyDataRecord(UInt_t* record)
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
