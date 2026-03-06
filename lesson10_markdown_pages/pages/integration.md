@page lesson10_integration Integration Notes

# Integration Notes

This page is also Markdown-based. It uses normal headings and lists, then adds
Doxygen-specific commands only for generated navigation and cross-links.

@tableofcontents

## Markdown Features That Work Well

The following are natural to write in Markdown pages:

- headings
- numbered lists
- bullet lists
- fenced code blocks
- inline code

For example, a bring-up sequence can be written like this:

```c
(void)boot_control_init();
(void)boot_control_request_boot(BOOT_SLOT_B);
```

## What Still Comes from Doxygen

Markdown does not replace the Doxygen engine. Doxygen still gives you:

- `@ref` cross-links into APIs
- `@snippet` extraction from source files
- `@example` pages
- `@dot` diagrams
- source browser pages
- call graphs and caller graphs

That means you can keep the document readable without losing navigation into
the real code, such as:

- [boot_control.h](/home/mohan/Experiments/doxygen/lesson10_markdown_pages/include/boot_control.h)
- [boot_control.c](/home/mohan/Experiments/doxygen/lesson10_markdown_pages/src/boot_control.c)


## Suggested Reading Order

1. Main page in the generated documentation.
2. @ref lesson10_overview "Boot Control Overview".
3. The module page for @ref lesson10_boot_control.
4. The example page listed under Examples.
