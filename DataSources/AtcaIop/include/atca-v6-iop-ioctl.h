/**
 * IOCTL Definitions for the Linux Device Driver
 * Should be synchronized with:
 * https://github.com/bernardocarvalho/atca-iop-stream/blob/master/linux-software/include/atca-v6-pcie-ioctl.h
 *
 */

#ifndef _ATCA_IOP_IOPTL_H_
#define _ATCA_IOP_IOPTL_H_

#include <sys/ioctl.h>

#define ADC_CHANNELS    16
#define DMA_BUFFS 8 // Number of DMA ch0 buffs

/* 256 + 3840 = 4096 B (PAGE_SIZE) data packet
typedef struct _DMA_CH1_PCKT {
    volatile uint32_t head_time_cnt;
    volatile uint32_t header; // h5431BACD
    volatile int32_t channel[60]; // 24 56
    volatile uint32_t foot_time_cnt;
    volatile uint32_t footer; // h5431BACD
    uint8_t page_fill[3840];
} DMA_CH1_PCKT;
*/
struct atca_eo_config {
  int32_t offset[ADC_CHANNELS];
};

struct atca_wo_config {
  int32_t offset[ADC_CHANNELS];
};

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
#define ATCA_PCIE_IOPT_IRQ_ENABLE _IO(ATCA_PCIE_IOP_MAGIC, 1)
#define ATCA_PCIE_IOPT_IRQ_DISABLE _IO(ATCA_PCIE_IOP_MAGIC, 2)
#define ATCA_PCIE_IOPT_ACQ_ENABLE _IO(ATCA_PCIE_IOP_MAGIC, 3)
#define ATCA_PCIE_IOPT_ACQ_DISABLE _IO(ATCA_PCIE_IOP_MAGIC, 4)
#define ATCA_PCIE_IOPT_DMA_ENABLE _IO(ATCA_PCIE_IOP_MAGIC, 5)
#define ATCA_PCIE_IOPT_DMA_DISABLE _IO(ATCA_PCIE_IOP_MAGIC, 6)
#define ATCA_PCIE_IOPT_SOFT_TRIG _IO(ATCA_PCIE_IOP_MAGIC, 7)
#define ATCA_PCIE_IOPG_STATUS _IOR(ATCA_PCIE_IOP_MAGIC, 8, u_int32_t)
#define ATCA_PCIE_IOPS_RDTMOUT _IOW(ATCA_PCIE_IOP_MAGIC, 9, u_int32_t)
#define ATCA_PCIE_IOPS_DMA_SIZE _IOW(ATCA_PCIE_IOP_MAGIC, 10, u_int32_t)
#define ATCA_PCIE_IOPG_DMA_SIZE _IOR(ATCA_PCIE_IOP_MAGIC, 11, u_int32_t)
#define ATCA_PCIE_IOPT_DMA_RESET _IO(ATCA_PCIE_IOP_MAGIC, 12)
#define ATCA_PCIE_IOPT_STREAM_ENABLE _IO(ATCA_PCIE_IOP_MAGIC, 13)
#define ATCA_PCIE_IOPT_STREAM_DISABLE _IO(ATCA_PCIE_IOP_MAGIC, 14)
#define ATCA_PCIE_IOPT_CHOP_ON  _IO(ATCA_PCIE_IOP_MAGIC, 15)
#define ATCA_PCIE_IOPT_CHOP_OFF _IO(ATCA_PCIE_IOP_MAGIC, 16)
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

#define ATCA_PCIE_IOPS_CHOP_DISABLE _IOW(ATCA_PCIE_IOP_MAGIC, 27, u_int32_t)
#define ATCA_PCIE_IOPG_CHOP_DISABLE _IOR(ATCA_PCIE_IOP_MAGIC, 28, u_int32_t)

#define ATCA_PCIE_IOPG_DACS_REG _IOR(ATCA_PCIE_IOP_MAGIC, 29, u_int32_t)

#define ATCA_PCIE_IOP_MAXNR 29

#endif /* _ATCA_IOP_IOPTL_H_ */

//  vim: syntax=cpp ts=4 sw=4 sts=4 sr et
