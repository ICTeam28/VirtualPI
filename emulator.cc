// This file is part of the PiEMU Project
// Licensing information can be found in the LICENSE file
// (C) 2014 Nandor Licker. All rights reserved.
#include "common.h"


// -----------------------------------------------------------------------------
Emulator::Emulator(const Args& args)
  : args(args)
  , mem(*this, args.ram, args.vram)
{
  mem.LoadImage(args.image, 0);

  // Initialise the ARM state
  memset(&armState, 0, sizeof(ARMState));
  armState.mem = &mem;
  armState.iset = ARM_IS_ARM;
  armState.pc = 0;
}


// -----------------------------------------------------------------------------
Emulator::~Emulator()
{
}


// -----------------------------------------------------------------------------
void Emulator::Run()
{
  while (!armState.hang)
  {
    switch (armState.iset)
    {
      case ARM_IS_ARM:   ARMExecute(&armState); continue;
      case ARM_IS_THUMB: ThumbExecute(&armState); continue;
    }
  }
}


// -----------------------------------------------------------------------------
void Emulator::DumpState(std::ostream& os)
{
  os << "THUMB State:" << std::endl;

  for (int i = 0; i < 8; ++i)
  {
    os << "$r" << i << ": "
       << std::setfill('0') << std::setw(8) << std::hex
       << armState.r[i] << std::endl;
  }

  os << "N:" << (armState.n ? 1 : 0) << " "
     << "Z:" << (armState.z ? 1 : 0) << " "
     << "C:" << (armState.c ? 1 : 0) << " "
     << "V:" << (armState.v ? 1 : 0) << std::endl;
}