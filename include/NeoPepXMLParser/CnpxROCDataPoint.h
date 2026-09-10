#ifndef _CNPXROCDATAPOINT_H
#define _CNPXROCDATAPOINT_H

#include "NeoPepXMLParser/NeoPepXMLExport.h"

#include <iostream>

class NEOPEPXML_EXPORT CnpxROCDataPoint {
public:

  CnpxROCDataPoint();
  
  void write(FILE* f);

  double error;
  double min_prob;
  unsigned int num_corr;
  unsigned int num_incorr;
  double sensitivity;

private:

};

#endif