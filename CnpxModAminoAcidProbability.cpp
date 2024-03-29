#include "CnpxModAminoAcidProbability.h"

using namespace std;

CnpxModAminoAcidProbability::CnpxModAminoAcidProbability(){
  position=0;
  probability = 0;
  oscore = 0;
  mscore = 0;
  direct_oscore = 0;
  direct_mscore = 0;
  cterm_score = 0;
  nterm_score = 0;
  shift='n';
}


void CnpxModAminoAcidProbability::write(FILE* f) {

  fprintf(f, "<mod_aminoacid_probability position=\"%d\"", position);
  fprintf(f, " probability=\"%.3lf\"", probability);
  fprintf(f, " oscore=\"%.3lf\"", oscore);
  fprintf(f, " mscore=\"%.3lf\"", mscore);
  fprintf(f, " direct_oscore=\"%.3lf\"", direct_oscore);
  fprintf(f, " direct_mscore=\"%.3lf\"", direct_mscore);
  fprintf(f, " cterm_score=\"%.3lf\"", cterm_score);
  fprintf(f, " nterm_score=\"%.3lf\"", nterm_score);
  fprintf(f, " shift=\"%c\"", shift);
  fprintf(f, "/>\n");

}