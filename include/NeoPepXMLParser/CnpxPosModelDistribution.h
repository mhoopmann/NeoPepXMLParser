#ifndef _CNPXPOSMODELDISTRIBUTION_H
#define _CNPXPOSMODELDISTRIBUTION_H

#include "NeoPepXMLParser/NeoPepXMLExport.h"

#include "CnpxParameter.h"
#include <iostream>
#include <string>
#include <vector>

class NEOPEPXML_EXPORT CnpxPosModelDistribution {
public:
  CnpxPosModelDistribution();

  void write(FILE* f);

  std::string type;

  std::vector<CnpxParameter> parameter;

private:

};

#endif
