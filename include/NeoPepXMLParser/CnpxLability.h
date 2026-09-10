#ifndef _CNPXLABILITY_H
#define _CNPXLABILITY_H

#include "NeoPepXMLParser/NeoPepXMLExport.h"

#include <string>

class NEOPEPXML_EXPORT CnpxLability {
public:

  CnpxLability();

  void write(FILE* f);

  int numlosses;
  double pval;
  double probability;
  double oscore;
  double mscore;
  double cterm_score;
  double nterm_score;

private:

};

#endif
