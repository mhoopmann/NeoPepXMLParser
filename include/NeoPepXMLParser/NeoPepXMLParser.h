#ifndef _NEOPEPXMLPARSER_H
#define _NEOPEPXMLPARSER_H

#include "CnpxMSMSPipelineAnalysis.h"
#include "NeoPepXMLStructs.h"

#include "CnpxUIPipeline.h"
#include "CnpxUIPSM.h"
#include "CnpxUIRunSummary.h"
#include "CnpxUISpectra.h"

#include <cstdint>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

// The std::filesystem::path overloads of read() and write() are declared only when the
// including translation unit is compiled as C++17 or later, so this header remains usable
// from C++11 code. The library itself is always built as C++17, so the symbols exist either way.
#if (defined(__cplusplus) && __cplusplus >= 201703L) || (defined(_MSVC_LANG) && _MSVC_LANG >= 201703L)
#if __has_include(<filesystem>)
#include <filesystem>
#define NPX_HAS_FILESYSTEM 1
#endif
#endif
#ifndef NPX_HAS_FILESYSTEM
#define NPX_HAS_FILESYSTEM 0
#endif

#define NPX_VERSION "1.0.6"
#define NPX_DATE "2025 OCT 10"

// Opaque expat parser handle. Forward-declared so that users of this header do not need the
// expat headers on their include path; expat.h is included only by NeoPepXMLParser.cpp.
struct XML_ParserStruct;

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
  void setFilterProbability(double probability);
  void setFilterRunSummary(std::string str);
  void setFilterSearchHit(std::string str);
  // Prints a percentage progress meter to stdout while read() runs. Off by default so the
  // library stays silent inside applications that own their console or have none.
  void setProgressOutput(bool enabled);
  bool setRunSummaries(const size_t pipeIndex);
  bool setSpectra(const size_t pipeIndex, const size_t runIndex);
  size_t size();
  bool read(const char* fn);
  std::string versionNeo(); //returns version information
  bool write(const char* fn, bool tabs=false);
#if NPX_HAS_FILESYSTEM
  // On Windows these open the file through its wide-character path, so file names outside
  // the ANSI code page work. The const char* overloads above use the ANSI code page.
  bool read(const std::filesystem::path& fn);
  bool write(const std::filesystem::path& fn, bool tabs=false);
#endif

  //Functions for XML Parsing (expat callbacks; not intended to be called directly)
  void characters(const char *s, int len);
  void endElement(const char *el);
  void startElement(const char *el, const char **attr);

protected:
  bool                killRead;
  XML_ParserStruct*   parser;
  std::vector<pepXMLElement> activeEl;
  int version;  //1=1.1, 2=1.2, etc.


  //Functions for XML Parsing
  inline const char* getAttrValue(const char* name, const char **attr) {
    for (int i = 0; attr[i]; i += 2) {
      if (isAttr(name, attr[i])) return attr[i + 1];
    }
    return "";
  }
  inline bool isAttr(const char *n1, const char *n2) { return (strcmp(n1, n2) == 0); }
  inline bool isElement(const char *n1, const char *n2)	{ return (strcmp(n1, n2) == 0); }

private:

  CnpxUIPSM psm;
  size_t sz;
  double probFilter;
  std::string rsFilter;
  std::string shFilter;
  bool showProgress;

  double pProb;
  double iProb;

  std::string elements[PEPXML_NUM_ELEMENTS];

  void calcSize();
  void init();
  // Parses an open file. fileSize only drives the progress meter; displayName appears in
  // error messages.
  bool readFile(FILE* fptr, std::uintmax_t fileSize, const std::string& displayName);

};

#endif
