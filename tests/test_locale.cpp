// Host applications may run under a locale whose decimal separator is a comma. Neither parsing
// nor writing may care.
#include "NeoPepXMLParser/NeoPepXMLParser.h"
#include "test_support.h"

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include <clocale>
#include <cstdio>
#include <cstring>

using Catch::Approx;
using namespace npxtest;

namespace {

// Switches the process to a comma-decimal locale for the lifetime of the object. `active` is
// false when the machine has no such locale, which is common on minimal Linux images.
struct CommaLocale {
  bool active = false;
  CommaLocale() {
    const char* candidates[] = {"de_DE.UTF-8", "de_DE.utf8", "de-DE", "de_DE", "German_Germany.1252"};
    for (const char* name : candidates) {
      if (std::setlocale(LC_ALL, name) != nullptr) break;
    }
    char buffer[16];
    std::snprintf(buffer, sizeof(buffer), "%.1f", 1.5);
    active = std::strchr(buffer, ',') != nullptr;
    if (!active) std::setlocale(LC_ALL, "C");
  }
  ~CommaLocale() { std::setlocale(LC_ALL, "C"); }
};

}  // namespace

TEST_CASE("parsing is unaffected by a comma-decimal locale", "[locale]") {
  CommaLocale locale;
  if (!locale.active) SKIP("no comma-decimal locale is available on this machine");

  NeoPepXMLParser xml;
  REQUIRE(xml.read(dataPath(fixtureName()).c_str()));
  const CnpxMSMSPipelineAnalysis& p = xml.msms_pipeline_analysis[0];
  const CnpxMSMSRunSummary& r = p.msms_run_summary[0];

  CHECK(p.analysis_summary[0].interprophet_summary[0].est_tot_num_correct_psm == Approx(36510.9));
  CHECK(p.analysis_summary[4].decoy_analysis[0].point[0].num_corr_pp == Approx(16308.0000000003));
  CHECK(r.search_summary[0].aminoacid_modification[0].massdiff == Approx(15.9949));
  CHECK(r.spectrum_query[0].precursor_neutral_mass == Approx(880.583145));
  CHECK(r.spectrum_query[0].retention_time_sec == Approx(2.1));
  CHECK(r.spectrum_query[5].search_result[0].search_hit[0].analysis_result[1].interprophet_result.probability == Approx(0.999999));
  CHECK(r.spectrum_query[28].search_result[0].search_hit[0].modification_info[0].mod_aminoacid_mass[0].mass == Approx(147.035385));
}

TEST_CASE("writing is unaffected by a comma-decimal locale", "[locale]") {
  CommaLocale locale;
  if (!locale.active) SKIP("no comma-decimal locale is available on this machine");

  const std::string out = outputPath("locale_write.pep.xml");
  NeoPepXMLParser xml;
  REQUIRE(xml.read(dataPath(fixtureName()).c_str()));
  REQUIRE(xml.write(out.c_str()));
  const std::string written = readBytes(out);
  const std::string expected = readBytes(dataPath(std::string("expected/") + fixtureName()));
  INFO(describeDifference(written, expected));
  CHECK(written == expected);

  // The modified-peptide string formats masses as well.
  CnpxSearchHit& hit = xml.msms_pipeline_analysis[0].msms_run_summary[0].spectrum_query[28].search_result[0].search_hit[0];
  CHECK(hit.getModifiedPeptide() == "HQGVM[147.04]VGM[147.04]GQK");

  // And the guard restores the caller's locale afterwards.
  char buffer[16];
  std::snprintf(buffer, sizeof(buffer), "%.1f", 1.5);
  CHECK(std::strchr(buffer, ',') != nullptr);
}
