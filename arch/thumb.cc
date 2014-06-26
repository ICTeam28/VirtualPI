// This file is part of the PiEMU Project
// Licensing information can be found in the LICENSE file
// (C) 2014 Nandor Licker. All rights reserved.
#include "common.h"


// -----------------------------------------------------------------------------
static void Branch(THUMBState *, uint16_t) FORCEINLINE;
static void LSL(THUMBState *, int32_t &, int32_t, int32_t) FORCEINLINE;
static void LSR(THUMBState *, int32_t &, int32_t, int32_t) FORCEINLINE;
static void ASR(THUMBState *, int32_t &, int32_t, int32_t) FORCEINLINE;
static void ADD(THUMBState *, int32_t &, int32_t, int32_t) FORCEINLINE;
static void SUB(THUMBState *, int32_t &, int32_t, int32_t) FORCEINLINE;
static void MOV(THUMBState *, int32_t &, int32_t) FORCEINLINE;
static void CMP(THUMBState *, int32_t, int32_t) FORCEINLINE;


// -----------------------------------------------------------------------------
static inline void Branch(THUMBState *t, uint16_t op)
{
  int32_t off;

  assert((op & 0xF000) == 0xD000);
  assert(((op & 0x0F00) != 0x0E00) && ((op & 0x0F00) != 0x0E00));

  // Sign extend offset & adjust for pipelining
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
    default:
    {
      __builtin_unreachable();
    }
  }
}


// -----------------------------------------------------------------------------
static inline void LSL(THUMBState *t, int32_t &r, int32_t a, int32_t b)
{
  asm volatile
    ( "movl   %[A], %%eax      \n\t"
      "test   %[I], %[I]       \n\t"
      "jnz    1f               \n\t"
      "shll   %%cl, %%eax      \n\t"
      "movl   %%eax, %[R]      \n\t"
      "sets   %[N]             \n\t"
      "setz   %[Z]             \n\t"
      "setc   %[C]             \n\t"
      "jmp    2f               \n\t"
      "1:                      \n\t"
      "shll   %%cl, %%eax      \n\t"
      "movl   %%eax, %[R]      \n\t"
      "2:                      \n\t"
    : [N] "=m" (t->n)
    , [Z] "=m" (t->z)
    , [C] "+m" (t->c)
    , [V] "=m" (t->v)
    , [R] "=m" (r)
    : [A] "g"  (a)
    , [B] "c"  (b)
    , [I] "q"  (t->itt)
    : "memory", "cc", "eax"
    );
}


// -----------------------------------------------------------------------------
static inline void LSR(THUMBState *t, int32_t &r, int32_t a, int32_t b)
{
  asm volatile
    ( "movl   %[A], %%eax      \n\t"
      "test   %[I], %[I]       \n\t"
      "jnz    1f               \n\t"
      "shrl   %%cl, %%eax      \n\t"
      "movl   %%eax, %[R]      \n\t"
      "sets   %[N]             \n\t"
      "setz   %[Z]             \n\t"
      "setc   %[C]             \n\t"
      "jmp    2f               \n\t"
      "1:                      \n\t"
      "shrl   %%cl, %%eax      \n\t"
      "movl   %%eax, %[R]      \n\t"
      "2:                      \n\t"
    : [N] "=m" (t->n)
    , [Z] "=m" (t->z)
    , [C] "+m" (t->c)
    , [V] "=m" (t->v)
    , [R] "=m" (r)
    : [A] "g"  (a)
    , [B] "c"  (b)
    , [I] "q"  (t->itt)
    : "memory", "cc", "eax"
    );
}


// -----------------------------------------------------------------------------
static inline void ASR(THUMBState *t, int32_t &r, int32_t a, int32_t b)
{
  asm volatile
    ( "movl   %[A], %%eax      \n\t"
      "test   %[I], %[I]       \n\t"
      "jnz    1f               \n\t"
      "sarl   %%cl, %%eax      \n\t"
      "movl   %%eax, %[R]      \n\t"
      "sets   %[N]             \n\t"
      "setz   %[Z]             \n\t"
      "setc   %[C]             \n\t"
      "jmp    2f               \n\t"
      "1:                      \n\t"
      "sarl   %%cl, %%eax      \n\t"
      "movl   %%eax, %[R]      \n\t"
      "2:                      \n\t"
    : [N] "=m" (t->n)
    , [Z] "=m" (t->z)
    , [C] "+m" (t->c)
    , [V] "=m" (t->v)
    , [R] "=m" (r)
    : [A] "g"  (a)
    , [B] "c"  (b)
    , [I] "q"  (t->itt)
    : "memory", "cc", "eax"
    );
}


// -----------------------------------------------------------------------------
static inline void ADD(THUMBState *t, int32_t &r, int32_t a, int32_t b)
{
  asm volatile
    ( "test   %[I], %[I]       \n\t"
      "jnz    1f               \n\t"
      "movl   %[B], %%eax      \n\t"
      "addl   %[A], %%eax      \n\t"
      "movl   %%eax, %[R]      \n\t"
      "sets   %[N]             \n\t"
      "setz   %[Z]             \n\t"
      "setc   %[C]             \n\t"
      "seto   %[V]             \n\t"
      "jmp    2f               \n\t"
      "1:                      \n\t"
      "movl   %[B], %%eax      \n\t"
      "addl   %[A], %%eax      \n\t"
      "movl   %%eax, %[R]      \n\t"
      "2:                      \n\t"
    : [N] "=m" (t->n)
    , [Z] "=m" (t->z)
    , [C] "=m" (t->c)
    , [V] "=m" (t->v)
    , [R] "=m" (r)
    : [A] "g"  (a)
    , [B] "g"  (b)
    , [I] "q"  (t->itt)
    : "memory", "cc", "eax"
    );
}


// -----------------------------------------------------------------------------
static inline void SUB(THUMBState *t, int32_t &r, int32_t a, int32_t b)
{
  asm volatile
    ( "test   %[I], %[I]       \n\t"
      "jnz    1f               \n\t"
      "movl   %[B], %%eax      \n\t"
      "subl   %[A], %%eax      \n\t"
      "movl   %%eax, %[R]      \n\t"
      "sets   %[N]             \n\t"
      "setz   %[Z]             \n\t"
      "setc   %[C]             \n\t"
      "seto   %[V]             \n\t"
      "jmp    2f               \n\t"
      "1:                      \n\t"
      "movl   %[B], %%eax      \n\t"
      "subl   %[A], %%eax      \n\t"
      "movl   %%eax, %[R]      \n\t"
      "2:                      \n\t"
    : [N] "=m" (t->n)
    , [Z] "=m" (t->z)
    , [C] "=m" (t->c)
    , [V] "=m" (t->v)
    , [R] "=m" (r)
    : [A] "g"  (a)
    , [B] "g"  (b)
    , [I] "q"  (t->itt)
    : "memory", "cc", "eax"
    );
}


// -----------------------------------------------------------------------------
static inline void MOV(THUMBState *t, int32_t &r, int32_t a)
{
  asm volatile
    ( "movzbl %%al, %%eax      \n\t"
      "movl   %%eax, %[R]      \n\t"
      "test   %[I], %[I]       \n\t"
      "jnz    1f               \n\t"
      "test   %%eax, %%eax     \n\t"
      "sets   %[N]             \n\t"
      "setz   %[Z]             \n\t"
      "1:                      \n\t"
    : [N] "=m" (t->n)
    , [Z] "=m" (t->z)
    , [R] "=m" (r)
    : [O] "a"  (a)
    , [I] "q"  (t->itt)
    : "memory", "cc"
    );
}


// -----------------------------------------------------------------------------
static inline void CMP(THUMBState *t, int32_t a, int32_t b)
{
  asm volatile
    ( "test   %[I], %[I]       \n\t"
      "jnz    1f               \n\t"
      "movl   %[B], %%eax      \n\t"
      "cmpl   %[A], %%eax      \n\t"
      "sets   %[N]             \n\t"
      "setz   %[Z]             \n\t"
      "setc   %[C]             \n\t"
      "seto   %[V]             \n\t"
      "1:"
    : [N] "=m" (t->n)
    , [Z] "=m" (t->z)
    , [C] "=m" (t->c)
    , [V] "=m" (t->v)
    : [A] "g"  (a)
    , [B] "g"  (b)
    , [I] "q"  (t->itt)
    : "memory", "cc", "eax"
    );
}


// -----------------------------------------------------------------------------
void ThumbExecute(Emulator *emu)
{
  register uint16_t op = 1;
  register THUMBState *t;

  void *jmp[] =
  {
    &&op_00, &&op_01, &&op_02, &&op_03, &&op_04, &&op_05, &&op_06, &&op_07,
    &&op_08, &&op_09, &&op_0A, &&op_0B, &&op_0C, &&op_0D, &&op_0E, &&op_0F,
    &&op_10, &&op_11, &&op_12, &&op_13, &&op_14, &&op_15, &&op_16, &&op_17,
    &&op_18, &&op_19, &&op_1A, &&op_1B, &&op_1C, &&op_1D, &&op_1E, &&op_1F,
    &&op_20, &&op_21, &&op_22, &&op_23, &&op_24, &&op_25, &&op_26, &&op_27,
    &&op_28, &&op_29, &&op_2A, &&op_2B, &&op_2C, &&op_2D, &&op_2E, &&op_2F,
    &&op_30, &&op_31, &&op_32, &&op_33, &&op_34, &&op_35, &&op_36, &&op_37,
    &&op_38, &&op_39, &&op_3A, &&op_3B, &&op_3C, &&op_3D, &&op_3E, &&op_3F,
    &&op_40, &&op_41, &&op_42, &&op_43, &&op_44, &&op_45, &&op_46, &&op_47,
    &&op_48, &&op_49, &&op_4A, &&op_4B, &&op_4C, &&op_4D, &&op_4E, &&op_4F,
    &&op_50, &&op_51, &&op_52, &&op_53, &&op_54, &&op_55, &&op_56, &&op_57,
    &&op_58, &&op_59, &&op_5A, &&op_5B, &&op_5C, &&op_5D, &&op_5E, &&op_5F,
    &&op_60, &&op_61, &&op_62, &&op_63, &&op_64, &&op_65, &&op_66, &&op_67,
    &&op_68, &&op_69, &&op_6A, &&op_6B, &&op_6C, &&op_6D, &&op_6E, &&op_6F,
    &&op_70, &&op_71, &&op_72, &&op_73, &&op_74, &&op_75, &&op_76, &&op_77,
    &&op_78, &&op_79, &&op_7A, &&op_7B, &&op_7C, &&op_7D, &&op_7E, &&op_7F,
  };

  t = &emu->thumbState;

  end:
  {
    // Decodes the instruction & jumps to handler
    op = emu->mem.GetInstrWord(t->pc);
    t->pc += 2;
    goto *jmp[op >> 9];
  }

  op_00: op_01: op_02: op_03: // LSL Rd, Rs, #Offset5
    LSL(t, t->r[(op >> 0) & 7], t->r[(op >> 3) & 7], (op >> 6) & 0x1F);
    goto end;

  op_04: op_05: op_06: op_07: // LSR Rd, Rs, #Offset5
    LSR(t, t->r[(op >> 0) & 7], t->r[(op >> 3) & 7], (op >> 6) & 0x1F);
    goto end;

  op_08: op_09: op_0A: op_0B: // ASR Rd, Rs, #Offset5
    ASR(t, t->r[(op >> 0) & 7], t->r[(op >> 3) & 7], (op >> 6) & 0x1F);
    goto end;

  op_0C: // ADD Rr, Rb, Ra
    ADD(t, t->r[(op >> 0) & 7], t->r[(op >> 6) & 7], t->r[(op >> 3) & 7]);
    goto end;

  op_0D: // SUB Rr, Rb, Ra
    SUB(t, t->r[(op >> 0) & 7], t->r[(op >> 6) & 7], t->r[(op >> 3) & 7]);
    goto end;

  op_0E: // ADD Rr, Rb, #Offset3
    ADD(t, t->r[(op >> 0) & 7], (op >> 6) & 7, t->r[(op >> 3) & 7]);
    goto end;

  op_0F: // SUB Rr, Rb, #Offset3
    SUB(t, t->r[(op >> 0) & 7], (op >> 6) & 7, t->r[(op >> 3) & 7]);
    goto end;

  op_10: op_11: op_12: op_13: // MOV Rr, #Offset8
    MOV(t, t->r[(op >> 8) & 7], op & 0xFF);
    goto end;

  op_14: op_15: op_16: op_17: // CMP Rd, #Offset8
    CMP(t, t->r[(op >> 8) & 7], op & 0xFF);
    goto end;

  op_18: op_19: op_1A: op_1B: // ADD Rd, #Offset8
    ADD(t, t->r[(op >> 0) & 7], (op >> 6) & 7, t->r[(op >> 3) & 7]);
    goto end;

  op_1C: op_1D: op_1E: op_1F: // SUB Rd, #Offset8
    SUB(t, t->r[(op >> 8) & 7], op & 0xFF, t->r[(op >> 8) & 7]);
    goto end;

  op_20:
  {
    switch ((op >> 6) & 7)
    {
      // AND Rd, Rs
      case 0:
      {
        goto end;
      }
      // EOR Rd, Rs
      case 1:
      {
        goto end;
      }
      // LSL Rd, Rs
      case 2:
      {
        goto end;
      }
      // LSR Rd, Rs
      case 3:
      {
        goto end;
      }
      // ASR Rd, Rs
      case 4:
      {
        goto end;
      }
      // ADC Rd, Rs
      case 5:
      {
        goto end;
      }
      // SBC Rd, Rs
      case 6:
      {
        goto end;
      }
      // ROR Rd, Rs
      case 7:
      {
        goto end;
      }
      default:
      {
        /* LCOV_EXCL_LINE */
        __builtin_unreachable();
      }
    }
  }
  op_21:
  {
    switch ((op >> 6) & 7)
    {
      // TST Rd, Rs
      case 0:
      {
        goto end;
      }
      // NEG Rd, Rs
      case 1:
      {
        goto end;
      }
      // CMP Rd, Rs
      case 2:
      {
        goto end;
      }
      // CMN Rd, Rs
      case 3:
      {
        goto end;
      }
      // ORR Rd, Rs
      case 4:
      {
        goto end;
      }
      // MUL Rd, Rs
      case 5:
      {
        goto end;
      }
      // BIC Rd, Rs
      case 6:
      {
        goto end;
      }
      // MVN Rd, Rs
      case 7:
      {
        goto end;
      }
      default:
      {
        /* LCOV_EXCL_LINE */
        __builtin_unreachable();
      }
    }
  }

  op_22: op_23:
  {
    fprintf(stderr, "%02x\n", op >> 9);
    goto end;
  }

  // LDR Rd, [PC, #Imm]
  op_24: op_25: op_26: op_27:
  {
    fprintf(stderr, "%02x\n", op >> 9);
    goto end;
  }

  // STR Rd, [Rb, Ro]
  op_28:
  {
    fprintf(stderr, "%02x\n", op >> 9);
    goto end;
  }

  // STRH Rd, [Rb, Ro]
  op_29:
  {
    fprintf(stderr, "%02x\n", op >> 9);
    goto end;
  }

  // STRB Rd, [Rb, Ro]
  op_2A:
  {
    fprintf(stderr, "%02x\n", op >> 9);
    goto end;
  }

  // LDRH Rd, [Rb, Ro]
  op_2B:
  {
    fprintf(stderr, "%02x\n", op >> 9);
    goto end;
  }

  // LDR Rd, [Rb, Ro] L
  op_2C:
  {
    fprintf(stderr, "%02x\n", op >> 9);
    goto end;
  }

  // LDSB Rd, [Rb, Ro]
  op_2D:
  {
    fprintf(stderr, "%02x\n", op >> 9);
    goto end;
  }

  // LDRB Rd, [Rb, Ro]
  op_2E:
  {
    fprintf(stderr, "%02x\n", op >> 9);
    goto end;
  }

  // LDSH Rd, [Rb, Ro]
  op_2F:
  {
    fprintf(stderr, "%02x\n", op >> 9);
    goto end;
  }

  // STR Rd, [Rb, #Imm]
  op_30: op_31: op_32: op_33:
  {
    fprintf(stderr, "%02x\n", op >> 9);
    goto end;
  }

  // LDR Rd, [Rb, #Imm]
  op_34: op_35: op_36: op_37:
  {
    fprintf(stderr, "%02x\n", op >> 9);
    goto end;
  }

  // STRB Rd, [Rb, #Imm]
  op_38: op_39: op_3A: op_3B:
  {
    fprintf(stderr, "%02x\n", op >> 9);
    goto end;
  }

  // LDRB Rd, [Rb, #Imm]
  op_3C: op_3D: op_3E: op_3F:
  {
    fprintf(stderr, "%02x\n", op >> 9);
    goto end;
  }

  // STRH Rd, [Rb, #Imm]
  op_40: op_41: op_42: op_43:
  {
    fprintf(stderr, "%02x\n", op >> 9);
    goto end;
  }

  // LDRH Rd, [Rb, #Imm]
  op_44: op_45: op_46: op_47:
  {
    fprintf(stderr, "%02x\n", op >> 9);
    goto end;
  }

  // STR Rd, [SP, #Imm]
  op_48: op_49: op_4A: op_4B:
  {
    fprintf(stderr, "%02x\n", op >> 9);
    goto end;
  }

  // LDR Rd, [SP, #Imm]
  op_4C: op_4D: op_4E: op_4F:
  {
    fprintf(stderr, "%02x\n", op >> 9);
    goto end;
  }

  op_50: op_51: op_52: op_53:
  {
    // ADD Rd, PC, #Imm8
    ADD(t, t->r[(op >> 8) & 3], t->pc, (op & 0xFF) << 2);
    goto end;
  }
  op_54: op_55: op_56: op_57:
  {
    // ADD Rd, SP, #Imm8
    ADD(t, t->r[(op >> 8) & 3], t->sp, (op & 0xFF) << 2);
    goto end;
  }

  op_58: op_59: op_5A: op_5B: op_5C: op_5D: op_5E: op_5F:
  {
    // Miscellaneous
    switch ((op >> 5) & 0x7F)
    {
      case 0x00 ... 0x03:
      {
        // ADD SP, SP, #Imm7
        goto end;
      }
      case 0x04 ... 0x07:
      {
        // SUB SP, SP, #Imm7
        goto end;
      }
      case 0x10 ... 0x11:
      {
        // SXTH
        goto end;
      }
      case 0x12 ... 0x13:
      {
        // SXTB
        goto end;
      }
      case 0x14 ... 0x15:
      {
        // UXTH
        goto end;
      }
      case 0x16 ... 0x17:
      {
        // UXTB
        goto end;
      }
      case 0x20 ... 0x2F:
      {
        // PUSH
        goto end;
      }
      case 0x33:
      {
        // CPS
        goto end;
      }
      case 0x50 ... 0x51:
      {
        // REV
        goto end;
      }
      case 0x52 ... 0x53:
      {
        // REV16
        goto end;
      }
      case 0x54 ... 0x55:
      {
        // REVSH
        goto end;
      }
      case 0x60 ... 0x6F:
      {
        // POP
        goto end;
      }
      case 0x70 ... 0x77:
      {
        // BKPT
        goto end;
      }
      case 0x78 ... 0x7F:
      {
        if (op & 0xF)
        {
          // IT
          goto end;
        }

        switch ((op >> 4) & 0xF)
        {
          case 0x0:
          {
            // NOP
            goto end;
          }
          case 0x1:
          {
            // YIELD
            goto end;
          }
          case 0x2:
          {
            // WFE
            goto end;
          }
          case 0x3:
          {
            // WFI
            goto end;
          }
          case 0x4:
          {
            // SEV
            goto end;
          }
        }
      }
      default:
      {
        // TODO
        __builtin_trap();
      }
    }
  }

  // STMIA Rb!, { Rlist }
  op_60: op_61: op_62: op_63:
  {
    fprintf(stderr, "%02x\n", op >> 9);
    goto end;
  }

  // LDMIA Rb!, { Rlist }
  op_64: op_65: op_66: op_67:
  {
    fprintf(stderr, "%02x\n", op >> 9);
    goto end;
  }

  op_68: op_69: op_6A: op_6B: op_6C: op_6D: op_6E:
  {
    // Bcc label
    Branch(t, op);
    goto end;
  }

  op_6F:
  {
    if ((op >> 8) & 0x1)
    {
      // SWI - quit for now
      return;
      goto end;
    }
    else
    {
      // UND
      goto end;
    }
  }

  // B label
  op_70: op_71: op_72: op_73:
  {
    fprintf(stderr, "%02x\n", op >> 9);
    goto end;
  }

  op_74: op_75: op_76: op_77:
  {
    // TODO
    __builtin_trap();
  }

  // BLO
  op_78: op_79: op_7A: op_7B:
  {
    fprintf(stderr, "%02x\n", op >> 9);
    goto end;
  }

  // BLH
  op_7C: op_7D: op_7E: op_7F:
  {
    fprintf(stderr, "%02x\n", op >> 9);
    goto end;
  }
}
