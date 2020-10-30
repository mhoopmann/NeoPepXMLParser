#include "CnpxAnalysisSummary.h"

using namespace std;

void CnpxAnalysisSummary::write(FILE* f){
  size_t i;

  fprintf(f, "<analysis_summary analysis=\"%s\"", analysis.c_str());
  fprintf(f, " time=\"%4d-%02d-%02dT%02d:%02d:%02d\"", time.date.year, time.date.month, time.date.day, time.time.hour, time.time.minute, time.time.second);
  if (version.size()>0) fprintf(f, " version=\"%s\"", version.c_str());
  fprintf(f, ">\n");

  for (i = 0; i<ptmprophet_summary.size(); i++) ptmprophet_summary[i].write(f);
  for (i = 0; i<quantic_summary.size(); i++) quantic_summary[i].write(f);
  for (i = 0; i<xpresslabelfree_summary.size(); i++) xpresslabelfree_summary[i].write(f);
  for(i=0;i<interprophet_summary.size();i++) interprophet_summary[i].write(f);
  for (i = 0; i < peptideprophet_summary.size(); i++) peptideprophet_summary[i].write(f);
  for (i = 0; i<interact_summary.size(); i++) interact_summary[i].write(f);
  for (i = 0; i<decoy_analysis_summary.size(); i++) decoy_analysis_summary[i].write(f);
  for (i = 0; i<decoy_analysis.size(); i++) decoy_analysis[i].write(f);

  fprintf(f,"</analysis_summary>\n");
}