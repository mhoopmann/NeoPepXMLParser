// Helpers shared by the test sources: fixture locations, byte-level file access, and an XML
// element inventory used to prove that a round trip loses nothing.
#ifndef NEOPEPXML_TEST_SUPPORT_H
#define NEOPEPXML_TEST_SUPPORT_H

#include <filesystem>
#include <map>
#include <string>

namespace npxtest {

// The first fixture: Comet search, PeptideProphet, iProphet, decoy analysis. See tests/data/README.md.
const char* fixtureName();

// Absolute path of a file under tests/data.
std::string dataPath(const std::string& relative);

// Absolute path for a scratch file a test may create in the build tree.
std::string outputPath(const std::string& name);

// Whole file as bytes; empty when the file cannot be opened.
std::string readBytes(const std::string& path);
std::string readBytes(const std::filesystem::path& path);
bool writeBytes(const std::string& path, const std::string& bytes);

// Count of each element name in an XML text: "<name" followed by whitespace, '/', or '>'.
std::map<std::string, int> elementInventory(const std::string& xml);

// Describes the first difference between two byte strings, or returns "" when they are equal.
std::string describeDifference(const std::string& a, const std::string& b);

}  // namespace npxtest

#endif
