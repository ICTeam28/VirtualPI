// This file is part of the PiEMU Project
// Licensing information can be found in the LICENSE file
// (C) 2014 Nandor Licker. All rights reserved.
#ifndef __EMULATOR_H__
#define __EMULATOR_H__

/**
 * Emulator instance
 */
class Emulator
{
public:

  /**
   * Emulator arguments
   */
  struct Args
  {
    /**
     * RAM + VRAM
     */
    size_t ram;

    /**
     * VRAM
     */
    size_t vram;

    /**
     * Path to image
     */
    std::string image;
  };

  /**
   * Creates a new emulator instance
   */
  Emulator(const Args& args);

  /**
   * Copy constructor forbidden
   */
  Emulator(const Emulator&) =delete;

  /**
   * Destroys an emulator instance
   */
  ~Emulator();

  /**
   * Runs the emulator
   */
  void Run();

  /**
   * Copying forbidden
   */
  Emulator& operator = (const Emulator&) =delete;

  /**
   * Dumps the THUMB state
   */
  void DumpState(std::ostream& os);

private:

  /// Configuration
  const Args &args;

  /// Memory module
  Memory mem;

  /// ARM state
  ARMData armData;

  /// Thumb
  friend void ThumbExecute(Emulator *emu);
};

#endif /*__EMULATOR_H__*/
