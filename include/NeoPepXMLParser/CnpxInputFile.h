#ifndef _CNPXINPUTFILE_H
#define _CNPXINPUTFILE_H

#include "NeoPepXMLParser/NeoPepXMLExport.h"

#include <string>
#include <vector>

class NEOPEPXML_EXPORT CnpxInputFile {
public:

  void write(FILE* f);

  std::string directory;
  std::string name;

private:

};

#endif