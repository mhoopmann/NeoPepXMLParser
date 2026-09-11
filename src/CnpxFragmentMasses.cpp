#include "NeoPepXMLParser/CnpxFragmentMasses.h"

using namespace std;

CnpxFragmentMasses::CnpxFragmentMasses(){
  channel = -1;
  mz = 0;
}

void CnpxFragmentMasses::write(FILE* f, int tabs){

  int t = tabs;
  if (t>-1) t++;
  NPXprintTabs(f, tabs);

  fprintf(f, "<fragment_masses");
  fprintf(f, " channel=\"%d\"", channel);
  fprintf(f, " mz=\"%.5lf\"", mz);
  fprintf(f, "/>\n");

}