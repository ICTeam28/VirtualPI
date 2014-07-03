@ This file is part of the PiEMU Project
@ Licensing information can be found in the LICENSE file
@ (C) 2014 Nandor Licker. All rights reserved.

entry:
  .code 32
  ldr r6, =(thumb + 1)
  bx  r6
thumb:
  .code 16
  mov r1, #50
  mov r8, r1
  mov r9, r8
  mov r7, r8

  mov r1, #100
  add r8, r1
  add r9, r8
  add r1, r8

  mov r2, #150
  mov r3, #250

  cmp r8, r2
  bne 9f

  cmp r1, r8
  beq 9f

  cmp r9, r8
  beq 9f

  ldr r7, =0xFFFFFFFF
9:
  bkpt

