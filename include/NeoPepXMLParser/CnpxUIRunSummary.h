#ifndef _CNPXUIRUNSUMMARY_H
#define _CNPXUIRUNSUMMARY_H

#include "NeoPepXMLParser/NeoPepXMLExport.h"

#include "CnpxMSMSRunSummary.h"
#include <vector>

class NEOPEPXML_EXPORT CnpxUIRunSummary {
public:
  CnpxUIRunSummary();
  ~CnpxUIRunSummary();

  CnpxMSMSRunSummary& operator[](const size_t& index);

  size_t getPipelineIndex();
  void set(std::vector<CnpxMSMSRunSummary>* p, size_t index);
  size_t size();

private:
  size_t pipelineIndex;
  std::vector<CnpxMSMSRunSummary>* runs;

};

#endif 
