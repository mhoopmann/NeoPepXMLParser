#ifndef _CNPXXPRESSLABELFREESUMMARY_H
#define _CNPXXPRESSLABELFREESUMMARY_H

#include "NeoPepXMLParser/NeoPepXMLExport.h"

#include "CnpxInputFile.h"
#include "CnpxMixtureModel.h"
#include "CnpxROCErrorData.h"
#include <string>
#include <vector>

class NEOPEPXML_EXPORT CnpxXpressLabelFreeSummary {
public:
  CnpxXpressLabelFreeSummary();

  void write(FILE* f);

  std::string author;
  std::string version;

  std::string masstol;
  std::string ppmtol;
  std::string min_num_chromatogram_points;
  std::string min_num_isotope_peaks;


private:

};

#endif