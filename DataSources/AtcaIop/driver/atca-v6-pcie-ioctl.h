/**
 * IOCTL Definitions for the Linux Device Driver
 *
 *
 */

#ifndef _ATCA_PCIE_IOCTL_H_
#define _ATCA_PCIE_IOCTL_H_

#include "atca-v6-pcie.h"

/*
 * IOCTL definitions
 */

/* Please use a different 8-bit number in your code */
/*See  /Documentation/ioctl-number.txt*/
#define ATCA_PCIE_IOP_MAGIC 'k'
/* S means "Set": thru a pointer
 * T means "Tell": directly with the argument value
 * G menas "Get": reply by setting thru a pointer
 * Q means "Qry": response is on the return value
 * X means "eXchange": G and S atomically
 * H means "sHift": T and Q atomically
 */

/**********************************************************************
 *                         IOCTL FUNCTIONS                            *
 *********************************************************************/
#define ATCA_PCIE_IOPT_IRQ_ENABLE _IO(ATCA_PCIE_IOP_MAGIC,  1)
#define ATCA_PCIE_IOPT_IRQ_DISABLE _IO(ATCA_PCIE_IOP_MAGIC, 2)
#define ATCA_PCIE_IOPT_ACQ_ENABLE _IO(ATCA_PCIE_IOP_MAGIC,  3)
#define ATCA_PCIE_IOPT_ACQ_DISABLE _IO(ATCA_PCIE_IOP_MAGIC, 4)
#define ATCA_PCIE_IOPT_DMA_ENABLE _IO(ATCA_PCIE_IOP_MAGIC,  5)
#define ATCA_PCIE_IOPT_DMA_DISABLE _IO(ATCA_PCIE_IOP_MAGIC, 6)
#define ATCA_PCIE_IOPT_SOFT_TRIG _IO(ATCA_PCIE_IOP_MAGIC,   7)
#define ATCA_PCIE_IOPG_STATUS   _IOR(ATCA_PCIE_IOP_MAGIC,   8, u_int32_t)
#define ATCA_PCIE_IOPS_RDTMOUT  _IOW(ATCA_PCIE_IOP_MAGIC,   9, u_int32_t)
#define ATCA_PCIE_IOPS_DMA_SIZE _IOW(ATCA_PCIE_IOP_MAGIC,   10, u_int32_t)
#define ATCA_PCIE_IOPG_DMA_SIZE _IOR(ATCA_PCIE_IOP_MAGIC,   11, u_int32_t)
#define ATCA_PCIE_IOPT_DMA_RESET _IO(ATCA_PCIE_IOP_MAGIC,   12)
#define ATCA_PCIE_IOPT_STREAM_ENABLE _IO(ATCA_PCIE_IOP_MAGIC, 13)
#define ATCA_PCIE_IOPT_STREAM_DISABLE _IO(ATCA_PCIE_IOP_MAGIC, 14)
#define ATCA_PCIE_IOPT_CHOP_ON  _IO(ATCA_PCIE_IOP_MAGIC,    15)
#define ATCA_PCIE_IOPT_CHOP_OFF _IO(ATCA_PCIE_IOP_MAGIC,    16)
#define ATCA_PCIE_IOPT_CHOP_DEFAULT_1  _IO(ATCA_PCIE_IOP_MAGIC, 17)
#define ATCA_PCIE_IOPT_CHOP_DEFAULT_0  _IO(ATCA_PCIE_IOP_MAGIC, 18)
#define ATCA_PCIE_IOPS_CHOP_COUNTERS _IOW(ATCA_PCIE_IOP_MAGIC, 19, u_int32_t)
#define ATCA_PCIE_IOPG_CHOP_COUNTERS _IOR(ATCA_PCIE_IOP_MAGIC, 20, u_int32_t)
#define ATCA_PCIE_IOPS_EO_OFFSETS                            \
  _IOW(ATCA_PCIE_IOP_MAGIC, 21, struct atca_eo_config)
#define ATCA_PCIE_IOPS_WO_OFFSETS                            \
  _IOW(ATCA_PCIE_IOP_MAGIC, 22, struct atca_wo_config)
#define ATCA_PCIE_IOPG_EO_OFFSETS                            \
  _IOR(ATCA_PCIE_IOP_MAGIC, 23, struct atca_eo_config)
#define ATCA_PCIE_IOPG_WO_OFFSETS                            \
  _IOR(ATCA_PCIE_IOP_MAGIC, 24, struct atca_wo_config)

#define ATCA_PCIE_IOPS_CONTROL _IOW(ATCA_PCIE_IOP_MAGIC,     25, u_int32_t)
#define ATCA_PCIE_IOPG_CONTROL _IOR(ATCA_PCIE_IOP_MAGIC,     26, u_int32_t)

#define ATCA_PCIE_IOP_MAXNR 26

/*
 *#define ATCA_PCIE_IOPG_TMRGATE       _IOR(ATCA_PCIE_IOP_MAGIC, 13,
 *u_int32_t)
 *
 */

/*
#define ATCA_PCIE_IOPS_CONFIG_ACQ _IOW(ATCA_PCIE_IOP_MAGIC, 2, u_int32_t)
#define ATCA_PCIE_IOPT_DSP_HOLD _IO(ATCA_PCIE_IOP_MAGIC, 1)
#define ATCA_PCIE_IOPG_IRQ_CNT _IOR(ATCA_PCIE_IOP_MAGIC, 7, u_int32_t)
#define ATCA_PCIE_IOPT_SOFT_TRIG _IO(ATCA_PCIE_IOP_MAGIC, 8)
#define ATCA_PCIE_IOPS_TMOUT      _IOW(ATCA_PCIE_IOP_MAGIC, 11, u_int32_t)
#define ATCA_PCIE_IOPS_CHAN       _IOW(PCIE_ADC_IOC_MAGIC, 12, u_int32_t)
*/
#endif /* _ATCA_PCIE_IOCTL_H_ */
