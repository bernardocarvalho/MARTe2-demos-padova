# MARTe2 Training Demo Repository
Respository containing all the demos for MARTe2 Training Session
## Setting up environment
In this tutorial we assume a clean CentOS 7 installation (https://www.centos.org/download/).

### Download all the needed software
Open a terminal, install epel repository and update the distribution:

 `yum -y install epel-release`

 `yum -y update`

Install git:

 `yum -y install git`

Install all the standard development tools and the cmake3 compiler:

 `yum -y groups install "Development Tools"`

 `yum -y install wget cmake3`

Solving dependencies for MARTe2 and EPICS:

 `yum -y install ncurses-devel readline-devel`

Install Python and Perl Parse utilities for open62541:

 `yum -y install python-dateutil python-six perl-ExtUtils-ParseXS`

Now let's clone MARTe2 Core and MARTe2 components with git:

 `git clone https://vcis-gitlab.f4e.europa.eu/aneto/MARTe2.git`

 `git clone -b #351_OPCUA https://vcis-gitlab.f4e.europa.eu/aneto/MARTe2-components.git`

Download EPICS and move it to /opt/EPICS directory:

 `code`
 
Clone open62541 v0.3 library:

 `git clone -b 0.3 https://github.com/open62541/open62541.git`

### Building libraries and frameworks
Start building the open62541 library:

 `mkdir open62541/build`

 `cd open62541/build`

 `cmake3 ..`

 `make`

Compile MARTe2:

 `cd ../../MARTe2`

 `make -f Makefile.x86-linux`

Compile EPICS:

 `cd /opt/EPICS/base-7.0-dev`

 `make`

Finally compile MARTe2 Components (insert your own paths):

 `export MARTe2_DIR=/home/MARTe2`

 `export OPEN62541_LIB=/home/open62541/build/bin`

 `export OPEN62541_INCLUDE=/home/open62541/build`

 `export EPICS_BASE=/opt/EPICS/base-7.0-dev`

 `export EPICSPVA=/opt/EPICS/base-7.0-dev`

 `export EPICS_HOST_ARCH=linux-x86_64`
 
 `cd /path-to/MARTe2-components`

 `make -f Makefile.linux` 

Now everything should work correctly. 

### Official Docker Image
We released a docker image based on CentOS 7 with all the settings needed for running the examples. Download here: https://link.com

Run a container with a virtual terminal session and interactive option:

 `docker run -it marte2-training`

The image comes with all the environment variable already set up.
