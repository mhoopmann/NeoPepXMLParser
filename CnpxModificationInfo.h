#ifndef _CNPXMODIFICATIONINFO_H
#define _CNPXMODIFICATIONINFO_H

#include "CnpxModAminoAcidMass.h"

#include <string>
#include <vector>

class CnpxModificationInfo {
public:
  CnpxModificationInfo();

  CnpxModAminoAcidMass* addModAminoAcidMass(int position, double mass);
  void write(FILE* f);

  double mod_cterm_mass;
  double mod_nterm_mass;
  std::string modified_peptide;

  //std::vector<CnpxAminoAcidSubstitution> aminoacid_substitution;
  std::vector<CnpxModAminoAcidMass> mod_aminoacid_mass;

private:

};

#endif
