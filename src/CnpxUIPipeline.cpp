#include "NeoPepXMLParser/CnpxUIPipeline.h"

#include "NeoPepXMLParser/NeoPepXMLError.h"

using namespace std;

CnpxUIPipeline::CnpxUIPipeline(){
  pipeline=NULL;
}

CnpxUIPipeline::~CnpxUIPipeline(){
  pipeline=NULL;
}

CnpxMSMSPipelineAnalysis& CnpxUIPipeline::operator[](const size_t& index){
  if (pipeline == NULL){
    throw npxRangeError("CnpxUIPipeline object is pointing to NULL.");
  } else if (index >= pipeline->size()){
    throw npxRangeError("Requested msms_pipeline_analysis beyond CnpxUIPipeline boundary.");
  }
  return pipeline->at(index);
}

void CnpxUIPipeline::set(std::vector<CnpxMSMSPipelineAnalysis>* p){
  pipeline=p;
}

size_t CnpxUIPipeline::size(){
  if (pipeline == NULL) return 0;
  else return pipeline->size();
}