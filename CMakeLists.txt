cmake_minimum_required(VERSION 3.21)

project(NeoPepXMLParser
  VERSION 1.1.0
  DESCRIPTION "Library for reading, editing, and writing pepXML files"
  HOMEPAGE_URL "https://github.com/mhoopmann/NeoPepXMLParser"
  LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# ---------------------------------------------------------------------------
# Dependencies
# ---------------------------------------------------------------------------
# Resolves expat into the interface target NeoPepXMLParser_expat. See
# cmake/NeoPepXMLExpat.cmake for the supported ways of supplying it.
include(NeoPepXMLExpat)

# ---------------------------------------------------------------------------
# Library
# ---------------------------------------------------------------------------
set(NEOPEPXML_SOURCES
  src/CnpxAffectedChannel.cpp
  src/CnpxAlternativeProtein.cpp
  src/CnpxAminoAcidModification.cpp
  src/CnpxAminoAcidSubstitution.cpp
  src/CnpxAnalysisResult.cpp
  src/CnpxAnalysisSummary.cpp
  src/CnpxAnalysisTimestamp.cpp
  src/CnpxContributingChannel.cpp
  src/CnpxDatabaseRefreshTimestamp.cpp
  src/CnpxDecoyAnalysis.cpp
  src/CnpxDecoyAnalysisSummary.cpp
  src/CnpxDistributionPoint.cpp
  src/CnpxEnzymaticSearchConstraint.cpp
  src/CnpxErrorPoint.cpp
  src/CnpxFragmentMasses.cpp
  src/CnpxInputFile.cpp
  src/CnpxIntensity.cpp
  src/CnpxInteractSummary.cpp
  src/CnpxInterprophetResult.cpp
  src/CnpxInterprophetSummary.cpp
  src/CnpxIsotopicContributions.cpp
  src/CnpxLability.cpp
  src/CnpxLibraResult.cpp
  src/CnpxLibraSummary.cpp
  src/CnpxLinkedPeptide.cpp
  src/CnpxMSMSPipelineAnalysis.cpp
  src/CnpxMSMSRunSummary.cpp
  src/CnpxMixtureModel.cpp
  src/CnpxMixtureModelDistribution.cpp
  src/CnpxMixture_Model.cpp
  src/CnpxModAminoAcidMass.cpp
  src/CnpxModAminoAcidProbability.cpp
  src/CnpxModTerminalProbability.cpp
  src/CnpxModificationInfo.cpp
  src/CnpxNegModelDistribution.cpp
  src/CnpxPTMProphetResult.cpp
  src/CnpxPTMProphetSummary.cpp
  src/CnpxParameter.cpp
  src/CnpxPepXMLQuantResult.cpp
  src/CnpxPeptideProphetResult.cpp
  src/CnpxPeptideprophetSummary.cpp
  src/CnpxPosModelDistribution.cpp
  src/CnpxQuanticResult.cpp
  src/CnpxQuanticSummary.cpp
  src/CnpxROCDataPoint.cpp
  src/CnpxROCErrorData.cpp
  src/CnpxSampleEnzyme.cpp
  src/CnpxSearchDatabase.cpp
  src/CnpxSearchHit.cpp
  src/CnpxSearchResult.cpp
  src/CnpxSearchScore.cpp
  src/CnpxSearchScoreSummary.cpp
  src/CnpxSearchSummary.cpp
  src/CnpxSpecificity.cpp
  src/CnpxSpectrumQuery.cpp
  src/CnpxTerminalModification.cpp
  src/CnpxUIPSM.cpp
  src/CnpxUIPipeline.cpp
  src/CnpxUIRunSummary.cpp
  src/CnpxUISpectra.cpp
  src/CnpxXLink.cpp
  src/CnpxXLinkScore.cpp
  src/CnpxXpressLabelFreeResult.cpp
  src/CnpxXpressLabelFreeSummary.cpp
  src/NeoPepXMLParser.cpp
  src/NeoPepXMLStructs.cpp
)

# Every header is public: users navigate the parsed object tree directly, so the
# element classes are part of the API, not an implementation detail.
set(NEOPEPXML_HEADERS
  include/NeoPepXMLParser/CnpxAffectedChannel.h
  include/NeoPepXMLParser/CnpxAlternativeProtein.h
  include/NeoPepXMLParser/CnpxAminoAcidModification.h
  include/NeoPepXMLParser/CnpxAminoAcidSubstitution.h
  include/NeoPepXMLParser/CnpxAnalysisResult.h
  include/NeoPepXMLParser/CnpxAnalysisSummary.h
  include/NeoPepXMLParser/CnpxAnalysisTimestamp.h
  include/NeoPepXMLParser/CnpxContributingChannel.h
  include/NeoPepXMLParser/CnpxDatabaseRefreshTimestamp.h
  include/NeoPepXMLParser/CnpxDecoyAnalysis.h
  include/NeoPepXMLParser/CnpxDecoyAnalysisSummary.h
  include/NeoPepXMLParser/CnpxDistributionPoint.h
  include/NeoPepXMLParser/CnpxEnzymaticSearchConstraint.h
  include/NeoPepXMLParser/CnpxErrorPoint.h
  include/NeoPepXMLParser/CnpxFragmentMasses.h
  include/NeoPepXMLParser/CnpxInputFile.h
  include/NeoPepXMLParser/CnpxIntensity.h
  include/NeoPepXMLParser/CnpxInteractSummary.h
  include/NeoPepXMLParser/CnpxInterprophetResult.h
  include/NeoPepXMLParser/CnpxInterprophetSummary.h
  include/NeoPepXMLParser/CnpxIsotopicContributions.h
  include/NeoPepXMLParser/CnpxLability.h
  include/NeoPepXMLParser/CnpxLibraResult.h
  include/NeoPepXMLParser/CnpxLibraSummary.h
  include/NeoPepXMLParser/CnpxLinkedPeptide.h
  include/NeoPepXMLParser/CnpxMSMSPipelineAnalysis.h
  include/NeoPepXMLParser/CnpxMSMSRunSummary.h
  include/NeoPepXMLParser/CnpxMixtureModel.h
  include/NeoPepXMLParser/CnpxMixtureModelDistribution.h
  include/NeoPepXMLParser/CnpxMixture_Model.h
  include/NeoPepXMLParser/CnpxModAminoAcidMass.h
  include/NeoPepXMLParser/CnpxModAminoAcidProbability.h
  include/NeoPepXMLParser/CnpxModInfoDataType.h
  include/NeoPepXMLParser/CnpxModTerminalProbability.h
  include/NeoPepXMLParser/CnpxModificationInfo.h
  include/NeoPepXMLParser/CnpxNegModelDistribution.h
  include/NeoPepXMLParser/CnpxPTMProphetResult.h
  include/NeoPepXMLParser/CnpxPTMProphetSummary.h
  include/NeoPepXMLParser/CnpxParameter.h
  include/NeoPepXMLParser/CnpxPepXMLQuantResult.h
  include/NeoPepXMLParser/CnpxPeptideProphetResult.h
  include/NeoPepXMLParser/CnpxPeptideprophetSummary.h
  include/NeoPepXMLParser/CnpxPosModelDistribution.h
  include/NeoPepXMLParser/CnpxQuanticResult.h
  include/NeoPepXMLParser/CnpxQuanticSummary.h
  include/NeoPepXMLParser/CnpxROCDataPoint.h
  include/NeoPepXMLParser/CnpxROCErrorData.h
  include/NeoPepXMLParser/CnpxSampleEnzyme.h
  include/NeoPepXMLParser/CnpxSearchDatabase.h
  include/NeoPepXMLParser/CnpxSearchHit.h
  include/NeoPepXMLParser/CnpxSearchResult.h
  include/NeoPepXMLParser/CnpxSearchScore.h
  include/NeoPepXMLParser/CnpxSearchScoreSummary.h
  include/NeoPepXMLParser/CnpxSearchSummary.h
  include/NeoPepXMLParser/CnpxSequenceSearchConstraint.h
  include/NeoPepXMLParser/CnpxSpecificity.h
  include/NeoPepXMLParser/CnpxSpectrumQuery.h
  include/NeoPepXMLParser/CnpxTerminalModification.h
  include/NeoPepXMLParser/CnpxUIPSM.h
  include/NeoPepXMLParser/CnpxUIPipeline.h
  include/NeoPepXMLParser/CnpxUIRunSummary.h
  include/NeoPepXMLParser/CnpxUISpectra.h
  include/NeoPepXMLParser/CnpxXLink.h
  include/NeoPepXMLParser/CnpxXLinkScore.h
  include/NeoPepXMLParser/CnpxXpressLabelFreeResult.h
  include/NeoPepXMLParser/CnpxXpressLabelFreeSummary.h
  include/NeoPepXMLParser/NeoPepXMLParser.h
  include/NeoPepXMLParser/NeoPepXMLStructs.h
)

add_library(NeoPepXMLParser STATIC ${NEOPEPXML_SOURCES} ${NEOPEPXML_HEADERS})
add_library(NeoPepXMLParser::NeoPepXMLParser ALIAS NeoPepXMLParser)

target_include_directories(NeoPepXMLParser
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)

# The library itself is compiled as C++17. Its public headers only require C++11, so
# consumers are not forced onto a newer standard; hence PRIVATE.
target_compile_features(NeoPepXMLParser PRIVATE cxx_std_17)
set_target_properties(NeoPepXMLParser PROPERTIES CXX_EXTENSIONS OFF)

# Only NeoPepXMLParser.cpp includes expat.h; the public header forward-declares the parser
# handle. Consumers of the static library still link expat (CMake carries the PRIVATE link
# dependencies of a static library through to them) but never need its headers.
target_link_libraries(NeoPepXMLParser PRIVATE NeoPepXMLParser_expat)

# expat.h decorates its API with __declspec(dllimport) on Windows unless XML_STATIC is
# defined. Always defining it is safe: it is required against a static expat, and against an
# import library the linker still resolves the undecorated names through the library thunks.
target_compile_definitions(NeoPepXMLParser PRIVATE XML_STATIC)

if(MSVC)
  target_compile_options(NeoPepXMLParser PRIVATE /W4)
  # fopen, sprintf, strncpy, and friends are used deliberately; this silences the
  # MSVC-only deprecation warnings about them.
  target_compile_definitions(NeoPepXMLParser PRIVATE _CRT_SECURE_NO_WARNINGS)
else()
  target_compile_options(NeoPepXMLParser PRIVATE -Wall -Wextra)
endif()
