/*
 * Generated with the FPGA Interface C API Generator 17.0.0
 * for NI-RIO 17.0.0 or later.
 */

#ifndef __NiFpga_DAC0_h__
#define __NiFpga_DAC0_h__

#ifndef NiFpga_Version
   #define NiFpga_Version 1700
#endif

#include "NiFpga.h"

/**
 * The filename of the FPGA bitfile.
 *
 * This is a #define to allow for string literal concatenation. For example:
 *
 *    static const char* const Bitfile = "C:\\" NiFpga_DAC0_Bitfile;
 */
#define NiFpga_DAC0_Bitfile "NiFpga_DAC0.lvbitx"

/**
 * The signature of the FPGA bitfile.
 */
static const char* const NiFpga_DAC0_Signature = "C00A8A8DB0CEC19CCDA0D58D81AC0BC3";

typedef enum
{
   NiFpga_DAC0_ControlBool_stop = 0x8112,
} NiFpga_DAC0_ControlBool;

typedef enum
{
   NiFpga_DAC0_ControlSgl_Mod1AO0 = 0x810C,
} NiFpga_DAC0_ControlSgl;

#endif
