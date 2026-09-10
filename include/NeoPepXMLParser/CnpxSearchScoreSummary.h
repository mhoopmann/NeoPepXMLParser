#ifndef _CNPXSEARCHSCORESUMMARY_H
#define _CNPXSEARCHSCORESUMMARY_H

#include "NeoPepXMLParser/NeoPepXMLExport.h"

#include "CnpxParameter.h"
#include <string>
#include <vector>

class NEOPEPXML_EXPORT CnpxSearchScoreSummary {
public:

  CnpxSearchScoreSummary();
  CnpxSearchScoreSummary(bool b);

  bool present();
  void write(FILE* f);

  std::vector<CnpxParameter> parameter;

private:
  bool active;

};

#endif
