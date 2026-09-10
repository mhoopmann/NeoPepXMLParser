#ifndef _CNPXROCERRORDATA_H
#define _CNPXROCERRORDATA_H

#include "NeoPepXMLParser/NeoPepXMLExport.h"

#include "CnpxROCDataPoint.h"
#include "CnpxErrorPoint.h"

#include <string>
#include <vector>

class NEOPEPXML_EXPORT CnpxROCErrorData {
public:
  CnpxROCErrorData();

  void write(FILE* f);

  std::string charge;
  double charge_est_correct;

  std::vector<CnpxROCDataPoint> roc_data_point;
  std::vector<CnpxErrorPoint> error_point;

private:

};

#endif