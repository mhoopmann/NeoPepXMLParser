#ifndef _CNPXISOTOPICCONTRIBUTIONS_H
#define _CNPXISOTOPICCONTRIBUTIONS_H

#include "NeoPepXMLParser/NeoPepXMLExport.h"

#include "NeoPepXMLStructs.h"
#include "CnpxContributingChannel.h"
#include <string>
#include <vector>

class NEOPEPXML_EXPORT CnpxIsotopicContributions {
public:

  void write(FILE* f, int tabs = -1);

  std::vector<CnpxContributingChannel> contributing_channel;

private:

};

#endif