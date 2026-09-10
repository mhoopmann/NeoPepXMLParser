#ifndef NEOPEPXMLSTRUCTS_H
#define NEOPEPXMLSTRUCTS_H

#include <cstdio>
#include <string>

// Namespace declarations written on the msms_pipeline_analysis root element.
extern const std::string npx_xmlns;
extern const std::string npx_xmlns_xsi;
extern const std::string npx_xsi_schemaLocation;

enum pepXMLElement:int{
  pxAffectedChannel,
  pxAlternativeProtein,
  pxAminoAcidModification,
  pxAminoAcidSubstitution,
  pxAnalysisResult,
  pxAnalysisSummary,
  pxAnalysisTimestamp,
  pxBin,
  pxContributingChannel,
  pxDatabaseRefreshTimestamp,
  pxDatasetDerivation,
  pxDecoyAnalysis,
  pxDecoyAnalysisSummary,
  pxDistributionPoint,
  pxEnzymaticSearchConstraint,
  pxErrorPoint,
  pxFragmentMasses,
  pxInputfile,
  pxIntensity,
  pxInteractSummary,
  pxInterprophetResult,
  pxInterprophetSummary,
  pxIsotopicContributions,
  pxLability,
  pxLibraResult,
  pxLibraSummary,
  pxLinkedPeptide,
  pxMixture_Model,
  pxMixturemodel,
  pxMixturemodelDistribution,
  pxModAminoAcidMass,
  pxModAminoAcidProbability,
  pxModificationInfo,
  pxModTerminalProbability,
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
  void clear();
  // Parses "YYYY-MM-DDThh:mm:ss". A string shorter than two characters clears the value.
  void parseDateTime(const char* dt);
  void parseDateTime(const std::string& s);
  std::string write();
} npxDateTime;

// Converts the leading decimal number in s to a double the way atof does, but independent of
// the process locale. pepXML always uses '.' as the decimal separator, whereas atof honors
// whatever LC_NUMERIC the host application has set. Returns 0.0 when s holds no number.
double npxAtof(const char* s);

// Reports a missing required attribute on element el and terminates the process.
void NPXerrMsg(const std::string& el, const std::string& attr);

// Writes 'tabs' spaces to f; used to indent nested elements when writing.
void NPXprintTabs(FILE* f, int tabs);

#endif
