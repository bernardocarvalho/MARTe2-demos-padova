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
#define ATCA_MINOR_COUNT (64) // Max 14 * 3 device minor number

#define FPGA_FW_VERSION  0x02010000   // MAJOR/MINOR/PATCH Version numbers

//#define MINOR_NUMBERS 2 // 0xffff

/* Node name of the char device */

#define NODENAMEFMT "atca_v6_%d"
#define DMACH0NODENAMEFMT "atca_v6_dmart_%d"
#define DACNODENAMEFMT "atca_v6_dac_%d"

#define DRV_NAME "atca_v6"

/* Maximum number of devices on as 14-slot ATCA crate*/
#define MAXDEVICES 12

#define DMA_BUFFS 8 // Number of DMA Buffs

/* board PCI id */
#define PCI_DEVICE_ID_FPGA 0x0040

#define NUM_BARS 2
