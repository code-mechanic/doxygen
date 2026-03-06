# Lesson 8

This lesson focuses on documenting compile-time configuration in embedded C.

- board feature flags
- board-selected peripheral instance and pin mapping
- compile-time buffer sizes
- variant-dependent APIs that exist only for certain builds

The sample is a telemetry UART driver controlled by a board configuration header.
The goal is to make the generated docs explain what changes between variants
without forcing someone to read `#if` blocks line by line.
