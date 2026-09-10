// npxAtof and npxDateTime: the small helpers every parsed number and timestamp passes through.
#include "NeoPepXMLParser/NeoPepXMLStructs.h"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("npxAtof converts decimal text like atof", "[numeric]") {
  CHECK(npxAtof("1.5") == 1.5);
  CHECK(npxAtof("880.583145") == 880.583145);
  CHECK(npxAtof("-0.984016") == -0.984016);
  CHECK(npxAtof("4.83E+01") == 48.3);
  CHECK(npxAtof("3e2") == 300.0);
  CHECK(npxAtof("0") == 0.0);
}

TEST_CASE("npxAtof keeps atof's leniency", "[numeric]") {
  SECTION("leading whitespace is skipped") { CHECK(npxAtof("  \t2.5") == 2.5); }
  SECTION("an explicit plus sign is accepted") { CHECK(npxAtof("+2.25") == 2.25); }
  SECTION("conversion stops at the first non-numeric character") { CHECK(npxAtof("12abc") == 12.0); }
  SECTION("no number at all yields zero") {
    CHECK(npxAtof("abc") == 0.0);
    CHECK(npxAtof("") == 0.0);
    CHECK(npxAtof("   ") == 0.0);
  }
}

TEST_CASE("npxDateTime parses and writes the pepXML timestamp format", "[numeric]") {
  npxDateTime dt;
  dt.parseDateTime("2026-04-15T15:36:33");
  CHECK(dt.date.year == 2026);
  CHECK(dt.date.month == 4);
  CHECK(dt.date.day == 15);
  CHECK(dt.time.hour == 15);
  CHECK(dt.time.minute == 36);
  CHECK(dt.time.second == 33);
  CHECK(dt.write() == "2026-04-15T15:36:33");

  SECTION("single-digit fields are zero-padded on output") {
    dt.parseDateTime("2025-1-2T3:4:5");
    CHECK(dt.write() == "2025-01-02T03:04:05");
  }
  SECTION("a string shorter than two characters clears the value") {
    dt.parseDateTime("");
    CHECK(dt.date.year == 0);
    CHECK(dt.time.second == 0);
    CHECK(dt.write() == "   0-00-00T00:00:00");
  }
  SECTION("the std::string overload matches the C-string one") {
    npxDateTime other;
    other.parseDateTime(std::string("2026-04-15T15:36:33"));
    CHECK(other.write() == dt.write());
  }
}
