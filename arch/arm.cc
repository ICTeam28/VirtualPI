// This file is part of the PiEMU Project
// Licensing information can be found in the LICENSE file
// (C) 2014 Nandor Licker. All rights reserved.
#include "common.h"

// -------------------------------------------------------------------------------------------------
static inline void UND(ARMData *a)
{

}


// -------------------------------------------------------------------------------------------------
template<bool S, uint32_t SH>
static inline uint32_t OperandReg(ARMData *a, uint32_t op)
{
  uint32_t reg, rs, shift;

  assert(a);

  reg = a->r[op & 0xF];

  if (SH & 1)
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

  switch (SH)
  {
    case 0x0: // LSL
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
    case 0x1: // LSR
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
    case 0x2: // ASR
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
    case 0x3: // ROR
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
template<bool I, bool P, bool U, bool B, bool W, bool L, uint32_t SH>
static inline void SingleDataTrans(ARMData *a, uint32_t op)
{
  uint32_t rn, rd, offset, base, addr;

  assert(a);

  rn = (op >> 16) & 0xF;
  rd = (op >> 12) & 0xF;
  base = a->r[rn];

  // Register or immediate offset
  if (I)
  {
    offset = OperandReg<0, SH> (a, op);
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
void ARMExecute(ARMData *a)
{
  register uint32_t op;

  a->pc += 4;
  while (true)
  {
    // Fetch next instruction and ajust the program counter
    // to take into account the pipelining effect
    a->pc += 4;
    op = a->mem->ReadInstrLong(a->pc - 8);

    // Decode the instruction
    switch (((op >> 16) & 0xFF0) | ((op >> 4) & 0xF))
    {
      case 0x121:
      {
        if ((op & 0x0FFFFFF0) == 0x012FFF10)
        {
          a->pc = a->r[op & 0xF];
          return;
        }
        continue;
      }
      case 0x590 ... 0x59F: SingleDataTrans<0, 1, 1, 0, 0, 1, 0>(a, op); continue;
      default:
      {
        std::cerr << std::hex << (((op >> 16) & 0xFF0) | ((op >> 4) & 0xF)) << std::endl;
        continue;
      }
    }
  }
}
