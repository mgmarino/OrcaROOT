#!/bin/sh
 
#Sets up the OrcaRoot Environment Variables

export ORDIR=@CMAKE_INSTALL_PREFIX@
export PATH=$ORDIR/bin\:$PATH
export DYLD_LIBRARY_PATH=$ORDIR/lib\:$DYLD_LIBRARY_PATH
#TODO: PKG-Config
