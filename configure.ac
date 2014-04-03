#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.


AC_PREREQ(2.50)
AC_INIT([OrcaROOT])
AC_CONFIG_SRCDIR([Applications/orcaroot.cc])

echo "Run ./configure --help for a list of options"
echo

AC_CANONICAL_HOST
AC_PROG_CXX
AC_LANG(C++)

# defaults, these will work for linux
# but NOT in screen.  Perhaps a fix for this?
BOLD=`tput bold`
OFFBOLD=`tput rmso`

# doing host os specific things
EXTRALIBFLAGS=""
EXTRACPPFLAGS=""
SOFLAGS="-shared"
ORLIBEXTSHARED=so
case "${host_os}" in
  darwin*)
    EXTRALIBFLAGS="-bind_at_load" 
    SOFLAGS="-dynamiclib -install_name @rpath/"
    ORLIBEXTSHARED=dylib
    ORIGINFLAGS="@loader_path/../lib"
    BOLD=`tput smso`
;;
  linux*)
    SOFLAGS="-shared -Wl,-soname,"
    EXTRALIBFLAGS="$LDFLAGS" 
    ORIGINFLAGS=\'\$\$ORIGIN/../lib\',--enable-new-dtags
  ;;
  *)
  AC_MSG_ERROR([Don't know how to build dynamic libraries on your system])
esac
AC_SUBST(ORIGINFLAGS)

# doing host cpu (i.e. architecture) specific things

case "${host_cpu}" in
  x86_64*)
  EXTRACPPFLAGS="-fPIC"
  echo "Compiling on an x86_64 chipset...Setting -fPIC flag."
  # only on amd64 should one compile static libs with this flag
  # Other tested architectures currently compile PIC libraries
  # and so it is no problem to link a dynamic library using the PIC
  # libs
esac

AC_SUBST(EXTRALIBFLAGS)
AC_SUBST(EXTRACPPFLAGS)
AC_SUBST(SOFLAGS)
AC_SUBST(ORLIBEXTSHARED)
# end host

#Find root
echo ""
echo "${BOLD}Checking for ROOT: ${OFFBOLD} "
m4_include(root.m4)

ROOT_PATH(5.34.02,[can_build_bindings=yes], 
          [AC_MSG_NOTICE([ROOT not new enough to build python bindings, >= 5.34.02 required])]) 

ROOT_PATH(5.22,AC_DEFUN(HAVE_ROOT), AC_MSG_ERROR([
  The correct version of ROOT cannot be found. Please ensure that it is installed and that either \$ROOTSYS 
  is defined corectly or root-config is in the path.  
  One can pass --with-rootsys=/path/to/rootsys to configure to specify the correct path.]))

ROOT_FEATURE([xml], [], 
  AC_MSG_ERROR([
   ROOT is not installed with XML enabled which is a required feature of OrcaROOT. 
   Please reinstall ROOT using during the ROOT install process: 

   "./configure --enable-xml" OR
   "./configure --with-xml-incdir=/path/to/include  --with-xml-libdir=/path/to/lib"

   where /path/to/include specifies the location of libxml/tree.h and 
   /path/to/lib specifies the location of libxml2])
  )

ROOT_FEATURE([thread], [], 
  AC_MSG_ERROR([
   ROOT is not installed with threading enabled which is a required feature of OrcaROOT. 
   This means ROOT did not find the correct thread library (pthread) during configure.
   Please reinstall ROOT using during the ROOT install process: 

   "./configure --with-thread-libdir=/path/to/lib"

   /path/to/lib specifies the location of libpthread])
  )


ORROOT_HAS_FFTW=""
ROOTFFTWLIB=""
ROOT_FEATURE([fftw3], 
  [
  # fftw is installed with root.
  ORROOT_HAS_FFTW="-DORROOT_HAS_FFTW"
  ROOTFFTWLIB="-lFFTW"
  ], 
  [ AC_MSG_NOTICE([ROOT is not installed with fftw, some functionality is disabled.])] )
AC_SUBST(ORROOT_HAS_FFTW)
AC_SUBST(ROOTFFTWLIB)

PYTHON_VERSION=2.5
ROOT_FEATURE([python], [ AS_IF([test x"$can_build_bindings" = xyes], [ROOT_HAS_PYTHON=yes]) ])
  
#########################################################################
# Do some checks related to ROOT and dictionary building
#########################################################################
AC_CHECK_SIZEOF([pthread_t], [], [#include <pthread.h>;])
AC_CHECK_SIZEOF([pthread_rwlock_t], [], [#include <pthread.h>;])

echo "${BOLD}ROOT seems to be successfully installed. ${OFFBOLD}"
#end root

#########################################################################
#Find python, this is necessary for some of the build 
#########################################################################
m4_include(aclocal.m4)
m4_include(python.m4)
AM_PATH_PYTHON([$PYTHON_VERSION],
               [HAVE_PYTHON=yes
                PYTHONLIBDIR=`$PYTHON -c 'from distutils.sysconfig import get_python_lib; print "%s/config" % get_python_lib(0,1)'`
                PYTHONLIB=`$PYTHON -c 'from distutils.sysconfig import get_python_version; print "python%s" % get_python_version()'`
                ],
               [HAVE_PYTHON=no
                AC_MSG_ERROR([python not found, required for build])])
AC_SUBST(HAVE_PYTHON)
AC_SUBST(PYTHONLIBDIR)
AC_SUBST(PYTHONLIB)
PYTHON_HDR_LOC=`$PYTHON -c 'import sys; sys.stdout.write(sys.prefix + "/include/python" + sys.version[[:3]])'`
save_CPPFLAGS=$CPPFLAGS
CPPFLAGS="$CPPFLAGS -I$PYTHON_HDR_LOC"
AC_CHECK_HEADER([Python.h])
CPPFLAGS=$save_CPPFLAGS

BUILD_PY_BINDINGS=""
PYTHON_API_DIR=""
PYROOT_API_DIR=""
AS_IF([test x"$ROOT_HAS_PYTHON" = xyes],
[
  AS_IF([test x"$ac_cv_header_Python_h" != x ], 
  [
    AC_MSG_CHECKING([if we can build python bindings])
    save_CPPFLAGS=$CPPFLAGS
    save_LIBS=$LIBS
    save_LDFLAGS="$LDFLAGS"
    CPPFLAGS="$CPPFLAGS -I$PYTHON_HDR_LOC -I$ROOTINCDIR -I$ROOTSYS/bindings/pyroot/src"
    LIBS="$LIBS $ROOTLIBS -lPyROOT"
    LDFLAGS="$LDFLAGS -L$ROOTLIBDIR"

    AC_LINK_IFELSE( 
      [ AC_LANG_PROGRAM(
        [#include "PyROOT.h"
         #include "MemoryRegulator.h"
         #include "MethodProxy.h"
        ],
        [
         PyROOT::TMemoryRegulator::RetrieveObject(0, 0);
         PyROOT::MethodProxy_CheckExact((TObject*)0);
        ]) ], 
        [BUILD_PY_BINDINGS="Bindings" 
         PYTHON_API_DIR=$PYTHON_HDR_LOC
         PYROOT_API_DIR=$ROOTSYS/bindings/pyroot/src
         AC_MSG_RESULT(yes) 
         ],
        [AC_MSG_RESULT(no)]) 
     CPPFLAGS=$save_CPPFLAGS
     LDFLAGS=$save_LDFLAGS
     LIBS=$save_LIBS
  ])
])

AC_SUBST(PYTHON_BINDINGS, [$BUILD_PY_BINDINGS])
AC_SUBST(PYROOT_API_DIR)
AC_SUBST(PYTHON_API_DIR)


# we also must check for endianness 
# throw an error if it can't tell. 
ORBIG_ENDIAN_MACHINE=""
echo
echo "${BOLD}Checking Endian-ness: ${OFFBOLD}"
AC_C_BIGENDIAN(is_big_endian=yes, is_big_endian=no,)
if test x"$is_big_endian" = xyes; then
  ORBIG_ENDIAN_MACHINE="-DORBIG_ENDIAN_MACHINE"
fi
AC_SUBST(ORBIG_ENDIAN_MACHINE)

echo ""

# checking to compile as debug?
AC_ARG_ENABLE(debug, AC_HELP_STRING([--enable-debug],[Enables compiling with debug flags.]),
  [ if test "x$enableval"="xyes"; then
     enable_debug=yes
    else
     enable_debug=no
    fi
   ] 
  ,[enable_debug=no])
if test "x$enable_debug" = "xyes" ; then
  EXTRACXXFLAGS="-g"
  echo
  echo "${BOLD}Configuring with debug flags ON. ${OFFBOLD}"
else
  EXTRACXXFLAGS="-O2"
  echo ""
  echo "${BOLD}Configuring with debug flags OFF. ${OFFBOLD}"
  echo "Debug flags enabled by passing --enable-debug to configure."
fi
INSTALLDIR=`pwd`

echo "Installing OrcaROOT in: $INSTALLDIR"

AC_SUBST(EXTRACXXFLAGS)
AC_SUBST(INSTALLDIR)


AC_CONFIG_FILES([
                 buildTools/config.mk
                 buildTools/BasicMakefile
                 buildTools/BasicAppMakefile
                 Makefile])
AC_OUTPUT
echo 
echo "**********************************************************************************"
echo "**********************************************************************************"
echo
echo "You have successfully configured OrcaRoot.  Please now compile by issuing"
echo "the command:"
echo 
echo "     make"
echo 


