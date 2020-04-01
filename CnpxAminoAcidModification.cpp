#include "CnpxAminoAcidModification.h"

using namespace std;

CnpxAminoAcidModification::CnpxAminoAcidModification(){
  aminoacid.clear();
  binary.clear();
  description.clear();
  massdiff=0;
  mass=0;
  variable.clear();
  peptide_terminus.clear();
  protein_terminus.clear();
  symbol.clear();
}

void CnpxAminoAcidModification::write(FILE* f){

  fprintf(f, "<aminoacid_modification aminoacid=\"%s\"", aminoacid.c_str());
  fprintf(f, " massdiff=\"%.6lf\"", massdiff);
  fprintf(f, " mass=\"%.6lf\"", mass);
  fprintf(f, " variable=\"%s\"", variable.c_str());
  if(peptide_terminus.size()>0) fprintf(f, " peptide_terminus=\"%s\"",peptide_terminus.c_str());
  if (protein_terminus.size()>0) fprintf(f, " protein_terminus=\"%s\"", protein_terminus.c_str());
  if (symbol.size()>0) fprintf(f, " symbol=\"%s\"", symbol.c_str());
  if (binary.size()>0) fprintf(f, " binary=\"%s\"", binary.c_str());
  if (description.size()>0) fprintf(f, " description=\"%s\"", description.c_str());
  fprintf(f, "/>\n");

}
