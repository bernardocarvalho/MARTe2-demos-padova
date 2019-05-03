# MARTe2 Training Demo Repository:w

Respository containing all the demos for MARTe2 Training Session
## Setting up environment
In this tutorial we assume a clean CentOS 7 installation (https://www.centos.org/download/).

### Download all the needed software
Open a terminal, install epel repository and update the distribution:

 `yum -y install epel-release`

 `yum -y update`

Install git:

 `yum -y install git`

Install all the standard development tools, the cmake3 compiler and octave:

 `yum -y groups install "Development Tools"`

 `yum -y install wget cmake3 octave`

Solving dependencies for MARTe2 and EPICS:

 `yum -y install ncurses-devel readline-devel`

Install Python and Perl Parse utilities for open62541:

 `yum -y install python-dateutil python-six perl-ExtUtils-ParseXS`

Install MDSplus

 `yum -y install http://www.mdsplus.org/dist/el6/stable/RPMS/noarch/mdsplus-repo-7.50-0.el6.noarch.rpm`

 `yum -y install mdsplus-kernel* mdsplus-java* mdsplus-python* mdsplus-devel*`

Now craete a folder named Projects and clone MARTe2 Core and MARTe2 components with git:

 `mkdir ~/Projects`

 `git clone https://vcis-gitlab.f4e.europa.eu/aneto/MARTe2.git MARTe2-dev`

 `git clone -b #351_OPCUA https://vcis-gitlab.f4e.europa.eu/aneto/MARTe2-components.git`

Download EPICS and move it to /opt/EPICS directory:

 `git clone -b R7.0.2.2 https://github.com/epics-base/epics-base.git`

 `mkdir EPICS`

 `mv epics-base EPICS/base-7.0-dev`
 
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

Compile MARTe2:

 `cd ../../MARTe2-dev`

 `make -f Makefile.x86-linux`

Compile EPICS:

 `cd ~/Projects/EPICS/base-7.0-dev`

 `make`

Compile SDN: 

 `cd ~/Projects/SDN_1.0.12_nonCCS`

 `make`

Finally compile MARTe2 Components (insert your own paths):

 `export MARTe2_DIR=~/Projects/MARTe2-dev`

 `export OPEN62541_LIB=~/Projects/open62541/build/bin`

 `export OPEN62541_INCLUDE=~/Projects/open62541/build`

 `export EPICS_BASE=~/Projects/EPICS/base-7.0-dev`

 `export EPICSPVA=~/Projects/EPICS/base-7.0-dev`

 `export EPICS_HOST_ARCH=linux-x86_64`

 `export SDN_CORE_INCLUDE_DIR=~/Projects/SDN_1.0.12_nonCCS/src/main/c++/include/`

 `export SDN_CORE_LIBRARY_DIR=~/Projects/SDN_1.0.12_nonCCS/target/lib/`
 
 `cd ~/Projects/MARTe2-components`

 `make -f Makefile.linux` 

Disable firewall rules (otherwise the communication with EPICS may not work):

 `iptable -F`

Now everything should work correctly. 

## Official Docker Image
We released a docker image based on CentOS 7 with all the settings needed for running the examples. Download here: https://link.com

To execute the image with your host folder mapped into the container, just run:

 `cd ~/Projects

 `docker run -it -v $(pwd):/root/Projects:Z marte2-training`

The image comes with all the environment variable already set up.
