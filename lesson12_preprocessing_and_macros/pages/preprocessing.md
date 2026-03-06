@page lesson12_preprocessing Preprocessing and Macro Expansion

# Preprocessing and Macro Expansion

This lesson is about the part of Doxygen that tries to understand code after
preprocessor-like expansion. That matters in embedded C because APIs are often
shaped by:

- build flags from the compiler command line
- macro-generated enum members
- macro-generated tables
- conditional declarations hidden behind `#if`

@tableofcontents

## Why It Matters

If Doxygen does not see the same symbols that the compiler sees, the generated
documentation can be misleading. A function that exists in a particular build
may disappear from the docs, or macro-generated declarations may look unreadable.

## Settings Used in This Lesson

The current [Doxyfile](/home/mohan/Experiments/doxygen/lesson12_preprocessing_and_macros/Doxyfile) enables:

- `ENABLE_PREPROCESSING`
- `MACRO_EXPANSION`
- `EXPAND_ONLY_PREDEF`
- `PREDEFINED`
- `EXPAND_AS_DEFINED`

Those settings tell Doxygen:

1. preprocess the input,
2. honor the two build-style feature flags,
3. expand the macro lists that define channel items and labels.

## Macro-Generated Channels

In [diag_log.h](/home/mohan/Experiments/doxygen/lesson12_preprocessing_and_macros/include/diag_log.h),
the `diag_channel_t` enum is built from `DIAG_CHANNEL_ENUM_ITEMS`.

That means the readable enum entries in the docs come from macro expansion,
not from handwritten enum lines.

## Example Snippet

@snippet diag_log_example.c basic_usage

## Practical Result

Because preprocessing is configured correctly, the module page shows the current
build variant instead of a generic, half-parsed view of the header.

@example diag_log_example.c
Full example showing the diagnostic-log initialization path.
