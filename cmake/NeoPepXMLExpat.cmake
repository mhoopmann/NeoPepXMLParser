# Locates expat and exposes it as the INTERFACE target NeoPepXMLParser_expat, which is exported
# with the package as NeoPepXMLParser::expat.
#
# NeoPepXMLParser deliberately does not insist on a single source of expat. The mode, recorded
# in NEOPEPXML_EXPAT_MODE, is chosen in this order:
#
#   EXTERNAL  NEOPEPXML_EXPAT_INCLUDE_DIR is set.
#             Compile against a specific copy of the headers, for example the one bundled with
#             MSToolkit (<MSToolkit>/include/extern). Optionally set NEOPEPXML_EXPAT_LIBRARY to
#             the matching library. Without a library only the static NeoPepXMLParser can be
#             built, and the consuming application supplies expat itself at final link, as it
#             already does when it links MSToolkit.
#
#   FETCH     NEOPEPXML_FETCH_EXPAT is ON.
#             Download libexpat at the pinned NEOPEPXML_EXPAT_GIT_TAG and build it as a static
#             library alongside NeoPepXMLParser. Its headers, library, and CMake package are
#             installed into the same prefix, which is what makes a dev-kit archive
#             self-contained. The shared NeoPepXMLParser links this expat internally without
#             exporting its symbols.
#
#   FIND      Neither of the above.
#             find_package(EXPAT): a system, vcpkg, Homebrew, or CMAKE_PREFIX_PATH installation.
#
# NEOPEPXML_EXPAT_HAS_LIBRARY reports whether the chosen mode provides a library to link, which
# the shared NeoPepXMLParser requires.

set(NEOPEPXML_EXPAT_INCLUDE_DIR "" CACHE PATH
  "Directory containing expat.h; selects EXTERNAL mode")
set(NEOPEPXML_EXPAT_LIBRARY "" CACHE FILEPATH
  "Expat library to link in EXTERNAL mode (optional for a static-only build)")
option(NEOPEPXML_FETCH_EXPAT
  "Download and build a pinned libexpat instead of using an installed one" OFF)
set(NEOPEPXML_EXPAT_GIT_TAG "R_2_8_4" CACHE STRING
  "libexpat git tag built when NEOPEPXML_FETCH_EXPAT is ON")

add_library(NeoPepXMLParser_expat INTERFACE)
set_target_properties(NeoPepXMLParser_expat PROPERTIES EXPORT_NAME expat)

# Builds libexpat inside a function so the variables that steer its build do not leak into the
# rest of this project.
function(neopepxml_fetch_expat)
  include(FetchContent)
  FetchContent_Declare(libexpat
    GIT_REPOSITORY https://github.com/libexpat/libexpat.git
    GIT_TAG        ${NEOPEPXML_EXPAT_GIT_TAG}
    GIT_SHALLOW    TRUE
    SOURCE_SUBDIR  expat)

  # libexpat reads these with expat_shy_set(), which honors values that are already defined.
  set(EXPAT_BUILD_TOOLS OFF)
  set(EXPAT_BUILD_EXAMPLES OFF)
  set(EXPAT_BUILD_TESTS OFF)
  set(EXPAT_BUILD_DOCS OFF)
  set(EXPAT_BUILD_FUZZERS OFF)
  set(EXPAT_BUILD_PKGCONFIG OFF)
  set(EXPAT_SHARED_LIBS OFF)
  set(EXPAT_ENABLE_INSTALL ON)
  set(EXPAT_MSVC_STATIC_CRT OFF)

  # libexpat installs AUTHORS and its changelog into CMAKE_INSTALL_DOCDIR, which GNUInstallDirs
  # has already cached as this project's doc directory. Give expat its own.
  set(CMAKE_INSTALL_DOCDIR "${CMAKE_INSTALL_DATAROOTDIR}/doc/expat")

  # Position-independent so the static expat can be folded into the shared NeoPepXMLParser,
  # and hidden visibility so it is not re-exported from there. A re-exported copy could be
  # interposed by, or interpose, another libexpat loaded into the same process.
  set(CMAKE_POSITION_INDEPENDENT_CODE ON)
  set(CMAKE_C_VISIBILITY_PRESET hidden)
  set(CMAKE_VISIBILITY_INLINES_HIDDEN ON)

  FetchContent_MakeAvailable(libexpat)
endfunction()

if(NEOPEPXML_EXPAT_INCLUDE_DIR)
  set(NEOPEPXML_EXPAT_MODE EXTERNAL)
  if(NOT EXISTS "${NEOPEPXML_EXPAT_INCLUDE_DIR}/expat.h")
    message(FATAL_ERROR
      "NEOPEPXML_EXPAT_INCLUDE_DIR is ${NEOPEPXML_EXPAT_INCLUDE_DIR} but no expat.h exists there.")
  endif()
  target_include_directories(NeoPepXMLParser_expat INTERFACE
    $<BUILD_INTERFACE:${NEOPEPXML_EXPAT_INCLUDE_DIR}>)
  if(NEOPEPXML_EXPAT_LIBRARY)
    target_link_libraries(NeoPepXMLParser_expat INTERFACE "${NEOPEPXML_EXPAT_LIBRARY}")
    set(NEOPEPXML_EXPAT_HAS_LIBRARY TRUE)
    message(STATUS "NeoPepXMLParser: expat headers ${NEOPEPXML_EXPAT_INCLUDE_DIR}, library ${NEOPEPXML_EXPAT_LIBRARY}")
  else()
    set(NEOPEPXML_EXPAT_HAS_LIBRARY FALSE)
    message(STATUS "NeoPepXMLParser: expat headers ${NEOPEPXML_EXPAT_INCLUDE_DIR}; no library given, the consuming application links expat")
  endif()

elseif(NEOPEPXML_FETCH_EXPAT)
  set(NEOPEPXML_EXPAT_MODE FETCH)
  neopepxml_fetch_expat()
  FetchContent_GetProperties(libexpat SOURCE_DIR NEOPEPXML_EXPAT_SOURCE_DIR)
  target_include_directories(NeoPepXMLParser_expat INTERFACE
    $<BUILD_INTERFACE:$<TARGET_PROPERTY:expat,INTERFACE_INCLUDE_DIRECTORIES>>)
  # Linking the in-tree target by name lets CMake rewrite it as expat::expat in the installed
  # package, because libexpat exports that target from the same prefix.
  target_link_libraries(NeoPepXMLParser_expat INTERFACE expat)
  set(NEOPEPXML_EXPAT_HAS_LIBRARY TRUE)
  message(STATUS "NeoPepXMLParser: expat ${NEOPEPXML_EXPAT_GIT_TAG} built from source")

else()
  set(NEOPEPXML_EXPAT_MODE FIND)
  find_package(EXPAT)
  if(NOT EXPAT_FOUND)
    message(FATAL_ERROR
      "NeoPepXMLParser requires expat. Install it or point CMAKE_PREFIX_PATH at it, set "
      "-DNEOPEPXML_EXPAT_INCLUDE_DIR=<directory containing expat.h> to use a specific copy such "
      "as MSToolkit's, or configure with -DNEOPEPXML_FETCH_EXPAT=ON to build a pinned libexpat.")
  endif()
  target_include_directories(NeoPepXMLParser_expat INTERFACE
    $<BUILD_INTERFACE:$<TARGET_PROPERTY:EXPAT::EXPAT,INTERFACE_INCLUDE_DIRECTORIES>>)
  target_link_libraries(NeoPepXMLParser_expat INTERFACE EXPAT::EXPAT)
  set(NEOPEPXML_EXPAT_HAS_LIBRARY TRUE)
  message(STATUS "NeoPepXMLParser: expat ${EXPAT_VERSION_STRING} via find_package")
endif()
