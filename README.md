# NeoPepXMLParser

A C++ library for reading, editing, and writing [pepXML](http://tools.proteomecenter.org/wiki/index.php?title=Formats:pepXML) files.
The whole document is parsed into an object tree that mirrors the pepXML schema, element for
element, so anything the file contains can be inspected, changed, and written back out.

```cpp
#include "NeoPepXMLParser/NeoPepXMLParser.h"

int main() {
  NeoPepXMLParser xml;
  if (!xml.read("some.pep.xml")) return 1;
  // Navigate and edit: xml.msms_pipeline_analysis[i].msms_run_summary[j].spectrum_query[k]...
  return xml.write("edited.pep.xml") ? 0 : 1;
}
```

The class names follow the schema's element names, including its inconsistencies
(`CnpxMixtureModel` and `CnpxMixture_Model` are two different elements). That is deliberate.

Licensed under the Apache License 2.0. The bundled expat is MIT licensed; see `LICENSE.expat`.

## Getting the library

Each release on the [Releases page](https://github.com/mhoopmann/NeoPepXMLParser/releases)
ships a self-contained **dev kit** per platform. Unpack it and point your project at it; no
build from source is needed.

| Kit | Contents |
|---|---|
| `NeoPepXMLParser-<version>-windows-x64.zip` | Visual Studio 2022 (v143), Release and Debug, dynamic CRT (`/MD`, `/MDd`) |
| `NeoPepXMLParser-<version>-linux-x64.tar.gz` | GCC on Ubuntu 22.04, Release |
| `NeoPepXMLParser-<version>-macos-universal.tar.gz` | Apple Clang, arm64 + x86_64, macOS 11 or newer, Release |

Inside every kit:

```
include/NeoPepXMLParser/   public headers (plus the generated export and version headers)
include/expat*.h           the bundled expat headers
lib/                       static library, shared library import file (Windows), bundled expat, CMake package
bin/                       shared library (Windows)
LICENSE  LICENSE.expat  README.md  VERSION
```

Official releases are tagged `vX.Y.Z`. Builds from the `dev` branch are published as
pre-releases tagged `dev-<version>-<run>`; the rolling `dev-latest` pre-release always holds the
newest one under fixed file names such as `NeoPepXMLParser-dev-latest-windows-x64.zip`. The
`VERSION` file inside a kit, and `NeoPepXMLParser::versionNeo()` at runtime, identify a build
exactly (for example `1.1.0-dev.14+a1b2c3d`).

## Using a kit

### With CMake

```cmake
find_package(NeoPepXMLParser CONFIG REQUIRED)   # e.g. -DCMAKE_PREFIX_PATH=<unpacked kit>
target_link_libraries(myapp PRIVATE NeoPepXMLParser::NeoPepXMLParser)
```

`NeoPepXMLParser::NeoPepXMLParser` is the static library. Set `NeoPepXMLParser_SHARED_LIBS=ON`
before `find_package()` to get the shared one instead, or name `NeoPepXMLParser::static` or
`NeoPepXMLParser::shared` directly. Include paths, the expat dependency, and the static-library
define are all carried by the target.

### Without CMake

Add `<kit>/include` to the include path, then either

- **static:** link `NeoPepXMLParser_static.lib` (`NeoPepXMLParser_staticd.lib` for Debug) plus the
  bundled `libexpatMD.lib` (`libexpatdMD.lib`), and define `NEOPEPXML_STATIC_DEFINE`. On Linux
  and macOS the files are `libNeoPepXMLParser.a` and `libexpat.a`; the define is harmless there
  but still correct.
- **shared:** link `NeoPepXMLParser.lib` (`NeoPepXMLParserd.lib`) and ship `NeoPepXMLParser.dll`
  (`NeoPepXMLParserd.dll`) next to your executable. expat is already inside the DLL. On Linux
  and macOS link `libNeoPepXMLParser.so` or `.dylib`.

The Windows kit requires Visual Studio 2022 or newer and the dynamic CRT. Mixing a Release
library into a Debug build, or the reverse, fails at link time by design.

**Applications that already link MSToolkit** carry their own copy of expat. Link the static
NeoPepXMLParser and leave the kit's `libexpat*` out, so there is exactly one expat in the
program.

### API notes

- `read()` and `write()` accept `const char*` and, in C++17 code, `std::filesystem::path`. The
  path overloads open through the wide-character API on Windows, so non-ASCII paths work.
- The library is silent by default. `setProgressOutput(true)` turns on the percentage meter
  that `read()` prints to stdout.
- Numbers are parsed independently of the process locale. An application that calls
  `setlocale()` with a comma-decimal locale reads the same values as any other.
- Files are read and written in binary mode, so output is byte-identical on every platform.
- The public headers compile as C++11; the library itself is built as C++17.
- **No ABI guarantee.** The classes expose their data members directly, so any change to them
  changes the binary layout. Rebuild against each kit you upgrade to; the shared library's
  soversion equals the full version for that reason.

## Building from source

Requirements: CMake 3.21 or newer and a C++17 compiler (MSVC 2022, GCC, Clang, Apple Clang).

```sh
cmake --preset windows-x64          # or linux-x64, macos-universal
cmake --build --preset windows-x64-release
ctest --preset windows-x64-release
cpack -C "Release;Debug"            # from build/windows-x64; produces the kit archive
```

The kit presets download and build a pinned libexpat (`NEOPEPXML_EXPAT_GIT_TAG`) and bundle
it. expat can also come from elsewhere:

| Mode | Configure with | Notes |
|---|---|---|
| Bundled | `-DNEOPEPXML_FETCH_EXPAT=ON` | What the kits use. Self-contained. |
| Installed | (default) | `find_package(EXPAT)`: system package, vcpkg, Homebrew, or `CMAKE_PREFIX_PATH`. |
| Explicit | `-DNEOPEPXML_EXPAT_INCLUDE_DIR=<dir>` | Headers only, for example MSToolkit's `include/extern`. Static library only unless `NEOPEPXML_EXPAT_LIBRARY` is also given. The `windows-x64-mstoolkit` preset does this against a sibling MSToolkit clone. |

Other options: `NEOPEPXML_BUILD_STATIC`, `NEOPEPXML_BUILD_SHARED`, `NEOPEPXML_BUILD_EXAMPLES`,
`NEOPEPXML_BUILD_TESTS`, `NEOPEPXML_INSTALL`, and `NEOPEPXML_INSTALL_DOCDIR` (`.` puts the
license and README at the top of an installed tree, as the kits do).

The project can also be consumed in-tree with `add_subdirectory()` or `FetchContent`; the same
`NeoPepXMLParser::NeoPepXMLParser` target is defined.

## Development and releases

- `main` holds released code. Work happens on `dev`.
- Every push to `dev` runs the build on Windows, Linux, and macOS and publishes dev kits as
  pre-releases (see above). Pull requests are built and tested but publish nothing.
- Official releases are cut by running the **Release** workflow manually from `main`. It
  publishes `v<version>` as a pre-release for inspection; promoting it to a full release is a
  separate manual step in the GitHub UI. The version is set once, in `project()` in
  `CMakeLists.txt`.

See `CHANGELOG.md` for what changed in each version.
