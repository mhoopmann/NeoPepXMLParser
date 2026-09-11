# Modernization History (2026-09-10 / 2026-09-11)

Archival record of the restructuring of NeoPepXMLParser from a flat source tree with a GNU
Makefile and a Visual Studio project into a CMake library with prebuilt dev kits, CI, a test
suite, and error reporting. Every item is done; the status of what came after lives in
[progress.md](progress.md).

Nothing here needs to be read to work in the repo today. It records *why* the code and the
repository look the way they do, so that decisions are not relitigated by accident. Commit
hashes refer to the `dev` branch; `v1.1.0` is merge commit 76ab348 on `main`.

---

## Decisions

Settled in discussion with the repository owner before any file changed. Treat as fixed unless
the owner reopens them.

### DEC-1 — Class names mirror the pepXML schema, quirks included
`CnpxMixtureModel` and `CnpxMixture_Model` are two different elements (`mixturemodel` and
`mixture_model`); `CnpxPeptideprophetSummary` and `CnpxPeptideProphetResult` follow the
schema's own casing. This is deliberate. Do not propose renames.

### DEC-2 — Pure CMake, Pitchfork layout, no committed IDE projects
Option chosen over keeping a hand-maintained vcxproj beside CMake, even as a transitional shim,
because a second build description tends to outlive its deadline. All headers are public and
live in `include/NeoPepXMLParser/` because consumers navigate the parsed object tree directly.
Consumers that used the flat tree (Telescope) moved to a dev kit; the owner controls those.

### DEC-3 — expat comes from wherever the consumer says
Three modes in `cmake/NeoPepXMLExpat.cmake`: an explicit header directory (MSToolkit's bundled
copy, the historical arrangement), `find_package(EXPAT)`, or a pinned libexpat built from
source. The kits use the third so they are self-contained; the shared library links expat
internally with hidden visibility so two expats in one process cannot interpose each other.

### DEC-4 — Both static and shared, no ABI guarantee
Windows static library carries a `_static` suffix; Debug binaries carry `d`. The classes
expose their data members, so any change alters the binary layout; rather than pimpl on
67 classes, the README states there is no ABI guarantee and the soversion equals the full
version. `NeoPepXMLParser::NeoPepXMLParser` refers to the static library by default.

### DEC-5 — C++17 inside, C++11 at the boundary
The library compiles as C++17 (`std::filesystem`, `std::from_chars`); the public headers stay
usable from C++11 consumers, checked by compiling the example as C++11 in CI.

### DEC-6 — Version and release mechanics
1.1.0 was the first release from the new structure (not 2.0.0, by the owner's choice). The
version lives once in `project(VERSION ...)`; a generated header carries it into the code and
`versionNeo()` reports it with a dev suffix and commit hash. Work happens on `dev`; every push
publishes dated (`dev-<version>-<run>`) and rolling (`dev-latest`) pre-release kits; the
Release workflow is manual, refuses any ref but `main`, and refuses to overwrite a promoted
release. Platforms: x64 Windows (Release and Debug), x86_64 Linux built on Ubuntu 22.04,
universal macOS. Nothing else.

### DEC-7 — Error handling contract
`read()` and `write()` return bool and never throw; `lastError()` explains. Reference-returning
accessors throw `npxRangeError`. The library is silent unless `setDiagnosticOutput(true)`.
Unknown elements are skipped with a warning rather than failing the read. The writers'
"required attribute" checks were removed outright rather than kept as warnings.

### DEC-8 — Fixtures
Real files, trimmed and redacted, named after what they exercise, under 100 KB, with a
committed expected output. Test framework: Catch2 v3, pinned by archive checksum.

---

## Structure

### STRUCT-1 — Layout and CMake build
**Commits:** 02f1e19 (line endings), 3d8d3c7 (layout and build), 36e4fb8 (see CODE-1).
Headers to `include/NeoPepXMLParser/`, sources to `src/`, Makefile removed, `.gitattributes`
and `.gitignore` added. The index was already LF, so the normalization commit changed no
content; it was still made separately so the moves kept rename detection. The Makefile had a
dead `realclean` target, a nonexistent include path, and comments copied from MSToolkit.

### STRUCT-2 — Libraries, package, dev kits
**Commit:** 8f993ee. Separate static and shared targets from the same sources (an object
library would leak dllexport into the static archive). Export macros from
`GenerateExportHeader` on all 67 classes and the free functions; hidden visibility on the
shared library; DLL export table verified to contain no expat symbols. Install rules export
`NeoPepXMLParser::static`, `::shared`, and `::expat`; the package config resolves expat for the
mode the package was built with. CPack produces `NeoPepXMLParser-<version>-<os>-<arch>` as zip
on Windows and tar.gz elsewhere (zip cannot hold the `.so` symlinks). A kit was consumed by a
C++14 static and a C++17 shared test program in Release and Debug before commit. The old VS
project had whole-program optimization on, which made a 71 MB static library that only the
exact same compiler could link; the new one is 3 MB.

### STRUCT-3 — Presets, example, workflows
**Commits:** 71c5c5d, 57f89a9 (see BUG-5). `CMakePresets.json` for the three kit builds plus a
`windows-x64-mstoolkit` preset for local work against MSToolkit's expat headers. A reusable
`build.yml` matrix (windows-2022, ubuntu-22.04, macos-14) called by `ci.yml`, `dev-build.yml`,
and `release.yml`; third-party actions pinned to commit SHAs. The example program doubles as a
CTest smoke test compiled as C++11. The first push went green on all three platforms with no
changes; macOS binaries were confirmed to be two-architecture fat files by reading their
headers.

---

## Code

### CODE-1 — Platform handling and I/O
**Commit:** 36e4fb8. The shim block in `NeoPepXMLStructs.h` selected seek/tell macros through
user-supplied defines (`GCC`, `OSX`, `MINGW`) that no compiler sets, and served three lines of
progress-meter code; replaced by `<cstdint>` and `std::filesystem::file_size`. expat's parser
handle is forward-declared so consumers need no expat headers; only `NeoPepXMLParser.cpp`
includes `expat.h`. Files are read and written in binary mode for byte-identical output.
`std::filesystem::path` overloads open through the wide API on Windows. The progress meter
became opt-in. Static helpers and namespace strings moved out of the header, which alone
removed 93 of the 101 level-4 warnings.

### CODE-2 — Locale-independent parsing
**Commit:** 36e4fb8. The parser used `atof` 106 times; under a comma-decimal process locale
every mass and probability was silently truncated at the decimal point. `npxAtof` uses
`std::from_chars` where the standard library provides it and `strtod` in an explicit "C"
locale otherwise (libc++). Verified under a German locale before commit.

### CODE-3 — Locale-independent writing
**Commit:** ebd455f. The same bug in the other direction, found by the test suite: `fprintf`
honors `LC_NUMERIC`, so the writer produced commas. Rather than touch about 90 formatting calls,
`src/NeoPepXMLLocale.h` switches only the calling thread's numeric locale to "C" for the
duration of `write()`, the pipeline-analysis writer, and `getModifiedPeptide()`, and restores
it. Per-thread locale on Windows, `uselocale` on POSIX.

### CODE-4 — Error handling
**Commit:** 271bbaa. Twelve `exit()` calls removed. `npxDiagnostic`, `npxError`,
`npxRangeError` in `NeoPepXMLError.h`; `lastError()`, `warnings()`, `setDiagnosticOutput()` on
the parser. Unknown elements are skipped by a depth counter in `startElement`/`endElement`;
throwing through expat's C frames would be undefined behavior, so the read path never throws.
The 40 "required attribute" checks and `NPXerrMsg` were removed: they inferred absence from an
empty or zero value, rejected valid files (BUG-2), and could only abort. `write()` now checks
the stream and removes a partial file on error.

---

## Bugs found along the way

### BUG-1 — Uninitialized members in `CnpxPeptideprophetSummary`
**Commit:** 36e4fb8. The constructor declared locals named `est_tot_num_correct` and `min_prob`
instead of assigning the members. Exposed by MSVC level-4 warnings during STRUCT-1.

### BUG-2 — Writer rejected `value=""`
**Commit:** d327a18. Comet emits `<parameter name="mass_offsets" value=""/>`; the writer took an
empty value as a missing required attribute and terminated. Every Comet result file failed to
write back. Found while building the first fixture (TEST-1).

### BUG-3 — `setFilterRunSummary()` stripped every search hit
**Commit:** 2f1d0f9. A stub in the `search_hit` handler, present since the filters were added in
3882eb7 (2020-06-19), was guarded by the run-summary filter instead of the search-hit filter
and compared an `int` from `compare()` against `std::string::npos`. Nothing had ever called the
filters, so it went unnoticed for six years. The stub was deleted; the query-level filter now
tolerates queries without hits.

### BUG-4 — `CnpxUIPSM` parameter lists never filled
**Commit:** e6b2fe5 (fix by the owner). The loops copied each Prophet score into a local and
never pushed it. Design confirmed as one PeptideProphet and one iProphet result per query, so
clear-then-fill per PSM is correct; only the push was missing.

### BUG-5 — Dev tags created on the wrong commit
**Commit:** 57f89a9. The release action creates a missing tag on the default branch unless told
otherwise, so `dev-1.1.0-1` and `dev-1.1.0-2` pointed at the pre-restructure tip of `main`
while their kits were correct. Both workflows now pass `target_commitish`; the owner moved the
two tags by hand.

---

## Tests

### TEST-1 — First fixture
**Commit:** 39442ea. `tests/data/comet_peptideprophet_iprophet.pep.xml`, 94 KB, from a 133 MB
TPP 7.2.0 result (Comet 2025.03, PeptideProphet, iProphet, decoy analysis). Every summary
section kept verbatim, model points thinned, 30 spectrum queries chosen so each combination of
charge, modification, alternative protein, and probability range appears once, paths redacted.
The source file is LF; an early measurement claiming CRLF was wrong and briefly led to CRLF
being added and then reverted. On Git Bash for Windows, awk and sed strip carriage returns
while perl keeps them. Recipe in `tests/data/README.md`.

### TEST-2 — Catch2 suite
**Commit:** 7a2d70b, extended by 2f1d0f9, e6b2fe5, ebd455f, 271bbaa. Catch2 v3.16.0 by
FetchContent, pinned by SHA-256 of the release archive, built only with `NEOPEPXML_BUILD_TESTS`.
Round trip with byte-identical second write and a committed expected output; element inventory;
CRLF input; about a hundred hand-verified values; filters; UI layer; helpers; locale; error
paths; non-ASCII paths; throwing accessors. Three findings (CODE-3, BUG-3, BUG-4) were first
committed as `[!mayfail]` tests so the run stayed green until each was fixed.

---

## Documentation

### DOC-1 — README, CHANGELOG, fixture recipe, this folder
**Commits:** df5f8c9, eca4005, fa4db94, and the commit adding `docs/`. The README covers
obtaining a kit, consuming it with and without CMake, the API changes, the source build, and
the release process; the changelog records 1.1.0 and the open 1.1.1 section.

---

## Releases

### REL-1 — v1.1.0
Merged `dev` to `main` via PR #1 (76ab348) and released on 2026-09-10 by the owner. Kits for all
three platforms attached; promoted to a full release the same day. `dev-1.1.0-1` and
`dev-1.1.0-2` preceded it. The 1.1.1 cycle opened with 57f89a9.
