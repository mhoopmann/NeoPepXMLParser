# Test suite. Catch2 is fetched only for this build and pinned by the checksum of its release
# archive; it is never installed or exported.
include(FetchContent)
FetchContent_Declare(Catch2
  URL      https://github.com/catchorg/Catch2/archive/refs/tags/v3.16.0.tar.gz
  URL_HASH SHA256=0957cae5821b17ce07f0833aaa52b5137643a8382203221f363a8303c109af34)
FetchContent_MakeAvailable(Catch2)
list(APPEND CMAKE_MODULE_PATH "${catch2_SOURCE_DIR}/extras")
include(Catch)

add_executable(neopepxml_tests
  test_support.h
  test_support.cpp
  test_numeric.cpp
  test_roundtrip.cpp
  test_values_summaries.cpp
  test_values_queries.cpp
  test_locale.cpp
  test_filters_ui.cpp
  test_errors.cpp
  test_paths.cpp)
target_link_libraries(neopepxml_tests PRIVATE
  NeoPepXMLParser::NeoPepXMLParser
  Catch2::Catch2WithMain)
target_compile_features(neopepxml_tests PRIVATE cxx_std_17)

# Where the fixtures live, and where a test may write scratch files.
target_compile_definitions(neopepxml_tests PRIVATE
  NEOPEPXML_TEST_DATA_DIR="${CMAKE_CURRENT_SOURCE_DIR}/data"
  NEOPEPXML_TEST_OUTPUT_DIR="${CMAKE_CURRENT_BINARY_DIR}")

if(MSVC)
  target_compile_options(neopepxml_tests PRIVATE /W4)
  target_compile_definitions(neopepxml_tests PRIVATE _CRT_SECURE_NO_WARNINGS)
else()
  target_compile_options(neopepxml_tests PRIVATE -Wall -Wextra)
endif()

# One CTest entry per Catch2 test case, so ctest output names what failed.
catch_discover_tests(neopepxml_tests)
