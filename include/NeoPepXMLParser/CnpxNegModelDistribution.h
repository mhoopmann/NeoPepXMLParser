#ifndef _CNPXNEGMODELDISTRIBUTION_H
#define _CNPXNEGMODELDISTRIBUTION_H

#include "NeoPepXMLParser/NeoPepXMLExport.h"

#include "CnpxParameter.h"
#include <iostream>
#include <string>
#include <vector>

class NEOPEPXML_EXPORT CnpxNegModelDistribution {
public:
  CnpxNegModelDistribution();

  void write(FILE* f);

  std::string type;

  std::vector<CnpxParameter> parameter;

private:

};

#endif
