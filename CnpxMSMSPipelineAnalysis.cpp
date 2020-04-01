#include "CnpxMSMSPipelineAnalysis.h"

using namespace std;


CnpxMSMSRunSummary* CnpxMSMSPipelineAnalysis::addMSMSRunSummary(std::string baseName, std::string rawDataType, std::string rawData){
  CnpxMSMSRunSummary rs;
  rs.base_name=baseName;
  rs.raw_data_type=rawDataType;
  rs.raw_data=rawData;
  msms_run_summary.push_back(rs);
  return &msms_run_summary.back();
}

void CnpxMSMSPipelineAnalysis::write(FILE* f){
  size_t i;
  fprintf(f, "<msms_pipeline_analysis date=\"%4d-%02d-%02dT%02d:%02d:%02d\"", date.date.year, date.date.month, date.date.day, date.time.hour, date.time.minute, date.time.second);
  fprintf(f, " xmlns=\"http://regis-web.systemsbiology.net/pepXML\"");
  fprintf(f, " xsi:schemaLocation=\"http://regis-web.systemsbiology.net/pepXML /tools/bin/TPP/tpp/schema/pepXML_v122.xsd\"");
  fprintf(f, " summary_xml=\"%s\">\n", summary_xml.c_str());

  for(i=0;i<analysis_summary.size();i++) analysis_summary[i].write(f);
  for(i=0;i<msms_run_summary.size();i++) msms_run_summary[i].write(f);

  fprintf(f, "</msms_pipeline_analysis>\n");
}