/*******************************/
/* Configuration of the driver */
/*******************************/

/* Enable/disable IRQ handling */ //#define ENABLE_IRQ

/* The name of the module */
//#define MODNAME "atca_ioc_int"

/* Major number is allocated dynamically */

/* Minor  number is set to 0  */

/* The number of available minor numbers */
#define ATCA_MINOR_BASE (0)    //
#define ATCA_MINOR_COUNT (255) //

//#define MINOR_NUMBERS 2 // 0xffff

/* Node name of the char device */

#define NODENAMEFMT "atca_v6_%d"
#define DMACH0NODENAMEFMT "atca_v6_dma_ch0_%d"
#define DMACH1NODENAMEFMT "atca_v6_dma_ch1_%d"

#define DRV_NAME "atca_v6" //"atca_v6_drv"

/* Maximum number of devices*/
#define MAXDEVICES 14

#define DMA_BUFFS 8 // Number of DMA Buffs

/* board PCI id */
#define PCI_DEVICE_ID_FPGA 0x0040

#define NUM_BARS 2
