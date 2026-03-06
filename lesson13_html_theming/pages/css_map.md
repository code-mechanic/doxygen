@page lesson13_css_map CSS Selector Guide

# CSS Selector Guide

When you theme Doxygen, the main difficulty is figuring out which selectors map
to visible regions of the generated HTML.

@tableofcontents

## Useful Selectors

The stylesheet in this lesson targets these areas:

- `#top`, `#titlearea`, `#projectname`, `#projectbrief`
- `.tabs`, `.tabs2`, `.tabs3`, `.tablist li a`
- `div.contents`
- `.memitem`, `.memtitle`, `.memproto`, `.memdoc`
- `dl.section dt`
- `div.fragment`, `div.line`

## Reading Strategy

Use your browser inspector on the generated HTML once, then keep a small list of
selectors that match the sections you actually care about. For most projects,
you do not need to restyle every class in Doxygen.

## Theme Scope

This lesson focuses on visual theming only. It does not yet change the page
layout structure with `LAYOUT_FILE` or custom header/footer templates.

That means the HTML still behaves like standard Doxygen, but the visual design
is more intentional and easier to brand for internal documentation sites.
