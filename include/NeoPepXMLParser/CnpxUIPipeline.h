#ifndef _CNPXUIPIPELINE_H
#define _CNPXUIPIPELINE_H

#include "NeoPepXMLParser/NeoPepXMLExport.h"

#include "CnpxMSMSPipelineAnalysis.h"
#include <vector>

class NEOPEPXML_EXPORT CnpxUIPipeline {
public:
  CnpxUIPipeline();
  ~CnpxUIPipeline();

  CnpxMSMSPipelineAnalysis& operator[](const size_t& index);

  void set(std::vector<CnpxMSMSPipelineAnalysis>* p);
  size_t size();

private:
  std::vector<CnpxMSMSPipelineAnalysis>* pipeline;

};

#endif 
