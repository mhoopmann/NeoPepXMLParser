#ifndef _CNPXAMINOACIDMODIFICATION_H
#define _CNPXAMINOACIDMODIFICATION_H

#include <iostream>
#include <string>

class CnpxAminoAcidModification {
public:
  CnpxAminoAcidModification();

  void write(FILE* f);

  std::string aminoacid;
  std::string binary;
  std::string description;
  double massdiff;
  double mass;
  std::string variable;
  std::string peptide_terminus;
  std::string protein_terminus;
  std::string symbol;
  
private:

};

#endif