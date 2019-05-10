/*
 * Generated with the FPGA Interface C API Generator 17.0.0
 * for NI-RIO 17.0.0 or later.
 */

#ifndef __NiFpga_Test1_h__
#define __NiFpga_Test1_h__

#ifndef NiFpga_Version
   #define NiFpga_Version 1700
#endif

#include "NiFpga.h"

/**
 * The filename of the FPGA bitfile.
 *
 * This is a #define to allow for string literal concatenation. For example:
 *
 *    static const char* const Bitfile = "C:\\" NiFpga_Test1_Bitfile;
 */
#define NiFpga_Test1_Bitfile "NiFpga_Test1.lvbitx"

/**
 * The signature of the FPGA bitfile.
 */
static const char* const NiFpga_Test1_Signature = "41D583976C70E26C9B86EBB10D2E9F27";

typedef enum
{
   NiFpga_Test1_IndicatorBool_SPIDoneRX = 0x8146,
   NiFpga_Test1_IndicatorBool_boh = 0x8156,
} NiFpga_Test1_IndicatorBool;

typedef enum
{
   NiFpga_Test1_IndicatorU32_Tick_Count_Ticks = 0x810C,
} NiFpga_Test1_IndicatorU32;

typedef enum
{
   NiFpga_Test1_ControlBool_stop = 0x8152,
   NiFpga_Test1_ControlBool_stop2 = 0x8126,
   NiFpga_Test1_ControlBool_use_RT_MXI = 0x8112,
   NiFpga_Test1_ControlBool_use_counter = 0x813A,
} NiFpga_Test1_ControlBool;

typedef enum
{
   NiFpga_Test1_ControlU8_options = 0x814E,
} NiFpga_Test1_ControlU8;

typedef enum
{
   NiFpga_Test1_ControlU16_DAC_value = 0x812A,
   NiFpga_Test1_ControlU16_DAC_value1 = 0x812E,
   NiFpga_Test1_ControlU16_DAC_value2 = 0x8132,
   NiFpga_Test1_ControlU16_DAC_value3 = 0x8136,
   NiFpga_Test1_ControlU16_DacResolution = 0x811A,
   NiFpga_Test1_ControlU16_end_frame = 0x815A,
   NiFpga_Test1_ControlU16_maxV = 0x8116,
} NiFpga_Test1_ControlU16;

typedef enum
{
   NiFpga_Test1_ControlI32_Timeout = 0x811C,
} NiFpga_Test1_ControlI32;

typedef enum
{
   NiFpga_Test1_ControlU32_cycleTimeDAC_ticks = 0x8120,
   NiFpga_Test1_ControlU32_cycle_ticks = 0x815C,
   NiFpga_Test1_ControlU32_packet_size = 0x813C,
} NiFpga_Test1_ControlU32;

typedef enum
{
   NiFpga_Test1_TargetToHostFifoU16_FIFO = 2,
   NiFpga_Test1_TargetToHostFifoU16_FIFO2 = 1,
} NiFpga_Test1_TargetToHostFifoU16;

typedef enum
{
   NiFpga_Test1_HostToTargetFifoU16_FIFO3 = 0,
} NiFpga_Test1_HostToTargetFifoU16;

#endif
