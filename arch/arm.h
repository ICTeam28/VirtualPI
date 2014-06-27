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
 * ARM operation mode
 */
enum ARMMode
{
  ARM_MODE_USR = 0x10,
  ARM_MODE_FIQ = 0x11,
  ARM_MODE_IRQ = 0x12,
  ARM_MODE_SVC = 0x13,
  ARM_MODE_ABT = 0x17,
  ARM_MODE_UND = 0x1B,
  ARM_MODE_SYS = 0x1F
};


/**
 * ARM unit state
 */
struct ARMState
{
  /// r0 - r15
  uint32_t r[0xF];

  /// Program counter
  uint8_t PC;

  /// Negative flag
  uint8_t N;

  /// Zero flag
  uint8_t Z;

  /// Carry flag
  uint8_t C;

  /// Overflow flag
  uint8_t V;

  /// IRQ bit
  uint8_t I;

  /// FIQ bit
  uint8_t F;

  /// Thumb bit
  uint8_t T;
};


#endif /*__ARM_H__*/
