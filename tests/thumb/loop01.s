@ This file is part of the PiEMU Project
@ Licensing information can be found in the LICENSE file
@ (C) 2014 Nandor Licker. All rights reserved.

entry:
  .code 32
  ldr r6, =thumb
  bx  r6
thumb:
  .code 16
  mov     r1, #255
1:
  sub     r1, #1
  bne     1b
  .long   0
