// This file is part of the PiEMU Project
// Licensing information can be found in the LICENSE file
// (C) 2014 Nandor Licker. All rights reserved.
#ifndef __THUMB_H__
#define __THUMB_H__


/**
 * THUMB state
 */
struct THUMBState
{
  /// r0 - r7
  int32_t r[8];

  /// Stack Pointer
  uint32_t sp;

  /// Program Counter
  uint32_t pc;

  /// Link Register
  uint32_t lr;

  /// Zero flag
  uint32_t z;

  /// Negative flag
  uint32_t n;

  /// Carry flag
  uint32_t c;

  /// Overflow flag
  uint32_t v;

  /// ITT state
  uint8_t  itt;
};


/**
 * Executes a thumb instruction
 * @param emu Pointer to the emulator state
 */
void ThumbExecute(Emulator *emu);


/**
 * Switch to THUMB mode
 */
void ThumbSwitch(Emulator *emu);


#endif /*__THUMB_H__*/