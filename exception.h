// This file is part of the PiEMU Project
// Licensing information can be found in the LICENSE file
// (C) 2014 Nandor Licker. All rights reserved.
#ifndef __EXCEPTION_H__
#define __EXCEPTION_H__

/**
 * Class supporting formatted error messages
 */
class Exception : public std::exception
{
public:

  /**
   * Constructs a new exception object
   */
  Exception()
    : os("")
  {
  }

  /**
   * Copy constructor
   */
  Exception(const Exception& e)
    : os(e.os.str())
  {
  }

  /**
   * Constructs a new exception object, including source information
   * @param file  Name of the throwing file
   * @param line  Line number
   * @param func  Name of the function
   */
  Exception(const std::string& file, int line, const std::string& func);

  /**
   * Returns the error message
   */
  const char *what() const throw()
  {
    return os.str().c_str();
  }

  /**
   * Appends a message to the end of stream
   * @param T type of data
   * @param t stuff to be written
   */
  template<typename T>
  Exception& operator << (const T& t)
  {
    os << t;
    return *this;
  }

private:

  /**
   * Format stream
   */
  std::ostringstream os;

};

/**
 * Macro to throw nice exceptions
 */
#define EXCEPT throw Exception(__FILE__, __LINE__, __FUNCTION__)

#endif /*__EXCEPTION_H__*/
