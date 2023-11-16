################################################################################
#
# ATCA Virtex 6 PCIe Vivado Project General   Linux driver
# This program is free software; you can redistribute it and/or modify it
# under the terms and conditions of the GNU General Public License,
# version 2, as published by the Free Software Foundation.
#
# This program is distributed in the hope it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
# FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
# more details.
#
# You should have received a copy of the GNU General Public License along with
# this program; if not, write to the Free Software Foundation, Inc.,
# 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
#
# The full GNU General Public License is included in this distribution in
# the file called "COPYING".
#
# Contact Information:
#
# Instituto de Plasmas e Fusao Nuclear, Instituto Superior Tecnico,
# Universidade  de Lisboa, 1049-001 Lisboa, Portugal.
#
################################################################################
# lspci -vv -d:040
# Rescan PCIe instructions
#  echo 1 > /sys/bus/pci/devices/0000\:04\:00.0/remove
#  echo 1 > /sys/bus/pci/rescan
###########################################################################
# Driver files

# core driver files

TARGET_MODULE := atca_v6_stream

# CFILES :=  atca-pcie-drv.c unlocked-ioctl.c

#KVER = $(shell uname -r)
#KDIR = /lib/modules/$(KVER)/build

# Comment/uncomment the following line to disable/enable debugging
DEBUG = y

# Add your debugging flag (or not) to CFLAGS
ifeq ($(DEBUG),y)
	DEBFLAGS = -O -g -DATCA_DEBUG # "-O" is needed to expand inlines
else
	DEBFLAGS = -O2
endif

#EXTRA_CFLAGS += -I$(LDDINC)
EXTRA_CFLAGS += $(DEBFLAGS)

ifneq ($(KERNELRELEASE),)
	$(TARGET_MODULE)-objs := atca-pcie-drv.o atca-pcie-unlocked-ioctl.o
	obj-m := $(TARGET_MODULE).o
else
	BUILDSYSTEM_DIR:=/lib/modules/$(shell uname -r)/build
	PWD:=$(shell pwd)

all:
	$(MAKE) -C $(BUILDSYSTEM_DIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(BUILDSYSTEM_DIR) M=$(PWD) clean
	@/bin/rm -f *.ko modules.order *.mod.c *.o *.o.ur-safe .*.o.cmd

install: all
	$(MAKE) -C $(BUILDSYSTEM_DIR) M=$(PWD) modules_install
	depmod -A

endif

