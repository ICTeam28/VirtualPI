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
}


// -----------------------------------------------------------------------------
Emulator::~Emulator()
{

}


// -----------------------------------------------------------------------------
void Emulator::Run()
{

}
