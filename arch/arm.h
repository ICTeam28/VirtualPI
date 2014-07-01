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
  ARM_EXC_NONE,           ///< No exception present
  ARM_EXC_RESET,          ///< Reset interrupt
  ARM_EXC_UND,            ///< Undefined interrupt
  ARM_EXC_SWI,            ///< Software interrupt
  ARM_EXC_SMC,            ///< System monitor
  ARM_EXC_PABT,           ///< Prefetch abort
  ARM_EXC_DABT,           ///< Data abort
  ARM_EXC_IRQ,            ///< Interrupt request
  ARM_EXC_FIQ,            ///< Fast interrupt request
  ARM_EXC_BKPT,           ///< Software breakpoint
};


/**
 * ARM unit state
 */
struct ARMState
{
  union
  {
    struct
    {
      int32_t   r0;
      int32_t   r1;
      int32_t   r2;
      int32_t   r3;
      int32_t   r4;
      int32_t   r5;
      int32_t   r6;
      int32_t   r7;
      int32_t   r8;
      int32_t   r9;
      int32_t   r10;
      int32_t   r11;
      int32_t   r12;
      uint32_t  sp;        ///< Stack pointer
      uint32_t  lr;        ///< Link register
      uint32_t  pc;        ///< Program counter
    };

    int32_t     r[0x10];   ///< All registers
  };

  uint8_t       itt;       ///< ITT state
  uint8_t       t;         ///< THUMB bit
  uint8_t       f;         ///< FIQ enable
  uint8_t       i;         ///< Interrupt enable
  uint8_t       a;         ///< Imprecise abort
  uint8_t       e;         ///< Data endiannes
  uint8_t       ge;        ///< Greater than or equal to
  uint8_t       j;         ///< Java bit
  uint8_t       q;         ///< Stick overflow
  union
  {
    struct
    {
      uint8_t   n;        ///< Negative flag
      uint8_t   z;        ///< Zero flag
      uint8_t   c;        ///< Carry flag
      uint8_t   v;        ///< Overflow flag
    };
    uint32_t    flags;    ///< Combined flags
  };

  ARMMode       mode;     ///< Operating mode
  ARMException  exc;      ///< Exception pending
  Memory       *mem;      ///< Memory module
  volatile bool hang;     ///< Temporary exit flag
};


/**
 * Starts executing ARM instructions. This function exists when
 * an exception occurs or when the master thread signals it to quit
 *
 * @param emu Pointer to the emulator state
 */
void ARMExecute(ARMState *emu);


#endif /*__ARM_H__*/
