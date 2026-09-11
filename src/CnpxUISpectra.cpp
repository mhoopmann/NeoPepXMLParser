#include "NeoPepXMLParser/CnpxUISpectra.h"

#include "NeoPepXMLParser/NeoPepXMLError.h"

using namespace std;

CnpxUISpectra::CnpxUISpectra(){
  pipelineIndex=0;
  runSummaryIndex=0;
  spectra = NULL;
}

CnpxUISpectra::~CnpxUISpectra(){
  spectra=NULL;
}

CnpxSpectrumQuery& CnpxUISpectra::operator[](const size_t& index){
  if(spectra==NULL){
    throw npxRangeError("CnpxUISpectra::operator[]: CnpxUISpectra object is pointing to NULL.");
  } else if(index>=spectra->size()){
    throw npxRangeError("CnpxUISpectra::operator[]: Requested spectrum beyond CnpxUISpectra boundary.");
  }
  return spectra->at(index);
}

CnpxSearchHit& CnpxUISpectra::getHit(const size_t& queryIndex, const size_t& rank){
  if (spectra == NULL) throw npxRangeError("CnpxUISpectra::getHit(): no spectra are selected.");
  if(rank<1){
    throw npxRangeError("CnpxUISpectra::getHit(): user requested rank less than 1.");
  }
  if(queryIndex>=spectra->size()){
    throw npxRangeError("CnpxUISpectra::getHit(): spectrum_query index out of bounds.");
  } else if (spectra->at(queryIndex).search_result.empty()) {
    throw npxRangeError("CnpxUISpectra::getHit(): the spectrum_query has no search_result.");
  } else if(rank>spectra->at(queryIndex).search_result[0].search_hit.size()){
    throw npxRangeError("CnpxUISpectra::getHit(): requested rank out of bounds.");
  }
  return spectra->at(queryIndex).search_result[0].search_hit[rank-1];
}

size_t CnpxUISpectra::getPipelineIndex(){
  return pipelineIndex;
}

size_t CnpxUISpectra::getRunSummaryIndex(){
  return runSummaryIndex;
}

void CnpxUISpectra::set(std::vector<CnpxSpectrumQuery>* p, const size_t pipeIndex, const size_t runIndex){
  pipelineIndex=pipeIndex;
  runSummaryIndex=runIndex;
  spectra=p;
}

size_t CnpxUISpectra::size(){
  if(spectra==NULL) return 0;
  else return spectra->size();
}