@ This file is part of the PiEMU Project
@ Licensing information can be found in the LICENSE file
@ (C) 2014 Nandor Licker. All rights reserved.

entry:
  .code 32
  ldr r6, =thumb
  bx  r6
thumb:
  .code 16
1:
  mov     r1, #0
  mov     r2, #1
  add     r1, r2
  b       1b
