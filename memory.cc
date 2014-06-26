// This file is part of the PiEMU Project
// Licensing information can be found in the LICENSE file
// (C) 2014 Nandor Licker. All rights reserved.
#include "common.h"


// -----------------------------------------------------------------------------
Memory::Memory(Emulator &emu, uint32_t ramSize, uint32_t vramSize)
  : emu(emu)
  , ramSize(ramSize)
  , ram(nullptr)
  , vramSize(vramSize)
  , vram(nullptr)
{
  ram = new uint8_t[ramSize];
  memset(ram, 0, ramSize);
  vram = ram + ramSize - vramSize;
}


// -----------------------------------------------------------------------------
Memory::~Memory()
{
  delete[] ram;
}


// -----------------------------------------------------------------------------
void Memory::LoadImage(const std::string& image, size_t start)
{
  std::ifstream file;
  size_t size;

  file.open(image, std::ios::binary);
  if (!file.is_open())
  {
    EXCEPT << "Cannot open file '" << image << "'";
  }

  file.seekg(0, file.end);
  size = file.tellg();
  file.seekg(0, file.beg);

  if (start + size >= ramSize - vramSize)
  {
    EXCEPT << "Image '" << image << "' too large";
  }

  if (!file.read((char*)(ram + start), size))
  {
    EXCEPT << "Cannot read '" << image << "'";
  }
}
