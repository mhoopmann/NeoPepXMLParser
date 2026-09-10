#ifndef _CNPXXLINKSCORE_H
#define _CNPXXLINKSCORE_H

#include "NeoPepXMLParser/NeoPepXMLExport.h"

#include "NeoPepXMLStructs.h"
#include <string>
#include <stdio.h>

class NEOPEPXML_EXPORT CnpxXLinkScore {
public:

  void write(FILE* f, int tabs=-1);

  std::string name;
  std::string type;
  std::string value;

private:

};

#endif 
