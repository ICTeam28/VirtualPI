// This file is part of the PiEMU Project
// Licensing information can be found in the LICENSE file
// (C) 2014 Nandor Licker. All rights reserved.
#include "common.h"


// -----------------------------------------------------------------------------
Exception::Exception(const std::string& file, int line, const std::string& func)
{
  size_t len;
  std::string name;

  if ((len = file.find_last_of('/')) != std::string::npos)
  {
    name = file.substr(len + 1);
  }
  else
  {
    name = file;
  }

  os << "[" << name << ":" << line << " " << func << "] ";
}
