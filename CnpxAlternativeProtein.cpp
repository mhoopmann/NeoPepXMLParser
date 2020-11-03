#include "CnpxAlternativeProtein.h"

CnpxAlternativeProtein::CnpxAlternativeProtein() {
  num_tol_term=-1;
  protein_mw=0;
}

void CnpxAlternativeProtein::write(FILE* f) {
  fprintf(f, "<alternative_protein protein=\"%s\"", protein.c_str());
  if(protein_descr.size()>0) fprintf(f, " protein_descr=\"%s\"",protein_descr.c_str());
  if (num_tol_term>-1) fprintf(f, " num_tol_term=\"%d\"", num_tol_term);
  if (protein_mw>0) fprintf(f, " num_tol_term=\"%.4lf\"", protein_mw);
  if (peptide_prev_aa.size()>0) fprintf(f, " peptide_prev_aa=\"%s\"", peptide_prev_aa.c_str());
  if (peptide_next_aa.size()>0) fprintf(f, " peptide_next_aa=\"%s\"", peptide_next_aa.c_str());
  if (peptide_start_pos>0) fprintf(f, " peptide_start_pos=\"%d\"", peptide_start_pos);
  fprintf(f, "/>\n");
}
