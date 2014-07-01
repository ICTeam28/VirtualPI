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
  mov     r1, #2
  mov     r2, #1

  sub     r2, r1
  bhs     1f
  mov     r3, #0
  b       2f
1:
  mov     r3, #255
2:

  mov     r4, #3
  sub     r5, r4, #7
  ble     1f
  mov     r6, #0
  b       3f
1:
  bmi     2f
  mov     r6, #128
2:
  mov     r6, #255
3:

  mov     r7, #12
  cmp     r7, #7
  bgt     1f
  mov     r7, #0
  b       2f
1:
  mov     r7, #255
2:

  bkpt

