// Hand-verified values from the fixture's summary sections. These pin the semantics of parsing:
// a formatting change shows up in the round-trip tests, a wrong number shows up here.
#include "NeoPepXMLParser/NeoPepXMLParser.h"
#include "test_support.h"

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

using Catch::Approx;
using namespace npxtest;

TEST_CASE("pipeline analysis and analysis summaries", "[values]") {
  NeoPepXMLParser xml;
  REQUIRE(xml.read(dataPath(fixtureName()).c_str()));
  REQUIRE(xml.msms_pipeline_analysis.size() == 1);
  CnpxMSMSPipelineAnalysis& p = xml.msms_pipeline_analysis[0];
  CHECK(p.date.write() == "2026-04-15T15:36:33");
  CHECK(p.summary_xml == "data/Human_DDA_A_01.ipro.pep.xml");

  REQUIRE(p.analysis_summary.size() == 5);
  CHECK(p.analysis_summary[0].analysis == "interprophet");
  CHECK(p.analysis_summary[1].analysis == "peptideprophet");
  CHECK(p.analysis_summary[2].analysis == "database_refresh");
  CHECK(p.analysis_summary[3].analysis == "interact");
  CHECK(p.analysis_summary[4].analysis == "decoy");
  CHECK(p.analysis_summary[4].time.write() == "2026-04-15T15:37:16");

  SECTION("iProphet summary") {
    REQUIRE(p.analysis_summary[0].interprophet_summary.size() == 1);
    const CnpxInterprophetSummary& ip = p.analysis_summary[0].interprophet_summary[0];
    CHECK(ip.options == "THREADS=16");
    CHECK(ip.est_tot_num_correct_psm == Approx(36510.9));
    CHECK(ip.est_tot_num_correct_pep == Approx(30669.3));
    REQUIRE(ip.inputfile.size() == 1);
    CHECK(ip.inputfile[0].name == "Human_DDA_A_01.pep.xml");
    REQUIRE(ip.roc_error_data.size() == 1);
    const CnpxROCErrorData& roc = ip.roc_error_data[0];
    CHECK(roc.charge == "all");
    REQUIRE(roc.roc_data_point.size() == 24);
    CHECK(roc.roc_data_point[0].min_prob == Approx(0.9999));
    CHECK(roc.roc_data_point[0].sensitivity == Approx(0.6853));
    CHECK(roc.roc_data_point[0].num_corr == 25020u);
    CHECK(roc.roc_data_point[0].num_incorr == 0u);
    REQUIRE(roc.error_point.size() == 30);
    CHECK(roc.error_point[0].min_prob == Approx(1.0));
    CHECK(roc.error_point[0].num_corr == 2u);
    REQUIRE(ip.mixturemodel.size() == 4);
    CHECK(ip.mixturemodel[0].name == "NRS");
    CHECK(ip.mixturemodel[0].pos_bandwidth == Approx(0.702f));
    CHECK(ip.mixturemodel[2].name == "NSM");
    CHECK(ip.mixturemodel[2].neg_bandwidth == Approx(0.20723f));
    CHECK(ip.mixturemodel[3].name == "NSP");
    for (const CnpxMixtureModel& m : ip.mixturemodel) CHECK(m.point.size() == 20);
  }
  SECTION("the PeptideProphet summary element is present but empty") {
    CHECK(p.analysis_summary[1].peptideprophet_summary.empty());
  }
  SECTION("interact summary") {
    REQUIRE(p.analysis_summary[3].interact_summary.size() == 1);
    const CnpxInteractSummary& is = p.analysis_summary[3].interact_summary[0];
    CHECK(is.filename == "data/Human_DDA_A_01.pep.xml");
    CHECK(is.directory == "data");
    REQUIRE(is.inputfile.size() == 1);
    CHECK(is.inputfile[0].name == "20260415_Human_DDA_A_01.pep.xml");
  }
  SECTION("decoy analysis, whose attributes are single-quoted in the file") {
    REQUIRE(p.analysis_summary[4].decoy_analysis_summary.size() == 1);
    CHECK(p.analysis_summary[4].decoy_analysis_summary[0].decoy_string == "DECOY");
    CHECK(p.analysis_summary[4].decoy_analysis_summary[0].uniq_iproph_peps == "Y");
    REQUIRE(p.analysis_summary[4].decoy_analysis.size() == 1);
    REQUIRE(p.analysis_summary[4].decoy_analysis[0].point.size() == 20);
    const npxPointD& pt = p.analysis_summary[4].decoy_analysis[0].point[0];
    CHECK(pt.num_corr_pp == Approx(16308.0000000003));
    CHECK(pt.num_corr_pp_decoy == Approx(16308.0));
    CHECK(pt.prob_cutoff == Approx(1.0));
  }
}

TEST_CASE("run summary and search summary", "[values]") {
  NeoPepXMLParser xml;
  REQUIRE(xml.read(dataPath(fixtureName()).c_str()));
  const CnpxMSMSRunSummary& r = xml.msms_pipeline_analysis[0].msms_run_summary[0];
  CHECK(r.base_name == "data/20260415_Human_DDA_A_01");
  CHECK(r.msManufacturer == "UNKNOWN");
  CHECK(r.raw_data_type == "raw");
  CHECK(r.raw_data == ".mzML");
  REQUIRE(r.sample_enzyme.size() == 1);
  CHECK(r.sample_enzyme[0].name == "trypsin");
  REQUIRE(r.sample_enzyme[0].specificity.size() == 1);
  CHECK(r.sample_enzyme[0].specificity[0].cut == "KR");
  CHECK(r.sample_enzyme[0].specificity[0].no_cut == "P");
  CHECK(r.sample_enzyme[0].specificity[0].sense == "C");
  REQUIRE(r.analysis_timestamp.size() == 3);
  CHECK(r.analysis_timestamp[0].analysis == "interprophet");
  CHECK(r.analysis_timestamp[2].analysis == "database_refresh");

  REQUIRE(r.search_summary.size() == 1);
  const CnpxSearchSummary& s = r.search_summary[0];
  CHECK(s.search_engine == "Comet");
  CHECK(s.search_engine_version == "2025.03 rev. 0 (0207ce7)");
  CHECK(s.precursor_mass_type == "monoisotopic");
  CHECK(s.fragment_mass_type == "monoisotopic");
  CHECK(s.search_id == 1);
  REQUIRE(s.search_database.size() == 1);
  CHECK(s.search_database[0].local_path == "data/UP000005640_human_DECOY.fasta");
  CHECK(s.search_database[0].type == "AA");
  REQUIRE(s.enzymatic_search_constraint.size() == 1);
  CHECK(s.enzymatic_search_constraint[0].enzyme == "Trypsin");
  CHECK(s.enzymatic_search_constraint[0].max_num_internal_cleavages == 2);
  CHECK(s.enzymatic_search_constraint[0].min_number_termini == 1);
  REQUIRE(s.aminoacid_modification.size() == 2);
  CHECK(s.aminoacid_modification[0].aminoacid == "M");
  CHECK(s.aminoacid_modification[0].massdiff == Approx(15.9949));
  CHECK(s.aminoacid_modification[0].mass == Approx(147.035385));
  CHECK(s.aminoacid_modification[0].variable == "Y");
  CHECK(s.aminoacid_modification[1].aminoacid == "C");
  CHECK(s.aminoacid_modification[1].variable == "N");

  SECTION("all 116 Comet parameters, including the one with an empty value") {
    CHECK(s.parameter.size() == 116);
    bool sawEmpty = false;
    bool sawDatabase = false;
    for (const CnpxParameter& prm : s.parameter) {
      if (prm.name == "mass_offsets") { sawEmpty = true; CHECK(prm.value.empty()); }
      if (prm.name == "database_name") { sawDatabase = true; CHECK(prm.value == "data/UP000005640_human_DECOY.fasta"); }
    }
    CHECK(sawEmpty);
    CHECK(sawDatabase);
  }
}
