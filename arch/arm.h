// This file is part of the PiEMU Project
// Licensing information can be found in the LICENSE file
// (C) 2014 Nandor Licker. All rights reserved.
#ifndef __ARM_H__
#define __ARM_H__

/**
 * Forward declaration of the memory module
 */
class Memory;


/**
 * ARM state
 */
enum ARMState
{
  ARM_STATE_ARM,
  ARM_STATE_THUMB,
  ARM_STATE_JAZELLE
};

/**
 * ARM operating modes
 */
enum ARMMode
{
  ARM_MODE_USR = 0x10,
  ARM_MODE_FIQ = 0x11,
  ARM_MODE_IRQ = 0x12,
  ARM_MODE_SVC = 0x13,
  ARM_MODE_MON = 0x16,
  ARM_MODE_ABT = 0x17,
  ARM_MODE_UND = 0x1B,
  ARM_MODE_SYS = 0x1F
};


/**
 * ARM exceptions
 */
enum ARMException
{
  /// No exception present
  ARM_EXC_NONE,

  /// Reset interrupt
  ARM_EXC_RESET,

  /// Undefined interrupt
  ARM_EXC_UND,

  /// Software interrupt
  ARM_EXC_SWI,

  /// System monitor
  ARM_EXC_SMC,

  /// Prefetch abort
  ARM_EXC_PABT,

  /// Data abort
  ARM_EXC_DABT,

  /// Interrupt request
  ARM_EXC_IRQ,

  /// Fast interrupt request
  ARM_EXC_FIQ,

  /// Software breakpoint
  ARM_EXC_BKPT,

  /// Switch to ARM
  ARM_EXC_TOARM,

  /// Switch to THUMB
  ARM_EXC_TOTHUMB
};


/**
 * ARM unit state
 */
struct ARMData
{
  /// Registers
  union
  {
    struct
    {
      uint32_t r0;
      uint32_t r1;
      uint32_t r2;
      uint32_t r3;
      uint32_t r4;
      uint32_t r5;
      uint32_t r6;
      uint32_t r7;
      uint32_t r8;
      uint32_t r9;
      uint32_t r10;
      uint32_t r11;
      uint32_t r12;
      uint32_t sp;
      uint32_t lr;
      uint32_t pc;
    };

    int32_t r[0x10];
  };

  /// Arithmetic flags
  union
  {
    struct
    {
      uint8_t n;
      uint8_t z;
      uint8_t c;
      uint8_t v;
    };

    /// Combined flags
    uint32_t flags;
  };

  /// ITT state
  uint8_t  itt;

  /// Interrupt enable
  uint8_t i;

  /// FIQ enable
  uint8_t f;

  /// Exception pending
  ARMException exc;

  /// Memory module
  Memory *mem;
};


/**
 * Starts executing ARM instructions. This function exists when
 * an exception occurs or when the master thread signals it to quit
 *
 * @param emu Pointer to the emulator state
 */
void ARMExecute(ARMData *emu);


#endif /*__ARM_H__*/
