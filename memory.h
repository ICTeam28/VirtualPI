// This file is part of the PiEMU Project
// Licensing information can be found in the LICENSE file
// (C) 2014 Nandor Licker. All rights reserved.
#ifndef __MEMORY_H__
#define __MEMORY_H__


/**
 * Emulator foward declaration
 */
class Emulator;


/**
 * Memory module
 */
class Memory
{
public:

  /**
   * Creates a new memory module
   */
  Memory(Emulator &emu, uint32_t sdramSize, uint32_t vramSize);

  /**
   * Destroys the memory module
   */
  ~Memory();

  /**
   * Loads an image into RAM
   * @param image Path to an image file
   * @param start Address of the image file
   */
  void LoadImage(const std::string& image, size_t start);

  /**
   * Returns a word from memory
   */
  uint16_t GetInstrWord(uint32_t addr)
  {
    return *((uint16_t*)(ram + addr));
  }

  /**
   * Returns a long from memory
   */
  uint32_t GetLong(uint32_t addr)
  {
    return *((uint32_t*)(ram + addr));
  }

private:

  /// Reference to the emulator
  Emulator &emu;

  /// Size of SDRAM
  uint32_t ramSize;

  /// RAM
  uint8_t *ram;

  /// Size of VRAM
  uint32_t vramSize;

  /// VRAM (pointer inside sdram)
  uint8_t *vram;

  /// ROM (pointer inside sdram)
  uint8_t *rom;
};


#endif /*__MEMORY_H__*/
