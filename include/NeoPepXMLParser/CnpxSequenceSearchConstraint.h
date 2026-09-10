#ifndef _CNPXSEQUENCESEARCHCONSTRAINT_H
#define _CNPXSEQUENCESEARCHCONSTRAINT_H

#include "NeoPepXMLParser/NeoPepXMLExport.h"

#include <iostream>
#include <string>

class NEOPEPXML_EXPORT CnpxSequenceSearchConstraint {
public:
  CnpxSequenceSearchConstraint();

  void write(FILE* f);

  std::string sequence;

private:

};

#endif