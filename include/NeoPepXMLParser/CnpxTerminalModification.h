#ifndef _CNPXTERMINALMODIFICATION_H
#define _CNPXTERMINALMODIFICATION_H

#include "NeoPepXMLParser/NeoPepXMLExport.h"

#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>

class NEOPEPXML_EXPORT CnpxTerminalModification {
public:
  CnpxTerminalModification();

  void write(FILE* f);

  std::string description;
  double mass;
  double massdiff;
  std::string protein_terminus;
  std::string symbol;
  std::string terminus;
  std::string variable;

private:

};

#endif
