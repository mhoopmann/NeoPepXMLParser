#ifndef _CNPXANALYSISRESULT_H
#define _CNPXANALYSISRESULT_H

#include "CnpxInterprophetResult.h"
#include "CnpxPeptideProphetResult.h"
#include <string>

class CnpxAnalysisResult {
public:

  CnpxAnalysisResult();

  void write(FILE* f);

  std::string analysis;
  int id;

  CnpxInterprophetResult interprophet_result;
  CnpxPeptideProphetResult peptide_prophet_result;

private:

};

#endif
