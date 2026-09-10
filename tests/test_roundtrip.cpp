// Read the fixture, write it, read that, write again. Nothing may be lost, the second write must
// reproduce the first byte for byte, and the first must match the committed expected output.
//
// The expected file (tests/data/expected/) is the writer's own output, committed once. When the
// output format changes on purpose, regenerate it deliberately and review the diff.
#include "NeoPepXMLParser/NeoPepXMLParser.h"
#include "test_support.h"

#include <catch2/catch_test_macros.hpp>

using namespace npxtest;

TEST_CASE("round trip of the fixture", "[roundtrip]") {
  const std::string fixture = dataPath(fixtureName());
  const std::string pass1 = outputPath("roundtrip_pass1.pep.xml");
  const std::string pass2 = outputPath("roundtrip_pass2.pep.xml");

  NeoPepXMLParser first;
  REQUIRE(first.read(fixture.c_str()));
  REQUIRE(first.write(pass1.c_str()));
  NeoPepXMLParser second;
  REQUIRE(second.read(pass1.c_str()));
  REQUIRE(second.write(pass2.c_str()));

  const std::string bytes1 = readBytes(pass1);
  const std::string bytes2 = readBytes(pass2);
  REQUIRE_FALSE(bytes1.empty());

  SECTION("a second write is byte-identical to the first") {
    INFO(describeDifference(bytes1, bytes2));
    CHECK(bytes1 == bytes2);
  }

  SECTION("every element survives with the same multiplicity") {
    const auto in = elementInventory(readBytes(fixture));
    const auto out = elementInventory(bytes1);
    REQUIRE_FALSE(in.empty());
    for (const auto& entry : in) {
      CAPTURE(entry.first);
      const auto found = out.find(entry.first);
      REQUIRE(found != out.end());
      CHECK(found->second == entry.second);
    }
    CHECK(in.size() == out.size());
  }

  SECTION("the written file matches the committed expected output") {
    const std::string expected = readBytes(dataPath(std::string("expected/") + fixtureName()));
    REQUIRE_FALSE(expected.empty());
    INFO(describeDifference(bytes1, expected));
    CHECK(bytes1 == expected);
  }

  SECTION("indented output carries the same data") {
    const std::string tabbed = outputPath("roundtrip_tabbed.pep.xml");
    const std::string pass3 = outputPath("roundtrip_pass3.pep.xml");
    REQUIRE(first.write(tabbed.c_str(), true));
    NeoPepXMLParser third;
    REQUIRE(third.read(tabbed.c_str()));
    REQUIRE(third.write(pass3.c_str()));
    const std::string bytes3 = readBytes(pass3);
    INFO(describeDifference(bytes1, bytes3));
    CHECK(bytes1 == bytes3);
  }
}

TEST_CASE("CRLF input parses identically and is written back with LF", "[roundtrip]") {
  // TPP itself writes LF, so a CRLF variant of the fixture is synthesized here; files that pass
  // through Windows text-mode tools commonly arrive that way.
  std::string crlf = readBytes(dataPath(fixtureName()));
  REQUIRE(crlf.find('\r') == std::string::npos);
  for (size_t pos = 0; (pos = crlf.find('\n', pos)) != std::string::npos; pos += 2) {
    crlf.replace(pos, 1, "\r\n");
  }
  const std::string in = outputPath("roundtrip_crlf_input.pep.xml");
  const std::string out = outputPath("roundtrip_crlf_output.pep.xml");
  REQUIRE(writeBytes(in, crlf));

  NeoPepXMLParser xml;
  REQUIRE(xml.read(in.c_str()));
  REQUIRE(xml.write(out.c_str()));
  const std::string written = readBytes(out);
  const std::string expected = readBytes(dataPath(std::string("expected/") + fixtureName()));
  CHECK(written.find('\r') == std::string::npos);
  INFO(describeDifference(written, expected));
  CHECK(written == expected);
}
