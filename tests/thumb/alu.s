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
  lsl r1, r2
  beq 9f
  bmi 9f
  bcs 9f
  cmp r1, #4
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

  @ Test #7
  mov r7, #7

  ldr r1, =0x00000001
  ldr r2, =0xFFFFFFFF
  ldr r3, =0x00000001
  ldr r4, =0x00000001
  add r2, r4
  adc r1, r3
  bmi 9f
  bcs 9f
  beq 9f
  bvs 9f
  cmp r2, #0
  bne 9f
  cmp r1, #3
  bne 9f

  @ Test #8
  mov r7, #8

  ldr r1, =0x00000001
  ldr r2, =0xFFFFFFFF
  ldr r3, =0x00000002
  ldr r4, =0x00000000
  sub r4, r2
  sbc r3, r1
  bmi 9f
  bcc 9f
  bne 9f
  bvs 9f
  cmp r4, #1
  bne 9f
  cmp r3, #0
  bne 9f

  @ Avoid branch out of range...
  b   1f
9:
  bkpt
1:

  @ Test #9
  mov r7, #9

  ldr r1, =0xF0F0F0F0
  mov r2, #5
  ldr r3, =0x87878787
  ror r1, r2
  bcc 9f
  beq 9f
  bpl 9f
  cmp r1, r3
  bne 9f

  @ Test #10
  mov r7, #10

  ldr r1, =0x80000000
  mov r2, #0
  tst r1, r1
  beq 9f
  bpl 9f
  tst r2, r2
  bne 9f
  bmi 9f

  @ Test #11
  mov r7, #11

  ldr r1, =0xFFFFFFFF
  neg r1, r1
  bmi 9f
  beq 9f
  bcc 9f
  bvs 9f
  cmp r1, #1
  bne 9f

  @ Test #12
  mov r7, #12

  ldr r1, =0xFFFFFFFF
  mov r2, #1
  cmn r1, r2
  bmi 9f
  bne 9f
  bcc 9f
  bvs 9f

  @ Test #13
  mov r7, #13

  ldr r1, =0xF0F0F0F0
  ldr r2, =0x0F0F0F0F
  ldr r3, =0xFFFFFFFF
  orr r1, r2
  bpl 9f
  beq 9f
  cmp r1, r3
  bne 9f

  @ Test #14
  mov r7, #14

  mov r1, #2
  ldr r2, =-6
  ldr r3, =-12
  mul r1, r2
  beq 9f
  bpl 9f
  cmp r1, r3
  bne 9f

  @ Test #15
  mov r7, #15

  mov r1, #2
  mov r2, #1
  add r1, r2
  mov r3, #0
  mvn r3, r3
  add r3, #1
  bcc 9f

  @ Avoid branch out of range...
  b   1f
9:
  bkpt
1:

  @ Test #16
  mov r7, #16

  ldr r5, =-2147483648
  add r5, r5
  bvc 9f

  @ Test #17
  mov r7, #17
  mov r1, #2
  mov r2, #1
  sub r2, r1
  bpl 9f
  beq 9f
  bvs 9f
  bcs 9f

  @ Test #18
  mov r7, #18
  mov r4, #3
  sub r5, r4, #7
  bge 9f
  bpl 9f

  @ Test #19
  mov r7, #19
  mov r1, #255
  lsl r1, #31
  bcc 9f
  bpl 9f
  beq 9f

  @ Test #20
  mov r7, #20
  mov r3, #255
  lsr r3, #8
  bcc 9f
  bne 9f

  @ Test #21
  mov r7, #21
  ldr r5, =-1024
  asr r5, #10
  bpl 9f
  beq 9f

  add r7, #1
9:
  bkpt
