#ifndef _CNPXMODAMINOACIDMASS_H
#define _CNPXMODAMINOACIDMASS_H

#include "NeoPepXMLParser/NeoPepXMLExport.h"

#include "NeoPepXMLStructs.h"
#include <string>
#include <vector>

class NEOPEPXML_EXPORT CnpxModAminoAcidMass {
public:
  CnpxModAminoAcidMass();

  void write(FILE* f, int tabs=-1);

  std::string id;
  double mass;
  int position;
  std::string source;
  double staticMass;
  double variable;
   
private:

};

#endif
