#ifndef _CNPXXLINKSCORE_H
#define _CNPXXLINKSCORE_H

#include <string>
#include <stdio.h>

class CnpxXLinkScore {
public:

  void write(FILE* f);

  std::string name;
  std::string type;
  std::string value;

private:

};

#endif 
