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
  std::string name, ext, output, expected;
  std::ostringstream out;

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
      output = std::string(path) + "k";
    }

    if (type == FTW_F && ext == "o")
    {
      std::cerr << "[Running " << name << "] ";
      ++testCount;

      // Read in reference output
      std::ifstream t(output.c_str());
      if (!t.is_open())
      {
        throw std::runtime_error("Cannot open '" + output + "'");
      }

      expected = std::string(std::istreambuf_iterator<char>(t),
                             std::istreambuf_iterator<char>());

      // Run the emulator
      out.clear();
      {
        args.image = path;
        Emulator emu(args);
        emu.Run();
        emu.DumpState(out);
      }

      if (expected == out.str())
      {
        ++testPass;
        std::cerr << "Passed" << std::endl;
      }
      else
      {
        std::cerr << "Failed" << std::endl
                  << "Output:" << std::endl << out.str() << std::endl
                  << "Expected:" << std::endl <<expected << std::endl;
      }
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
    std::cerr << "Not enough arguments" << std::endl;
    return EXIT_FAILURE;
  }

  if (ftw(argv[1], RunTest, 5) == -1)
  {
    std::cerr << "Error: Traversal failed" << std::endl;
    return EXIT_FAILURE;
  }

  std::cout << "Result: " << testPass << "/" << testCount << std::endl;
  return EXIT_SUCCESS;
}

