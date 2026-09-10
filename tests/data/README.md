# Test fixtures

Real pepXML files, trimmed and redacted so the test suite stays small and fast. Each fixture keeps
every summary section of its source verbatim (search parameters, analysis summaries, models,
timestamps) and a small, deliberately varied subset of spectrum queries. File paths are replaced
with neutral relative ones; nothing else is edited, quirks included. Files in this directory are
exempt from line-ending conversion (see `.gitattributes`) because tests compare bytes.

| Fixture | Source | Exercises |
|---|---|---|
| `comet_peptideprophet_iprophet.pep.xml` | Human DDA run searched with Comet 2025.03, validated with PeptideProphet and iProphet, with decoy analysis. TPP 7.2.0 on Windows, schema pepXML_v123. | The canonical TPP pipeline output: `search_summary` with all 117 Comet parameters (one with an empty value), `aminoacid_modification`, `sample_enzyme`/`specificity`, `enzymatic_search_constraint`; `analysis_summary` for interprophet with `roc_error_data`, `error_point`, and four `mixturemodel` blocks; an empty peptideprophet `analysis_summary`; `interact_summary`/`inputfile`; `decoy_analysis_summary`/`decoy_analysis` (single-quoted attributes); `analysis_timestamp`/`database_refresh_timestamp`; 30 `spectrum_query` with `search_hit`, `search_score`, `modification_info`/`mod_aminoacid_mass`, `alternative_protein`, and both `peptideprophet_result` and `interprophet_result` with `search_score_summary`/`parameter`. |

## How `comet_peptideprophet_iprophet.pep.xml` was made

Source: `Human_DDA_A_01.ipro.pep.xml`, 133 MB, 71,733 spectrum queries, one run summary.

- **Spectrum queries:** 30. For charges 2 and 3, the first query in each combination of
  modified {yes, no} x alternative protein {yes, no} x iProphet probability {>= 0.99,
  in between, <= 0.01}; for charge 4 the first high, middle, and low; for charge 5 the first
  high and low; for charge 6 the first one. Original order and `index` attributes are kept, so
  indexes are not contiguous.
- **Model points:** every 25th `point` in each `mixturemodel` (20 per model); every 5th `point`
  in `decoy_analysis`.
- **Redaction:** `d:\TPP\data\Booster\...` became `data/...`; the database
  `d:/TPP/data/dbase/UP000005640_human_GC_May2025_DECOY.fasta` became
  `data/UP000005640_human_DECOY.fasta`, in the search database, the refresh timestamp, and the
  `database_name` parameter.
- **Verified:** reads, writes, and re-reads with identical element counts; a second write is
  byte-identical to the first. Building this fixture is what exposed the empty-`value`
  parameter bug fixed in 1.1.1.

## Adding a fixture

1. Profile the source: element inventory, and the spread of charge, modifications, alternative
   proteins, and probabilities across spectrum queries.
2. Keep all summary sections. Thin repetitive model points. Choose a stratified subset of spectrum
   queries so each combination of features appears at least once.
3. Redact paths only. Keep everything else verbatim, including quirks such as empty attributes
   or single-quoted values; those are the cases worth testing.
   Keep the source's line endings as well (this fixture is LF, as TPP writes it). On Git Bash for
   Windows, awk and sed strip carriage returns while perl keeps them, so check
   `tr -cd "" < file | wc -c` before and after processing a CRLF source.
4. Round-trip the result through the library before committing it.
5. Name the file after what it exercises, aim for under 100 KB, and add a row to the table above
   with the source, selection, and coverage.
