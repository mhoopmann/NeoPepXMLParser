#ifndef _CNPXERRORPOINT_H
#define _CNPXERRORPOINT_H

#include "NeoPepXMLParser/NeoPepXMLExport.h"

#include <iostream>

class NEOPEPXML_EXPORT CnpxErrorPoint {
public:
  CnpxErrorPoint();

  void write(FILE* f);

  double error;
  double min_prob;
  unsigned int num_corr;
  unsigned int num_incorr;

private:

};

#endif