#ifndef _CNPXCONTRIBUTINGCHANNEL_H
#define _CNPXCONTRIBUTINGCHANNEL_H

#include "NeoPepXMLParser/NeoPepXMLExport.h"

#include "NeoPepXMLStructs.h"
#include "CnpxAffectedChannel.h"
#include <string>
#include <vector>

class NEOPEPXML_EXPORT CnpxContributingChannel {
public:
  CnpxContributingChannel();

  void write(FILE* f, int tabs = -1);

  int channel;

  std::vector<CnpxAffectedChannel> affected_channel;

private:

};

#endif