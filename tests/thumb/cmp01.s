@ This file is part of the PiEMU Project
@ Licensing information can be found in the LICENSE file
@ (C) 2014 Nandor Licker. All rights reserved.

entry:
  .code 32
  ldr r6, =(thumb + 1)
  bx  r6
thumb:
  .code 16
  mov     r1, #100
  mov     r2, #200
  cmp     r1, r2
  bge     1f
  mov     r3, #1
  b       2f
1:
  mov     r3, #2
2:
  swi     #0xF
