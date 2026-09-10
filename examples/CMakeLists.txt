# The example links the default NeoPepXMLParser::NeoPepXMLParser target exactly as an external
# consumer would, and is registered with CTest as a smoke test.
add_executable(neopepxml_example read_write.cpp)
target_link_libraries(neopepxml_example PRIVATE NeoPepXMLParser::NeoPepXMLParser)
# Deliberately older than the library's own C++17: the public headers promise C++11
# compatibility, and this is where that promise is checked. (MSVC's floor is C++14.)
target_compile_features(neopepxml_example PRIVATE cxx_std_11)
if(MSVC)
  target_compile_options(neopepxml_example PRIVATE /W4)
else()
  target_compile_options(neopepxml_example PRIVATE -Wall -Wextra)
endif()

if(NEOPEPXML_BUILD_TESTS)
  add_test(NAME example_read_write
    COMMAND neopepxml_example "${CMAKE_CURRENT_BINARY_DIR}/example.pep.xml")
endif()
