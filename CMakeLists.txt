cmake_minimum_required(VERSION 3.21)

project(NeoPepXMLParser
  VERSION 1.1.0
  DESCRIPTION "Library for reading, editing, and writing pepXML files"
  HOMEPAGE_URL "https://github.com/mhoopmann/NeoPepXMLParser"
  LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(CMakePackageConfigHelpers)
include(GenerateExportHeader)
include(GNUInstallDirs)

# ---------------------------------------------------------------------------
# Options
# ---------------------------------------------------------------------------
option(NEOPEPXML_BUILD_STATIC "Build the static library" ON)
option(NEOPEPXML_BUILD_SHARED "Build the shared library" ON)
option(NEOPEPXML_INSTALL "Generate install rules and the CMake package" ${PROJECT_IS_TOP_LEVEL})
option(NeoPepXMLParser_SHARED_LIBS
  "Make NeoPepXMLParser::NeoPepXMLParser refer to the shared library instead of the static one" OFF)
set(NEOPEPXML_VERSION_SUFFIX "" CACHE STRING
  "Pre-release identifier appended to the version as -<suffix>, e.g. dev.14; empty for releases")
set(NEOPEPXML_VERSION_METADATA "" CACHE STRING
  "Build metadata appended to the full version as +<metadata>, e.g. the short commit hash")
set(NEOPEPXML_INSTALL_DOCDIR "${CMAKE_INSTALL_DOCDIR}" CACHE STRING
  "Where README, VERSION, and license files are installed, relative to the prefix; '.' for a dev-kit archive")

if(NOT NEOPEPXML_BUILD_STATIC AND NOT NEOPEPXML_BUILD_SHARED)
  message(FATAL_ERROR "At least one of NEOPEPXML_BUILD_STATIC and NEOPEPXML_BUILD_SHARED must be ON.")
endif()

# Debug binaries carry a 'd' postfix so Debug and Release can be installed side by side.
if(NOT DEFINED CMAKE_DEBUG_POSTFIX)
  set(CMAKE_DEBUG_POSTFIX d)
endif()

# NEOPEPXML_PACKAGE_VERSION names archives (1.1.0 or 1.1.0-dev.14); NEOPEPXML_VERSION_FULL is
# what versionNeo() reports and may add build metadata (1.1.0-dev.14+a1b2c3d).
set(NEOPEPXML_PACKAGE_VERSION "${PROJECT_VERSION}")
if(NEOPEPXML_VERSION_SUFFIX)
  string(APPEND NEOPEPXML_PACKAGE_VERSION "-${NEOPEPXML_VERSION_SUFFIX}")
endif()
set(NEOPEPXML_VERSION_FULL "${NEOPEPXML_PACKAGE_VERSION}")
if(NEOPEPXML_VERSION_METADATA)
  string(APPEND NEOPEPXML_VERSION_FULL "+${NEOPEPXML_VERSION_METADATA}")
endif()

# ---------------------------------------------------------------------------
# Dependencies
# ---------------------------------------------------------------------------
# Defines NeoPepXMLParser_expat, NEOPEPXML_EXPAT_MODE and NEOPEPXML_EXPAT_HAS_LIBRARY. See the
# module for the three ways expat can be supplied.
include(NeoPepXMLExpat)

if(NEOPEPXML_BUILD_SHARED AND NOT NEOPEPXML_EXPAT_HAS_LIBRARY)
  message(FATAL_ERROR
    "The shared library must link expat, but only expat headers were supplied. Set "
    "NEOPEPXML_EXPAT_LIBRARY as well, or configure with -DNEOPEPXML_BUILD_SHARED=OFF.")
endif()

# ---------------------------------------------------------------------------
# Generated headers
# ---------------------------------------------------------------------------
set(NEOPEPXML_GENERATED_INCLUDE_DIR "${CMAKE_CURRENT_BINARY_DIR}/include")
configure_file(cmake/NeoPepXMLVersion.h.in
  "${NEOPEPXML_GENERATED_INCLUDE_DIR}/NeoPepXMLParser/NeoPepXMLVersion.h" @ONLY)

# ---------------------------------------------------------------------------
# Sources
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

# ---------------------------------------------------------------------------
# Library targets
# ---------------------------------------------------------------------------
# The static and shared libraries are separate targets compiled from the same sources rather
# than one object library feeding both. On Windows the objects differ (dllexport versus no
# decoration), so sharing them would leak export directives into the static library.
function(neopepxml_configure_target target)
  target_sources(${target} PRIVATE ${NEOPEPXML_SOURCES} ${NEOPEPXML_HEADERS})
  target_include_directories(${target}
    PUBLIC
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
      $<BUILD_INTERFACE:${NEOPEPXML_GENERATED_INCLUDE_DIR}>
      $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    PRIVATE
      # Only NeoPepXMLParser.cpp includes expat.h; consumers never need its headers.
      $<TARGET_PROPERTY:NeoPepXMLParser_expat,INTERFACE_INCLUDE_DIRECTORIES>)

  # The library itself is compiled as C++17. Its public headers only require C++11, so
  # consumers are not forced onto a newer standard; hence PRIVATE.
  target_compile_features(${target} PRIVATE cxx_std_17)
  set_target_properties(${target} PROPERTIES
    CXX_EXTENSIONS OFF
    DEFINE_SYMBOL NEOPEPXML_EXPORTS
    VERSION ${PROJECT_VERSION}
    # The classes expose their data members directly, so any change to them changes the binary
    # layout. No ABI compatibility is promised between releases; the soversion says so.
    SOVERSION ${PROJECT_VERSION})

  # expat.h decorates its API with __declspec(dllimport) on Windows unless XML_STATIC is
  # defined. Always defining it is safe: it is required against a static expat, and against an
  # import library the linker still resolves the undecorated names through the library thunks.
  target_compile_definitions(${target} PRIVATE XML_STATIC)

  if(MSVC)
    target_compile_options(${target} PRIVATE /W4)
    # fopen, sscanf, and friends are used deliberately; this silences the MSVC-only
    # deprecation warnings about them.
    target_compile_definitions(${target} PRIVATE _CRT_SECURE_NO_WARNINGS)
  else()
    target_compile_options(${target} PRIVATE -Wall -Wextra)
  endif()
endfunction()

set(NEOPEPXML_EXPORT_TARGETS NeoPepXMLParser_expat)

if(NEOPEPXML_BUILD_STATIC)
  add_library(NeoPepXMLParser_static STATIC)
  add_library(NeoPepXMLParser::static ALIAS NeoPepXMLParser_static)
  neopepxml_configure_target(NeoPepXMLParser_static)
  # Consumers of the static library see the export macros expand to nothing.
  target_compile_definitions(NeoPepXMLParser_static PUBLIC NEOPEPXML_STATIC_DEFINE)
  # A static library links nothing itself; expat is a requirement passed on to the final link.
  target_link_libraries(NeoPepXMLParser_static INTERFACE NeoPepXMLParser_expat)
  # On Windows the DLL's import library would otherwise share this file name.
  if(WIN32)
    set(neopepxml_static_name NeoPepXMLParser_static)
  else()
    set(neopepxml_static_name NeoPepXMLParser)
  endif()
  set_target_properties(NeoPepXMLParser_static PROPERTIES
    EXPORT_NAME static
    OUTPUT_NAME ${neopepxml_static_name}
    # So the static library can also be linked into a consumer's shared library.
    POSITION_INDEPENDENT_CODE ON)
  list(APPEND NEOPEPXML_EXPORT_TARGETS NeoPepXMLParser_static)
endif()

if(NEOPEPXML_BUILD_SHARED)
  add_library(NeoPepXMLParser_shared SHARED)
  add_library(NeoPepXMLParser::shared ALIAS NeoPepXMLParser_shared)
  neopepxml_configure_target(NeoPepXMLParser_shared)
  target_link_libraries(NeoPepXMLParser_shared PRIVATE NeoPepXMLParser_expat)
  set_target_properties(NeoPepXMLParser_shared PROPERTIES
    EXPORT_NAME shared
    OUTPUT_NAME NeoPepXMLParser
    # Only symbols marked NEOPEPXML_EXPORT leave the shared library on Linux and macOS.
    CXX_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN ON)
  list(APPEND NEOPEPXML_EXPORT_TARGETS NeoPepXMLParser_shared)
endif()

# NeoPepXMLParser::NeoPepXMLParser is the conventional target for consumers that add this
# project with add_subdirectory() or FetchContent. It refers to the static library unless
# NeoPepXMLParser_SHARED_LIBS is ON or no static library is built.
if(NeoPepXMLParser_SHARED_LIBS AND TARGET NeoPepXMLParser_shared)
  add_library(NeoPepXMLParser::NeoPepXMLParser ALIAS NeoPepXMLParser_shared)
elseif(TARGET NeoPepXMLParser_static)
  add_library(NeoPepXMLParser::NeoPepXMLParser ALIAS NeoPepXMLParser_static)
else()
  add_library(NeoPepXMLParser::NeoPepXMLParser ALIAS NeoPepXMLParser_shared)
endif()

# ---------------------------------------------------------------------------
# Export header
# ---------------------------------------------------------------------------
# Generated from the shared target when there is one, so the header carries the dllexport /
# dllimport / visibility logic. Static consumers define NEOPEPXML_STATIC_DEFINE (propagated
# from the static target) and get empty macros from the same header.
if(TARGET NeoPepXMLParser_shared)
  set(neopepxml_export_source NeoPepXMLParser_shared)
else()
  set(neopepxml_export_source NeoPepXMLParser_static)
endif()
set(NEOPEPXML_EXPORT_HEADER_EXTRA [=[
/* The exported classes hold standard-library members such as std::string and std::vector.
   MSVC warns (C4251) that those need a DLL interface. That is inherent to any C++ DLL API and
   is expected here, so the warning is silenced for translation units that use the DLL. */
#if defined(_MSC_VER) && !defined(NEOPEPXML_STATIC_DEFINE)
#pragma warning(disable : 4251)
#endif
]=])
generate_export_header(${neopepxml_export_source}
  BASE_NAME NEOPEPXML
  EXPORT_MACRO_NAME NEOPEPXML_EXPORT
  NO_EXPORT_MACRO_NAME NEOPEPXML_NO_EXPORT
  DEPRECATED_MACRO_NAME NEOPEPXML_DEPRECATED
  STATIC_DEFINE NEOPEPXML_STATIC_DEFINE
  EXPORT_FILE_NAME "${NEOPEPXML_GENERATED_INCLUDE_DIR}/NeoPepXMLParser/NeoPepXMLExport.h"
  CUSTOM_CONTENT_FROM_VARIABLE NEOPEPXML_EXPORT_HEADER_EXTRA)

# ---------------------------------------------------------------------------
# Install, package config, and dev-kit archives
# ---------------------------------------------------------------------------
if(NEOPEPXML_INSTALL)
  install(TARGETS ${NEOPEPXML_EXPORT_TARGETS}
    EXPORT NeoPepXMLParserTargets
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})
  install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
  install(DIRECTORY "${NEOPEPXML_GENERATED_INCLUDE_DIR}/" DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

  set(NEOPEPXML_CMAKE_INSTALL_DIR ${CMAKE_INSTALL_LIBDIR}/cmake/NeoPepXMLParser)
  install(EXPORT NeoPepXMLParserTargets
    NAMESPACE NeoPepXMLParser::
    DESTINATION ${NEOPEPXML_CMAKE_INSTALL_DIR})
  configure_package_config_file(cmake/NeoPepXMLParserConfig.cmake.in
    "${CMAKE_CURRENT_BINARY_DIR}/NeoPepXMLParserConfig.cmake"
    INSTALL_DESTINATION ${NEOPEPXML_CMAKE_INSTALL_DIR})
  write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/NeoPepXMLParserConfigVersion.cmake"
    COMPATIBILITY SameMajorVersion)
  install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/NeoPepXMLParserConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/NeoPepXMLParserConfigVersion.cmake"
    DESTINATION ${NEOPEPXML_CMAKE_INSTALL_DIR})

  # Documentation and provenance: the version stamp lets anyone identify an unpacked kit.
  file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/VERSION" "${NEOPEPXML_VERSION_FULL}\n")
  install(FILES LICENSE README.md "${CMAKE_CURRENT_BINARY_DIR}/VERSION"
    DESTINATION ${NEOPEPXML_INSTALL_DOCDIR})
  if(NEOPEPXML_EXPAT_MODE STREQUAL "FETCH")
    install(FILES "${NEOPEPXML_EXPAT_SOURCE_DIR}/expat/COPYING"
      DESTINATION ${NEOPEPXML_INSTALL_DOCDIR} RENAME LICENSE.expat)
  endif()

  # CPack wraps the install tree into NeoPepXMLParser-<version>-<os>-<arch>.{zip,tar.gz}.
  # A tarball is used outside Windows because zip cannot hold the shared library's symlinks.
  if(WIN32)
    set(neopepxml_os windows)
    set(CPACK_GENERATOR ZIP)
  elseif(APPLE)
    set(neopepxml_os macos)
    set(CPACK_GENERATOR TGZ)
  else()
    string(TOLOWER "${CMAKE_SYSTEM_NAME}" neopepxml_os)
    set(CPACK_GENERATOR TGZ)
  endif()
  if(APPLE AND CMAKE_OSX_ARCHITECTURES)
    list(LENGTH CMAKE_OSX_ARCHITECTURES neopepxml_arch_count)
    if(neopepxml_arch_count GREATER 1)
      set(neopepxml_arch universal)
    else()
      set(neopepxml_arch "${CMAKE_OSX_ARCHITECTURES}")
    endif()
  else()
    string(TOLOWER "${CMAKE_SYSTEM_PROCESSOR}" neopepxml_arch)
    if(neopepxml_arch MATCHES "^(amd64|x86_64)$")
      set(neopepxml_arch x64)
    elseif(neopepxml_arch MATCHES "^(aarch64|arm64)$")
      set(neopepxml_arch arm64)
    endif()
  endif()
  set(CPACK_PACKAGE_NAME NeoPepXMLParser)
  set(CPACK_PACKAGE_VERSION "${NEOPEPXML_PACKAGE_VERSION}")
  set(CPACK_PACKAGE_FILE_NAME "NeoPepXMLParser-${NEOPEPXML_PACKAGE_VERSION}-${neopepxml_os}-${neopepxml_arch}")
  set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "${PROJECT_DESCRIPTION}")
  set(CPACK_PACKAGE_HOMEPAGE_URL "${PROJECT_HOMEPAGE_URL}")
  set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
  set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY ON)
  include(CPack)
endif()
