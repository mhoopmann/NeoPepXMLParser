#include "CnpxXLinkScore.h"

using namespace std;

void CnpxXLinkScore::write(FILE* f) {
  fprintf(f, "<xlink_score name=\"%s\" value=\"%s\"", name.c_str(), value.c_str());
  if(type.size()>0) fprintf(f," type=\"%s\"",type.c_str());
  fprintf(f,"/>\n");
}