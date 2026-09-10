// Failure paths that must report false instead of crashing.
#include "NeoPepXMLParser/NeoPepXMLParser.h"
#include "test_support.h"

#include <catch2/catch_test_macros.hpp>

using namespace npxtest;

TEST_CASE("reading a file that does not exist returns false", "[errors]") {
  NeoPepXMLParser xml;
  CHECK_FALSE(xml.read(outputPath("does_not_exist.pep.xml").c_str()));
}

TEST_CASE("reading an empty file returns false", "[errors]") {
  const std::string path = outputPath("empty.pep.xml");
  REQUIRE(writeBytes(path, ""));
  NeoPepXMLParser xml;
  CHECK_FALSE(xml.read(path.c_str()));
}

TEST_CASE("reading a truncated document returns false", "[errors]") {
  const std::string whole = readBytes(dataPath(fixtureName()));
  REQUIRE(whole.size() > 4000);
  const std::string path = outputPath("truncated.pep.xml");
  REQUIRE(writeBytes(path, whole.substr(0, 4000)));
  NeoPepXMLParser xml;
  CHECK_FALSE(xml.read(path.c_str()));
}

TEST_CASE("writing to an unwritable location returns false", "[errors]") {
  NeoPepXMLParser xml;
  xml.addMSMSPipelineAnalysis("2026-09-10T00:00:00", "x.pep.xml");
  CHECK_FALSE(xml.write(outputPath("no_such_dir/out.pep.xml").c_str()));
}
