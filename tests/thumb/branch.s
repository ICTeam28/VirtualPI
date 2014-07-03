@ This file is part of the PiEMU Project
@ Licensing information can be found in the LICENSE file
@ (C) 2014 Nandor Licker. All rights reserved.

entry:
  .arm
  ldr r6, =(thumb + 1)
  bx  r6
thumb:
  .thumb
1:
  mov   r7, #0
  ldr   r5, =-1
  mov   r6, #1

  @ Jump on N clear
  mov   r1, #255
  bmi   9f
  bpl   1f
  orr   r7, r6
  b     2f
1:
  bic   r5, r6
2:
  lsl   r6, #1

  @ Jump on N set
  lsl   r1, #31
  bpl   9f
  bmi   1f
  orr   r7, r6
  b     2f
1:
  bic   r5, r6
2:
  lsl   r6, #1

  @ Jump on Z clear
  mov   r1, #0
  bne   9f
  beq   1f
  orr   r7, r6
  b     2f
1:
  bic   r5, r6
2:
  lsl   r6, #1

  @ Jump on Z set
  mvn   r1, r1
  beq   9f
  bne   1f
  orr   r7, r6
  b     2f
1:
  bic   r5, r6
2:
  lsl   r6, #1

  @ Jump on C clear
  ldr   r1, =0x80000000
  ldr   r2, =0x80000000
  add   r2, r1
  bcc   9f
  bcs   1f
  orr   r7, r6
  b     2f
1:
  bic   r5, r6
2:
  lsl   r6, #1

  @ Jump on C set
  ldr   r1, =0x40000000
  ldr   r2, =0x40000000
  add   r2, r1
  bcs   9f
  bcc   1f
  orr   r7, r6
  b     2f
1:
  bic   r5, r6
2:
  lsl   r6, #1

  @ Jump on V clear
  ldr   r1, =0x80000000
  ldr   r2, =0x80000000
  add   r2, r1
  bvc   9f
  bvs   1f
  orr   r7, r6
  b     2f
1:
  bic   r5, r6
2:
  lsl   r6, #1

  @ Jump on V set
  ldr   r1, =0x20000000
  ldr   r2, =0x20000000
  add   r2, r1
  bvs   9f
  bvc   1f
  orr   r7, r6
  b     2f
1:
  bic   r5, r6
2:
  lsl   r6, #1

  mov   r1, #1
  mov   r2, #2

  @ Jump on hi/ls
  cmp   r1, r2
  bhi   9f
  bls   1f
  orr   r7, r6
  b     2f
1:
  bic   r5, r6
2:
  lsl   r6, #1

  @ Jump on ls/hi
  cmp   r2, r1
  bls   9f
  bhi   1f
  orr   r7, r6
  b     2f
1:
  bic   r5, r6
2:
  lsl   r6, #1

  ldr   r1, =-2
  ldr   r2, =2

  @ Jump on ge/lt
  cmp   r1, r2
  bge   9f
  blt   1f
  orr   r7, r6
  b     2f
1:
  bic   r5, r6
2:
  lsl   r6, #1

  @ Jump on lt/ge
  cmp   r2, r1
  blt   9f
  bge   1f
  orr   r7, r6
  b     2f
1:
  bic   r5, r6
2:
  lsl   r6, #1

  @ Jump on gt/le
  cmp   r1, r2
  bgt   9f
  ble   1f
  orr   r7, r6
  b     2f
1:
  bic   r5, r6
2:
  lsl   r6, #1

  @ Jump on lt/ge
  cmp   r2, r1
  ble   9f
  bgt   1f
  orr   r7, r6
  b     2f
1:
  bic   r5, r6
2:
  lsl   r6, #1

  @ Branch back
  mov   r2, #0
1:
  add   r2, #1
  cmp   r2, #2
  beq   2f
  b     1b
2:

  @ Loop
  mov   r2, #5
1:
  sub   r2, #1
  bne   1b

  mov   r4, #2
  b     8f
9:
  mov   r4, #1
8:
  bkpt
