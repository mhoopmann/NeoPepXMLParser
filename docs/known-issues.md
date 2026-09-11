# Known issues

Outstanding defects and quirks in the library, kept current so nobody has to rediscover them.
Plans and unstarted work are in [progress.md](progress.md); resolved items are recorded in
[history.md](history.md) under BUG-n. When an entry here is fixed, move it there.

Last reviewed: 2026-09-11.

## Open defects

None known. Every defect found during the 2026-09 modernization (BUG-1 through BUG-5) is fixed
on `dev`.

## Quirks and limitations, by design or not yet worth fixing

- **Non-const accessors.** `present()` on `CnpxPeptideProphetResult`, `CnpxInterprophetResult`,
  and `CnpxSearchScoreSummary`, and `npxDateTime::write()`, are non-const, so they cannot be
  called through a const reference. Harmless; fix when convenient (a const qualifier each).
- **`killRead` is inert.** `NeoPepXMLParser::killRead` is set to false and checked in the read
  loop, but nothing sets it to true. Either remove it or give it a purpose (an abort hook).
- **Mixed include styles.** Public headers include each other by bare file name; sources use the
  `NeoPepXMLParser/` prefix. Both compile; the mix is cosmetic.
- **Locale tests skip on Linux CI.** The Ubuntu runner has no comma-decimal locale, so the two
  `[locale]` tests are skipped there. Windows and macOS run them, which covers both
  implementations of the locale guard.
- **Parameters in unexpected places are dropped with a warning.** `parameter` elements outside a
  `search_summary`, `search_score_summary`, or model distribution are recorded as misplaced and
  not stored. This mirrors the previous behavior, which printed and dropped them.
- **Skipped content is lost on write.** By design (DEC-7): an unknown element is not carried
  through. A file containing one will not round-trip byte-for-byte; `warnings()` says so.
- **Static-library consumers must define `NEOPEPXML_STATIC_DEFINE`** when not using the CMake
  package. Documented in the README; a common first stumble on Windows.

## Untested areas

- Element writers for PTMProphet, Libra, XPRESS label-free, Quantic, and crosslink (Kojak)
  elements are exercised by no fixture yet. They compile and were read-verified only against a
  133 MB Comet/Prophet file that does not contain them.
- The POSIX branch of the locale guard (`src/NeoPepXMLLocale.h`) and the `strtod_l` fallback of
  `npxAtof` run only on the macOS CI runner.
