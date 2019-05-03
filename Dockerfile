#MARTe2 Training Docker Image
FROM centos:latest

LABEL maintainer="Luca Porzio <luca.porzio@ext.f4e.europa.eu>"

#Setting path for workdir
ARG WORK_PATH=/home/Project
#Setting workdir
WORKDIR $WORK_PATH
#Installing all the dependencies needed
RUN yum -y install epel-release 
RUN yum -y update && yum -y install cmake3 git
RUN yum -y groups install "Development Tools"
RUN yum -y install ncurses-devel readline-devel
RUN yum -y install python-dateutil python-six perl-ExtUtils-ParseXS 
#Getting all the MARTe2 repositories
RUN git clone https://vcis-gitlab.f4e.europa.eu/aneto/MARTe2.git
RUN git clone -b \#351_OPCUA https://vcis-gitlab.f4e.europa.eu/aneto/MARTe2-components.git
RUN git clone -b develop https://vcis-gitlab.f4e.europa.eu/aneto/MARTe2-demos-padova.git
#Getting EPICS
#Getting open62541 
RUN git clone -b 0.3 https://github.com/open62541/open62541.git
#Compiling open62541 library
RUN mkdir open62541/build && cd open62541/build && cmake3 .. && make
#Compiling MARTe2 Core
RUN cd MARTe2 && make -f Makefile.x86-linux
#Compiling EPICS
#RUN cd /opt/EPICS/base-7.0-dev && make clean && make
#Compiling MARTe2-components
RUN export MARTe2_DIR=$WORK_PATH/MARTe2 && export OPEN62541_LIB=$WORK_PATH/open62541/build/bin && \
export OPEN62541_INCLUDE=$WORK_PATH/open62541/build && export EPICS_BASE=/opt/EPICS/base-7.0-dev && \
export EPICSPVA=/opt/EPICS/base-7.0-dev && export EPICS_HOST_ARCH=linux-x86_64 && cd MARTe2-components && \
make -f Makefile.linux
#Environment variables
ENV MARTe2_DIR=/home/Project/MARTe2
ENV OPEN62541_LIB=/home/Project/open62541/build/bin
ENV OPEN62541_INCLUDE=/home/Project/open62541/build
ENV EPICS_BASE=/opt/EPICS/base-7.0-dev
ENV EPICSPVA=/opt/EPICS/base-7.0-dev
ENV EPICS_HOST_ARCH=linux-x86_64
COPY Startup.sh .
#Running Startup.sh as entrypoint script
ENTRYPOINT ["/home/Project/Startup.sh"]
#Keep running
CMD ["/bin/bash"]
