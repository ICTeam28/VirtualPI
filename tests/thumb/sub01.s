@ This file is part of the PiEMU Project
@ Licensing information can be found in the LICENSE file
@ (C) 2014 Nandor Licker. All rights reserved.

entry:
  .code 32
  ldr r6, =(thumb + 1)
  bx  r6
thumb:
  .code 16
1:
  mov     r1, #255
  mov     r2, #255
  sub     r1, r2
  mov     r4, #255
  sub     r4, #255
  swi     0x0
