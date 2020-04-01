#ifndef _CNPXMODAMINOACIDMASS_H
#define _CNPXMODAMINOACIDMASS_H

#include <string>
#include <vector>

class CnpxModAminoAcidMass {
public:
  CnpxModAminoAcidMass();

  void write(FILE* f);

  std::string id;
  double mass;
  int position;
  std::string source;
  double staticMass;
  double variable;
   
private:

};

#endif
