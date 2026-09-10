// The std::filesystem::path overloads, including a file name outside the ASCII range. On Windows
// these go through the wide-character API; the const char* overloads could not open this file.
#include "NeoPepXMLParser/NeoPepXMLParser.h"
#include "test_support.h"

#include <catch2/catch_test_macros.hpp>

#include <filesystem>
#include <system_error>

using namespace npxtest;
namespace fs = std::filesystem;

TEST_CASE("path overloads read and write a file with a non-ASCII name", "[paths]") {
  const fs::path source = fs::path(dataPath(fixtureName()));
  const fs::path dir = fs::path(outputPath(""));
  // Universal character names keep this source file ASCII while the file names are not.
  const fs::path input = dir / fs::u8path(u8"fixture_üñî_测试.pep.xml");
  const fs::path output = dir / fs::u8path(u8"output_üñî_测试.pep.xml");

  std::error_code ec;
  fs::copy_file(source, input, fs::copy_options::overwrite_existing, ec);
  REQUIRE_FALSE(ec);

  NeoPepXMLParser xml;
  REQUIRE(xml.read(input));
  CHECK(xml.size() == 30);
  REQUIRE(xml.write(output));
  REQUIRE(fs::exists(output));

  NeoPepXMLParser again;
  REQUIRE(again.read(output));
  CHECK(again.size() == 30);

  const std::string expected = readBytes(dataPath(std::string("expected/") + fixtureName()));
  const std::string written = readBytes(output);
  INFO(describeDifference(written, expected));
  CHECK(written == expected);
}

TEST_CASE("a std::string converts to the path overload", "[paths]") {
  const std::string source = dataPath(fixtureName());
  NeoPepXMLParser xml;
  REQUIRE(xml.read(source));
  CHECK(xml.size() == 30);
}
