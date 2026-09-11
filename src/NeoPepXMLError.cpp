#include "NeoPepXMLParser/NeoPepXMLError.h"

#include <sstream>

std::string npxDiagnostic::text() const {
  std::ostringstream out;
  out << message;
  if (!element.empty() || line > 0) {
    out << " (";
    if (!element.empty()) out << "element '" << element << "'";
    if (!element.empty() && line > 0) out << ", ";
    if (line > 0) out << "line " << line;
    out << ")";
  }
  return out.str();
}

npxError::npxError(const std::string& what) : std::runtime_error(what) {}
npxError::~npxError() {}

npxRangeError::npxRangeError(const std::string& what) : npxError(what) {}
npxRangeError::~npxRangeError() {}
