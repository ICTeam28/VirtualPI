@ This file is part of the PiEMU Project
@ Licensing information can be found in the LICENSE file
@ (C) 2014 Nandor Licker. All rights reserved.

entry:
  .code 32
  ldr r0, =(thumb + 1)
  bx  r0
thumb:
  .code 16
1:
  @ Test #1
  mov r7, #1
  ldr r1, =0xFFFFFFFF
  ldr r2, =0x80000000

  and r1, r2
  beq 9f
  bpl 9f
  cmp r2, r1
  bne 9f

  @ Test #2
  mov r7, #2
  mov r2, #0

  and r2, r2
  bne 9f
  bmi 9f
  cmp r2, #0
  bne 9f

  @ Test #3
  mov r7, #3
  ldr r1, =0xFFFFFFFF
  ldr r2, =0x80000000
  ldr r3, =0x7FFFFFFF

  eor r1, r2
  beq 9f
  bmi 9f
  cmp r1, r3
  bne 9f

  @ Test #4
  mov r7, #4
  mov r1, #1
  mov r2, #2
  mov r3, #4

  lsl r1, r2
  beq 9f
  bmi 9f
  bcs 9f
  cmp r1, r3
  bne 9f

  @ Test #5
  mov r7, #5
  mov r1, #1
  mov r2, #1

  lsr r1, r2
  bne 9f
  bmi 9f
  bcc 9f
  cmp r1, #0
  bne 9f

  @ Test #6
  mov r7, #6
  ldr r1, =-1
  ldr r3, =-1
  mov r2, #5
  asr r1, r2
  beq 9f
  bpl 9f
  bcc 9f
  cmp r3, r1
  bne 9f

  add r7, #1
9:
  bkpt
