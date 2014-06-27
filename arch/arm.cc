// This file is part of the PiEMU Project
// Licensing information can be found in the LICENSE file
// (C) 2014 Nandor Licker. All rights reserved.
#include "common.h"


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
    switch (((op >> 20) & 0xFF) | ((op >> 4) & 0xF))
    {
      default:
      {
        std::cerr << op << std::endl;
        goto und;
      }
    }
  }

  und:
  {
    return;
  }
}
