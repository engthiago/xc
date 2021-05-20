scriptName=`basename "$0"`
# Must run as root so that we can shutdown backuppc and mount drives
if [ $(whoami) != "root" ]; then
    echo "You must run this script as root."
    echo "Use 'sudo sh $scriptName' and enter the password when prompted."
    exit 1
fi

# print information about non-free packages
echo "Some packages are in the \"contrib\" and \"non-free\" areas of the Debian distribution so these areas should be included in the sources.list file before running this script."

# verify that the user wants to continue, but do not verify if a parameter DoNotAsk was given with script start
if [ $1 != "DoNotAsk" ]; then
    read -p "Continue (y/n)?" REPLY
    if [ $REPLY != "y" ]; then
        echo "Exiting..."
        exit 1
    fi
fi


# packages installed by package manager apt-get
# tested on Ubuntu 18.04 Bionic Beaver
packages_build="\
    git         \
    cmake       \
    g++         \
    gfortran    \
    libboost-all-dev"
sudo apt-get install -y $packages_build

packages_lib="\
    libarpack2-dev              \
    libarpack++2-dev            \
    libcgal-dev                 \
    libcgal-qt5-dev             \
    libdb-dev                   \
    libf2c2-dev                 \
    libqt5svg5-dev              \
    libglib2.0-dev              \
    libgmp3-dev                 \
    libgmsh-dev                 \
    libgmsh3                    \
    libgtk2.0-dev               \
    libgtkgl2.0-dev             \
    libgtkglextmm-x11-1.2-dev   \
    libgtkmm-2.4-dev            \
    libgtkglext1-dev          \
    libgts-bin                  \
    libgts-dev                  \
    liblapack-dev               \
    libmpfr-dev                 \
    libmysql++-dev              \
    libplot-dev                 \
    libsqlite3-dev              \
    libsuperlu-dev              \
    libsuitesparse-dev          \
    libvtk6-dev                 \
    libx11-dev                  \
    libmetis-dev"
sudo apt-get install -y $packages_lib

packages_dev="\
    python-dev          \
    cimg-dev  \
    petsc-dev \
    tcl-dev"
sudo apt-get install -y $packages_dev

packages_python="\
    python-vtk6         \
    python-numpy        \
    python-scipy        \
    python-sympy        \
    python-matplotlib   \
    python-pandas       \
    python-sklearn      \
    python-pip"
sudo apt-get install -y $packages_python

packages_div="\
    gnuplot \
    bc"
# bc is needed by xc_utils verification tests
sudo apt-get install -y $packages_div

# free disk space by cleaning install files
sudo apt-get clean


# mayavi installation. Ubuntu 'mayavi' package seems to require VTK 6,
# so we use pip (to reconsider because we already use VTK 6 LCPT 24/09/2018)
sudo -H pip install mayavi
sudo -H pip install ezdxf
sudo -H pip install pyexcel
sudo -H pip install pyexcel-ods
sudo -H pip install dxfwrite # To replace with ezdxf
# cairo installation. 
sudo -H pip install pycairo
