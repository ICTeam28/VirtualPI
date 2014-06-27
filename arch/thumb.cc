// This file is part of the PiEMU Project
// Licensing information can be found in the LICENSE file
// (C) 2014 Nandor Licker. All rights reserved.
#include "common.h"


// -------------------------------------------------------------------------------------------------
static void BCC(THUMBState*, uint16_t) FORCEINLINE;
static void BAL(THUMBState*, uint16_t) FORCEINLINE;
static void BLO(THUMBState*, uint16_t) FORCEINLINE;
static void BLH(THUMBState*, uint16_t) FORCEINLINE;
static void ADD(THUMBState*, int32_t&, int32_t) FORCEINLINE;
static void AND(THUMBState*, int32_t&, int32_t) FORCEINLINE;
static void EOR(THUMBState*, int32_t&, int32_t) FORCEINLINE;
static void LSL(THUMBState*, int32_t&, int32_t) FORCEINLINE;
static void LSR(THUMBState*, int32_t&, int32_t) FORCEINLINE;
static void ASR(THUMBState*, int32_t&, int32_t) FORCEINLINE;
static void ADC(THUMBState*, int32_t&, int32_t) FORCEINLINE;
static void SBC(THUMBState*, int32_t&, int32_t) FORCEINLINE;
static void ROR(THUMBState*, int32_t&, int32_t) FORCEINLINE;
static void TST(THUMBState*, int32_t&, int32_t) FORCEINLINE;
static void NEG(THUMBState*, int32_t&, int32_t) FORCEINLINE;
static void CMP(THUMBState*, int32_t&, int32_t) FORCEINLINE;
static void CMN(THUMBState*, int32_t&, int32_t) FORCEINLINE;
static void ORR(THUMBState*, int32_t&, int32_t) FORCEINLINE;
static void MUL(THUMBState*, int32_t&, int32_t) FORCEINLINE;
static void BIC(THUMBState*, int32_t&, int32_t) FORCEINLINE;
static void MVN(THUMBState*, int32_t&, int32_t) FORCEINLINE;
static void MOV(THUMBState*, int32_t&, int32_t) FORCEINLINE;
static void LSL(THUMBState*, int32_t&, int32_t, int32_t) FORCEINLINE;
static void LSR(THUMBState*, int32_t&, int32_t, int32_t) FORCEINLINE;
static void ASR(THUMBState*, int32_t&, int32_t, int32_t) FORCEINLINE;
static void ADD(THUMBState*, int32_t&, int32_t, int32_t) FORCEINLINE;
static void SUB(THUMBState*, int32_t&, int32_t, int32_t) FORCEINLINE;
static void LDR(Memory*, int32_t&, uint32_t) FORCEINLINE;
static void STR(Memory*, int32_t&, uint32_t) FORCEINLINE;
static void LDRB(Memory*, int32_t&, uint32_t) FORCEINLINE;
static void LDSB(Memory*, int32_t&, uint32_t) FORCEINLINE;
static void STRB(Memory*, int32_t&, uint32_t) FORCEINLINE;
static void LDRH(Memory*, int32_t&, uint32_t) FORCEINLINE;
static void LDSH(Memory*, int32_t&, uint32_t) FORCEINLINE;
static void STRH(Memory*, int32_t&, uint32_t) FORCEINLINE;
static void STMIA(Memory*, int32_t&, uint8_t) FORCEINLINE;
static void LDMIA(Memory*, int32_t&, uint8_t) FORCEINLINE;


// -------------------------------------------------------------------------------------------------
static inline void BCC(THUMBState *t, uint16_t op)
{
  int32_t off;

  assert((op & 0xF000) == 0xD000);
  assert(((op & 0x0F00) != 0x0E00) && ((op & 0x0F00) != 0x0E00));

  // Sign extnext offset & adjust for pipelining
  if ((off = (op & 0xFF)) & 0x80)
  {
    off |= ~0xFF;
  }
  off = (off << 1) + 2;

  // Check condition
  switch ((op >> 8) & 0xF)
  {
    case 0x0: t->pc += (t->z)                  ? off : 0; return;
    case 0x1: t->pc += (!t->z)                 ? off : 0; return;
    case 0x2: t->pc += (t->c)                  ? off : 0; return;
    case 0x3: t->pc += (!t->c)                 ? off : 0; return;
    case 0x4: t->pc += (t->n)                  ? off : 0; return;
    case 0x5: t->pc += (!t->n)                 ? off : 0; return;
    case 0x6: t->pc += (t->v)                  ? off : 0; return;
    case 0x7: t->pc += (!t->v)                 ? off : 0; return;
    case 0x8: t->pc += (t->c && !t->z)         ? off : 0; return;
    case 0x9: t->pc += (!t->c || t->z)         ? off : 0; return;
    case 0xA: t->pc += (t->n == t->v)          ? off : 0; return;
    case 0xB: t->pc += (t->n != t->v)          ? off : 0; return;
    case 0xC: t->pc += (!t->z && t->n == t->v) ? off : 0; return;
    case 0xD: t->pc += (t->z || t->n != t->v)  ? off : 0; return;
    default: /* LCOV_EXCL_START */ __builtin_unreachable();
  }
}

// -------------------------------------------------------------------------------------------------
static inline void BAL(THUMBState *t, uint16_t op)
{
  int32_t off;

  if ((off = (op & 0x7FF)) & 0x80)
  {
    off |= ~0x7FF;
  }

  t->pc += off + 2;
}


// -------------------------------------------------------------------------------------------------
static inline void BLO(THUMBState*, uint16_t)
{

}


// -------------------------------------------------------------------------------------------------
static inline void BLH(THUMBState*, uint16_t)
{

}


// -------------------------------------------------------------------------------------------------
static inline void ADD(THUMBState *t, int32_t &r, int32_t a)
{
  asm volatile
    ( "addl   %[A], %[R]       \n\t"
      "sets   %[N]             \n\t"
      "setz   %[Z]             \n\t"
      "setc   %[C]             \n\t"
      "seto   %[V]             \n\t"
    : [N] "=m" (t->n)
    , [Z] "=m" (t->z)
    , [C] "=m" (t->c)
    , [V] "=m" (t->v)
    , [R] "=r" (r)
    : [A] "g"  (a)
    : "memory", "cc"
    );
}


// -------------------------------------------------------------------------------------------------
static inline void AND(THUMBState *t, int32_t &d, int32_t s)
{
  __builtin_trap();
}


// -------------------------------------------------------------------------------------------------
static inline void EOR(THUMBState *t, int32_t &d, int32_t s)
{
  __builtin_trap();
}


// -------------------------------------------------------------------------------------------------
static inline void LSL(THUMBState *t, int32_t &d, int32_t s)
{
  __builtin_trap();
}


// -------------------------------------------------------------------------------------------------
static inline void LSR(THUMBState *t, int32_t &d, int32_t s)
{
  __builtin_trap();
}


// -------------------------------------------------------------------------------------------------
static inline void ASR(THUMBState *t, int32_t &d, int32_t s)
{
  __builtin_trap();
}


// -------------------------------------------------------------------------------------------------
static inline void ADC(THUMBState *t, int32_t &d, int32_t s)
{
  __builtin_trap();
}


// -------------------------------------------------------------------------------------------------
static inline void SBC(THUMBState *t, int32_t &d, int32_t s)
{
  __builtin_trap();
}


// -------------------------------------------------------------------------------------------------
static inline void ROR(THUMBState *t, int32_t &d, int32_t s)
{
  __builtin_trap();
}


// -------------------------------------------------------------------------------------------------
static inline void TST(THUMBState *t, int32_t &d, int32_t s)
{
  __builtin_trap();
}


// -------------------------------------------------------------------------------------------------
static inline void NEG(THUMBState *t, int32_t &d, int32_t s)
{
  __builtin_trap();
}


// -------------------------------------------------------------------------------------------------
static inline void CMP(THUMBState *t, int32_t &d, int32_t s)
{
  asm volatile
    ( "movl   %[D], %%eax      \n\t"
      "cmpl   %[S], %%eax      \n\t"
      "sets   %[N]             \n\t"
      "setz   %[Z]             \n\t"
      "setc   %[C]             \n\t"
      "seto   %[V]             \n\t"
    : [N] "=m" (t->n)
    , [Z] "=m" (t->z)
    , [C] "=m" (t->c)
    , [V] "=m" (t->v)
    : [D] "g"  (d)
    , [S] "g"  (s)
    : "memory", "cc", "eax"
    );
}


// -------------------------------------------------------------------------------------------------
static inline void CMN(THUMBState *t, int32_t &d, int32_t s)
{
  asm volatile
    ( "movl   %[D], %%eax      \n\t"
      "addl   %[S], %%eax      \n\t"
      "sets   %[N]             \n\t"
      "setz   %[Z]             \n\t"
      "setc   %[C]             \n\t"
      "seto   %[V]             \n\t"
    : [N] "=m" (t->n)
    , [Z] "=m" (t->z)
    , [C] "=m" (t->c)
    , [V] "=m" (t->v)
    : [D] "g"  (d)
    , [S] "g"  (s)
    : "memory", "cc", "eax"
    );
}


// -------------------------------------------------------------------------------------------------
static inline void ORR(THUMBState *t, int32_t &d, int32_t s)
{
  __builtin_trap();
}


// -------------------------------------------------------------------------------------------------
static inline void MUL(THUMBState *t, int32_t &d, int32_t s)
{
  __builtin_trap();
}


// -------------------------------------------------------------------------------------------------
static inline void BIC(THUMBState *t, int32_t &d, int32_t s)
{
  __builtin_trap();
}


// -------------------------------------------------------------------------------------------------
static inline void MVN(THUMBState *t, int32_t &d, int32_t s)
{
  asm volatile
    ( "movl   %[S], %[D]       \n\t"
      "notl   %[D]             \n\t"
      "test   %[D], %[D]       \n\t"
      "sets   %[N]             \n\t"
      "setz   %[Z]             \n\t"
    : [N] "=g" (t->n)
    , [Z] "=g" (t->z)
    , [D] "=r" (d)
    : [S] "r"  (s)
    : "memory", "cc"
    );
}


// -------------------------------------------------------------------------------------------------
static inline void MOV(THUMBState *t, int32_t &d, int32_t s)
{
  asm volatile
    ( "movl   %[S], %[D]       \n\t"
      "test   %[S], %[S]       \n\t"
      "sets   %[N]             \n\t"
      "setz   %[Z]             \n\t"
    : [N] "=g" (t->n)
    , [Z] "=g" (t->z)
    , [D] "=r" (d)
    : [S] "r"  (s)
    : "memory", "cc"
    );
}


// -------------------------------------------------------------------------------------------------
static inline void LSL(THUMBState *t, int32_t &r, int32_t a, int32_t b)
{
  asm volatile
    ( "movl   %[A], %%eax      \n\t"
      "shll   %%cl, %%eax      \n\t"
      "movl   %%eax, %[R]      \n\t"
      "sets   %[N]             \n\t"
      "setz   %[Z]             \n\t"
      "setc   %[C]             \n\t"
    : [N] "=g" (t->n)
    , [Z] "=g" (t->z)
    , [C] "=g" (t->c)
    , [R] "=g" (r)
    : [A] "g"  (a)
    , [B] "c"  (b)
    : "memory", "cc", "eax"
    );
}


// -------------------------------------------------------------------------------------------------
static inline void LSR(THUMBState *t, int32_t &r, int32_t a, int32_t b)
{
  asm volatile
    ( "movl   %[A], %%eax      \n\t"
      "shrl   %%cl, %%eax      \n\t"
      "movl   %%eax, %[R]      \n\t"
      "sets   %[N]             \n\t"
      "setz   %[Z]             \n\t"
      "setc   %[C]             \n\t"
    : [N] "=g" (t->n)
    , [Z] "=g" (t->z)
    , [C] "=g" (t->c)
    , [R] "=g" (r)
    : [A] "g"  (a)
    , [B] "c"  (b)
    : "memory", "cc", "eax"
    );
}


// -------------------------------------------------------------------------------------------------
static inline void ASR(THUMBState *t, int32_t &r, int32_t a, int32_t b)
{
  asm volatile
    ( "movl   %[A], %%eax      \n\t"
      "sarl   %%cl, %%eax      \n\t"
      "movl   %%eax, %[R]      \n\t"
      "sets   %[N]             \n\t"
      "setz   %[Z]             \n\t"
      "setc   %[C]             \n\t"
    : [N] "=g" (t->n)
    , [Z] "=g" (t->z)
    , [C] "=g" (t->c)
    , [R] "=g" (r)
    : [A] "g"  (a)
    , [B] "c"  (b)
    : "memory", "cc", "eax"
    );
}


// -------------------------------------------------------------------------------------------------
static inline void ADD(THUMBState *t, int32_t &r, int32_t a, int32_t b)
{
  asm volatile
    ( "movl   %[B], %%eax      \n\t"
      "addl   %[A], %%eax      \n\t"
      "movl   %%eax, %[R]      \n\t"
      "sets   %[N]             \n\t"
      "setz   %[Z]             \n\t"
      "setc   %[C]             \n\t"
      "seto   %[V]             \n\t"
    : [N] "=m" (t->n)
    , [Z] "=m" (t->z)
    , [C] "=m" (t->c)
    , [V] "=m" (t->v)
    , [R] "=m" (r)
    : [A] "g"  (a)
    , [B] "g"  (b)
    : "memory", "cc", "eax"
    );
}


// -------------------------------------------------------------------------------------------------
static inline void SUB(THUMBState *t, int32_t &r, int32_t a, int32_t b)
{
  asm volatile
    ( "movl   %[B], %%eax      \n\t"
      "subl   %[A], %%eax      \n\t"
      "movl   %%eax, %[R]      \n\t"
      "sets   %[N]             \n\t"
      "setz   %[Z]             \n\t"
      "setc   %[C]             \n\t"
      "seto   %[V]             \n\t"
    : [N] "=m" (t->n)
    , [Z] "=m" (t->z)
    , [C] "=m" (t->c)
    , [V] "=m" (t->v)
    , [R] "=m" (r)
    : [A] "g"  (a)
    , [B] "g"  (b)
    : "memory", "cc", "eax"
    );
}


// -------------------------------------------------------------------------------------------------
static inline void LDR(Memory *m, int32_t&, uint32_t addr)
{
  std::cerr << std::hex << std::endl << addr << " " << m->GetLong(addr) << std::endl;
}


// -------------------------------------------------------------------------------------------------
static inline void STR(Memory*, int32_t&, uint32_t)
{
  std::cerr << "STR" << std::endl;
}


// -------------------------------------------------------------------------------------------------
static inline void LDRB(Memory*, int32_t&, uint32_t)
{
  std::cerr << "LDRB" << std::endl;
}


// -------------------------------------------------------------------------------------------------
static inline void STRB(Memory*, int32_t&, uint32_t)
{
  std::cerr << "STRB" << std::endl;
}


// -------------------------------------------------------------------------------------------------
static inline void LDRH(Memory*, int32_t&, uint32_t)
{
  std::cerr << "LDRH" << std::endl;
}


// -------------------------------------------------------------------------------------------------
static inline void STRH(Memory*, int32_t&, uint32_t)
{
  std::cerr << "STRH" << std::endl;
}


// -------------------------------------------------------------------------------------------------
static inline void LDSB(Memory*, int32_t&, uint32_t)
{
  std::cerr << "LDSB" << std::endl;
}


// -------------------------------------------------------------------------------------------------
static inline void LDSH(Memory*, int32_t&, uint32_t)
{
  std::cerr << "LDSH" << std::endl;
}


// -------------------------------------------------------------------------------------------------
static inline void STMIA(Memory*, int32_t&, uint8_t)
{
  std::cerr << "STMIA" << std::endl;
}


// -------------------------------------------------------------------------------------------------
static inline void LDMIA(Memory*, int32_t&, uint8_t)
{
  std::cerr << "LDMIA" << std::endl;
}


// -------------------------------------------------------------------------------------------------
void ThumbExecute(Emulator *emu)
{
  register uint32_t flags;
  register uint16_t op = 1;
  register Memory *m;
  register int32_t *r;
  register int hilo;
  THUMBState *t;
  int32_t off;

  // Cache some pointers
  t = &emu->thumbState;
  r = t->r;
  m = &emu->mem;

  // Initial PC adjustment
  hilo = (t->pc >> 1) & 1;
  t->pc += hilo ? 2 : 0;

  while (1)
  {
    // If instruction is in ITT block, it does not affect flags.
    // In order to avoid affecting flags, we save the flags at
    // the start of every cycle and restore them afterwards
    if (t->itt)
    {
      t->flags = flags;
    }
    flags = t->flags;


    // Ajust PC for the pipelining effect. If the address is not
    // a multiple of 4, PC must point to the next 4 byte aligned
    // address, otherwise PC is advanced by 4 relative to the
    // current address
    if ((hilo = !hilo))
    {
      op = emu->mem.GetInstrWord(t->pc);
      t->pc += 4;
    }
    else
    {
      op = emu->mem.GetInstrWord(t->pc - 2);
    }


    // Decode the instructions. The most significant 7 bits should
    // be enough to distinguish most instrucitions, but additional
    // switch statements are required for some categories such as
    // ALU operations and miscellaneous instructions
    switch (op >> 9)
    {
      case 0x00 ... 0x03: LSL  (t, r[op & 7], r[(op >> 3) & 7], (op >> 6) & 31);           continue;
      case 0x04 ... 0x07: LSR  (t, r[op & 7], r[(op >> 3) & 7], (op >> 6) & 31);           continue;
      case 0x08 ... 0x0B: ASR  (t, r[op & 7], r[(op >> 3) & 7], (op >> 6) & 31);           continue;
      case 0x28:          STR  (m, r[op & 7], r[(op >> 3) & 7] + r[(op >> 6) & 7]);        continue;
      case 0x29:          STRH (m, r[op & 7], r[(op >> 3) & 7] + r[(op >> 6) & 7]);        continue;
      case 0x2A:          STRB (m, r[op & 7], r[(op >> 3) & 7] + r[(op >> 6) & 7]);        continue;
      case 0x2B:          LDRH (m, r[op & 7], r[(op >> 3) & 7] + r[(op >> 6) & 7]);        continue;
      case 0x2C:          LDR  (m, r[op & 7], r[(op >> 3) & 7] + r[(op >> 6) & 7]);        continue;
      case 0x2D:          LDSB (m, r[op & 7], r[(op >> 3) & 7] + r[(op >> 6) & 7]);        continue;
      case 0x2E:          LDRB (m, r[op & 7], r[(op >> 3) & 7] + r[(op >> 6) & 7]);        continue;
      case 0x2F:          LDSH (m, r[op & 7], r[(op >> 3) & 7] + r[(op >> 6) & 7]);        continue;
      case 0x30 ... 0x33: STR  (m, r[op & 7], r[(op >> 3) & 7] + ((op >> 6 << 2) & 0x7F)); continue;
      case 0x34 ... 0x37: LDR  (m, r[op & 7], r[(op >> 3) & 7] + ((op >> 6 << 2) & 0x7F)); continue;
      case 0x38 ... 0x3B: STRB (m, r[op & 7], r[(op >> 3) & 7] + ((op >> 6) & 0x1F));      continue;
      case 0x3C ... 0x3F: LDRB (m, r[op & 7], r[(op >> 3) & 7] + ((op >> 6) & 0x1F));      continue;
      case 0x40 ... 0x43: STRH (m, r[op & 7], r[(op >> 3) & 7] + ((op >> 6 << 1) & 0x3F)); continue;
      case 0x44 ... 0x47: LDRH (m, r[op & 7], r[(op >> 3) & 7] + ((op >> 6 << 1) & 0x3F)); continue;
      case 0x0C:          ADD  (t, r[op & 7], r[(op >> 6) & 7], r[(op >> 3) & 7]);         continue;
      case 0x0D:          SUB  (t, r[op & 7], r[(op >> 6) & 7], r[(op >> 3) & 7]);         continue;
      case 0x0E:          ADD  (t, r[op & 7], (op >> 6) & 7, r[(op >> 3) & 7]);            continue;
      case 0x0F:          SUB  (t, r[op & 7], (op >> 6) & 7, r[(op >> 3) & 7]);            continue;
      case 0x24 ... 0x27: LDR  (m, r[op & 7], t->pc + ((op & 0xFF) << 2));                 continue;
      case 0x10 ... 0x13: MOV  (t, r[(op >> 8) & 7], op & 0xFF);                           continue;
      case 0x14 ... 0x17: CMP  (t, r[(op >> 8) & 7], op & 0xFF);                           continue;
      case 0x18 ... 0x1B: ADD  (t, r[(op >> 8) & 7], op & 0xFF, r[(op >> 8) & 7]);         continue;
      case 0x1C ... 0x1F: SUB  (t, r[(op >> 8) & 7], op & 0xFF, r[(op >> 8) & 7]);         continue;
      case 0x48 ... 0x4B: STR  (m, r[(op >> 8) & 7], t->sp + ((op & 0xFF) << 2));          continue;
      case 0x4C ... 0x4F: LDR  (m, r[(op >> 8) & 7], t->sp + ((op & 0xFF) << 2));          continue;
      case 0x60 ... 0x63: STMIA(m, r[(op >> 8) & 7], op & 0xFF);                           continue;
      case 0x64 ... 0x67: LDMIA(m, r[(op >> 8) & 7], op & 0xFF);                           continue;
      case 0x68 ... 0x6E: BCC  (t, op);                                                    continue;
      case 0x70 ... 0x73: BAL  (t, op);                                                    continue;
      case 0x78 ... 0x7B: BLO  (t, op >> 9);                                               continue;
      case 0x7C ... 0x7F: BLH  (t, op >> 9);                                               continue;
      case 0x50 ... 0x53: r[(op >> 8) & 3] = t->pc + ((op & 0xFF) << 2);                   continue;
      case 0x54 ... 0x57: r[(op >> 8) & 3] = t->sp + ((op & 0xFF) << 2);                   continue;
      case 0x22 ... 0x23:
        switch ((op >> 6) & 0xF)
        {
          case 0x1: ADD(t, r[0 + (op & 7)], r[8 + ((op >> 3) & 7)]); continue;
          case 0x2: ADD(t, r[8 + (op & 7)], r[0 + ((op >> 3) & 7)]); continue;
          case 0x3: ADD(t, r[8 + (op & 7)], r[8 + ((op >> 3) & 7)]); continue;
          case 0x5: CMP(t, r[0 + (op & 7)], r[8 + ((op >> 3) & 7)]); continue;
          case 0x6: CMP(t, r[8 + (op & 7)], r[0 + ((op >> 3) & 7)]); continue;
          case 0x7: CMP(t, r[8 + (op & 7)], r[8 + ((op >> 3) & 7)]); continue;
          case 0x9: MOV(t, r[0 + (op & 7)], r[8 + ((op >> 3) & 7)]); continue;
          case 0xA: MOV(t, r[8 + (op & 7)], r[0 + ((op >> 3) & 7)]); continue;
          case 0xB: MOV(t, r[8 + (op & 7)], r[8 + ((op >> 3) & 7)]); continue;
          case 0xC: // Bx
          case 0xD: // Bx
          default: goto und;
        }
      case 0x20 ... 0x21:
        switch ((op >> 6) & 0xF)
        {
          case 0x0: AND(t, r[op & 7], r[(op >> 3) & 7]); continue;
          case 0x1: EOR(t, r[op & 7], r[(op >> 3) & 7]); continue;
          case 0x2: LSL(t, r[op & 7], r[(op >> 3) & 7]); continue;
          case 0x3: LSR(t, r[op & 7], r[(op >> 3) & 7]); continue;
          case 0x4: ASR(t, r[op & 7], r[(op >> 3) & 7]); continue;
          case 0x5: ADC(t, r[op & 7], r[(op >> 3) & 7]); continue;
          case 0x6: SBC(t, r[op & 7], r[(op >> 3) & 7]); continue;
          case 0x7: ROR(t, r[op & 7], r[(op >> 3) & 7]); continue;
          case 0x8: TST(t, r[op & 7], r[(op >> 3) & 7]); continue;
          case 0x9: NEG(t, r[op & 7], r[(op >> 3) & 7]); continue;
          case 0xA: CMP(t, r[op & 7], r[(op >> 3) & 7]); continue;
          case 0xB: CMN(t, r[op & 7], r[(op >> 3) & 7]); continue;
          case 0xC: ORR(t, r[op & 7], r[(op >> 3) & 7]); continue;
          case 0xD: MUL(t, r[op & 7], r[(op >> 3) & 7]); continue;
          case 0xE: BIC(t, r[op & 7], r[(op >> 3) & 7]); continue;
          case 0xF: MVN(t, r[op & 7], r[(op >> 3) & 7]); continue;
          default: /* LCOV_EXCL_START */ __builtin_unreachable();
        }
      case 0x58 ... 0x5F:
        switch ((op >> 5) & 0x7F)
        {
          case 0x00 ... 0x03: /*ADD SP, SP, #Imm7*/ continue;
          case 0x04 ... 0x07: /*SUB SP, SP, #Imm7*/ continue;
          case 0x10 ... 0x11: /*SXTH*/              continue;
          case 0x12 ... 0x13: /*SXTB*/              continue;
          case 0x14 ... 0x15: /*UXTH*/              continue;
          case 0x16 ... 0x17: /*UXTB*/              continue;
          case 0x20 ... 0x2F: /*PUSH*/              continue;
          case 0x33:          /*CPS*/               continue;
          case 0x50 ... 0x51: /*REV*/               continue;
          case 0x52 ... 0x53: /*REV16*/             continue;
          case 0x54 ... 0x55: /*REVSH*/             continue;
          case 0x60 ... 0x6F: /*POP*/               continue;
          case 0x70 ... 0x77: /*BKPT*/              continue;
          case 0x78 ... 0x7F:
          {
            if (op & 0xF)
            {
              // IT
              continue;
            }

            switch ((op >> 4) & 0xF)
            {
              case 0x0: /*NOP*/   continue;
              case 0x1: /*YIELD*/ continue;
              case 0x2: /*WFE*/   continue;
              case 0x3: /*WFI*/   continue;
              case 0x4: /*SEV*/   continue;
            }
          }
          default: /* UND */ continue;
        }
      case 0x74 ... 0x77: /*THUMB2*/ __builtin_trap(); continue;
      case 0x6F:
        if ((op >> 8) & 0x1)
        {
          // SWI - quit for now
          goto swi;
        }
        else
        {
          // UND
          goto und;
        }
    }
  }

  // Swap to ARM
  bx:
  {
    return;
  }

  // Software interrupt
  swi:
  {
    return;
  }

  // Undefined interrupt
  und:
  {
    return;
  }
}
