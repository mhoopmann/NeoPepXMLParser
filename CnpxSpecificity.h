#ifndef _CNPXSPECIFICITY_H
#define _CNPXSPECIFICITY_H

#include <string>

class CnpxSpecificity {
public:
  CnpxSpecificity();

  void write(FILE* f);

  std::string cut;
  unsigned int min_spacing;
  std::string no_cut;
  std::string sense;

private:

};

#endif
