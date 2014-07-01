@ This file is part of the PiEMU Project
@ Licensing information can be found in the LICENSE file
@ (C) 2014 Nandor Licker. All rights reserved.

entry:
  .code 32
  ldr r6, =(thumb + 1)
  bx  r6
thumb:
  .code 16
  mov     sp, pc
  ldr     r1, =0x1234567
  ldr     r2, =0x7654321
  ldr     r3, [sp]

  bkpt

