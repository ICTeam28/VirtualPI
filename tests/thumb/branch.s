@ This file is part of the PiEMU Project
@ Licensing information can be found in the LICENSE file
@ (C) 2014 Nandor Licker. All rights reserved.

entry:
  .arm
  ldr r0, =(thumb + 1)
  bx  r0


thumb:
  .thumb
1:
  @ Test #1 - Jump on N clear
  mov   r7, #1
  mov   r1, #255

  bmi   9f
  bpl   1f
  b     9f
1:

  @ Test #2 - Jump on N set
  mov   r7, #2
  mov   r1, #255
  lsl   r1, #31

  bpl   9f
  bmi   1f
  b     9f
1:

  @ Test #3 - Jump on Z clear
  mov   r7, #3
  mov   r1, #0

  bne   9f
  beq   1f
  b     9f
1:

  @ Test #4 - Jump on Z set
  mov   r7, #4
  mov   r1, #1

  beq   9f
  bne   1f
  b     9f
1:

  @ Test #5 - Jump on C clear
  mov   r7, #5
  ldr   r1, =0x80000000
  ldr   r2, =0x80000000
  add   r2, r1

  bcc   9f
  bcs   1f
  b     9f
1:

  @ Test #6 - Jump on C set
  mov   r7, #6
  ldr   r1, =0x40000000
  ldr   r2, =0x40000000
  add   r2, r1

  bcs   9f
  bcc   1f
  b     9f
1:

  @ Test #7 - Jump on V clear
  mov   r7, #7
  ldr   r1, =0x80000000
  ldr   r2, =0x80000000
  add   r2, r1

  bvc   9f
  bvs   1f
  b     9f
1:

  @ Test #8 - Jump on V set
  mov   r7, #8
  ldr   r1, =0x20000000
  ldr   r2, =0x20000000
  add   r2, r1

  bvs   9f
  bvc   1f
  b     9f
1:

  @ Test  #9 - Jump on hi/ls
  mov   r7, #9
  mov   r1, #1
  mov   r2, #2
  cmp   r1, r2

  bhi   9f
  bls   1f
  b     9f
1:

  @ Test #10 - Jump on ls/hi
  mov   r7, #10
  mov   r1, #1
  mov   r2, #2
  cmp   r2, r1

  bls   9f
  bhi   1f
  b     9f
1:

  @ Test #11 - Jump on ge/lt
  mov   r7, #11
  ldr   r1, =-2
  ldr   r2, =2

  cmp   r1, r2
  bge   9f
  blt   1f
  b     9f
1:

  @ Test #12 - Jump on lt/ge
  mov   r7, #12
  ldr   r1, =-2
  ldr   r2, =2
  cmp   r2, r1

  blt   9f
  bge   1f
  b     9f
1:

  @ Test #13 - Jump on gt/le
  mov   r7, #13
  ldr   r1, =-2
  ldr   r2, =2
  cmp   r1, r2

  bgt   9f
  ble   1f
  b     9f
1:

  @ Test #14 - Jump on lt/ge
  mov   r7, #14
  cmp   r2, r1

  ble   9f
  bgt   1f
  b     9f
1:

  @ Test #15 - Branch back
  mov   r7, #15
  mov   r2, #0
1:
  add   r2, #1
  cmp   r2, #2
  beq   2f
  b     1b
2:

  @ Test #16 - Loop
  mov   r7, #16
  mov   r2, #5

1:
  sub   r2, #1
  bne   1b

  @ Test #17 - Branch to ARM
  mov   r7, #17
  ldr   r0, =bx_arm
  bx    r0
bx_arm:
  .arm
  mov   r1, #2
  ldr   r0, =(bx_thumb + 1)
  bx    r0
bx_thumb:
  .thumb
  cmp   r1, #2
  bne   9f

  add   r7, #1
9:
  bkpt
