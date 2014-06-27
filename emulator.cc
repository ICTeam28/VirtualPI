// This file is part of the PiEMU Project
// Licensing information can be found in the LICENSE file
// (C) 2014 Nandor Licker. All rights reserved.
#include "common.h"


// -----------------------------------------------------------------------------
Emulator::Emulator(const Args& args)
  : args(args)
  , mem(*this, args.ram, args.vram)
{
  memset(&thumbState, 0, sizeof(THUMBState));
  mem.LoadImage(args.image, 0);

  thumbState.mem = &mem;
  thumbState.pc = 8;
}


// -----------------------------------------------------------------------------
Emulator::~Emulator()
{
}


// -----------------------------------------------------------------------------
void Emulator::Run()
{
  ThumbExecute(this);
}


// -----------------------------------------------------------------------------
void Emulator::DumpTHUMB(std::ostream& os)
{
  os << "THUMB State:" << std::endl;

  for (int i = 0; i < 8; ++i)
  {
    os << "$r" << i << ": "
       << std::setfill('0') << std::setw(8) << std::hex
       << thumbState.r[i] << std::endl;
  }

  os << "N:" << (thumbState.n ? 1 : 0) << " "
     << "Z:" << (thumbState.z ? 1 : 0) << " "
     << "C:" << (thumbState.c ? 1 : 0) << " "
     << "V:" << (thumbState.v ? 1 : 0) << std::endl;
}