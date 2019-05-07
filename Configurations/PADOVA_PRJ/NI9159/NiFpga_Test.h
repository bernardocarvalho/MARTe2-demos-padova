/*
 * Generated with the FPGA Interface C API Generator 17.0.0
 * for NI-RIO 17.0.0 or later.
 */

#ifndef __NiFpga_Test_h__
#define __NiFpga_Test_h__

#ifndef NiFpga_Version
   #define NiFpga_Version 1700
#endif

#include "NiFpga.h"

/**
 * The filename of the FPGA bitfile.
 *
 * This is a #define to allow for string literal concatenation. For example:
 *
 *    static const char* const Bitfile = "C:\\" NiFpga_Test_Bitfile;
 */
#define NiFpga_Test_Bitfile "NiFpga_Test.lvbitx"

/**
 * The signature of the FPGA bitfile.
 */
static const char* const NiFpga_Test_Signature = "5580A3D7E3CD37531FCD6557CFCD3824";

typedef enum
{
   NiFpga_Test_IndicatorBool_SPIDoneRX = 0x813A,
   NiFpga_Test_IndicatorBool_boh = 0x814A,
} NiFpga_Test_IndicatorBool;

typedef enum
{
   NiFpga_Test_ControlBool_stop = 0x8146,
   NiFpga_Test_ControlBool_stop2 = 0x8122,
   NiFpga_Test_ControlBool_use_counter = 0x8112,
} NiFpga_Test_ControlBool;

typedef enum
{
   NiFpga_Test_ControlU8_options = 0x8142,
} NiFpga_Test_ControlU8;

typedef enum
{
   NiFpga_Test_ControlU16_DAC_value0 = 0x810E,
   NiFpga_Test_ControlU16_DAC_value1 = 0x8126,
   NiFpga_Test_ControlU16_DacResolution = 0x811A,
   NiFpga_Test_ControlU16_end_frame = 0x814E,
   NiFpga_Test_ControlU16_maxV = 0x8116,
} NiFpga_Test_ControlU16;

typedef enum
{
   NiFpga_Test_ControlU32_DAC_value2 = 0x8128,
   NiFpga_Test_ControlU32_DAC_value3 = 0x812C,
   NiFpga_Test_ControlU32_cycleTimeDAC = 0x811C,
   NiFpga_Test_ControlU32_cycle_ticks = 0x8150,
   NiFpga_Test_ControlU32_packet_size = 0x8130,
} NiFpga_Test_ControlU32;

typedef enum
{
   NiFpga_Test_TargetToHostFifoU16_FIFO = 1,
   NiFpga_Test_TargetToHostFifoU16_FIFO2 = 0,
} NiFpga_Test_TargetToHostFifoU16;

#endif
