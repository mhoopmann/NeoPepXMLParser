#ifndef _CNPXANALYSISRESULT_H
#define _CNPXANALYSISRESULT_H

#include "CnpxInterprophetResult.h"
#include "CnpxPeptideProphetResult.h"
#include "CnpxPepXMLQuantResult.h"
#include "CnpxPTMProphetResult.h"
#include "CnpxQuanticResult.h"
#include "CnpxXpressLabelFreeResult.h"
#include <string>

class CnpxAnalysisResult {
public:

  CnpxAnalysisResult();

  void write(FILE* f);

  std::string analysis;
  int id;

  CnpxInterprophetResult interprophet_result;
  CnpxPeptideProphetResult peptide_prophet_result;
  CnpxPepXMLQuantResult pepxmlquant_result;
  CnpxQuanticResult quantic_result;
  CnpxXpressLabelFreeResult expresslabelfree_result;
  CnpxPTMProphetResult ptmprophet_result;

  std::vector<CnpxParameter> parameter;

private:

};

#endif
