#ifndef _CNPXALTERNATIVEPROTEIN_H
#define _CNPXALTERNATIVEPROTEIN_H

#include <string>

class CnpxAlternativeProtein {
public:
  CnpxAlternativeProtein();

  void write(FILE* f);

  std::string protein;
  std::string protein_descr;
  int num_tol_term;
  double protein_mw;
  std::string peptide_prev_aa;
  std::string peptide_next_aa;
  int peptide_start_pos;

private:

};

#endif 
