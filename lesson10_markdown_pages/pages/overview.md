@page lesson10_overview Boot Control Overview

# Boot Control Overview

This page is written as a Markdown file, not a `.dox` file. The prose, headings,
lists, and code fences are standard Markdown, while Doxygen commands are mixed
in only where they add navigation or generated content.

@tableofcontents

## Why Markdown Pages

Markdown pages are useful when you want documentation that reads more like a
design note than a block of Doxygen markup. In an embedded codebase, that often
works well for:

- subsystem overviews
- integration notes
- bring-up checklists
- architecture summaries
- recovery or rollback flow descriptions

The related API module is @ref lesson10_boot_control.

## Boot Flow

The boot-control sample follows the flow below.

@dot
digraph boot_control_flow {
  rankdir=LR;
  node [shape=box];
  IDLE -> PENDING [label="boot_control_request_boot()"];
  PENDING -> CONFIRMED [label="boot_control_confirm_running_image()"];
  PENDING -> FAULT [label="boot_control_handle_failed_boot()"];
  CONFIRMED -> IDLE [label="boot_control_init()"];
  FAULT -> IDLE [label="boot_control_init()"];
}
@enddot

## Cross References

Pages written in Markdown can still link to APIs and types:

- @ref boot_control_request_boot
- @ref boot_control_confirm_running_image
- @ref boot_control_handle_failed_boot
- @ref boot_control_state_t
- @ref boot_image_info_t

## Snippet from the Example

The block below is pulled from the real example source with `@snippet`.

@snippet boot_control_example.c basic_usage

## Markdown Still Allows Doxygen Commands

You can keep the page mostly Markdown and only drop in Doxygen commands where
needed:

```text
@ref boot_control_request_boot
@snippet boot_control_example.c basic_usage
@dot ... @enddot
```

@example boot_control_example.c
Full example showing a simple boot-control confirmation flow.
