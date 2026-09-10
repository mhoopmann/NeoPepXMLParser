// Hand-verified values from representative spectrum queries in the fixture.
#include "NeoPepXMLParser/NeoPepXMLParser.h"
#include "test_support.h"

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

using Catch::Approx;
using namespace npxtest;

namespace {
const CnpxSpectrumQuery& query(NeoPepXMLParser& xml, size_t i) {
  return xml.msms_pipeline_analysis[0].msms_run_summary[0].spectrum_query[i];
}
CnpxSearchHit& hit(NeoPepXMLParser& xml, size_t i) {
  return xml.msms_pipeline_analysis[0].msms_run_summary[0].spectrum_query[i].search_result[0].search_hit[0];
}
}  // namespace

TEST_CASE("spectrum queries and search hits", "[values]") {
  NeoPepXMLParser xml;
  REQUIRE(xml.read(dataPath(fixtureName()).c_str()));
  REQUIRE(xml.msms_pipeline_analysis[0].msms_run_summary[0].spectrum_query.size() == 30);
  CHECK(xml.size() == 30);

  SECTION("first query: a charge 2 decoy with probability zero") {
    const CnpxSpectrumQuery& q = query(xml, 0);
    CHECK(q.spectrum == "20260415_Human_DDA_A_01.00008.00008.2");
    CHECK(q.start_scan == 8);
    CHECK(q.end_scan == 8);
    CHECK(q.index == 1);
    CHECK(q.assumed_charge == 2);
    CHECK(q.precursor_neutral_mass == Approx(880.583145));
    CHECK(q.retention_time_sec == Approx(2.1));
    REQUIRE(q.search_result.size() == 1);
    REQUIRE(q.search_result[0].search_hit.size() == 1);
    CnpxSearchHit& h = hit(xml, 0);
    CHECK(h.hit_rank == 1);
    CHECK(h.peptide == "AKAGLLPVL");
    CHECK(h.peptide_prev_aa == "R");
    CHECK(h.peptide_next_aa == "A");
    CHECK(h.protein == "DECOY0_sp_19303");
    CHECK(h.num_tot_proteins == 1);
    CHECK(h.num_matched_ions == 1);
    CHECK(h.tot_num_ions == 16);
    CHECK(h.calc_neutral_pep_mass == Approx(880.574589));
    CHECK(h.massdiff == Approx(0.008556));
    CHECK(h.modification_info.empty());
    REQUIRE(h.search_score.size() == 5);
    CHECK(h.search_score[0].name == "xcorr");
    CHECK(h.search_score[0].value == "0.3640");
    CHECK(h.search_score[4].name == "expect");
    CHECK(h.search_score[4].value == "4.83E+01");

    REQUIRE(h.analysis_result.size() == 2);
    CnpxAnalysisResult& pp = h.analysis_result[0];
    CHECK(pp.analysis == "peptideprophet");
    REQUIRE(pp.peptide_prophet_result.present());
    CHECK(pp.peptide_prophet_result.probability == Approx(0.0));
    CHECK(pp.peptide_prophet_result.all_ntt_prob == "(0.0000,0.0000,0.0000)");
    REQUIRE(pp.peptide_prophet_result.search_score_summary.parameter.size() == 7);
    CHECK(pp.peptide_prophet_result.search_score_summary.parameter[0].name == "fval");
    CHECK(pp.peptide_prophet_result.search_score_summary.parameter[0].value == "-3.8980");
    CnpxAnalysisResult& ip = h.analysis_result[1];
    CHECK(ip.analysis == "interprophet");
    REQUIRE(ip.interprophet_result.present());
    CHECK(ip.interprophet_result.probability == Approx(0.0));
    REQUIRE(ip.interprophet_result.search_score_summary.parameter.size() == 4);
    CHECK(ip.interprophet_result.search_score_summary.parameter[0].name == "nrs");
    CHECK(ip.interprophet_result.search_score_summary.parameter[0].value == "-1.5");
  }

  SECTION("sixth query: a confident hit shared by sixteen proteins") {
    CnpxSearchHit& h = hit(xml, 5);
    CHECK(h.peptide == "HAVSEGTK");
    CHECK(h.protein == "sp|P57053|H2BFS_HUMAN");
    CHECK(h.num_tot_proteins == 16);
    CHECK(h.alternative_protein.size() == 15);
    CHECK(h.analysis_result[0].peptide_prophet_result.probability == Approx(1.0));
    CHECK(h.analysis_result[1].interprophet_result.probability == Approx(0.999999));
  }

  SECTION("variable modifications: two oxidized methionines") {
    CnpxSearchHit& h = hit(xml, 28);
    CHECK(h.peptide == "HQGVMVGMGQK");
    REQUIRE(h.modification_info.size() == 1);
    CHECK(h.modification_info[0].modified_peptide == "HQGVM[147]VGM[147]GQK");
    REQUIRE(h.modification_info[0].mod_aminoacid_mass.size() == 2);
    CHECK(h.modification_info[0].mod_aminoacid_mass[0].position == 5);
    CHECK(h.modification_info[0].mod_aminoacid_mass[0].mass == Approx(147.035385));
    CHECK(h.modification_info[0].mod_aminoacid_mass[1].position == 8);
    CHECK(h.getModifiedPeptide() == "HQGVM[147.04]VGM[147.04]GQK");
    CHECK(h.analysis_result[0].peptide_prophet_result.probability == Approx(0.1416));
    CHECK(h.analysis_result[1].interprophet_result.probability == Approx(0.97406));
  }

  SECTION("static modifications: carbamidomethyl cysteines on a charge 5 precursor") {
    const CnpxSpectrumQuery& q = query(xml, 10);
    CHECK(q.assumed_charge == 5);
    CHECK(q.precursor_neutral_mass == Approx(2226.939384));
    CnpxSearchHit& h = hit(xml, 10);
    CHECK(h.peptide == "CTGHSAEHTQCGKPQESTGR");
    REQUIRE(h.modification_info.size() == 1);
    CHECK(h.modification_info[0].modified_peptide == "CTGHSAEHTQCGKPQESTGR");
    REQUIRE(h.modification_info[0].mod_aminoacid_mass.size() == 2);
    CHECK(h.modification_info[0].mod_aminoacid_mass[0].position == 1);
    CHECK(h.modification_info[0].mod_aminoacid_mass[1].position == 11);
    CHECK(h.modification_info[0].mod_aminoacid_mass[1].mass == Approx(160.030649));
  }

  SECTION("every decoy hit names a DECOY protein and every target hit does not") {
    int decoys = 0;
    for (size_t i = 0; i < 30; ++i) {
      const std::string& protein = hit(xml, i).protein;
      if (protein.compare(0, 5, "DECOY") == 0) ++decoys;
      else CHECK(protein.compare(0, 3, "sp|") == 0);
    }
    CHECK(decoys == 9);
  }
}
