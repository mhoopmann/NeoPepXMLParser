#ifndef _CNPXANALYSISSUMMARY_H
#define _CNPXANALYSISSUMMARY_H

#include "NeoPepXMLStructs.h"
#include "CnpxDecoyAnalysis.h"
#include "CnpxDecoyAnalysisSummary.h"
#include "CnpxInteractSummary.h"
#include "CnpxInterprophetSummary.h"
#include "CnpxPeptideProphetSummary.h"
#include <string>
#include <vector>

class CnpxAnalysisSummary {
public:
  void write(FILE* f);

  std::string analysis;
  npxDateTime time;
  std::string version;

  std::vector<CnpxDecoyAnalysis> decoy_analysis;
  std::vector<CnpxDecoyAnalysisSummary> decoy_analysis_summary;
  std::vector<CnpxInteractSummary> interact_summary;
  std::vector<CnpxInterprophetSummary> interprophet_summary;
  std::vector<CnpxPeptideprophetSummary> peptideprophet_summary;

private:

};

#endif