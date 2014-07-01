@ This file is part of the PiEMU Project
@ Licensing information can be found in the LICENSE file
@ (C) 2014 Nandor Licker. All rights reserved.

entry:
  .code 32
  ldr r6, =(thumb + 1)
  bx  r6
thumb:
  .code 16
  @ Short loop
  mov     r0, #255
1:
  sub     r0, #1
  bne     1b

  @ Long loop
  ldr     r0, =0x100000
  mov     r1, #0
1:
  add     r1, #1
  sub     r0, #1
  bne     1b

  @ Branch back
  mov     r2, #0
1:
  add     r2, #1
  cmp     r2, #2
  beq     2f
  b       1b
2:

  bkpt

