#ifndef _CNPXQUANTICRESULT_H
#define _CNPXQUANTICRESULT_H

#include "NeoPepXMLParser/NeoPepXMLExport.h"

#include <string>
#include <vector>

class NEOPEPXML_EXPORT CnpxQuanticResult {
public:

  CnpxQuanticResult();
  CnpxQuanticResult(bool b);

  bool present();
  void write(FILE* f);

  double antic;

private:
  bool active;

};

#endif
