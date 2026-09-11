#include "NeoPepXMLParser/CnpxUIRunSummary.h"

#include "NeoPepXMLParser/NeoPepXMLError.h"

using namespace std;


CnpxUIRunSummary::CnpxUIRunSummary(){
  pipelineIndex=0;
  runs=NULL;
}

CnpxUIRunSummary::~CnpxUIRunSummary(){
  runs=NULL;
}

CnpxMSMSRunSummary& CnpxUIRunSummary::operator[](const size_t& index){
  if (runs == NULL){
    throw npxRangeError("CnpxUIRunSummary object is pointing to NULL.");
  } else if (index >= runs->size()){
    throw npxRangeError("Requested msms_run_summary beyond CnpxUIRunSummary boundary.");
  }
  return runs->at(index);
}

size_t CnpxUIRunSummary::getPipelineIndex(){
  return pipelineIndex;
}

void CnpxUIRunSummary::set(std::vector<CnpxMSMSRunSummary>* p, size_t index){
  pipelineIndex=index;
  runs=p;
}

size_t CnpxUIRunSummary::size(){
  if (runs == NULL) return 0;
  else return runs->size();
}
