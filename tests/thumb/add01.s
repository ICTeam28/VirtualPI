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
  add     r1, r2

  @ Test carry
  mov     r3, #0
  mvn     r3, r3
  add     r3, #1
  bcc     1f
  mov     r4, #255
  b       2f
1:
  mov     r4, #0
2:

  @ Test overflow
  ldr     r5, =-2147483648
  mov     r6, r5
  add     r6, r5
  bvc     1f
  mov     r7, #255
  b       2f
1:
  mov     r7, #1
2:

  swi     0x0
