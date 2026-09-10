#ifndef _CNPXPARAMETER_H
#define _CNPXPARAMETER_H

#include "NeoPepXMLParser/NeoPepXMLExport.h"

#include "NeoPepXMLStructs.h"
#include <string>

class NEOPEPXML_EXPORT CnpxParameter {
public:

  void write(FILE* f, int tabs=-1);

  std::string name;
  std::string type;
  std::string value;

private:

};

#endif
