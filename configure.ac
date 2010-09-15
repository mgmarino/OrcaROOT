#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.


AC_PREREQ(2.50)
AC_INIT([OrcaROOT])
AC_CONFIG_SRCDIR([Applications/orcaroot.cc])

echo "Run ./configure --help for a list of options"
echo

AC_CANONICAL_HOST
AC_PROG_CXX

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
  SOFLAGS="-dynamiclib"
  ORLIBEXTSHARED=dylib
  BOLD=`tput smso`
esac

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
ROOT_PATH(5.22,AC_DEFUN(HAVE_ROOT), AC_MSG_ERROR([
  The correct version of ROOT cannot be found. Please ensure that it is installed and that either \$ROOTSYS 
  is defined corectly or root-config is in the path.  
  One can pass --with-rootsys=/path/to/rootsys to configure to specify the correct path.]))

AC_MSG_CHECKING([whether ROOT is compiled with xml support])
ROOTXML=`$ROOTCONF --has-xml`
if test ! x"$ROOTXML" = xyes; then
  AC_MSG_RESULT(no)
  AC_MSG_ERROR([
   ROOT is not installed with XML enabled which is a required feature of OrcaROOT. 
   Please reinstall ROOT using during the ROOT install process: 

   "./configure --enable-xml" OR
   "./configure --with-xml-incdir=/path/to/include  --with-xml-libdir=/path/to/lib"

   where /path/to/include specifies the location of libxml/tree.h and 
   /path/to/lib specifies the location of libxml2])
fi
AC_MSG_RESULT(yes)

AC_MSG_CHECKING([whether ROOT is compiled with thread support])
ROOTXML=`$ROOTCONF --has-thread`
if test ! x"$ROOTXML" = xyes; then
  AC_MSG_RESULT(no)
  AC_MSG_ERROR([
   ROOT is not installed with threading enabled which is a required feature of OrcaROOT. 
   This means ROOT did not find the correct thread library (pthread) during configure.
   Please reinstall ROOT using during the ROOT install process: 

   "./configure --with-thread-libdir=/path/to/lib"

   /path/to/lib specifies the location of libpthread])
fi
AC_MSG_RESULT(yes)


AC_MSG_CHECKING([whether ROOT is compiled with fftw support])
ROOTFFTW=`$ROOTCONF --has-fftw3` 
ORROOT_HAS_FFTW=""
ROOTFFTWLIB=""
if test x"$ROOTFFTW" = xyes; then
  AC_MSG_RESULT(yes)
  # fftw is installed with root.
  ORROOT_HAS_FFTW="-DORROOT_HAS_FFTW"
  ROOTFFTWLIB="-lFFTW"
else
  AC_MSG_RESULT(no)
  AC_MSG_NOTICE([ROOT is not installed with fftw, some functionality is disabled.])
fi
AC_SUBST(ORROOT_HAS_FFTW)
AC_SUBST(ROOTFFTWLIB)
  
echo "${BOLD}ROOT seems to be successfully installed. ${OFFBOLD}"
#end root


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


