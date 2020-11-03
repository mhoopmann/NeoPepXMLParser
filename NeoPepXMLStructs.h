#ifndef NEOPEPXMLSTRUCTS_H
#define NEOPEPXMLSTRUCTS_H

#include <string>

enum pepXMLElement:int{
  pxAlternativeProtein,
  pxAminoAcidModification,
  pxAnalysisResult,
  pxAnalysisSummary,
  pxAnalysisTimestamp,
  pxDatabaseRefreshTimestamp,
  pxDatasetDerivation,
  pxDecoyAnalysis,
  pxDecoyAnalysisSummary,
  pxDistributionPoint,
  pxEnzymaticSearchConstraint,
  pxErrorPoint,
  pxInputfile,
  pxInteractSummary,
  pxInterprophetResult,
  pxInterprophetSummary,
  pxLinkedPeptide,
  pxMixture_Model,
  pxMixturemodel,
  pxMixturemodelDistribution,
  pxModAminoAcidMass,
  pxModAminoAcidProbability,
  pxModificationInfo,
  pxMSMSPipelineAnalysis,
  pxMSMSRunSummary,
  pxNegmodelDistribution,
  pxParameter,
  pxPeptideProphetResult,
  pxPeptideprophetSummary,
  pxPepXMLQuantResult,
  pxPoint,
  pxPosmodelDistribution,
  pxPTMProphetResult,
  pxPTMProphetSummary,
  pxQuanticResult,
  pxQuanticSummary,
  pxROCDataPoint,
  pxROCErrorData,
  pxSampleEnzyme,
  pxSearchDatabase,
  pxSearchHit,
  pxSearchResult,
  pxSearchScore,
  pxSearchScoreSummary,
  pxSearchSummary,
  pxSpecificity,
  pxSpectrumQuery,
  pxTerminalModification,
  pxXLink,
  pxXLinkScore,
  pxXpressLabelFreeResult,
  pxXpressLabelFreeSummary,
  PEPXML_NUM_ELEMENTS
};

typedef struct npxDate {
  int year;
  int month;
  int day;
  npxDate() {
    year = 0;
    month = 0;
    day = 0;
  }
} npxDate;

typedef struct npxTime {
  int hour;
  int minute;
  int second;
  npxTime() {
    hour = 0;
    minute = 0;
    second = 0;
  }
} npxTime;

typedef struct npxDateTime{
  npxDate date;
  npxTime time;
  void clear(){
    date.day=0;
    date.month=0;
    date.year=0;
    time.hour=0;
    time.minute=0;
    time.second=0;
  }
  void parseDateTime(const char* dt){
    if(strlen(dt)<2){
      clear();
      return;
    }
    int x = sscanf(dt, "%d-%d-%dT%d:%d:%d", &date.year, &date.month, &date.day, &time.hour, &time.minute, &time.second);
  }
  void parseDateTime(std::string s){
    parseDateTime(s.c_str());
  }
  std::string write(){
    std::string s;
    char str[64];
    sprintf(str, "%4d-%02d-%02dT%02d:%02d:%02d", date.year, date.month, date.day, time.hour, time.minute, time.second);
    s=str;
    return s;
  }
} npxDateTime;

#endif
