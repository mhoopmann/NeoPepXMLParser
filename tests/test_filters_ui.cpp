// The read-time filters and the CnpxUI* convenience layer.
//
// One test is tagged [!mayfail]: it describes behavior the code does not have yet, so it reports
// without failing the run. Remove the tag when the underlying issue is fixed:
//   - CnpxUIPSM::setPSM() fills a parameter entry for each Prophet score but never pushes it,
//     so peptideProphet.parameters and iProphet.parameters stay empty.
#include "NeoPepXMLParser/NeoPepXMLParser.h"
#include "test_support.h"

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

using Catch::Approx;
using namespace npxtest;

namespace {
const std::vector<CnpxSpectrumQuery>& queries(NeoPepXMLParser& xml) {
  return xml.msms_pipeline_analysis[0].msms_run_summary[0].spectrum_query;
}
double iprophet(const CnpxSpectrumQuery& q) {
  return q.search_result[0].search_hit[0].analysis_result[1].interprophet_result.probability;
}
}  // namespace

TEST_CASE("probability filter keeps queries at or above the threshold", "[filters]") {
  SECTION("0.99 keeps the ten confident identifications") {
    NeoPepXMLParser xml;
    xml.setFilterProbability(0.99);
    REQUIRE(xml.read(dataPath(fixtureName()).c_str()));
    CHECK(xml.size() == 10);
    for (const CnpxSpectrumQuery& q : queries(xml)) CHECK(iprophet(q) >= 0.99);
  }
  SECTION("0.9 keeps thirteen") {
    NeoPepXMLParser xml;
    xml.setFilterProbability(0.9);
    REQUIRE(xml.read(dataPath(fixtureName()).c_str()));
    CHECK(xml.size() == 13);
  }
}

TEST_CASE("search-hit filter keeps queries whose top hit is the given peptide", "[filters]") {
  NeoPepXMLParser xml;
  xml.setFilterSearchHit("AKAGLLPVL");
  REQUIRE(xml.read(dataPath(fixtureName()).c_str()));
  REQUIRE(xml.size() == 1);
  CHECK(xml[0].peptide == "AKAGLLPVL");
}

TEST_CASE("run-summary filter keeps run summaries whose base name contains the string", "[filters]") {
  SECTION("a matching substring keeps the run") {
    NeoPepXMLParser xml;
    xml.setFilterRunSummary("20260415_Human");
    REQUIRE(xml.read(dataPath(fixtureName()).c_str()));
    CHECK(xml.msms_pipeline_analysis[0].msms_run_summary.size() == 1);
    CHECK(xml.size() == 30);
  }
  SECTION("a non-matching string drops the run") {
    NeoPepXMLParser xml;
    xml.setFilterRunSummary("no_such_run");
    REQUIRE(xml.read(dataPath(fixtureName()).c_str()));
    CHECK(xml.msms_pipeline_analysis[0].msms_run_summary.empty());
    CHECK(xml.size() == 0);
  }
}

TEST_CASE("run-summary filter leaves search hits alone", "[filters]") {
  NeoPepXMLParser xml;
  xml.setFilterRunSummary("20260415_Human");
  REQUIRE(xml.read(dataPath(fixtureName()).c_str()));
  for (const CnpxSpectrumQuery& q : queries(xml)) CHECK(q.search_result[0].search_hit.size() == 1);
}

TEST_CASE("UI layer exposes pipelines, runs, spectra, and PSMs", "[ui]") {
  NeoPepXMLParser xml;
  REQUIRE(xml.read(dataPath(fixtureName()).c_str()));
  CHECK(xml.uiPipelines.size() == 1);
  CHECK(xml.uiRunSummaries.size() == 1);
  CHECK(xml.uiRunSummaries[0].base_name == "data/20260415_Human_DDA_A_01");
  CHECK(xml.uiSpectra.size() == 30);
  CHECK(xml.uiSpectra[0].spectrum == "20260415_Human_DDA_A_01.00008.00008.2");
  CHECK(xml.uiSpectra.getPipelineIndex() == 0);
  CHECK(xml.uiSpectra.getRunSummaryIndex() == 0);

  SECTION("selecting runs and spectra reports out-of-range indexes") {
    CHECK(xml.setRunSummaries(0));
    CHECK_FALSE(xml.setRunSummaries(1));
    CHECK(xml.setSpectra(0, 0));
    CHECK_FALSE(xml.setSpectra(0, 1));
    CHECK_FALSE(xml.setSpectra(1, 0));
  }
  SECTION("a plain PSM") {
    CnpxUIPSM& p = xml[0];
    CHECK(p.spectrumID == "20260415_Human_DDA_A_01.00008.00008.2");
    CHECK(p.scanNumber == 8);
    CHECK(p.assumed_charge == 2);
    CHECK(p.precursorNeutralMass == Approx(880.583145));
    CHECK(p.calcNeutralMass == Approx(880.574589));
    CHECK(p.retentionTime == Approx(2.1));
    CHECK(p.peptide == "AKAGLLPVL");
    CHECK_FALSE(p.modified);
    CHECK(p.xlType == 0);
    REQUIRE(p.proteins.size() == 1);
    CHECK(p.proteins[0].protein == "DECOY0_sp_19303");
    CHECK(p.proteins[0].prevAA == 'R');
    CHECK(p.proteins[0].nextAA == 'A');
    REQUIRE(p.scores.size() == 5);
    CHECK(p.scores[0].name == "xcorr");
    CHECK(p.scores[0].value == "0.3640");
    CHECK(p.hasPeptideProphet);
    CHECK(p.hasIProphet);
    CHECK(p.peptideProphet.probability == Approx(0.0));
  }
  SECTION("a modified PSM with alternative proteins") {
    CnpxUIPSM& p = xml[28];
    CHECK(p.peptide == "HQGVMVGMGQK");
    CHECK(p.modified);
    CHECK(p.mod.modifiedPeptide == "HQGVM[147]VGM[147]GQK");
    REQUIRE(p.mod.mods.size() == 2);
    CHECK(p.mod.mods[0].pos == 5);
    CHECK(p.mod.mods[0].mass == Approx(147.035385));
    CHECK(p.mod.mods[1].pos == 8);
    CHECK(p.proteins.size() == 7);
    CHECK(p.proteins[0].protein == "sp|P62736|ACTA_HUMAN");
    CHECK(p.peptideProphet.probability == Approx(0.1416));
    CHECK(p.iProphet.probability == Approx(0.97406));
  }
  SECTION("all sixteen proteins of a shared peptide") {
    CHECK(xml[5].proteins.size() == 16);
  }
}

TEST_CASE("UI PSM carries the Prophet score parameters", "[ui][!mayfail]") {
  NeoPepXMLParser xml;
  REQUIRE(xml.read(dataPath(fixtureName()).c_str()));
  CnpxUIPSM& p = xml[0];
  CHECK(p.peptideProphet.parameters.size() == 7);
  CHECK(p.iProphet.parameters.size() == 4);
}

TEST_CASE("search-hit filter drops a query that has no hits instead of indexing one", "[filters]") {
  // Remove the first query's only search_hit from a copy of the fixture. Its peptide was the one
  // the filter asks for, so with the hit gone nothing may match, and nothing may crash.
  std::string text = readBytes(dataPath(fixtureName()));
  const size_t begin = text.find("<search_hit ");
  const size_t end = text.find("</search_hit>", begin);
  REQUIRE(begin != std::string::npos);
  REQUIRE(end != std::string::npos);
  text.erase(begin, end + std::string("</search_hit>").size() - begin);
  const std::string path = outputPath("filters_hitless_query.pep.xml");
  REQUIRE(writeBytes(path, text));

  NeoPepXMLParser xml;
  xml.setFilterSearchHit("AKAGLLPVL");
  REQUIRE(xml.read(path.c_str()));
  CHECK(xml.size() == 0);
}
