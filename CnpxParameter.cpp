#include "CnpxParameter.h"

using namespace std;

void CnpxParameter::write(FILE* f) {

  fprintf(f, "<parameter name=\"%s\" value=\"%s\"", name.c_str(), value.c_str());
  if (type.size() > 0) fprintf(f, " type=\"%s\"", type.c_str());
  fprintf(f, "/>\n");

}