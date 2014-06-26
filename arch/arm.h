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
