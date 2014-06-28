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
  memset(&armData, 0, sizeof(ARMData));
  armData.mem = &mem;
  armData.state = ARM_STATE_ARM;
  armData.pc = 0;
}


// -----------------------------------------------------------------------------
Emulator::~Emulator()
{
}


// -----------------------------------------------------------------------------
void Emulator::Run()
{
  ARMExecute(&armData);
  ThumbExecute(&armData);
}


// -----------------------------------------------------------------------------
void Emulator::DumpState(std::ostream& os)
{
  os << "THUMB State:" << std::endl;

  for (int i = 0; i < 8; ++i)
  {
    os << "$r" << i << ": "
       << std::setfill('0') << std::setw(8) << std::hex
       << armData.r[i] << std::endl;
  }

  os << "N:" << (armData.n ? 1 : 0) << " "
     << "Z:" << (armData.z ? 1 : 0) << " "
     << "C:" << (armData.c ? 1 : 0) << " "
     << "V:" << (armData.v ? 1 : 0) << std::endl;
}