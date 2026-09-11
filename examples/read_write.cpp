// Minimal NeoPepXMLParser example: create a document, write it, read it back.
//
// This is also what CI runs as a smoke test. It is compiled as a C++11 consumer of the public
// headers on purpose, so it verifies that they stay usable from pre-C++17 code.

#include "NeoPepXMLParser/NeoPepXMLParser.h"

#include <cstdio>
#include <string>

int main(int argc, char** argv) {
  const char* fileName = argc > 1 ? argv[1] : "example.pep.xml";

  NeoPepXMLParser out;
  printf("NeoPepXMLParser %s\n", out.versionNeo().c_str());

  // Build a document from scratch. Real applications usually start with read() instead and
  // then navigate out.msms_pipeline_analysis[i].msms_run_summary[j].spectrum_query[k]...
  out.addMSMSPipelineAnalysis("2026-09-10T12:00:00", "interact.pep.xml");
  if (!out.write(fileName)) {
    printf("could not write %s: %s\n", fileName, out.lastError().text().c_str());
    return 1;
  }

  NeoPepXMLParser in;
  in.setProgressOutput(true);  // opt-in percentage meter on stdout; off by default
  if (!in.read(fileName)) {
    printf("could not read %s: %s\n", fileName, in.lastError().text().c_str());
    return 1;
  }

  const std::string& summary = in.msms_pipeline_analysis[0].summary_xml;
  printf("read back %u pipeline analysis element(s); summary_xml=\"%s\"\n",
         (unsigned)in.msms_pipeline_analysis.size(), summary.c_str());
  return summary == "interact.pep.xml" ? 0 : 1;
}
