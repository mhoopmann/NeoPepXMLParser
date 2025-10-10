#ifndef _CNPXENZYMATICSEARCHCONSTRAINT_H
#define _CNPXENZYMATICSEARCHCONSTRAINT_H

#include "NeoPepXMLStructs.h"
#include <iostream>
#include <string>

class CnpxEnzymaticSearchConstraint {
public:
  CnpxEnzymaticSearchConstraint();

  void write(FILE* f, int tabs=-1);

  std::string enzyme;
  int max_num_internal_cleavages;
  int min_number_termini;

private:

};

#endif