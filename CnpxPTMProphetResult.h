#ifndef _CNPXPTMPROPHETRESULT_H
#define _CNPXPTMPROPHETRESULT_H

#include "CnpxModAminoAcidProbability.h"
#include "CnpxParameter.h"
#include <string>
#include <vector>

class CnpxPTMProphetResult {
public:

  CnpxPTMProphetResult();
  CnpxPTMProphetResult(bool b);

  bool present();
  void write(FILE* f);

  std::string ptm;
  std::string prior;
  std::string ptm_peptide;

  std::vector<CnpxParameter> parameter;
  std::vector<CnpxModAminoAcidProbability> mod_amino_acid_probability;

private:
  bool active;

};

#endif
