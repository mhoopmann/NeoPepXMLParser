#include "NeoPepXMLParser/NeoPepXMLStructs.h"

#include <cstdlib>
#include <cstring>
#include <iostream>

// std::from_chars is locale-independent by specification. MSVC and libstdc++ (GCC 11 and
// later) implement the floating-point overloads and advertise them through
// __cpp_lib_to_chars. libc++ historically did not, so those builds fall back to strtod
// evaluated in an explicit "C" locale.
#if __has_include(<charconv>)
#include <charconv>
#endif
#if defined(__cpp_lib_to_chars) && __cpp_lib_to_chars >= 201611L
#define NPX_HAVE_FROM_CHARS 1
#else
#define NPX_HAVE_FROM_CHARS 0
#include <clocale>
#if defined(__APPLE__)
#include <xlocale.h>
#endif
#endif

const std::string npx_xmlns = "http://regis-web.systemsbiology.net/pepXML";
const std::string npx_xmlns_xsi = "http://www.w3.org/2001/XMLSchema-instance";
const std::string npx_xsi_schemaLocation = "http://regis-web.systemsbiology.net/pepXML /tools/bin/TPP/tpp/schema/pepXML_v123.xsd";

void npxDateTime::clear() {
  date.day = 0;
  date.month = 0;
  date.year = 0;
  time.hour = 0;
  time.minute = 0;
  time.second = 0;
}

void npxDateTime::parseDateTime(const char* dt) {
  if (strlen(dt) < 2) {
    clear();
    return;
  }
  // Integer conversions are not affected by the process locale.
  sscanf(dt, "%d-%d-%dT%d:%d:%d", &date.year, &date.month, &date.day, &time.hour, &time.minute, &time.second);
}

void npxDateTime::parseDateTime(const std::string& s) {
  parseDateTime(s.c_str());
}

std::string npxDateTime::write() {
  char str[64];
  snprintf(str, sizeof(str), "%4d-%02d-%02dT%02d:%02d:%02d", date.year, date.month, date.day, time.hour, time.minute, time.second);
  return std::string(str);
}

double npxAtof(const char* s) {
  // Match atof's leniency: leading whitespace and an explicit plus sign are accepted.
  while (*s == ' ' || *s == '\t' || *s == '\n' || *s == '\r' || *s == '\f' || *s == '\v') ++s;
#if NPX_HAVE_FROM_CHARS
  if (*s == '+') ++s;
  double value = 0.0;
  // On failure from_chars leaves value untouched, so an unparsable string yields 0.0 just as
  // it would with atof.
  std::from_chars(s, s + strlen(s), value);
  return value;
#else
  static const locale_t cLocale = newlocale(LC_NUMERIC_MASK, "C", (locale_t)0);
  if (cLocale == (locale_t)0) return strtod(s, nullptr);
  return strtod_l(s, nullptr, cLocale);
#endif
}

void NPXerrMsg(const std::string& el, const std::string& attr) {
  std::cerr << el << "::" << attr << " required." << std::endl;
  exit(69);
}

void NPXprintTabs(FILE* f, int tabs) {
  for (int i = 0; i < tabs; i++) fprintf(f, " ");
}
