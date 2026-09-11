#ifndef NEOPEPXMLERROR_H
#define NEOPEPXMLERROR_H

#include "NeoPepXMLParser/NeoPepXMLExport.h"

#include <stdexcept>
#include <string>

// One recorded error or warning. NeoPepXMLParser::lastError() holds why the last read() or
// write() failed; NeoPepXMLParser::warnings() lists what the last read() skipped or tolerated.
struct NEOPEPXML_EXPORT npxDiagnostic {
  std::string message;   // what happened, in words
  std::string element;   // the element involved, or empty
  long line;             // 1-based line in the input, or 0 when not applicable

  npxDiagnostic() : line(0) {}
  bool empty() const { return message.empty(); }
  void clear() { message.clear(); element.clear(); line = 0; }
  // "message (element 'x', line N)", leaving out the parts that are not set.
  std::string text() const;
};

// Base of everything the library throws. Only the reference-returning accessors throw
// (NeoPepXMLParser::operator[] and the CnpxUI* classes); read() and write() report through
// their return value and lastError() instead, and never throw.
class NEOPEPXML_EXPORT npxError : public std::runtime_error {
public:
  explicit npxError(const std::string& what);
  virtual ~npxError();
};

// An index or rank outside the available range, or an accessor used before anything is loaded.
class NEOPEPXML_EXPORT npxRangeError : public npxError {
public:
  explicit npxRangeError(const std::string& what);
  virtual ~npxRangeError();
};

#endif
