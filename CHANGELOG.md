# Changelog

All notable changes to NeoPepXMLParser are recorded here. Versions follow
[Semantic Versioning](https://semver.org/); the version is set once in `CMakeLists.txt`.

## 1.1.1 (unreleased)

### Added

- A Catch2 test suite under `tests/`, run by CTest and in CI: round trip against a committed
  expected output, hand-verified values, filters, the UI layer, locale independence, error
  paths, and non-ASCII file names. The first fixture is a trimmed real Comet + PeptideProphet +
  iProphet result; see `tests/data/README.md`.

### Fixed

- Writing a `parameter` element whose `value` attribute is empty terminated the process. Comet
  emits `value=""` for unset parameters such as `mass_offsets`, so every Comet result file
  failed to write back.
- Dated dev releases (`dev-<version>-<run>`) now tag the commit that was built. Previously the
  tag was created on the default branch, so the source links of `dev-1.1.0-1` and
  `dev-1.1.0-2` pointed at the pre-restructure tree even though their kits were correct.

### Known limitations

Found by the new tests and covered by test cases tagged `[!mayfail]`, to be addressed with the
error-handling work:

- Writing is locale-sensitive: under a comma-decimal locale the numeric attributes are written
  with commas.
- `setFilterRunSummary()` also removes every `search_hit`.
- `CnpxUIPSM` never fills `peptideProphet.parameters` or `iProphet.parameters`.

## 1.1.0 (2026-09-10)

The repository was restructured into a conventional CMake library and now ships prebuilt dev
kits. Consumers of 1.0.x need to adjust three things: the include path, how the library is
built or obtained, and the progress meter default.

### Added

- Prebuilt dev kits for Windows x64 (Release and Debug), Linux x86_64, and universal macOS,
  published from every push to `dev` and from every release. Each kit contains the headers,
  static and shared libraries, a CMake package, and the bundled expat.
- A shared library, next to the static one. `NeoPepXMLParser::NeoPepXMLParser`,
  `NeoPepXMLParser::static`, and `NeoPepXMLParser::shared` CMake targets, exported for
  `find_package(NeoPepXMLParser CONFIG)` and available in-tree.
- `std::filesystem::path` overloads of `read()` and `write()` for C++17 consumers. On Windows
  they open through the wide-character API, so non-ASCII paths work.
- `setProgressOutput(bool)` to enable the read progress meter.
- Generated `NeoPepXMLVersion.h` with `NPX_VERSION_MAJOR`, `NPX_VERSION_MINOR`,
  `NPX_VERSION_PATCH`, `NPX_VERSION`, and `NPX_VERSION_FULL`. `versionNeo()` returns the full
  string, including a pre-release suffix and commit hash for dev builds.
- `npxAtof()`, a locale-independent replacement for `atof()` used throughout the parser.
- An `examples/read_write.cpp` program, built by default and run by CTest as a smoke test.
- Three ways to supply expat at configure time: an installed package, an explicit header
  directory such as MSToolkit's, or a pinned libexpat built from source and bundled.
- GitHub Actions: CI on pull requests, dev-kit publication from `dev`, and a manual-only
  release workflow that can run only from `main`.

### Changed

- Headers moved to `include/NeoPepXMLParser/` and sources to `src/`. Include the library as
  `#include "NeoPepXMLParser/NeoPepXMLParser.h"`.
- CMake replaces the GNU Makefile and the Visual Studio project.
- The public header no longer includes `expat.h`; consumers do not need expat headers on their
  include path. The callback members take `const char*` instead of `XML_Char*`.
- The progress meter is off by default. Call `setProgressOutput(true)` to restore it.
- Floating-point attributes are parsed independently of the process locale. Previously a host
  application running under a comma-decimal locale silently read every mass and probability
  truncated at the decimal point.
- Files are read and written in binary mode, so output no longer has CRLF line endings on
  Windows and is byte-identical across platforms.
- `NPX_DATE` was removed; the version string is the single identifier of a build.
- The library is compiled as C++17. The public headers remain usable from C++11.
- The helper functions and namespace strings in `NeoPepXMLStructs.h` are declared there and
  defined in `NeoPepXMLStructs.cpp` instead of being duplicated into every translation unit.

### Fixed

- `CnpxPeptideprophetSummary` left `min_prob` and `est_tot_num_correct` uninitialized; its
  constructor declared local variables of those names instead of assigning the members.
- `sprintf` into fixed buffers is now `snprintf`.

### Known limitations

- No ABI compatibility is promised between releases; rebuild against each kit.
- Error handling still terminates the process in a few places (missing required attributes,
  out-of-range indexes into the UI helpers). Replacing this with exceptions and return values
  is planned for a following release.

## 1.0.6 (2025-10-10)

Last release of the flat-layout, Makefile-and-Visual-Studio era. See the git history for
earlier changes.
