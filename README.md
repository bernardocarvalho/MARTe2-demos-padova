# MARTe2 Training Demo Repository

Repository containing all the demos for MARTe2 Training Session

## Setting up environment

It is assumed that a clean CentOS 7 installation is being used (https://www.centos.org/download/).

Alternatively, there is also a Dockerfile available (see below).

### Download all the needed software

Open a terminal, install epel repository and update the distribution:

 `yum -y install epel-release`

 `yum -y update`

Install git:

 `yum -y install git`

Install all the standard development tools, the cmake3 compiler and octave:

 `yum -y groups install "Development Tools"`

 `yum -y install wget cmake3 octave libxml2 libxml2-devel bc`

Solve dependencies for MARTe2 and EPICS:

 `yum -y install ncurses-devel readline-devel`

Install Python and Perl Parse utilities for open62541:

 `yum -y install python-dateutil python-six perl-ExtUtils-ParseXS`

Install MDSplus

 `yum -y install http://www.mdsplus.org/dist/el7/stable/RPMS/noarch/mdsplus-repo-7.50-0.el7.noarch.rpm`

 `yum -y install mdsplus-kernel* mdsplus-java* mdsplus-python* mdsplus-devel*`

Create a folder named Projects and clone MARTe2 Core,  MARTe2 components and the MARTe2 demos:

 `mkdir ~/Projects`
 
 `cd ~/Projects`

 `git clone https://vcis-gitlab.f4e.europa.eu/aneto/MARTe2.git MARTe2-dev`

 `git clone -b \#351_OPCUA https://vcis-gitlab.f4e.europa.eu/aneto/MARTe2-components.git`
 
 `git clone https://vcis-gitlab.f4e.europa.eu/aneto/MARTe2-demos-padova.git`

Download EPICS R70.2:

 `git clone -b R7.0.2 --recursive https://github.com/epics-base/epics-base.git epics-base-7.0.2`

Clone open62541 v0.3 library:

 `git clone -b 0.3 https://github.com/open62541/open62541.git`

Download SDN:

 `wget https://vcis-gitlab.f4e.europa.eu/aneto/MARTe2-demos-padova/raw/develop/Other/SDN_1.0.12_nonCCS.tar.gz`

 `tar zxvf SDN_1.0.12_nonCCS.tar.gz`

### Building libraries and frameworks
Start building the open62541 library:

 `mkdir ~/Projects/open62541/build`

 `cd ~/Projects/open62541/build`

 `cmake3 ..`

 `make`

Compile EPICS:

 `cd ~/Projects/epics-base-7.0.2`

 `echo "OP_SYS_CXXFLAGS += -std=c++11" >> configure/os/CONFIG_SITE.linux-x86_64.Common`

 `make`

Compile SDN: 

 `cd ~/Projects/SDN_1.0.12_nonCCS`

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

 `export OPEN62541_LIB=~/Projects/open62541/build/bin`

 `export OPEN62541_INCLUDE=~/Projects/open62541/build`

 `export EPICS_BASE=~/Projects/epics-base-7.0.2`

 `export EPICSPVA=~/Projects/epics-base-7.0.2`

 `export EPICS_HOST_ARCH=linux-x86_64`

 `export PATH=$PATH:$EPICS_BASE/bin/$EPICS_HOST_ARCH`

 `export SDN_CORE_INCLUDE_DIR=~/Projects/SDN_1.0.12_nonCCS/src/main/c++/include/`

 `export SDN_CORE_LIBRARY_DIR=~/Projects/SDN_1.0.12_nonCCS/target/lib/`

 `export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_DIR/Build/x86-linux/Core/:$EPICS_BASE/lib/$EPICS_HOST_ARCH:$SDN_CORE_LIBRARY_DIR`
 
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


