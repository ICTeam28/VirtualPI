// This file is part of the PiEMU Project
// Licensing information can be found in the LICENSE file
// (C) 2014 Nandor Licker. All rights reserved.
#ifndef __THUMB_H__
#define __THUMB_H__

/**
 * Forward declaration of the ARM state
 */
struct ARMState;


/**
 * Starts executing THUMB instructions. This function exists when
 * an exception occurs or when the master thread signals it to quit
 *
 * @param emu Pointer to the emulator state
 */
void ThumbExecute(ARMState *t);


#endif /*__THUMB_H__*/