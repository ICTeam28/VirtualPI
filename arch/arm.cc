// This file is part of the PiEMU Project
// Licensing information can be found in the LICENSE file
// (C) 2014 Nandor Licker. All rights reserved.
#include "common.h"

// -------------------------------------------------------------------------------------------------
template<bool I, bool P, bool U, bool B, bool W, bool L>
static inline void SingleDataTrans(ARMState *t, uint32_t op)
{

}


// -------------------------------------------------------------------------------------------------
void ARMExecute(ARMState *t)
{
  register uint32_t op;

  while (true)
  {
    // Fetch next instruction and ajust the program counter
    // to take into account the pipelining effect
    t->pc += 4;
    op = t->mem->GetInstrLong(t->pc - 4);

    // Decode the instruction
    switch (((op >> 16) & 0xFF0) | ((op >> 4) & 0xF))
    {
      case 0x121:
      {
        if ((op & 0x0FFFFFF0) == 0x012FFF10)
        {

        }
      }
      case 0x590 ... 0x59F: SingleDataTrans<0, 1, 1, 0, 0, 1>(t, op); continue;
      default:
      {
        std::cerr << std::hex << (((op >> 16) & 0xFF0) | ((op >> 4) & 0xF)) << std::endl;
        goto und;
      }
    }
  }

  und:
  {
    return;
  }
}
