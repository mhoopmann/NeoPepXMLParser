#ifndef _CNPXSEARCHSCORE_H
#define _CNPXSEARCHSCORE_H

#include "NeoPepXMLParser/NeoPepXMLExport.h"

#include "NeoPepXMLStructs.h"
#include <string>

class NEOPEPXML_EXPORT CnpxSearchScore {
public:

  void write(FILE* f, int tabs = -1);

  std::string name;
  std::string value;
 

private:

};

#endif
