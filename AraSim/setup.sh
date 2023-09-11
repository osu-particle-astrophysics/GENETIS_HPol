#!/bin/sh
# Setup script for trunk version of the ARA software

export ARA_SETUP_DIR="/cvmfs/ara.opensciencegrid.org/trunk/RHEL_7_x86_64"
# If the fake path in ARA_SETUP_DIR wasn't replaced, try the working directory
if [ ! -d "$ARA_SETUP_DIR" ]; then
	export ARA_SETUP_DIR=$(pwd)
fi

export ARA_UTIL_INSTALL_DIR="${ARA_SETUP_DIR%/}/ara_build"
export ARA_DEPS_INSTALL_DIR="${ARA_SETUP_DIR%/}/misc_build"
export ARA_ROOT_DIR="${ARA_SETUP_DIR%/}/source/AraRoot"

export LD_LIBRARY_PATH="$ARA_UTIL_INSTALL_DIR/lib:$ARA_DEPS_INSTALL_DIR/lib:$LD_LIBRARY_PATH"
export DYLD_LIBRARY_PATH="$ARA_UTIL_INSTALL_DIR/lib:$ARA_DEPS_INSTALL_DIR/lib:$DYLD_LIBRARY_PATH"
export PATH="$ARA_UTIL_INSTALL_DIR/bin:$ARA_DEPS_INSTALL_DIR/bin:$PATH"

# Run thisroot.sh using `.` instead of `source` to improve POSIX compatibility
. "${ARA_SETUP_DIR%/}/root_build/bin/thisroot.sh"

export SQLITE_ROOT="$ARA_DEPS_INSTALL_DIR"
export GSL_ROOT="$ARA_DEPS_INSTALL_DIR"
#export FFTW_LIBRARIES="$ARA_DEPS_INSTALL_DIR"
export FFTWSYS="$ARA_DEPS_INSTALL_DIR"

export BOOST_ROOT="$ARA_DEPS_INSTALL_DIR/include"
#export BOOST_LIB="$ARA_DEPS_INSTALL_DIR/lib"
#export LD_LIBRARY_PATH="$BOOST_LIB:$LD_LIBRARY_PATH"
#export DYLD_LIBRARY_PATH="$BOOST_LIB:$DYLD_LIBRARY_PATH"

export CMAKE_PREFIX_PATH="$ARA_DEPS_INSTALL_DIR"

export NUPHASE_INSTALL_DIR="$ARA_UTIL_INSTALL_DIR"

# activate python environment
#eval "$($ARA_DEPS_INSTALL_DIR/miniconda/bin/conda shell.bash hook)"


# Warn about incompatible gcc versions
export ARA_GCC_VERSION=$(strings -a "${ARA_SETUP_DIR%/}/source/AraSim/AraSim" | grep "GCC: (" | head -1 | cut -d " " -f 3)
export SYS_GCC_VERSION=$($(command -v gcc) --version | head -1 | cut -d " " -f 3)
if [ "$ARA_GCC_VERSION" = "4.8.5" ]; then
	case $SYS_GCC_VERSION in
		$ARA_GCC_VERSION )
			# gcc version exactly matches the version used to compile AraSim
			# (and presumably all other ARA software too then)
		;;
		4.* | 4.*.* )
			# gcc version is between 4.0.0 and 5.0.0
			echo "The ARA software was compiled with gcc version $ARA_GCC_VERSION."
			echo "Your system uses gcc version $SYS_GCC_VERSION, which should be similar enough."
		;;
		[123].* | [123].*.* )
			# gcc version is less than 4.0.0
			echo "The ARA software was compiled with gcc version $ARA_GCC_VERSION."
			echo "Your system uses gcc version $SYS_GCC_VERSION, which could result in some problems."
			echo "Consider using a new version of gcc."
		;;
		* )
			# gcc version is greater than 5.0.0
			# (at which point the string ABI changed)
			echo "The ARA software was compiled with gcc version $ARA_GCC_VERSION."
			echo "Your system uses gcc version $SYS_GCC_VERSION, which is likely to cause problems."
			echo "If you do any compilation against the ARA software you may need to add the '-D_GLIBCXX_USE_CXX11_ABI=0' flag."
		;;
	esac
fi
unset ARA_GCC_VERSION
unset SYS_GCC_VERSION
