#MARTe2 Training Docker Image
FROM almalinux:9

LABEL maintainer="Bernardo Carvalho <bernardo.carvalho@tecnico.ulisboa.pt>"

#Setting path for workdir
ARG WORK_PATH=~/Projects
#Setting workdir
WORKDIR $WORK_PATH
#Installing all the dependencies needed
RUN dnf -y install epel-release
RUN dnf -y update && dnf -y install cmake git zsh wget libxml2 libxml2-devel bc
RUN dnf -y groups install "Development Tools"
RUN dnf -y install ncurses-devel readline-devel
# RUN dnf -y install curl
RUN dnf -y install tmux
RUN dnf -y install python3-dateutil python3-six perl-ExtUtils-ParseXS perl-FindBin
# RUN dnf -y install http://www.mdsplus.org/dist/el6/stable/RPMS/noarch/mdsplus-repo-7.50-0.el6.noarch.rpm
RUN dnf -y install http://www.mdsplus.org/dist/rhel9/alpha/RPMS/noarch/mdsplus-alpha-repo-7.139-58.el9.noarch.rpm
RUN dnf -y install mdsplus-alpha-kernel* mdsplus-alpha-java* mdsplus-alpha-python* mdsplus-alpha-devel*
RUN curl -L git.io/antigen > /root/antigen.zsh
COPY zshrc-antigen.sh /root/.zshrc
# Getting EPICS
RUN cd /opt && git clone --recursive https://github.com/epics-base/epics-base.git
# Compiling EPICS
# RUN cd /opt/epics-base && echo "OP_SYS_CXXFLAGS += -std=c++11" >> configure/os/CONFIG_SITE.linux-x86_64.Common
RUN cd /opt/epics-base && make
# Environment variables
ENV MARTe2_DIR=/root/Projects/MARTe2-dev
ENV MARTe2_Components_DIR=/root/Projects/MARTe2-components
ENV EPICS_BASE=/opt/epics-base
ENV EPICSPVA=/opt/epics-base
ENV EPICS_HOST_ARCH=linux-x86_64
ENV MDSPLUS_DIR=/usr/local/mdsplus
ENV PATH=$PATH:/opt/epics-base/bin/linux-x86_64
ENV LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_DIR/Build/x86-linux/Core/:$EPICS_BASE/lib/$EPICS_HOST_ARCH
# Compiling MARTe2
# RUN mkdir /root/Projects
# RUN cd /root/Projects && git clone https://vcis-gitlab.f4e.europa.eu/aneto/MARTe2.git MARTe2-dev
# RUN cd /root/Projects/MARTe2-dev && make -f Makefile.linux
# RUN cd /root/Projects && git clone https://vcis-gitlab.f4e.europa.eu/aneto/MARTe2-components.git
# RUN cd /root/Projects/MARTe2-components && make -f Makefile.linux
# RUN cd /root/Projects && git clone https://github.com/bernardocarvalho/MARTe2-demos-padova.git
RUN /usr/bin/zsh /root/.zshrc
# RUN cd /root/Projects/MARTe2-demos-padova && git checkout isttok-marte && make -f Makefile.x86-linux