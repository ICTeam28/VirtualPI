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