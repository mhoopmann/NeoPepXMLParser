#ifndef _CNPXSAMPLEENZYME_H
#define _CNPXSAMPLEENZYME_H

#include "CnpxSpecificity.h"

#include <string>
#include <vector>

class CnpxSampleEnzyme {
public:
  CnpxSampleEnzyme();

  void write(FILE* f);

  std::string description;
  std::string fidelity;
  bool independent;
  std::string name;

  std::vector<CnpxSpecificity> specificity;

private:

};

#endif
