#include "CnpxModificationInfo.h"

CnpxModificationInfo::CnpxModificationInfo() {
  modified_peptide.clear();
  mod_cterm_mass = 0;
  mod_nterm_mass = 0;
  mod_aminoacid_mass.clear();
}

CnpxModAminoAcidMass* CnpxModificationInfo::addModAminoAcidMass(int position, double mass){
  CnpxModAminoAcidMass m;
  m.position = position;
  m.mass = mass;
  mod_aminoacid_mass.push_back(m);
  return &mod_aminoacid_mass.back();
}

void CnpxModificationInfo::write(FILE* f) {
  size_t i;

  fprintf(f, "<modification_info modified_peptide=\"%s\"", modified_peptide.c_str());
  if (mod_cterm_mass != 0) fprintf(f, " mod_cterm_mass=\"%.6lf\"", mod_cterm_mass);
  if (mod_nterm_mass != 0) fprintf(f, " mod_nterm_mass=\"%.6lf\"", mod_nterm_mass);
  fprintf(f, ">\n");

  for (i = 0; i<mod_aminoacid_mass.size(); i++) mod_aminoacid_mass[i].write(f);

  fprintf(f, "</modification_info>\n");

}