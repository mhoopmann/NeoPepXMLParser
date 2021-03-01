#include "CnpxXLink.h"

using namespace std;

CnpxXLink::CnpxXLink(){
  identifier.clear();
  mass = 0;
}

void CnpxXLink::write(FILE* f, int tabs){
  size_t i;
  fprintf(f, "<xlink identifier=\"%s\"", identifier.c_str());
  fprintf(f, " mass=\"%.6lf\">\n", mass);

  for(i=0;i<linked_peptide.size();i++) linked_peptide[i].write(f);
  for(i=0;i<xlink_score.size();i++) xlink_score[i].write(f);

  fprintf(f, "</xlink>\n");

}
