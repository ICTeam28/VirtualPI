@ This file is part of the PiEMU Project
@ Licensing information can be found in the LICENSE file
@ (C) 2014 Nandor Licker. All rights reserved.

entry:
  .code 32
  ldr r6, =(thumb + 1)
  bx  r6
thumb:
  .code 16

  @ Test #0
  mov   r7, #0

  ldr   r0, =0x8000
  sxth  r1, r0
  ldr   r2, =0xFFFF8000
  cmp   r1, r2
  bne   9f

  @ Test #1
  mov   r7, #1

  ldr   r0, =0x8000
  uxth  r1, r0
  ldr   r2, =0x00008000
  cmp   r1, r2
  bne   9f

  @ Test #2
  mov   r7, #2

  ldr   r0, =0x0080
  sxtb  r1, r0
  ldr   r2, =0xFFFFFF80
  cmp   r1, r2
  bne   9f

  @ Test #3
  mov   r7, #3

  ldr   r0, =0x0080
  uxtb  r1, r0
  ldr   r2, =0x00000080
  cmp   r1, r2
  bne   9f

  @ Test #4
  mov   r7, #4

  ldr   r0, =0x4000
  sxth  r1, r0
  ldr   r2, =0x00004000
  cmp   r1, r2
  bne   9f

  @ Test #5
  mov   r7, #5

  ldr   r0, =0x4000
  uxth  r1, r0
  ldr   r2, =0x00004000
  cmp   r1, r2
  bne   9f

  @ Test #6
  mov   r7, #6

  ldr   r0, =0x0040
  sxtb  r1, r0
  ldr   r2, =0x00000040
  cmp   r1, r2
  bne   9f

  @ Test #7
  mov   r7, #7

  ldr   r0, =0x0040
  uxtb  r1, r0
  ldr   r2, =0x00000040
  cmp   r1, r2
  bne   9f

  @ Test #8
  mov   r7, #8

  ldr   r0, =0x12345678
  rev   r0, r0
  ldr   r1, =0x78563412
  cmp   r0, r1
  bne   9f

  @ Text #9
  mov   r7, #9

  ldr   r0, =0x12345678
  rev16 r0, r0
  ldr   r1, =0x34127856
  cmp   r0, r1
  bne   9f

  @ Text #10
  mov   r7, #10
  ldr   r0, =0x0080
  revsh r0, r0
  ldr   r1, =0xFFFF8000
  cmp   r0, r1
  bne   9f

  add   r7, #1
9:
  bkpt
