// ORPyBinder.hh

#ifndef _ORPyBinder_hh_
#define _ORPyBinder_hh_

#include "TObject.h"
#include "TPyReturn.h"

class ORPyBinder : public TObject
{
  public:
    ORPyBinder() : fSelf(0) {}
    ~ORPyBinder();

  protected:

    PyObject* CallFunc( const char* funcname, PyObject* args = 0 ) const;

  private:
    void SetupSelf() const;
    mutable PyObject* fSelf;


    ClassDef(ORPyBinder, 0)

};

#endif
