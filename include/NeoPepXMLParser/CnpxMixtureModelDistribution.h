#ifndef _CNPXMIXTUREMODELDISTRIBUTION_H
#define _CNPXMIXTUREMODELDISTRIBUTION_H

#include "NeoPepXMLParser/NeoPepXMLExport.h"

#include "CnpxNegModelDistribution.h"
#include "CnpxPosModelDistribution.h"
#include <iostream>
#include <string>
#include <vector>

class NEOPEPXML_EXPORT CnpxMixtureModelDistribution {
public:
  CnpxMixtureModelDistribution();

  void write(FILE* f);

  std::string name;

  std::vector<CnpxNegModelDistribution> negmodel_distribution;
  std::vector<CnpxPosModelDistribution> posmodel_distribution;
  

private:

};

#endif

