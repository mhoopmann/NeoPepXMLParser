#ifndef _CNPXAMINOACIDMODIFICATION_H
#define _CNPXAMINOACIDMODIFICATION_H

#include "NeoPepXMLParser/NeoPepXMLExport.h"

#include "NeoPepXMLStructs.h"
#include <iostream>
#include <string>

class NEOPEPXML_EXPORT CnpxAminoAcidModification {
public:
  CnpxAminoAcidModification();

  void write(FILE* f, int tabs=-1);

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