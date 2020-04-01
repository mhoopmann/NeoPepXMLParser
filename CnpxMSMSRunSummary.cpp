#include "CnpxMSMSRunSummary.h"

using namespace std;


CnpxSearchSummary* CnpxMSMSRunSummary::addSearchSummary(std::string baseName, std::string searchEngine, std::string precursorMassType, std::string fragmentMassType, int searchID){
  CnpxSearchSummary s;
  s.base_name=baseName;
  s.search_engine=searchEngine;
  s.precursor_mass_type=precursorMassType;
  s.fragment_mass_type=fragmentMassType;
  s.search_id=searchID;
  search_summary.push_back(s);
  return &search_summary.back();
}

CnpxSpectrumQuery* CnpxMSMSRunSummary::addSpectrumQuery(std::string spec, int startScan, int endScan, double precursorNeutMass, int assumedCharge, int index){
  CnpxSpectrumQuery sq;
  sq.spectrum=spec;
  sq.start_scan=startScan;
  sq.end_scan=endScan;
  sq.precursor_neutral_mass=precursorNeutMass;
  sq.assumed_charge=assumedCharge;
  sq.index=index;
  spectrum_query.push_back(sq);
  return &spectrum_query.back();
}

void CnpxMSMSRunSummary::write(FILE* f){
  size_t i;

  fprintf(f, "<msms_run_summary base_name=\"%s\"", base_name.c_str());
  fprintf(f, " raw_data_type=\"%s\"", raw_data_type.c_str());
  fprintf(f, " raw_data=\"%s\"", raw_data.c_str());
  if (msManufacturer.size()>0) fprintf(f, " msManufacturer=\"%s\"", msManufacturer.c_str());
  if (msModel.size()>0) fprintf(f, " msModel=\"%s\"", msModel.c_str());
  if (msIonization.size()>0) fprintf(f, " msIonization=\"%s\"", msIonization.c_str());
  if (msMassAnalyzer.size()>0) fprintf(f, " msMassAnalyzer=\"%s\"", msMassAnalyzer.c_str());
  if (msDetector.size()>0) fprintf(f, " msDetector=\"%s\"", msDetector.c_str());
  fprintf(f, ">\n");

  for (i = 0; i<sample_enzyme.size(); i++) sample_enzyme[i].write(f);
  for (i = 0; i<search_summary.size(); i++) search_summary[i].write(f);
  for(i=0;i<analysis_timestamp.size(); i++) analysis_timestamp[i].write(f);
  for(i=0;i<spectrum_query.size();i++) spectrum_query[i].write(f);

  fprintf(f, "</msms_run_summary>\n");
}