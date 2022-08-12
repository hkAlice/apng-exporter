# APNG IDAT Exporter
Exports PNG with multiple fDATs (such as APNGs) to IDAT using the same base IHDR
Chunk CRCs are recalculated due struct changes between fDAT and IDAT. This will not work with certain APNGs.
Work in progress.

## Requirements

* C++17