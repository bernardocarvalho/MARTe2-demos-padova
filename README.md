# MARTe2 Training Demo Repository

Repository containing all the demos for MARTe2 Training Session

## Setting up environment

It is assumed that a clean CentOS/AlmaLinux 9 installation is being used (https://almalinux.org/get-almalinux).

Alternatively, there is also a Dockerfile available (see below).

### Download all the needed software

Open a terminal, install epel repository and update the distribution:

 `dnf -y install epel-release`

 `dnf -y update`

Install git:

 `dnf -y install git`

Install all the standard development tools, the cmake3 compiler and octave:

 `dnf -y groups install "Development Tools"`

 `dnf -y install wget cmake3 octave libxml2 libxml2-devel bc`

Solve dependencies for MARTe2 and EPICS:

 `dnf -y install ncurses-devel readline-devel`

Install Python and Perl Parse utilities for open62541:

 `dnf -y install python-dateutil python-six perl-ExtUtils-ParseXS`

Install MDSplus

 `dnf install http://www.mdsplus.org/dist/rhel9/alpha/RPMS/noarch/mdsplus-alpha-repo-7.139-58.el9.noarch.rpm`

 `dnf -y install mdsplus-kernel* mdsplus-java* mdsplus-python* mdsplus-devel*`

Create a folder named Projects and clone MARTe2 Core,  MARTe2 components and the MARTe2 demos:

 `mkdir ~/Projects`
 
 `cd ~/Projects`

 `git clone https://vcis-gitlab.f4e.europa.eu/aneto/MARTe2.git MARTe2-dev`

 `git clone https://vcis-gitlab.f4e.europa.eu/aneto/MARTe2-components.git`
 
 `git clone https://github.com/bernardocarvalho/MARTe2-demos-padova`
 
 `git checkout isttok-marte`

Download EPICS R7:

 `git clone --recursive https://github.com/epics-base/epics-base.git`

### Building libraries and frameworks

Compile EPICS:

 `cd ~/Projects/epics-base`

 `echo "OP_SYS_CXXFLAGS += -std=c++11" >> configure/os/CONFIG_SITE.linux-x86_64.Common`

 `make`

## Docker (optional)

In the root folder of this project there is a Dockerfile which includes all the demo dependencies.

 `cd ~/Projects/MARTe2-demos-padova`

 `docker build .`

Note that you need to map your local Projects directory with the /root/Projects directory in the container.

To execute the image with your host folder mapped into the container, run:

 `docker run -it -e DISPLAY=$DISPLAY -w /root/Projects -v ~/Projects:/root/Projects:Z -v /tmp/.X11-unix:/tmp/.X11-unix DOCKER_IMAGE_ID`

## Compilings the MARTe and the examples

Make sure that all the environment variables are correctly exported.

 `export MARTe2_DIR=~/Projects/MARTe2-dev`

 `export MARTe2_Components_DIR=~/Projects/MARTe2-components`

 `export EPICS_BASE=~/Projects/epics-base`

 `export EPICSPVA=~/Projects/epics-base`

 `export EPICS_HOST_ARCH=linux-x86_64`

 `export PATH=$PATH:$EPICS_BASE/bin/$EPICS_HOST_ARCH`

 `export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_DIR/Build/x86-linux/Core/:$EPICS_BASE/lib/$EPICS_HOST_ARCH`
 
 `cd ~/Projects/MARTe2-dev`

 `make -f Makefile.linux`

 `cd ~/Projects/MARTe2-components`

 `make -f Makefile.linux`

 `cd ~/Projects/MARTe2-demos-padova`

 `make -f Makefile.x86-linux`

Disable firewall rules (otherwise the communication with EPICS may not work):

 `iptable -F`

Export all variables permanently (assumes that the relative paths above were used!).
 `cp marte2-exports.sh /etc/profile.d/`


