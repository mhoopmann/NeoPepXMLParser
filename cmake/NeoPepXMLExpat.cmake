# Locates expat and exposes it as the interface target NeoPepXMLParser_expat.
#
# NeoPepXMLParser deliberately does not insist on a single source of expat. In order of
# priority:
#
#   1. NEOPEPXML_EXPAT_INCLUDE_DIR (cache PATH)
#        Directory containing expat.h. Use this to compile against a specific copy, for
#        example the one bundled with MSToolkit (<MSToolkit>/include/extern). Optionally set
#        NEOPEPXML_EXPAT_LIBRARY to the matching library. Without a library a static
#        NeoPepXMLParser still builds; the consuming application then supplies expat itself
#        at final link, as it already does when it links MSToolkit.
#
#   2. find_package(EXPAT)
#        A system, vcpkg, Homebrew, or CMAKE_PREFIX_PATH installation, through CMake's
#        built-in FindEXPAT module.
#
# A third mode that downloads and builds a pinned libexpat when neither of the above applies
# arrives together with the shared-library build, which needs expat as a library rather than
# headers alone.

set(NEOPEPXML_EXPAT_INCLUDE_DIR "" CACHE PATH
  "Directory containing expat.h; overrides find_package(EXPAT)")
set(NEOPEPXML_EXPAT_LIBRARY "" CACHE FILEPATH
  "Expat library to link when NEOPEPXML_EXPAT_INCLUDE_DIR is set (optional for a static build)")

add_library(NeoPepXMLParser_expat INTERFACE)

if(NEOPEPXML_EXPAT_INCLUDE_DIR)
  if(NOT EXISTS "${NEOPEPXML_EXPAT_INCLUDE_DIR}/expat.h")
    message(FATAL_ERROR
      "NEOPEPXML_EXPAT_INCLUDE_DIR is '${NEOPEPXML_EXPAT_INCLUDE_DIR}' but no expat.h exists there.")
  endif()
  target_include_directories(NeoPepXMLParser_expat INTERFACE "${NEOPEPXML_EXPAT_INCLUDE_DIR}")
  if(NEOPEPXML_EXPAT_LIBRARY)
    target_link_libraries(NeoPepXMLParser_expat INTERFACE "${NEOPEPXML_EXPAT_LIBRARY}")
    message(STATUS "NeoPepXMLParser: expat headers ${NEOPEPXML_EXPAT_INCLUDE_DIR}, library ${NEOPEPXML_EXPAT_LIBRARY}")
  else()
    message(STATUS "NeoPepXMLParser: expat headers ${NEOPEPXML_EXPAT_INCLUDE_DIR}; no library given, the consuming application links expat")
  endif()
else()
  find_package(EXPAT)
  if(NOT EXPAT_FOUND)
    message(FATAL_ERROR
      "NeoPepXMLParser requires expat. Install it or point CMAKE_PREFIX_PATH at it, or set "
      "-DNEOPEPXML_EXPAT_INCLUDE_DIR=<directory containing expat.h> to use a specific copy "
      "such as the one bundled with MSToolkit.")
  endif()
  target_link_libraries(NeoPepXMLParser_expat INTERFACE EXPAT::EXPAT)
  message(STATUS "NeoPepXMLParser: expat ${EXPAT_VERSION_STRING} via find_package")
endif()
