// This file is part of the PiEMU Project
// Licensing information can be found in the LICENSE file
// (C) 2014 Nandor Licker. All rights reserved.
#include "common.h"


// -----------------------------------------------------------------------------
// Exceptions
// -----------------------------------------------------------------------------
static inline void SWI(ARMState*) FORCEINLINE;
static inline void UND(ARMState*) FORCEINLINE;


// -----------------------------------------------------------------------------
template<bool S>
static inline uint32_t OpReg(ARMState *a, uint32_t op)
{
  uint32_t reg, rs, shift;

  assert(a);

  reg = a->r[op & 0xF];

  if ((op >> 4) & 1)
  {
    // if bit 4 is set shift is specified by bottom byte of register Rs(11 - 8),
    // but PC cannot be used as an operand
    if ((rs = (op >> 8) & 0xF) == 0xF)
    {
      UND(a);
    }

    shift = a->r[rs] & 0xFF;
  }
  else
  {
    // otherwise it is specified by a 5-bit unsigned integer (11 - 7)
    shift = (op >> 7) & 0x1F;
  }

  if (!shift)
  {
    return reg;
  }

  switch ((op >> 5) & 0xF)
  {
    case 0: // LSL
    {
      if (shift >= 32)
      {
        a->c = !S ? a->c : (shift == 32 && (reg & 0x1));
        return 0;
      }
      else
      {
        a->c = !S ? a->c : ((reg >> (32 - shift)) & 0x1);
        return reg << shift;
      }
    }
    case 1: // LSR
    {
      if (shift >= 32)
      {
        a->c = !S ? a->c : (shift == 32 && (reg >> 31));
        return 0;
      }
      else
      {
        a->c = !S ? a->c : ((reg >> (shift - 1)) & 0x1);
        return reg >> shift;
      }
    }
    case 2: // ASR
    {
      if (shift >= 32)
      {
        if (reg >> 31)
        {
          a->c = S ? 0 : a->c;
          return 0;
        }
        else
        {
          a->c = 1;
          return 0xFFFFFFFF;
        }
      }
      else
      {
        a->c = !S ? a->c : ((reg >> (shift - 1)) & 0x1);
        return ((int32_t)reg) >> shift;
      }
    }
    case 3: // ROR
    {
      if ((shift & ~0x1F) == 0)
      {
        a->c = !S ? a->c : (reg >> 31);
        return reg;
      }
      else
      {
        a->c = !S ? a->c : ((reg >> (shift - 1)) & 0x1);
        return (reg >> shift) | (reg << (32 - shift));
      }
    }
  }

  /* LCOV_EXCL_LINE*/ __builtin_unreachable();
}


// -----------------------------------------------------------------------------
static inline uint32_t OpImm(ARMState *a, uint32_t op)
{
  uint32_t rot, imm;

  rot = (op >> 8) & 0x0F;
  imm = (op >> 0) & 0xFF;

  return (imm >> (rot << 1)) | (imm << (32 - (rot << 1)));
}


// -----------------------------------------------------------------------------
template<bool I, bool P, bool U, bool B, bool W, bool L>
static inline void SingleDataTrans(ARMState *a, uint32_t op)
{
  uint32_t rn, rd, offset, base, addr;

  assert(a);

  rn = (op >> 16) & 0xF;
  rd = (op >> 12) & 0xF;
  base = a->r[rn];

  // Register or immediate offset
  if (I)
  {
    if (op & 0x10)
    {
      UND(a);
    }

    offset = OpReg<0> (a, op);
  }
  else
  {
    offset = op & 0xFFF;
  }

  // Pre or post indexing
  if (P)
  {
    base = U ? (base + offset) : (base - offset);
    addr = base;
  }
  else
  {
    addr = base;
    base = U ? (base + offset) : (base - offset);
  }

  // Load or store
  if (L)
  {
    // LDRB or LDR
    if (B)
    {
      a->r[rd] = a->mem->ReadByte(addr);
    }
    else
    {
      a->r[rd] = a->mem->ReadLong(addr);
    }
  }
  else
  {
    // STRB or STR
    if (B)
    {
      a->mem->WriteByte(addr, a->r[rd]);
    }
    else
    {
      a->mem->WriteLong(addr, a->r[rd]);
    }
  }

  // Writeback
  if (W || !P)
  {
    if (rn == 0xF)
    {
      UND(a);
    }

    a->r[rn] = base;
  }
}


// -----------------------------------------------------------------------------
static inline void MOV(ARMState *a, int32_t& d, int32_t x, uint32_t y)
{
  d = y;
}


// -----------------------------------------------------------------------------
static inline void MOVS(ARMState *a, int32_t& d, int32_t x, uint32_t y)
{
  asm volatile
    ( "movl   %[Y], %[D]       \n\t"
      "testl  %[D], %[D]       \n\t"
      "sets   %[N]             \n\t"
      "setz   %[Z]             \n\t"
    : [N] "=m" (a->n)
    , [Z] "=m" (a->z)
    , [D] "=g" (d)
    : [Y] "r"  (y)
    : "memory"
    );
}


// -----------------------------------------------------------------------------
static inline void ADD(ARMState *a, int32_t& d, int32_t x, uint32_t y)
{
  asm volatile
    ( "addl   %[X], %[Y]       \n\t"
      "movl   %[Y], %[D]       \n\t"
    : [D] "=g" (d)
    : [X] "g"  (x)
    , [Y] "r"  (y)
    : "memory", "cc", "eax"
    );
}


// -----------------------------------------------------------------------------
static inline void ADDS(ARMState *a, int32_t& d, int32_t x, uint32_t y)
{
  asm volatile
    ( "addl   %[X], %[Y]       \n\t"
      "movl   %[Y], %[D]       \n\t"
      "sets   %[N]             \n\t"
      "setz   %[Z]             \n\t"
      "setc   %[C]             \n\t"
      "seto   %[V]             \n\t"
    : [N] "=m" (a->n)
    , [Z] "=m" (a->z)
    , [C] "=m" (a->c)
    , [V] "=m" (a->v)
    , [D] "=g" (d)
    : [X] "g"  (x)
    , [Y] "r"  (y)
    : "memory", "cc", "eax"
    );
}


// -----------------------------------------------------------------------------
static inline void SWI(ARMState *a)
{

}


// -----------------------------------------------------------------------------
static inline void UND(ARMState *a)
{

}


// -----------------------------------------------------------------------------
void ARMExecute(ARMState *a)
{
  register uint32_t op;

  while (true)
  {
    // Check whether we have switched to THUMB or JAZELLE
    if (a->t || a->t)
    {
      return;
    }


    // Fetch next instruction and ajust the program counter
    // to take into account the pipelining effect
    op = a->mem->ReadInstrLong(a->pc);
    a->pc += 8;


    // Check for exceptional conditions. Interrupts are handled
    // in this state, so in those cases we jump to the
    // corresponding handlers. However, if there was a request
    // to switch to thumb code, we must exit the ARM state and
    // switch to thumb
    switch (a->exc)
    {
      case ARM_EXC_SWI:
      {
        return;
      }
      case ARM_EXC_UND:
      {
        return;
      }
      default:
      {
        break;
      }
    }


    // Decode the instruction based on bits [20:27] of
    // the instruction opcode
    switch (((op >> 20) & 0xFF))
    {
      case 0x00: break;
      case 0x01: break;
      case 0x02: break;
      case 0x03: break;
      case 0x04: break;
      case 0x05: break;
      case 0x06: break;
      case 0x07: break;
      case 0x08: break;
      case 0x09: break;
      case 0x0A: break;
      case 0x0B: break;
      case 0x0C: break;
      case 0x0D: break;
      case 0x0E: break;
      case 0x0F: break;
      case 0x10: break;
      case 0x11: break;
      case 0x12:
      {
        if ((op & 0x0FFFFFF0) == 0x012FFF10)
        {
          a->pc = a->r[op & 0xF];
          a->t = a->pc & 1;
          a->pc &= ~1;
          continue;
        }
        else
        {

        }
        break;
      }
      case 0x13: break;
      case 0x14: break;
      case 0x15: break;
      case 0x16: break;
      case 0x17: break;
      case 0x18: break;
      case 0x19: break;
      case 0x1A: break;
      case 0x1B: break;
      case 0x1C: break;
      case 0x1D: break;
      case 0x1E: break;
      case 0x1F: break;

      // AND Rd, Rn, #imm
      case 0x20:
      {
        std::cerr << "AND" << std::endl;
        __builtin_trap();
        break;
      }

      // ANDS Rd, Rn, #imm
      case 0x21:
      {
        std::cerr << "ANDS" << std::endl;
        __builtin_trap();
        break;
      }

      // EOR Rd, Rn, #imm
      case 0x22:
      {
        std::cerr << "EOR" << std::endl;
        __builtin_trap();
        break;
      }

      // EORS Rd, Rn, #imm
      case 0x23:
      {
        std::cerr << "EORS" << std::endl;
        __builtin_trap();
        break;
      }

      // SUB Rd, Rn, #imm
      case 0x24:
      {
        std::cerr << "SUB" << std::endl;
        __builtin_trap();
        break;
      }

      // SUBS Rd, Rn, #imm
      case 0x25:
      {
        std::cerr << "SUBS" << std::endl;
        __builtin_trap();
        break;
      }

      // RSB Rd, Rn, #imm
      case 0x26:
      {
        std::cerr << "RSB" << std::endl;
        __builtin_trap();
        break;
      }

      // RSBS  Rd, Rn, #imm
      case 0x27:
      {
        std::cerr << "RSBS" << std::endl;
        __builtin_trap();
        break;
      }

      // ADD Rd, Rn, #imm
      case 0x28:
      {
        ADD(a, a->r[(op >> 12) & 15], a->r[(op >> 16) & 15], OpImm(a, op));
        break;
      }

      // ADDS Rd, Rn, #imm
      case 0x29:
      {
        ADDS(a, a->r[(op >> 12) & 15], a->r[(op >> 16) & 15], OpImm(a, op));
        break;
      }

      // ADC Rd, Rn, #imm
      case 0x2A:
      {
        std::cerr << "ADC" << std::endl;
        __builtin_trap();
        break;
      }

      // ADCS Rd, Rn, #imm
      case 0x2B:
      {
        std::cerr << "ADCS" << std::endl;
        __builtin_trap();
        break;
      }

      // SBC  Rd, Rn, #imm
      case 0x2C:
      {
        std::cerr << "SBC" << std::endl;
        __builtin_trap();
        break;
      }

      // SBCS  Rd, Rn, #imm
      case 0x2D:
      {
        std::cerr << "SBCS" << std::endl;
        __builtin_trap();
        break;
      }

      // RSC Rd, Rn, #imm
      case 0x2E:
      {
        std::cerr << "RSC" << std::endl;
        __builtin_trap();
        break;
      }

      // RSCS  Rd, Rn, #imm
      case 0x2F:
      {
        std::cerr << "RSCS" << std::endl;
        __builtin_trap();
        break;
      }

      // TST Rn, #imm
      case 0x30:
      {
        std::cerr << "TST" << std::endl;
        __builtin_trap();
        break;
      }

      // ?
      case 0x31:
      {
        std::cerr << "" << std::endl;
        __builtin_trap();
        break;
      }

      // TEQ Rn, #imm
      case 0x32:
      {
        std::cerr << "TEQ" << std::endl;
        __builtin_trap();
        break;
      }

      // ?
      case 0x33:
      {
        std::cerr << "" << std::endl;
        __builtin_trap();
        break;
      }

      // CMP Rn, #imm
      case 0x34:
      {
        std::cerr << "CMP" << std::endl;
        __builtin_trap();
        break;
      }

      // ?
      case 0x35:
      {
        std::cerr << "" << std::endl;
        __builtin_trap();
        break;
      }

      // CMN Rn, #imm
      case 0x36:
      {
        std::cerr << "CMN" << std::endl;
        __builtin_trap();
        break;
      }

      // ?
      case 0x37:
      {
        std::cerr << "" << std::endl;
        __builtin_trap();
        break;
      }

      // ORR Rd, Rn, #imm
      case 0x38:
      {
        std::cerr << "ORR" << std::endl;
        __builtin_trap();
        break;
      }

      // ORRS Rd, Rn, #imm
      case 0x39:
      {
        std::cerr << "ORRS" << std::endl;
        __builtin_trap();
        break;
      }

      // MOV Rd, Rn, #imm
      case 0x3A:
      {
        MOV(a, a->r[(op >> 12) & 15], a->r[(op >> 16) & 15], OpImm(a, op));
        break;
      }

      // MOVS Rd, Rn, #imm
      case 0x3B:
      {
        MOVS(a, a->r[(op >> 12) & 15], a->r[(op >> 16) & 15], OpImm(a, op));
        break;
      }

      // BIC Rd, Rn, #imm
      case 0x3C:
      {
        std::cerr << "BIC" << std::endl;
        __builtin_trap();
        break;
      }

      // BICS Rd, Rn, #imm
      case 0x3D:
      {
        std::cerr << "BICS" << std::endl;
        __builtin_trap();
        break;
      }

      // MVN Rd, Rn, #imm
      case 0x3E:
      {
        std::cerr << "MVN" << std::endl;
        __builtin_trap();
        break;
      }

      // MVNS Rd, Rn, #imm
      case 0x3F:
      {
        std::cerr << "MVNS" << std::endl;
        __builtin_trap();
        break;
      }


      case 0x40: SingleDataTrans<0, 0, 0, 0, 0, 0>(a, op); break;
      case 0x41: SingleDataTrans<0, 0, 0, 0, 0, 1>(a, op); break;
      case 0x42: SingleDataTrans<0, 0, 0, 0, 1, 0>(a, op); break;
      case 0x43: SingleDataTrans<0, 0, 0, 0, 1, 1>(a, op); break;
      case 0x44: SingleDataTrans<0, 0, 0, 1, 0, 0>(a, op); break;
      case 0x45: SingleDataTrans<0, 0, 0, 1, 0, 1>(a, op); break;
      case 0x46: SingleDataTrans<0, 0, 0, 1, 1, 0>(a, op); break;
      case 0x47: SingleDataTrans<0, 0, 0, 1, 1, 1>(a, op); break;
      case 0x48: SingleDataTrans<0, 0, 0, 0, 0, 0>(a, op); break;
      case 0x49: SingleDataTrans<0, 0, 1, 0, 0, 1>(a, op); break;
      case 0x4A: SingleDataTrans<0, 0, 1, 0, 1, 0>(a, op); break;
      case 0x4B: SingleDataTrans<0, 0, 1, 0, 1, 1>(a, op); break;
      case 0x4C: SingleDataTrans<0, 0, 1, 1, 0, 0>(a, op); break;
      case 0x4D: SingleDataTrans<0, 0, 1, 1, 0, 1>(a, op); break;
      case 0x4E: SingleDataTrans<0, 0, 1, 1, 1, 0>(a, op); break;
      case 0x4F: SingleDataTrans<0, 0, 1, 1, 1, 1>(a, op); break;
      case 0x50: SingleDataTrans<0, 1, 0, 0, 0, 0>(a, op); break;
      case 0x51: SingleDataTrans<0, 1, 0, 0, 0, 1>(a, op); break;
      case 0x52: SingleDataTrans<0, 1, 0, 0, 1, 0>(a, op); break;
      case 0x53: SingleDataTrans<0, 1, 0, 0, 1, 1>(a, op); break;
      case 0x54: SingleDataTrans<0, 1, 0, 1, 0, 0>(a, op); break;
      case 0x55: SingleDataTrans<0, 1, 0, 1, 0, 1>(a, op); break;
      case 0x56: SingleDataTrans<0, 1, 0, 1, 1, 0>(a, op); break;
      case 0x57: SingleDataTrans<0, 1, 0, 1, 1, 1>(a, op); break;
      case 0x58: SingleDataTrans<0, 1, 0, 0, 0, 0>(a, op); break;
      case 0x59: SingleDataTrans<0, 1, 1, 0, 0, 1>(a, op); break;
      case 0x5A: SingleDataTrans<0, 1, 1, 0, 1, 0>(a, op); break;
      case 0x5B: SingleDataTrans<0, 1, 1, 0, 1, 1>(a, op); break;
      case 0x5C: SingleDataTrans<0, 1, 1, 1, 0, 0>(a, op); break;
      case 0x5D: SingleDataTrans<0, 1, 1, 1, 0, 1>(a, op); break;
      case 0x5E: SingleDataTrans<0, 1, 1, 1, 1, 0>(a, op); break;
      case 0x5F: SingleDataTrans<0, 1, 1, 1, 1, 1>(a, op); break;
      case 0x60: SingleDataTrans<1, 0, 0, 0, 0, 0>(a, op); break;
      case 0x61: SingleDataTrans<1, 0, 0, 0, 0, 1>(a, op); break;
      case 0x62: SingleDataTrans<1, 0, 0, 0, 1, 0>(a, op); break;
      case 0x63: SingleDataTrans<1, 0, 0, 0, 1, 1>(a, op); break;
      case 0x64: SingleDataTrans<1, 0, 0, 1, 0, 0>(a, op); break;
      case 0x65: SingleDataTrans<1, 0, 0, 1, 0, 1>(a, op); break;
      case 0x66: SingleDataTrans<1, 0, 0, 1, 1, 0>(a, op); break;
      case 0x67: SingleDataTrans<1, 0, 0, 1, 1, 1>(a, op); break;
      case 0x68: SingleDataTrans<1, 0, 0, 0, 0, 0>(a, op); break;
      case 0x69: SingleDataTrans<1, 0, 1, 0, 0, 1>(a, op); break;
      case 0x6A: SingleDataTrans<1, 0, 1, 0, 1, 0>(a, op); break;
      case 0x6B: SingleDataTrans<1, 0, 1, 0, 1, 1>(a, op); break;
      case 0x6C: SingleDataTrans<1, 0, 1, 1, 0, 0>(a, op); break;
      case 0x6D: SingleDataTrans<1, 0, 1, 1, 0, 1>(a, op); break;
      case 0x6E: SingleDataTrans<1, 0, 1, 1, 1, 0>(a, op); break;
      case 0x6F: SingleDataTrans<1, 0, 1, 1, 1, 1>(a, op); break;
      case 0x70: SingleDataTrans<1, 1, 0, 0, 0, 0>(a, op); break;
      case 0x71: SingleDataTrans<1, 1, 0, 0, 0, 1>(a, op); break;
      case 0x72: SingleDataTrans<1, 1, 0, 0, 1, 0>(a, op); break;
      case 0x73: SingleDataTrans<1, 1, 0, 0, 1, 1>(a, op); break;
      case 0x74: SingleDataTrans<1, 1, 0, 1, 0, 0>(a, op); break;
      case 0x75: SingleDataTrans<1, 1, 0, 1, 0, 1>(a, op); break;
      case 0x76: SingleDataTrans<1, 1, 0, 1, 1, 0>(a, op); break;
      case 0x77: SingleDataTrans<1, 1, 0, 1, 1, 1>(a, op); break;
      case 0x78: SingleDataTrans<1, 1, 0, 0, 0, 0>(a, op); break;
      case 0x79: SingleDataTrans<1, 1, 1, 0, 0, 1>(a, op); break;
      case 0x7A: SingleDataTrans<1, 1, 1, 0, 1, 0>(a, op); break;
      case 0x7B: SingleDataTrans<1, 1, 1, 0, 1, 1>(a, op); break;
      case 0x7C: SingleDataTrans<1, 1, 1, 1, 0, 0>(a, op); break;
      case 0x7D: SingleDataTrans<1, 1, 1, 1, 0, 1>(a, op); break;
      case 0x7E: SingleDataTrans<1, 1, 1, 1, 1, 0>(a, op); break;
      case 0x7F: SingleDataTrans<1, 1, 1, 1, 1, 1>(a, op); break;
      case 0x80: break;
      case 0x81: break;
      case 0x82: break;
      case 0x83: break;
      case 0x84: break;
      case 0x85: break;
      case 0x86: break;
      case 0x87: break;
      case 0x88: break;
      case 0x89: break;
      case 0x8A: break;
      case 0x8B: break;
      case 0x8C: break;
      case 0x8D: break;
      case 0x8E: break;
      case 0x8F: break;
      case 0x90: break;
      case 0x91: break;
      case 0x92: break;
      case 0x93: break;
      case 0x94: break;
      case 0x95: break;
      case 0x96: break;
      case 0x97: break;
      case 0x98: break;
      case 0x99: break;
      case 0x9A: break;
      case 0x9B: break;
      case 0x9C: break;
      case 0x9D: break;
      case 0x9E: break;
      case 0x9F: break;
      case 0xA0: break;
      case 0xA1: break;
      case 0xA2: break;
      case 0xA3: break;
      case 0xA4: break;
      case 0xA5: break;
      case 0xA6: break;
      case 0xA7: break;
      case 0xA8: break;
      case 0xA9: break;
      case 0xAA: break;
      case 0xAB: break;
      case 0xAC: break;
      case 0xAD: break;
      case 0xAE: break;
      case 0xAF: break;
      case 0xB0: break;
      case 0xB1: break;
      case 0xB2: break;
      case 0xB3: break;
      case 0xB4: break;
      case 0xB5: break;
      case 0xB6: break;
      case 0xB7: break;
      case 0xB8: break;
      case 0xB9: break;
      case 0xBA: break;
      case 0xBB: break;
      case 0xBC: break;
      case 0xBD: break;
      case 0xBE: break;
      case 0xBF: break;
      case 0xC0: break;
      case 0xC1: break;
      case 0xC2: break;
      case 0xC3: break;
      case 0xC4: break;
      case 0xC5: break;
      case 0xC6: break;
      case 0xC7: break;
      case 0xC8: break;
      case 0xC9: break;
      case 0xCA: break;
      case 0xCB: break;
      case 0xCC: break;
      case 0xCD: break;
      case 0xCE: break;
      case 0xCF: break;
      case 0xD0: break;
      case 0xD1: break;
      case 0xD2: break;
      case 0xD3: break;
      case 0xD4: break;
      case 0xD5: break;
      case 0xD6: break;
      case 0xD7: break;
      case 0xD8: break;
      case 0xD9: break;
      case 0xDA: break;
      case 0xDB: break;
      case 0xDC: break;
      case 0xDD: break;
      case 0xDE: break;
      case 0xDF: break;
      case 0xE0: break;
      case 0xE1: break;
      case 0xE2: break;
      case 0xE3: break;
      case 0xE4: break;
      case 0xE5: break;
      case 0xE6: break;
      case 0xE7: break;
      case 0xE8: break;
      case 0xE9: break;
      case 0xEA: break;
      case 0xEB: break;
      case 0xEC: break;
      case 0xED: break;
      case 0xEE: break;
      case 0xEF: break;
      case 0xF0: break;
      case 0xF1: break;
      case 0xF2: break;
      case 0xF3: break;
      case 0xF4: break;
      case 0xF5: break;
      case 0xF6: break;
      case 0xF7: break;
      case 0xF8: break;
      case 0xF9: break;
      case 0xFA: break;
      case 0xFB: break;
      case 0xFC: break;
      case 0xFD: break;
      case 0xFE: break;
      case 0xFF: break;
      default:
      {
        /* LCOV_EXCL_LINE */ __builtin_unreachable();
      }
    }


    // We increment by 8 at the beginning in order
    // for instructions to read in the correctly
    // adjusted PC, but we subtract only 4 in order
    // to point PC to the next instruction
    a->pc -= 4;
  }
}
