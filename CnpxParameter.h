#ifndef _CNPXPARAMETER_H
#define _CNPXPARAMETER_H

#include <string>

class CnpxParameter {
public:

  void write(FILE* f);

  std::string name;
  std::string type;
  std::string value;

private:

};

#endif
