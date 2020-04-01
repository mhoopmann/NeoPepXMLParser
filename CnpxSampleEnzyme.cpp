#include "CnpxSampleEnzyme.h"

using namespace std;

CnpxSampleEnzyme::CnpxSampleEnzyme(){
  description.clear();
  fidelity.clear();
  independent=true;
  name.clear();

  specificity.clear();
}

void CnpxSampleEnzyme::write(FILE* f){
  size_t i;
  
  fprintf(f, "<sample_enzyme name=\"%s\"", name.c_str());
  if(description.size()>0) fprintf(f, " description=\"%s\"", description.c_str());
  if (fidelity.size()>0) fprintf(f, " fidelity=\"%s\"", fidelity.c_str());
  if (!independent) fprintf(f, " independent=\"0");
  fprintf(f, ">\n");

  for(i=0;i<specificity.size();i++) specificity[i].write(f);

  fprintf(f,"</sample_enzyme>\n");

}