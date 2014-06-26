// This file is part of the PiEMU Project
// Licensing information can be found in the LICENSE file
// (C) 2014 Nandor Licker. All rights reserved.
#include "common.h"

// -----------------------------------------------------------------------------
ThumbExec ThumbMap[0x20] =
{

};
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

  // LSL Rd, Rs, #Offset5
  op_00: op_01: op_02: op_03:
  {
    fprintf(stderr, "%02x\n", op >> 9);
    goto end;
  }

  //  LSR Rd, Rs, #Offset5
  op_04: op_05: op_06: op_07:
  {
    fprintf(stderr, "%02x\n", op >> 9);
    goto end;
  }

  // ASR Rd, Rs, #Offset5
  op_08: op_09: op_0A: op_0B:
  {
    fprintf(stderr, "%02x\n", op >> 9);
    goto end;
  }

  // ADD Rr, Rb, Ra
  op_0C:
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
      , [R] "=m" (t->r[(op >> 0) & 7])
      : [A] "g"  (t->r[(op >> 6) & 7])
      , [B] "g"  (t->r[(op >> 3) & 7])
      : "memory", "cc", "eax"
      );
    goto end;
  }

  // SUB Rr, Rb, Ra
  op_0D:
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
      , [R] "=m" (t->r[(op >> 0) & 7])
      : [A] "g"  (t->r[(op >> 6) & 7])
      , [B] "g"  (t->r[(op >> 3) & 7])
      : "memory", "cc", "eax"
      );
    goto end;
  }

  // ADD Rr, Rb, #Offset3
  op_0E:
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
      , [R] "=m" (t->r[(op >> 0) & 7])
      : [A] "g"  ((op >> 6) & 7)
      , [B] "g"  (t->r[(op >> 3) & 7])
      : "memory", "cc", "eax"
      );
    goto end;
  }

  // SUB Rr, Rb, #Offset3
  op_0F:
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
      , [R] "=m" (t->r[(op >> 0) & 7])
      : [A] "g"  ((op >> 6) & 7)
      , [B] "g"  (t->r[(op >> 3) & 7])
      : "memory", "cc", "eax"
      );
    goto end;
  }

  // MOV Rr, #Offset8
  op_10: op_11: op_12: op_13:
  {
    asm volatile
      ( "movzbl %%al, %%eax      \n\t"
        "movl   %%eax, %[R]      \n\t"
        "test   %%eax, %%eax     \n\t"
        "sets   %[N]             \n\t"
        "setz   %[Z]             \n\t"
      : [N] "=m" (t->n)
      , [Z] "=m" (t->z)
      , [R] "=m" (t->r[(op >> 8) & 7])
      : [O] "a" (op & 0xFF)
      : "memory", "cc"
      );
    goto end;
  }

  // CMP Rd, #Offset8
  op_14:
  op_15:
  op_16:
  op_17:
  {
    fprintf(stderr, "%02x\n", op >> 9);
    goto end;
  }
  op_18:
  op_19:
  op_1A:
  op_1B:
  {
    // ADD Rd, #Offset8
    fprintf(stderr, "%02x\n", op >> 9);
    goto end;
  }
  op_1C:
  op_1D:
  op_1E:
  op_1F:
  {
    // SUB Rd, #Offset8
    asm volatile
      ( "subl   %[A], %[B]       \n\t"
        "sets   %[N]             \n\t"
        "setz   %[Z]             \n\t"
        "setc   %[C]             \n\t"
        "seto   %[V]             \n\t"
      : [N] "=m" (t->n)
      , [Z] "=m" (t->z)
      , [C] "=m" (t->c)
      , [V] "=m" (t->v)
      , [B] "+m" (t->r[(op >> 8) & 7])
      : [A] "r"  (op & 0xFF)
      : "memory", "cc"
      );
    goto end;
  }
  op_20:
    fprintf(stderr, "%02x\n", op >> 9);
    goto end;
  op_21:
    fprintf(stderr, "%02x\n", op >> 9);
    goto end;
  op_22:
    fprintf(stderr, "%02x\n", op >> 9);
    goto end;
  op_23:
    fprintf(stderr, "%02x\n", op >> 9);
    goto end;
  op_24:
    fprintf(stderr, "%02x\n", op >> 9);
    goto end;
  op_25:
    fprintf(stderr, "%02x\n", op >> 9);
    goto end;
  op_26:
    fprintf(stderr, "%02x\n", op >> 9);
    goto end;
  op_27:
    fprintf(stderr, "%02x\n", op >> 9);
    goto end;
  op_28:
    fprintf(stderr, "%02x\n", op >> 9);
    goto end;
  op_29:
    fprintf(stderr, "%02x\n", op >> 9);
    goto end;
  op_2A:
    fprintf(stderr, "%02x\n", op >> 9);
    goto end;
  op_2B:
    fprintf(stderr, "%02x\n", op >> 9);
    goto end;
  op_2C:
    fprintf(stderr, "%02x\n", op >> 9);
    goto end;
  op_2D:
    fprintf(stderr, "%02x\n", op >> 9);
    goto end;
  op_2E:
    fprintf(stderr, "%02x\n", op >> 9);
    goto end;
  op_2F:
    fprintf(stderr, "%02x\n", op >> 9);
    goto end;
  op_30:
    fprintf(stderr, "%02x\n", op >> 9);
    goto end;
  op_31:
    fprintf(stderr, "%02x\n", op >> 9);
    goto end;
  op_32:
    fprintf(stderr, "%02x\n", op >> 9);
    goto end;
  op_33:
    fprintf(stderr, "%02x\n", op >> 9);
    goto end;
  op_34:
    fprintf(stderr, "%02x\n", op >> 9);
    goto end;
  op_35:
    fprintf(stderr, "%02x\n", op >> 9);
    goto end;
  op_36:
    fprintf(stderr, "%02x\n", op >> 9);
    goto end;
  op_37:
    fprintf(stderr, "%02x\n", op >> 9);
    goto end;
  op_38:
    fprintf(stderr, "%02x\n", op >> 9);
    goto end;
  op_39:
    fprintf(stderr, "%02x\n", op >> 9);
    goto end;
  op_3A:
    fprintf(stderr, "%02x\n", op >> 9);
    goto end;
  op_3B:
    fprintf(stderr, "%02x\n", op >> 9);
    goto end;
  op_3C:
    fprintf(stderr, "%02x\n", op >> 9);
    goto end;
  op_3D:
    fprintf(stderr, "%02x\n", op >> 9);
    goto end;
  op_3E:
    fprintf(stderr, "%02x\n", op >> 9);
    goto end;
  op_3F:
    fprintf(stderr, "%02x\n", op >> 9);
    goto end;
  op_40:
    fprintf(stderr, "%02x\n", op >> 9);
    goto end;
  op_41:
    fprintf(stderr, "%02x\n", op >> 9);
    goto end;
  op_42:
    fprintf(stderr, "%02x\n", op >> 9);
    goto end;
  op_43:
    fprintf(stderr, "%02x\n", op >> 9);
    goto end;
  op_44:
    fprintf(stderr, "%02x\n", op >> 9);
    goto end;
  op_45:
    fprintf(stderr, "%02x\n", op >> 9);
    goto end;
  op_46:
    fprintf(stderr, "%02x\n", op >> 9);
    goto end;
  op_47:
    fprintf(stderr, "%02x\n", op >> 9);
    goto end;
  op_48:
    fprintf(stderr, "%02x\n", op >> 9);
    goto end;
  op_49:
    fprintf(stderr, "%02x\n", op >> 9);
    goto end;
  op_4A:
    fprintf(stderr, "%02x\n", op >> 9);
    goto end;
  op_4B:
    fprintf(stderr, "%02x\n", op >> 9);
    goto end;
  op_4C:
    fprintf(stderr, "%02x\n", op >> 9);
    goto end;
  op_4D:
    fprintf(stderr, "%02x\n", op >> 9);
    goto end;
  op_4E:
    fprintf(stderr, "%02x\n", op >> 9);
    goto end;
  op_4F:
    fprintf(stderr, "%02x\n", op >> 9);
    goto end;
  op_50:
    fprintf(stderr, "%02x\n", op >> 9);
    goto end;
  op_51:
    fprintf(stderr, "%02x\n", op >> 9);
    goto end;
  op_52:
    fprintf(stderr, "%02x\n", op >> 9);
    goto end;
  op_53:
    fprintf(stderr, "%02x\n", op >> 9);
    goto end;
  op_54:
    fprintf(stderr, "%02x\n", op >> 9);
    goto end;
  op_55:
    fprintf(stderr, "%02x\n", op >> 9);
    goto end;
  op_56:
    fprintf(stderr, "%02x\n", op >> 9);
    goto end;
  op_57:
    fprintf(stderr, "%02x\n", op >> 9);
    goto end;
  op_58:
    fprintf(stderr, "%02x\n", op >> 9);
    goto end;
  op_59:
    fprintf(stderr, "%02x\n", op >> 9);
    goto end;
  op_5A:
    fprintf(stderr, "%02x\n", op >> 9);
    goto end;
  op_5B:
    fprintf(stderr, "%02x\n", op >> 9);
    goto end;
  op_5C:
    fprintf(stderr, "%02x\n", op >> 9);
    goto end;
  op_5D:
    fprintf(stderr, "%02x\n", op >> 9);
    goto end;
  op_5E:
    fprintf(stderr, "%02x\n", op >> 9);
    goto end;
  op_5F:
    fprintf(stderr, "%02x\n", op >> 9);
    goto end;
  op_60:
    fprintf(stderr, "%02x\n", op >> 9);
    goto end;
  op_61:
    fprintf(stderr, "%02x\n", op >> 9);
    goto end;
  op_62:
    fprintf(stderr, "%02x\n", op >> 9);
    goto end;
  op_63:
    fprintf(stderr, "%02x\n", op >> 9);
    goto end;
  op_64:
    fprintf(stderr, "%02x\n", op >> 9);
    goto end;
  op_65:
    fprintf(stderr, "%02x\n", op >> 9);
    goto end;
  op_66:
    fprintf(stderr, "%02x\n", op >> 9);
    goto end;
  op_67:
    fprintf(stderr, "%02x\n", op >> 9);
    goto end;
  op_68:
    fprintf(stderr, "%02x\n", op >> 9);
    goto end;
  op_69:
    fprintf(stderr, "%02x\n", op >> 9);
    goto end;
  op_6A:
    fprintf(stderr, "%02x\n", op >> 9);
    goto end;
  op_6B:
    fprintf(stderr, "%02x\n", op >> 9);
    goto end;
  op_6C:
    fprintf(stderr, "%02x\n", op >> 9);
    goto end;
  op_6D:
    fprintf(stderr, "%02x\n", op >> 9);
    goto end;
  op_6E:
    fprintf(stderr, "%02x\n", op >> 9);
    goto end;
  op_6F:
    fprintf(stderr, "%02x\n", op >> 9);
    goto end;
  op_70:
    fprintf(stderr, "%02x\n", op >> 9);
    goto end;
  op_71:
    fprintf(stderr, "%02x\n", op >> 9);
    goto end;
  op_72:
    fprintf(stderr, "%02x\n", op >> 9);
    goto end;
  op_73:
    fprintf(stderr, "%02x\n", op >> 9);
    goto end;
  op_74:
    fprintf(stderr, "%02x\n", op >> 9);
    goto end;
  op_75:
    fprintf(stderr, "%02x\n", op >> 9);
    goto end;
  op_76:
    fprintf(stderr, "%02x\n", op >> 9);
    goto end;
  op_77:
    fprintf(stderr, "%02x\n", op >> 9);
    goto end;
  op_78:
    fprintf(stderr, "%02x\n", op >> 9);
    goto end;
  op_79:
    fprintf(stderr, "%02x\n", op >> 9);
    goto end;
  op_7A:
    fprintf(stderr, "%02x\n", op >> 9);
    goto end;
  op_7B:
    fprintf(stderr, "%02x\n", op >> 9);
    goto end;
  op_7C:
    fprintf(stderr, "%02x\n", op >> 9);
    goto end;
  op_7D:
    fprintf(stderr, "%02x\n", op >> 9);
    goto end;
  op_7E:
    fprintf(stderr, "%02x\n", op >> 9);
    goto end;
  op_7F:
    fprintf(stderr, "%02x\n", op >> 9);
    goto end;
}
