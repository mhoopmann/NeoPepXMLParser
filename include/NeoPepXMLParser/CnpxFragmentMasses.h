#ifndef _CNPXFRAGMENTMASSES_H
#define _CNPXFRAGMENTMASSES_H

#include "NeoPepXMLParser/NeoPepXMLExport.h"

#include "NeoPepXMLStructs.h"
#include <string>
#include <vector>

class NEOPEPXML_EXPORT CnpxFragmentMasses {
public:
  CnpxFragmentMasses();

  void write(FILE* f, int tabs = -1);

  int channel;
  double mz;
  double offset;

private:

};

#endif