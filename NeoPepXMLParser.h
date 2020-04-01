#ifndef _NEOPEPXMLPARSER_H
#define _NEOPEPXMLPARSER_H

#include "CnpxMSMSPipelineAnalysis.h"
#include "expat.h"

#include "CnpxUIPipeline.h"
#include "CnpxUIPSM.h"
#include "CnpxUIRunSummary.h"
#include "CnpxUISpectra.h"

#include <iostream>
#include <vector>

#define XMLCLASS		
#ifndef XML_STATIC
#define XML_STATIC	// to statically link the expat libraries
#endif


class NeoPepXMLParser {
public:
  NeoPepXMLParser();
  ~NeoPepXMLParser();

  //User interface
  CnpxUIPipeline uiPipelines;
  CnpxUIRunSummary uiRunSummaries;
  CnpxUISpectra uiSpectra; //shortcuts to set of spectrum_query of the slected msms_peipline_analysis and msms_run_summary

  std::vector<CnpxMSMSPipelineAnalysis> msms_pipeline_analysis;

  //Functions for user interface
  CnpxUIPSM& operator[](const size_t& index);

  void addMSMSPipelineAnalysis(std::string date, std::string summary_xml);
  bool setRunSummaries(const size_t pipeIndex);
  bool setSpectra(const size_t pipeIndex, const size_t runIndex);
  size_t size();
  bool read(const char* fn);
  bool write(const char* fn);

  //Functions for XML Parsing
  void characters(const XML_Char *s, int len);
  void endElement(const XML_Char *el);
  void startElement(const XML_Char *el, const XML_Char **attr);

protected:
  bool                killRead;
  XML_Parser				  parser;
  std::vector<pepXMLElement> activeEl;
  int version;  //1=1.1, 2=1.2, etc.


  //Functions for XML Parsing
  inline const char* getAttrValue(const char* name, const XML_Char **attr) {
    for (int i = 0; attr[i]; i += 2) {
      if (isAttr(name, attr[i])) return attr[i + 1];
    }
    return "";
  }
  inline bool isAttr(const char *n1, const XML_Char *n2) { return (strcmp(n1, n2) == 0); }
  inline bool isElement(const char *n1, const XML_Char *n2)	{ return (strcmp(n1, n2) == 0); }

private:

  CnpxUIPSM psm;
  size_t sz;

  std::string elements[PEPXML_NUM_ELEMENTS];
  
  void calcSize();
  void init();
  
};

#endif