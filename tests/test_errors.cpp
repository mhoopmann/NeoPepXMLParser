// Failure paths report through the return value and lastError(); out-of-range accessors throw.
#include "NeoPepXMLParser/NeoPepXMLParser.h"
#include "test_support.h"

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>

using Catch::Matchers::ContainsSubstring;
using namespace npxtest;

TEST_CASE("a successful read leaves no error and no warnings", "[errors]") {
  NeoPepXMLParser xml;
  REQUIRE(xml.read(dataPath(fixtureName()).c_str()));
  CHECK(xml.lastError().empty());
  CHECK(xml.warnings().empty());
}

TEST_CASE("reading a file that does not exist returns false and says so", "[errors]") {
  NeoPepXMLParser xml;
  CHECK_FALSE(xml.read(outputPath("does_not_exist.pep.xml").c_str()));
  CHECK_THAT(xml.lastError().message, ContainsSubstring("cannot open"));
  CHECK_THAT(xml.lastError().message, ContainsSubstring("does_not_exist"));
  CHECK(xml.lastError().line == 0);
}

TEST_CASE("reading an empty file returns false", "[errors]") {
  const std::string path = outputPath("empty.pep.xml");
  REQUIRE(writeBytes(path, ""));
  NeoPepXMLParser xml;
  CHECK_FALSE(xml.read(path.c_str()));
  CHECK_FALSE(xml.lastError().empty());
}

TEST_CASE("reading a truncated document reports the XML error and its line", "[errors]") {
  const std::string whole = readBytes(dataPath(fixtureName()));
  REQUIRE(whole.size() > 4000);
  const std::string path = outputPath("truncated.pep.xml");
  REQUIRE(writeBytes(path, whole.substr(0, 4000)));
  NeoPepXMLParser xml;
  CHECK_FALSE(xml.read(path.c_str()));
  CHECK_THAT(xml.lastError().message, ContainsSubstring("XML"));
  CHECK(xml.lastError().line > 0);
  CHECK_THAT(xml.lastError().text(), ContainsSubstring("line "));
}

TEST_CASE("an unknown element is skipped with a warning and everything else is intact", "[errors]") {
  std::string text = readBytes(dataPath(fixtureName()));
  const std::string anchor = "<search_result>\n";
  const size_t pos = text.find(anchor);
  REQUIRE(pos != std::string::npos);
  text.insert(pos + anchor.size(),
              "<mystery_element kind=\"new\"><mystery_child>text</mystery_child></mystery_element>\n");
  const std::string in = outputPath("errors_unknown_element.pep.xml");
  REQUIRE(writeBytes(in, text));

  NeoPepXMLParser xml;
  REQUIRE(xml.read(in.c_str()));
  CHECK(xml.lastError().empty());
  REQUIRE(xml.warnings().size() == 1);
  CHECK(xml.warnings()[0].element == "mystery_element");
  CHECK(xml.warnings()[0].line > 0);
  CHECK_THAT(xml.warnings()[0].message, ContainsSubstring("mystery_element"));
  CHECK(xml.size() == 30);
  CHECK(xml[0].peptide == "AKAGLLPVL");

  // The skipped element leaves no trace in the output.
  const std::string out = outputPath("errors_unknown_element_out.pep.xml");
  REQUIRE(xml.write(out.c_str()));
  CHECK(readBytes(out) == readBytes(dataPath(std::string("expected/") + fixtureName())));
}

TEST_CASE("writing to an unwritable location returns false and says so", "[errors]") {
  NeoPepXMLParser xml;
  xml.addMSMSPipelineAnalysis("2026-09-10T00:00:00", "x.pep.xml");
  CHECK_FALSE(xml.write(outputPath("no_such_dir/out.pep.xml").c_str()));
  CHECK_THAT(xml.lastError().message, ContainsSubstring("cannot open"));
}

TEST_CASE("a document built from scratch with empty fields still writes", "[errors]") {
  NeoPepXMLParser xml;
  xml.addMSMSPipelineAnalysis("", "");
  CnpxMSMSRunSummary* run = xml.msms_pipeline_analysis[0].addMSMSRunSummary("", "", "");
  CnpxSearchSummary* summary = run->addSearchSummary("", "", "", "", 0);
  summary->addParameter("empty", "");
  const std::string out = outputPath("errors_from_scratch.pep.xml");
  REQUIRE(xml.write(out.c_str()));
  CHECK(xml.lastError().empty());
  CHECK_THAT(readBytes(out), ContainsSubstring("<parameter name=\"empty\" value=\"\"/>"));
}

TEST_CASE("out-of-range accessors throw npxRangeError", "[errors]") {
  NeoPepXMLParser xml;
  REQUIRE(xml.read(dataPath(fixtureName()).c_str()));
  CHECK_THROWS_AS(xml[30], npxRangeError);
  CHECK_THROWS_AS(xml.uiPipelines[1], npxRangeError);
  CHECK_THROWS_AS(xml.uiRunSummaries[1], npxRangeError);
  CHECK_THROWS_AS(xml.uiSpectra[30], npxRangeError);
  CHECK_THROWS_AS(xml.uiSpectra.getHit(0, 0), npxRangeError);   // ranks start at 1
  CHECK_THROWS_AS(xml.uiSpectra.getHit(0, 2), npxRangeError);   // only one hit
  CHECK_THROWS_AS(xml.uiSpectra.getHit(30, 1), npxRangeError);
  CHECK(xml.uiSpectra.getHit(0, 1).peptide == "AKAGLLPVL");
  CHECK(xml[29].peptide == "IVGCSVHK");

  NeoPepXMLParser empty;  // nothing loaded at all
  CHECK_THROWS_AS(empty.uiPipelines[0], npxRangeError);
  CHECK_THROWS_AS(empty.uiSpectra[0], npxRangeError);
  CHECK_THROWS_AS(empty.uiSpectra.getHit(0, 1), npxRangeError);
  CHECK_THROWS_AS(empty[0], npxRangeError);
}
