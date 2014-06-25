// This file is part of the PiEMU Project
// Licensing information can be found in the LICENSE file
// (C) 2014 Nandor Licker. All rights reserved.
#include "common.h"


// -----------------------------------------------------------------------------
static int testCount = 0;
static int testPass = 0;


// -----------------------------------------------------------------------------
int RunTest(const char *path, const struct stat *sb, int type)
{
  size_t len;
  std::string name, ext;

  Emulator::Args args;
  args.ram = 64 << 20;
  args.vram = 32 << 20;
  args.image = "";

  try
  {
    name = path;
    ext = "";
    if ((len = name.find_last_of('/')) != std::string::npos)
    {
      name = name.substr(len + 1);
    }

    if ((len = name.find_last_of('.')) != std::string::npos)
    {
      ext = name.substr(len + 1);
    }

    if (type == FTW_F && ext == "o")
    {
      std::cerr << "[Running " << name << "] ";
      ++testCount;

      args.image = path;
      (Emulator(args)).Run();

      ++testPass;
      std::cerr << "Passed" << std::endl;
    }
  }
  catch (std::exception &e)
  {
    std::cerr << "Failed: " <<e.what() << std::endl;
  }
  catch (...)
  {
    std::cerr << "Failed: Unknown error" << std::endl;
  }

  return 0;
}


// -----------------------------------------------------------------------------
int main(int argc, char **argv)
{
  if (argc != 2)
  {
    /* LCOV_EXCL_START */
    std::cerr << "Not enough arguments" << std::endl;
    return EXIT_FAILURE;
    /* LCOV_EXCL_END */
  }

  if (ftw(argv[1], RunTest, 5) == -1)
  {
    /* LCOV_EXCL_START */
    std::cerr << "Error: Traversal failed" << std::endl;
    return EXIT_FAILURE;
    /* LCOV_EXCL_END */
  }

  std::cout << "Result: " << testPass << "/" << testCount << std::endl;
  return EXIT_SUCCESS;
}
