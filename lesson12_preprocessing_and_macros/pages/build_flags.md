@page lesson12_build_flags Build Flags and PREDEFINED

# Build Flags and PREDEFINED

`PREDEFINED` is Doxygen's way of simulating compiler `-D` flags for the
documentation build.

@tableofcontents

## Current Feature Flags

Lesson 12 pretends the build system defined these symbols:

- `DIAG_LOG_TARGET_EXT_FLASH=1`
- `DIAG_ENABLE_ISR_STAMP=1`

Those symbols are not defined in the source files. They are provided by
[Doxyfile](/home/mohan/Experiments/doxygen/lesson12_preprocessing_and_macros/Doxyfile) so Doxygen parses
the same conditional branches that the compiler would parse for this build.

## What Appears Because of PREDEFINED

On the module page for @ref lesson12_diag_log, you should see:

- `diag_log_flush_to_ext_flash()`
- `diag_log_stamp_from_isr()`

If those symbols were not predefined, both APIs would disappear from the docs.

## EXPAND_AS_DEFINED

This lesson also uses `EXPAND_AS_DEFINED` for:

- `DIAG_CHANNEL_ENUM_ITEMS`
- `DIAG_CHANNEL_LABEL_ITEMS`

That keeps the enum and label table readable without enabling uncontrolled
expansion of every macro in the codebase.

## Suggested Experiment

Change `PREDEFINED` in the Doxyfile so both symbols are `0`, regenerate the
docs, and compare the module page again.
