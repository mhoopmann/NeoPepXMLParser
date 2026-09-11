#include "NeoPepXMLParser/CnpxParameter.h"

using namespace std;

void CnpxParameter::write(FILE* f, int tabs) {
  // value is required by the schema but may legitimately be empty: Comet writes value="" for
  // unset parameters such as mass_offsets, and such files must round-trip.

  int t = tabs;
  if (t>-1) t++;

  NPXprintTabs(f, tabs);
  fprintf(f, "<parameter name=\"%s\" value=\"%s\"", name.c_str(), value.c_str());
  if (!type.empty()) fprintf(f, " type=\"%s\"", type.c_str());
  fprintf(f, "/>\n");

}