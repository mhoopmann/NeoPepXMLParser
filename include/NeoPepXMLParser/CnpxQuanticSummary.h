#ifndef _CNPXQUANTICSUMMARY_H
#define _CNPXQUANTICSUMMARY_H

#include "NeoPepXMLParser/NeoPepXMLExport.h"

#include "CnpxInputFile.h"
#include "CnpxMixtureModel.h"
#include "CnpxROCErrorData.h"
#include <string>
#include <vector>

class NEOPEPXML_EXPORT CnpxQuanticSummary {
public:
  CnpxQuanticSummary();

  void write(FILE* f);

  std::string options;
  std::string version;

  std::vector<CnpxInputFile> inputfile;

private:

};

#endif