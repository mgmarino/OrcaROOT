#include "ORPyBindUtilities.hh"
#include <string>
#include <cctype>

#if PY_VERSION_HEX < 0x03000000
template<class T>
Py_ssize_t or_buffer_length( PyObject* self )
{   
    Py_ssize_t nlen = (*(PyBuffer_Type.tp_as_sequence->sq_length))(self);
    return nlen/sizeof(T);
}

#define PYOR_PREPARE_BUFFER_TYPE(name) \
Py_ssize_t name##_buffer_length( PyObject* self) { return or_buffer_length< name ## _t>(self); } \
   PyTypeObject      PyOr##name##Buffer_Type;                                  \
   PySequenceMethods PyOr##name##Buffer_SeqMethods = *(PyBuffer_Type.tp_as_sequence);

   PYOR_PREPARE_BUFFER_TYPE( Char )
   PYOR_PREPARE_BUFFER_TYPE( Short )
   PYOR_PREPARE_BUFFER_TYPE( UShort )
   PYOR_PREPARE_BUFFER_TYPE( Int )
   PYOR_PREPARE_BUFFER_TYPE( UInt )
   PYOR_PREPARE_BUFFER_TYPE( Long )
   PYOR_PREPARE_BUFFER_TYPE( ULong )
   PYOR_PREPARE_BUFFER_TYPE( Float )
   PYOR_PREPARE_BUFFER_TYPE( Double )

//- constructor/destructor ------------------------------------------------------
#define PYOR_INSTALL_PYBUFFER_METHODS( name )                           \
       PyOr##name##Buffer_Type.tp_name            = (char*)"ORPy"#name"Buffer";       \
       PyOr##name##Buffer_Type.tp_base            = &PyBuffer_Type;                   \
       PyOr##name##Buffer_Type.tp_as_buffer       = PyBuffer_Type.tp_as_buffer;       \
       PyOr##name##Buffer_SeqMethods.sq_length    = (lenfunc) name ## _buffer_length; \
       PyOr##name##Buffer_Type.tp_as_sequence     = &PyOr##name##Buffer_SeqMethods;   \
       PyType_Ready( &PyOr##name##Buffer_Type );

class PyORGlobals {
    public:    
        PyORGlobals();
};

PyORGlobals::PyORGlobals() 
{
   PYOR_INSTALL_PYBUFFER_METHODS( Char )
   PYOR_INSTALL_PYBUFFER_METHODS( Short )
   PYOR_INSTALL_PYBUFFER_METHODS( UShort )
   PYOR_INSTALL_PYBUFFER_METHODS( Int )
   PYOR_INSTALL_PYBUFFER_METHODS( UInt )
   PYOR_INSTALL_PYBUFFER_METHODS( Long )
   PYOR_INSTALL_PYBUFFER_METHODS( ULong )
   PYOR_INSTALL_PYBUFFER_METHODS( Float )
   PYOR_INSTALL_PYBUFFER_METHODS( Double )
}

PyORGlobals gAllGlobals;
#endif


PyObject* ORPyBindUtils::ObjectFromBuffer(void* buf, 
        size_t len, size_t stride_size, bool is_signed, bool is_int)
{
  // Helper function to generate a PyObject from a buffer
#if PY_VERSION_HEX >= 0x03000000
    std::string type;
    switch (stride_size) {
        case 1: type = 'B'; break;    
        case 2: type = 'H'; break;    
        case 4: type = (is_int) ? 'I' : 'f'; break;
        case 8: type = (is_int) ? 'L' : 'd'; break;
        default: return NULL;
    }
    if (is_signed) type[0] = tolower(type[0]);

    // Fill the buffer with as much info as possible

    Py_buffer tempBuffer;
    int ret = PyBuffer_FillInfo(&tempBuffer, 0, buf, len, 1, PyBUF_SIMPLE);
    if (ret != 0) return NULL;

    // Now modify the information according to the type
    tempBuffer.format = (char*)calloc(2, 1); 
    tempBuffer.format[0] = type[0];
    tempBuffer.itemsize = stride_size;
    //tempBuffer.strides[0] = stride_size;
    //tempBuffer.shape[0] = len/stride_size;

    return PyMemoryView_FromBuffer(&tempBuffer);    
#else

    PyTypeObject* type;
    switch (stride_size) {
        case 1: type = &PyOrCharBuffer_Type; break;    
        case 2: type = (is_signed) ? &PyOrShortBuffer_Type : &PyOrUShortBuffer_Type; break;    
        case 4: type = (is_int) ? ((is_signed) ? &PyOrIntBuffer_Type : &PyOrUIntBuffer_Type) : &PyOrFloatBuffer_Type; break;    
        case 8: type = (is_int) ? ((is_signed) ? &PyOrLongBuffer_Type : &PyOrULongBuffer_Type) : &PyOrDoubleBuffer_Type; break;    
        default: return NULL;
    }

    PyObject* obj = PyBuffer_FromReadWriteMemory(buf, len*stride_size);
    if ( obj ) {
        Py_INCREF( (PyObject*)type);
        obj->ob_type = type; 
    }
    return obj;
#endif
}
