/**
 * ATCA IO CONTROL Integrator
 * Linux Device Driver
 * Internal definitions for all parts (prototypes, data, macros)
 *
 *
 */
#ifndef _DRIVER_COMMON_H
#define _DRIVER_COMMON_H

#include <linux/cdev.h>
#include <linux/errno.h>
#include <linux/fs.h>
#include <linux/interrupt.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/pci.h>
#include <linux/stat.h>
#include <linux/types.h>
#include <linux/version.h>
#include <linux/sched.h>
#include <linux/spinlock.h>
#include <linux/uaccess.h>
//#include <linux/wait.h>
#include <asm/atomic.h>
//#include <asm/msr.h>
//#include <asm/uaccess.h>
#include <linux/delay.h> /* usleep_range */
#include <linux/dma-mapping.h>

#include "atca-v6-pcie.h"
#include "atca-v6-pcie-ioctl.h"

/*************************************************************************/
/* Private data types and structures */

typedef struct _BAR_STRUCT {
  unsigned long phys;
  // unsigned long end;
  unsigned long psize;
  // unsigned long flags;
  void *vaddr;
} BAR_STRUCT;

typedef struct _DMA_BUF {
  void *addr_v;
  dma_addr_t addr_hw;
} DMA_BUF;

typedef struct _DMA_STRUCT {
  unsigned int buf_size;
  unsigned int buf_actv;
  dma_addr_t hw_actv;
  u32 *daddr_actv; // ptr to data on active buffer
  DMA_BUF buf[DMA_BUFFS];
} DMA_STRUCT;

typedef struct _DMA_RT_STRUCT {
  //dma_addr_t hw_actv;
  //u32 *daddr_actv; // ptr to data on active buffer
  DMA_BUF buf[DMA_BUFFS];
} DMA_RT_STRUCT;

typedef struct _READ_BUF {
  int count;
  int total;
  u32 *buf; // assume that ADC data is 32bit wide
} READ_BUF;

/*
 * 32 bit field
 */
typedef struct _STATUS_FLDS {
  u32 DmaStatus:8,
      Mmcm33Lck:1,
      Mmcm100Lck:1,
      Ad9511Status:1,
      Master:1,
      AcqOn:1,
      TrigRtm:1,
      rsv0 : 2,
      IpmcAdd : 16; // msb
//u32 RevId : 8, rsv0 : 22, DmaC : 1, rsv1 : 1; // msb
} STATUS_FLDS;

typedef struct _STATUS_REG {
  union {
    u32 reg32;
    STATUS_FLDS statFlds;
  };
} STATUS_REG;

typedef struct _COMMAND_REG {
  union {
    u32 reg32;
    struct {
      u32 Dma1 : 1, Dma2 : 1, rsv0 : 8,
          ChopOn   : 1, //  off 10
          ChopDflt : 1, //  off 11
          ChopRec  : 1, rsv00 : 7, //  off 12
          StreamE : 1, //  off 20
          rsv01 : 2,
          AcqE : 1, //  off 23
          StrG : 1, //  off 24
          rsv1 : 2, DmaE : 1, DmaRst : 1, rsv2 : 1, DmaIntE : 1, rsv3 : 1;
    } cmdFlds;
  };
} COMMAND_REG;

typedef struct _DMA_CURR_BUFF {
  union {
    u32 reg32;
    struct {
      u32 DmaBuffNum : 3, DmaSel : 1, rsv0 : 28;
    } dmaFlds;
  };
} DMA_CURR_BUFF;

typedef struct _PCIE_SHAPI_HREGS {
  volatile u32 shapiVersion;       /*Offset 0x00 ro */
  volatile u32 firstModAddress;    /*Offset 0x04 ro */
  volatile u32 hwIDhwVendorID;     /*Offset 0x08 ro*/
  volatile u32 devFwIDdevVendorID; /*Offset 0x0C ro */
  volatile u32 fwVersion;          /*Offset 0x10 ro */
  volatile u32 fwTimeStamp;        /*Offset 0x14 ro*/
  volatile u32 fwName[3];          /*Offset 0x18 ro*/
  volatile u32 devCapab;           /*Offset 0x24 ro*/
  volatile u32 devStatus;          /*Offset 0x28 ro*/
  volatile u32 devControl;         /*Offset 0x2C rw*/
  volatile u32 devIntMask;         /*Offset 0x30 rw*/
  volatile u32 devIntFlag;         /*Offset 0x34 ro*/
  volatile u32 devIntActive;       /*Offset 0x38 ro*/
  volatile u32 scratchReg;         /*Offset 0x3C rw*/
} PCIE_SHAPI_HREGS;

typedef struct _SHAPI_MOD_DMA_HREGS {
  volatile u32 shapiVersion;       /*Offset 0x00 ro */
  volatile u32 nextModAddress;     /*Offset 0x04 ro */
  volatile u32 modFwIDmodVendorID; /*Offset 0x08 ro*/
  volatile u32 modVersion;         /*Offset 0x0C ro */
  volatile u32 modName[2];         /*Offset 0x10 ro*/
  volatile u32 modCapab;           /*Offset 0x18 ro*/
  volatile u32 modStatus;          /*Offset 0x1C ro*/
  volatile u32 modControl;         /*Offset 0x20 rw*/
  volatile u32 modIntID;           /*Offset 0x24 rw*/
  volatile u32 modIntFlagClear;    /*Offset 0x28 ro*/
  volatile u32 modIntMask;         /*Offset 0x2C rw*/
  volatile u32 modIntFlag;         /*Offset 0x30 ro*/
  volatile u32 modIntActive;       /*Offset 0x34 ro*/
  volatile u32 _reserved1[2];      /*Offset 0x38 - 0x40 na */
  volatile u32 dmaStatus;          /* Offset 0x40 ro */
  volatile u32 dmaControl;         /* Offset 0x44 rw */
  volatile u32 dmaByteSize;        /* Offset 0x48 rw */
  volatile u32 dmaMaxBytes;        /* Offset 0x4C r */
  volatile u32 dmaTlpPayload;      /* Offset 0x50 ro */
  volatile u32 _reserved2;         /* Offset 0x54 na */
  volatile u32 chopCountrs;        /* Offset 0x58 rw */
  volatile u32 dmaDebug0;          /* Offset 0x5C ro */
  volatile u32 dmaDebug1;          /* Offset 0x60 ro */
  volatile u32 _reserved3[7];     /* Offset 0x64 - 0x7C na */
  volatile u32 dmaBusAddr[DMA_BUFFS];       /* Offset 0x80 rw */
  volatile u32 dmaPollBusAddr[DMA_BUFFS];   /* Offset 0xA0 rw */
  //volatile u32 _reserved4[7];      /* Offset 0xA4 na */
  volatile u32 eOffsets[32];       /* Offset 0xC0 rw */
  volatile u32 wOffsets[32];       /* Offset 0x180 rw */

  ////  EVENT_REGS                timingRegs[NUM_TIMERS];
} SHAPI_MOD_DMA_HREGS;

/*Structure for pcie access*/
typedef struct _PCIE_DEV {
  /* char device */
  struct pci_dev *pdev; /* pci device */
  struct cdev cdev;     /* linux char device structure   */
  dev_t devno;          /* char device number */
  struct device *dev;
  struct cdev dmach0_cdev; /* linux char device structure for DMA chan 0 */
  dev_t dmach0_devno;      /* char device number for DMA */
  struct device *dmach0_dev;
  struct cdev dmach1_cdev; /* linux char device structure for DMA chan 1 */
  dev_t dmach1_devno;      /* char device number for DMA */
  struct device *dmach1_dev;
  unsigned char irq;
  spinlock_t irq_lock; // static
  unsigned int got_regions;
  unsigned int msi_enabled;
  unsigned int counter;
  unsigned int counter_hw;
  unsigned int open_count;
  struct semaphore open_sem; // mutual exclusion semaphore
  wait_queue_head_t rd_q;    // read  queues
  long wt_tmout;             // read timeout
  atomic_t rd_condition;
  unsigned int mismatches;
  unsigned int max_buffer_count;
  unsigned int curr_buf;

  BAR_STRUCT memIO[2];
  DMA_STRUCT dmaIO;
  DMA_RT_STRUCT dmaRT;
  //DMA_BUF dmaPollBuff;

  PCIE_SHAPI_HREGS *pShapiHregs;
  SHAPI_MOD_DMA_HREGS *pModDmaHregs;
} PCIE_DEV;

/*I/O Macros*/

//#define PCIE_READ32(addr) ioread32(addr)
//#define PCIE_WRITE32(value, addr) iowrite32(value, addr)
//#define PCIE_FLUSH32() PCIE_READ32()

/*************************************************************************/
/* Some nice defines that make code more readable */
/* This is to print nice info in the log

#ifdef DEBUG
#define mod_info( args... ) \
do { printk( KERN_INFO "%s - %s : ", MODNAME , __FUNCTION__ );\
printk( args ); } while(0)
#define mod_info_dbg( args... ) \
do { printk( KERN_INFO "%s - %s : ", MODNAME , __FUNCTION__ );\
printk( args ); } while(0)
#else
#define mod_info( args... ) \
do { printk( KERN_INFO "%s: ", MODNAME );\
printk( args ); } while(0)
#define mod_info_dbg( args... )
#endif

#define mod_crit( args... ) \
do { printk( KERN_CRIT "%s: ", MODNAME );\
printk( args ); } while(0)

#define MIN(a, b) ((a) < (b) ? (a) : (b))

 **/

#endif // _DRIVER_COMMON_H
