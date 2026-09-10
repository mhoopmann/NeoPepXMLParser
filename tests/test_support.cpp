#include "test_support.h"

#include <fstream>
#include <sstream>

namespace npxtest {

const char* fixtureName() { return "comet_peptideprophet_iprophet.pep.xml"; }

std::string dataPath(const std::string& relative) {
  return std::string(NEOPEPXML_TEST_DATA_DIR) + "/" + relative;
}

std::string outputPath(const std::string& name) {
  return std::string(NEOPEPXML_TEST_OUTPUT_DIR) + "/" + name;
}

std::string readBytes(const std::filesystem::path& path) {
  std::ifstream in(path, std::ios::binary);
  if (!in) return std::string();
  std::ostringstream buffer;
  buffer << in.rdbuf();
  return buffer.str();
}

std::string readBytes(const std::string& path) {
  return readBytes(std::filesystem::path(path));
}

bool writeBytes(const std::string& path, const std::string& bytes) {
  std::ofstream out(std::filesystem::path(path), std::ios::binary);
  if (!out) return false;
  out.write(bytes.data(), static_cast<std::streamsize>(bytes.size()));
  return static_cast<bool>(out);
}

std::map<std::string, int> elementInventory(const std::string& xml) {
  std::map<std::string, int> counts;
  for (size_t i = 0; i < xml.size(); ++i) {
    if (xml[i] != '<') continue;
    size_t j = i + 1;
    while (j < xml.size() && ((xml[j] >= 'a' && xml[j] <= 'z') || xml[j] == '_')) ++j;
    if (j == i + 1) continue;  // "</", "<?", "<!" and anything that is not an opening tag
    if (j < xml.size() && (xml[j] == ' ' || xml[j] == '\t' || xml[j] == '\r' || xml[j] == '\n' ||
                           xml[j] == '/' || xml[j] == '>')) {
      counts[xml.substr(i + 1, j - i - 1)]++;
    }
  }
  return counts;
}

std::string describeDifference(const std::string& a, const std::string& b) {
  if (a == b) return std::string();
  size_t i = 0;
  while (i < a.size() && i < b.size() && a[i] == b[i]) ++i;
  std::ostringstream out;
  out << "sizes " << a.size() << " vs " << b.size() << ", first difference at byte " << i << "\n";
  auto context = [&](const std::string& s) {
    size_t from = i > 40 ? i - 40 : 0;
    return s.substr(from, 80);
  };
  out << "  a: ..." << context(a) << "...\n  b: ..." << context(b) << "...";
  return out.str();
}

}  // namespace npxtest
