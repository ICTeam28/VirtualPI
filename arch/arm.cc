// This file is part of the PiEMU Project
// Licensing information can be found in the LICENSE file
// (C) 2014 Nandor Licker. All rights reserved.
#include "common.h"

// -------------------------------------------------------------------------------------------------
template<bool I, bool P, bool U, bool B, bool W, bool L, uint32_t SH>
static inline void SingleDataTrans(ARMState *a, uint32_t op) FORCEINLINE;


// -------------------------------------------------------------------------------------------------
static inline void UND(ARMState *a)
{

}


// -------------------------------------------------------------------------------------------------
template<bool S>
static inline uint32_t OperandReg(ARMState *a, uint32_t op)
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


// -------------------------------------------------------------------------------------------------
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

    offset = OperandReg<0> (a, op);
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

// -------------------------------------------------------------------------------------------------
void ARMExecute(ARMState *a)
{
  register uint32_t op;

  a->pc += 4;
  while (true)
  {
    // Fetch next instruction and ajust the program counter
    // to take into account the pipelining effect
    a->pc += 4;
    op = a->mem->ReadInstrLong(a->pc - 8);

    // Check whether we have switched to THUMB or JAZELLE
    if (a->t || a->t)
    {
      a->pc -= 4;
      return;
    }

    // Check for exit conditions. Interrupts are handled in
    // this state, so in those cases we jump to the
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

    // Decode the instruction
    switch (((op >> 20) & 0xFF))
    {
      case 0x00: continue;
      case 0x01: continue;
      case 0x02: continue;
      case 0x03: continue;
      case 0x04: continue;
      case 0x05: continue;
      case 0x06: continue;
      case 0x07: continue;
      case 0x08: continue;
      case 0x09: continue;
      case 0x0A: continue;
      case 0x0B: continue;
      case 0x0C: continue;
      case 0x0D: continue;
      case 0x0E: continue;
      case 0x0F: continue;
      case 0x10: continue;
      case 0x11: continue;
      case 0x12:
      {
        if ((op & 0x0FFFFFF0) == 0x012FFF10)
        {
          a->pc = a->r[op & 0xF];
          if (a->pc & 1)
          {
            a->pc &= ~1;
            a->j = 0;
            a->t = 1;
          }
        }
        else
        {

        }
        continue;
      }

      case 0x13: continue;
      case 0x14: continue;
      case 0x15: continue;
      case 0x16: continue;
      case 0x17: continue;
      case 0x18: continue;
      case 0x19: continue;
      case 0x1A: continue;
      case 0x1B: continue;
      case 0x1C: continue;
      case 0x1D: continue;
      case 0x1E: continue;
      case 0x1F: continue;
      case 0x20: continue;
      case 0x21: continue;
      case 0x22: continue;
      case 0x23: continue;
      case 0x24: continue;
      case 0x25: continue;
      case 0x26: continue;
      case 0x27: continue;
      case 0x28: continue;
      case 0x29: continue;
      case 0x2A: continue;
      case 0x2B: continue;
      case 0x2C: continue;
      case 0x2D: continue;
      case 0x2E: continue;
      case 0x2F: continue;
      case 0x30: continue;
      case 0x31: continue;
      case 0x32: continue;
      case 0x33: continue;
      case 0x34: continue;
      case 0x35: continue;
      case 0x36: continue;
      case 0x37: continue;
      case 0x38: continue;
      case 0x39: continue;
      case 0x3A: continue;
      case 0x3B: continue;
      case 0x3C: continue;
      case 0x3D: continue;
      case 0x3E: continue;
      case 0x3F: continue;
      case 0x40: SingleDataTrans<0, 0, 0, 0, 0, 0>(a, op); continue;
      case 0x41: SingleDataTrans<0, 0, 0, 0, 0, 1>(a, op); continue;
      case 0x42: SingleDataTrans<0, 0, 0, 0, 1, 0>(a, op); continue;
      case 0x43: SingleDataTrans<0, 0, 0, 0, 1, 1>(a, op); continue;
      case 0x44: SingleDataTrans<0, 0, 0, 1, 0, 0>(a, op); continue;
      case 0x45: SingleDataTrans<0, 0, 0, 1, 0, 1>(a, op); continue;
      case 0x46: SingleDataTrans<0, 0, 0, 1, 1, 0>(a, op); continue;
      case 0x47: SingleDataTrans<0, 0, 0, 1, 1, 1>(a, op); continue;
      case 0x48: SingleDataTrans<0, 0, 0, 0, 0, 0>(a, op); continue;
      case 0x49: SingleDataTrans<0, 0, 1, 0, 0, 1>(a, op); continue;
      case 0x4A: SingleDataTrans<0, 0, 1, 0, 1, 0>(a, op); continue;
      case 0x4B: SingleDataTrans<0, 0, 1, 0, 1, 1>(a, op); continue;
      case 0x4C: SingleDataTrans<0, 0, 1, 1, 0, 0>(a, op); continue;
      case 0x4D: SingleDataTrans<0, 0, 1, 1, 0, 1>(a, op); continue;
      case 0x4E: SingleDataTrans<0, 0, 1, 1, 1, 0>(a, op); continue;
      case 0x4F: SingleDataTrans<0, 0, 1, 1, 1, 1>(a, op); continue;
      case 0x50: SingleDataTrans<0, 1, 0, 0, 0, 0>(a, op); continue;
      case 0x51: SingleDataTrans<0, 1, 0, 0, 0, 1>(a, op); continue;
      case 0x52: SingleDataTrans<0, 1, 0, 0, 1, 0>(a, op); continue;
      case 0x53: SingleDataTrans<0, 1, 0, 0, 1, 1>(a, op); continue;
      case 0x54: SingleDataTrans<0, 1, 0, 1, 0, 0>(a, op); continue;
      case 0x55: SingleDataTrans<0, 1, 0, 1, 0, 1>(a, op); continue;
      case 0x56: SingleDataTrans<0, 1, 0, 1, 1, 0>(a, op); continue;
      case 0x57: SingleDataTrans<0, 1, 0, 1, 1, 1>(a, op); continue;
      case 0x58: SingleDataTrans<0, 1, 0, 0, 0, 0>(a, op); continue;
      case 0x59: SingleDataTrans<0, 1, 1, 0, 0, 1>(a, op); continue;
      case 0x5A: SingleDataTrans<0, 1, 1, 0, 1, 0>(a, op); continue;
      case 0x5B: SingleDataTrans<0, 1, 1, 0, 1, 1>(a, op); continue;
      case 0x5C: SingleDataTrans<0, 1, 1, 1, 0, 0>(a, op); continue;
      case 0x5D: SingleDataTrans<0, 1, 1, 1, 0, 1>(a, op); continue;
      case 0x5E: SingleDataTrans<0, 1, 1, 1, 1, 0>(a, op); continue;
      case 0x5F: SingleDataTrans<0, 1, 1, 1, 1, 1>(a, op); continue;
      case 0x60: SingleDataTrans<1, 0, 0, 0, 0, 0>(a, op); continue;
      case 0x61: SingleDataTrans<1, 0, 0, 0, 0, 1>(a, op); continue;
      case 0x62: SingleDataTrans<1, 0, 0, 0, 1, 0>(a, op); continue;
      case 0x63: SingleDataTrans<1, 0, 0, 0, 1, 1>(a, op); continue;
      case 0x64: SingleDataTrans<1, 0, 0, 1, 0, 0>(a, op); continue;
      case 0x65: SingleDataTrans<1, 0, 0, 1, 0, 1>(a, op); continue;
      case 0x66: SingleDataTrans<1, 0, 0, 1, 1, 0>(a, op); continue;
      case 0x67: SingleDataTrans<1, 0, 0, 1, 1, 1>(a, op); continue;
      case 0x68: SingleDataTrans<1, 0, 0, 0, 0, 0>(a, op); continue;
      case 0x69: SingleDataTrans<1, 0, 1, 0, 0, 1>(a, op); continue;
      case 0x6A: SingleDataTrans<1, 0, 1, 0, 1, 0>(a, op); continue;
      case 0x6B: SingleDataTrans<1, 0, 1, 0, 1, 1>(a, op); continue;
      case 0x6C: SingleDataTrans<1, 0, 1, 1, 0, 0>(a, op); continue;
      case 0x6D: SingleDataTrans<1, 0, 1, 1, 0, 1>(a, op); continue;
      case 0x6E: SingleDataTrans<1, 0, 1, 1, 1, 0>(a, op); continue;
      case 0x6F: SingleDataTrans<1, 0, 1, 1, 1, 1>(a, op); continue;
      case 0x70: SingleDataTrans<1, 1, 0, 0, 0, 0>(a, op); continue;
      case 0x71: SingleDataTrans<1, 1, 0, 0, 0, 1>(a, op); continue;
      case 0x72: SingleDataTrans<1, 1, 0, 0, 1, 0>(a, op); continue;
      case 0x73: SingleDataTrans<1, 1, 0, 0, 1, 1>(a, op); continue;
      case 0x74: SingleDataTrans<1, 1, 0, 1, 0, 0>(a, op); continue;
      case 0x75: SingleDataTrans<1, 1, 0, 1, 0, 1>(a, op); continue;
      case 0x76: SingleDataTrans<1, 1, 0, 1, 1, 0>(a, op); continue;
      case 0x77: SingleDataTrans<1, 1, 0, 1, 1, 1>(a, op); continue;
      case 0x78: SingleDataTrans<1, 1, 0, 0, 0, 0>(a, op); continue;
      case 0x79: SingleDataTrans<1, 1, 1, 0, 0, 1>(a, op); continue;
      case 0x7A: SingleDataTrans<1, 1, 1, 0, 1, 0>(a, op); continue;
      case 0x7B: SingleDataTrans<1, 1, 1, 0, 1, 1>(a, op); continue;
      case 0x7C: SingleDataTrans<1, 1, 1, 1, 0, 0>(a, op); continue;
      case 0x7D: SingleDataTrans<1, 1, 1, 1, 0, 1>(a, op); continue;
      case 0x7E: SingleDataTrans<1, 1, 1, 1, 1, 0>(a, op); continue;
      case 0x7F: SingleDataTrans<1, 1, 1, 1, 1, 1>(a, op); continue;
      case 0x80: continue;
      case 0x81: continue;
      case 0x82: continue;
      case 0x83: continue;
      case 0x84: continue;
      case 0x85: continue;
      case 0x86: continue;
      case 0x87: continue;
      case 0x88: continue;
      case 0x89: continue;
      case 0x8A: continue;
      case 0x8B: continue;
      case 0x8C: continue;
      case 0x8D: continue;
      case 0x8E: continue;
      case 0x8F: continue;
      case 0x90: continue;
      case 0x91: continue;
      case 0x92: continue;
      case 0x93: continue;
      case 0x94: continue;
      case 0x95: continue;
      case 0x96: continue;
      case 0x97: continue;
      case 0x98: continue;
      case 0x99: continue;
      case 0x9A: continue;
      case 0x9B: continue;
      case 0x9C: continue;
      case 0x9D: continue;
      case 0x9E: continue;
      case 0x9F: continue;
      case 0xA0: continue;
      case 0xA1: continue;
      case 0xA2: continue;
      case 0xA3: continue;
      case 0xA4: continue;
      case 0xA5: continue;
      case 0xA6: continue;
      case 0xA7: continue;
      case 0xA8: continue;
      case 0xA9: continue;
      case 0xAA: continue;
      case 0xAB: continue;
      case 0xAC: continue;
      case 0xAD: continue;
      case 0xAE: continue;
      case 0xAF: continue;
      case 0xB0: continue;
      case 0xB1: continue;
      case 0xB2: continue;
      case 0xB3: continue;
      case 0xB4: continue;
      case 0xB5: continue;
      case 0xB6: continue;
      case 0xB7: continue;
      case 0xB8: continue;
      case 0xB9: continue;
      case 0xBA: continue;
      case 0xBB: continue;
      case 0xBC: continue;
      case 0xBD: continue;
      case 0xBE: continue;
      case 0xBF: continue;
      case 0xC0: continue;
      case 0xC1: continue;
      case 0xC2: continue;
      case 0xC3: continue;
      case 0xC4: continue;
      case 0xC5: continue;
      case 0xC6: continue;
      case 0xC7: continue;
      case 0xC8: continue;
      case 0xC9: continue;
      case 0xCA: continue;
      case 0xCB: continue;
      case 0xCC: continue;
      case 0xCD: continue;
      case 0xCE: continue;
      case 0xCF: continue;
      case 0xD0: continue;
      case 0xD1: continue;
      case 0xD2: continue;
      case 0xD3: continue;
      case 0xD4: continue;
      case 0xD5: continue;
      case 0xD6: continue;
      case 0xD7: continue;
      case 0xD8: continue;
      case 0xD9: continue;
      case 0xDA: continue;
      case 0xDB: continue;
      case 0xDC: continue;
      case 0xDD: continue;
      case 0xDE: continue;
      case 0xDF: continue;
      case 0xE0: continue;
      case 0xE1: continue;
      case 0xE2: continue;
      case 0xE3: continue;
      case 0xE4: continue;
      case 0xE5: continue;
      case 0xE6: continue;
      case 0xE7: continue;
      case 0xE8: continue;
      case 0xE9: continue;
      case 0xEA: continue;
      case 0xEB: continue;
      case 0xEC: continue;
      case 0xED: continue;
      case 0xEE: continue;
      case 0xEF: continue;
      case 0xF0: continue;
      case 0xF1: continue;
      case 0xF2: continue;
      case 0xF3: continue;
      case 0xF4: continue;
      case 0xF5: continue;
      case 0xF6: continue;
      case 0xF7: continue;
      case 0xF8: continue;
      case 0xF9: continue;
      case 0xFA: continue;
      case 0xFB: continue;
      case 0xFC: continue;
      case 0xFD: continue;
      case 0xFE: continue;
      case 0xFF: continue;
    }
  }
}
