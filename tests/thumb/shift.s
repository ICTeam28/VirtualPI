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
  @ LSL - carry out
  mov     r1, #255
  lsl     r1, #31
  bcc     1f
  mov     r2, #255
  b       2f
1:
  mov     r2, #0
2:

  @ LSR - carry out
  mov     r3, #255
  lsr     r3, #8
  bcc     1f
  mov     r4, #255
  b       2f
1:
  mov     r4, #0
2:

  @ ASR - negative number
  ldr     r5, =-1024
  asr     r5, #10
  bpl     1f
  mov     r6, #255
1:
  mov     r6, #0
2:

  bkpt

