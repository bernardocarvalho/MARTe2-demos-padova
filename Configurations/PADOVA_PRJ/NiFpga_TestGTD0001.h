/*
 * Generated with the FPGA Interface C API Generator 17.0.0
 * for NI-RIO 17.0.0 or later.
 */

#ifndef __NiFpga_TestGTD0001_h__
#define __NiFpga_TestGTD0001_h__

#ifndef NiFpga_Version
   #define NiFpga_Version 1700
#endif

#include "NiFpga.h"

/**
 * The filename of the FPGA bitfile.
 *
 * This is a #define to allow for string literal concatenation. For example:
 *
 *    static const char* const Bitfile = "C:\\" NiFpga_TestGTD0001_Bitfile;
 */
#define NiFpga_TestGTD0001_Bitfile "NiFpga_TestGTD0001.lvbitx"

/**
 * The signature of the FPGA bitfile.
 */
static const char* const NiFpga_TestGTD0001_Signature = "F95CF85CC0C3D97B7D6A36162E15746C";

typedef enum
{
   NiFpga_TestGTD0001_IndicatorBool_SPIDoneRX = 0x815E,
   NiFpga_TestGTD0001_IndicatorBool_SPIDoneRX2 = 0x8126,
   NiFpga_TestGTD0001_IndicatorBool_boh1 = 0x8122,
   NiFpga_TestGTD0001_IndicatorBool_boh2 = 0x811E,
} NiFpga_TestGTD0001_IndicatorBool;

typedef enum
{
   NiFpga_TestGTD0001_IndicatorU32_Tick_Count_Ticks = 0x810C,
} NiFpga_TestGTD0001_IndicatorU32;

typedef enum
{
   NiFpga_TestGTD0001_ControlBool_stop = 0x8152,
   NiFpga_TestGTD0001_ControlBool_stop2 = 0x814E,
   NiFpga_TestGTD0001_ControlBool_use_RT_MXI = 0x8132,
   NiFpga_TestGTD0001_ControlBool_use_counter = 0x813A,
} NiFpga_TestGTD0001_ControlBool;

typedef enum
{
   NiFpga_TestGTD0001_ControlU8_options = 0x8166,
   NiFpga_TestGTD0001_ControlU8_options2 = 0x812A,
} NiFpga_TestGTD0001_ControlU8;

typedef enum
{
   NiFpga_TestGTD0001_ControlU16_DAC_value = 0x8136,
   NiFpga_TestGTD0001_ControlU16_DAC_value1 = 0x811A,
   NiFpga_TestGTD0001_ControlU16_DAC_value2 = 0x8116,
   NiFpga_TestGTD0001_ControlU16_DAC_value3 = 0x8112,
   NiFpga_TestGTD0001_ControlU16_DacResolution = 0x8142,
   NiFpga_TestGTD0001_ControlU16_end_frame = 0x816A,
   NiFpga_TestGTD0001_ControlU16_maxV = 0x813E,
} NiFpga_TestGTD0001_ControlU16;

typedef enum
{
   NiFpga_TestGTD0001_ControlI32_Timeout = 0x8144,
} NiFpga_TestGTD0001_ControlI32;

typedef enum
{
   NiFpga_TestGTD0001_ControlU32_cycleTimeDAC_ticks = 0x8148,
   NiFpga_TestGTD0001_ControlU32_cycle_ticks = 0x816C,
   NiFpga_TestGTD0001_ControlU32_packet_size = 0x8154,
} NiFpga_TestGTD0001_ControlU32;

typedef enum
{
   NiFpga_TestGTD0001_TargetToHostFifoU16_FIFO = 2,
   NiFpga_TestGTD0001_TargetToHostFifoU16_FIFO2 = 1,
} NiFpga_TestGTD0001_TargetToHostFifoU16;

typedef enum
{
   NiFpga_TestGTD0001_HostToTargetFifoU16_FIFO3 = 0,
} NiFpga_TestGTD0001_HostToTargetFifoU16;

#endif
