#MARTe2 Training Docker Image
FROM centos:latest

LABEL maintainer="Luca Porzio <luca.porzio@ext.f4e.europa.eu>"

#Setting path for workdir
ARG WORK_PATH=~/Projects
#Setting workdir
WORKDIR $WORK_PATH
#Installing all the dependencies needed
RUN yum -y install epel-release 
RUN yum -y update && yum -y install cmake3 git octave wget libxml2 libxml2-devel bc
RUN yum -y groups install "Development Tools"
RUN yum -y install ncurses-devel readline-devel
RUN yum -y install python-dateutil python-six perl-ExtUtils-ParseXS 
RUN yum -y install http://www.mdsplus.org/dist/el6/stable/RPMS/noarch/mdsplus-repo-7.50-0.el6.noarch.rpm
RUN yum -y install mdsplus-kernel* mdsplus-java* mdsplus-python* mdsplus-devel*
#Getting EPICS
RUN cd /opt && git clone -b R7.0.2 --recursive https://github.com/epics-base/epics-base.git epics-base-7.0.2
#Getting open62541 
RUN cd /opt && git clone -b 0.3 https://github.com/open62541/open62541.git
#Getting SDN
RUN wget https://vcis-gitlab.f4e.europa.eu/aneto/MARTe2-demos-padova/raw/develop/Other/SDN_1.0.12_nonCCS.tar.gz || echo 0
RUN tar zxvf SDN_1.0.12_nonCCS.tar.gz && mv SDN_1.0.12_nonCCS /opt
#Compiling open62541 library
RUN mkdir /opt/open62541/build && cd /opt/open62541/build && cmake3 .. && make
#Compiling EPICS
RUN cd /opt/epics-base-7.0.2 && echo "OP_SYS_CXXFLAGS += -std=c++11" >> configure/os/CONFIG_SITE.linux-x86_64.Common
RUN cd /opt/epics-base-7.0.2 && make
#Compiling SDN
RUN cd /opt/SDN_1.0.12_nonCCS && make
#Environment variables
ENV MARTe2_DIR=/root/Projects/MARTe2-dev
ENV MARTe2_Components_DIR=/root/Projects/MARTe2-components
ENV OPEN62541_LIB=/opt/open62541/build/bin
ENV OPEN62541_INCLUDE=/opt/open62541/build
ENV EPICS_BASE=/opt/epics-base-7.0.2
ENV EPICSPVA=/opt/epics-base-7.0.2
ENV EPICS_HOST_ARCH=linux-x86_64
ENV SDN_CORE_INCLUDE_DIR=/opt/SDN_1.0.12_nonCCS/src/main/c++/include/
ENV SDN_CORE_LIBRARY_DIR=/opt/SDN_1.0.12_nonCCS/target/lib/
ENV PATH=$PATH:/opt/epics-base-7.0.2/bin/linux-x86_64
ENV LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_DIR/Build/x86-linux/Core/:$EPICS_BASE/lib/$EPICS_HOST_ARCH:$SDN_CORE_LIBRARY_DIR

