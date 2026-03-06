@page lesson13_theme HTML Theme Overview

# HTML Theme Overview

This lesson keeps the Doxygen HTML structure intact and changes the look through
configuration plus one custom CSS file.

@tableofcontents

## Theme Controls Used Here

The key settings in [Doxyfile](/home/mohan/Experiments/doxygen/lesson13_html_theming/Doxyfile)
are:

| Setting | Purpose |
| --- | --- |
| `HTML_COLORSTYLE_HUE` | Nudges the built-in palette before custom CSS is applied |
| `HTML_COLORSTYLE_SAT` | Adjusts the saturation of the built-in palette |
| `HTML_COLORSTYLE_GAMMA` | Adjusts brightness/contrast of the built-in palette |
| `HTML_EXTRA_STYLESHEET` | Adds your own CSS overrides |
| `PROJECT_LOGO` | Adds a project logo to the title area |

## What the Custom CSS Changes

The stylesheet in [theme.css](/home/mohan/Experiments/doxygen/lesson13_html_theming/theme.css)
changes:

- the top bar and title area
- tab styling
- content width and spacing
- API member cards
- code blocks
- note and warning labels

## Why the Extra Stylesheet Option Is the Practical Default

Replacing Doxygen's entire stylesheet is possible, but it is harder to maintain.
Using `HTML_EXTRA_STYLESHEET` lets you keep the default structure and override
only the parts you care about.

## Example Snippet

@snippet fault_journal_example.c basic_usage

## Where to Look in the Output

After generating the docs, compare:

- the title bar with the custom logo
- the module page for @ref lesson13_fault_journal
- the code block styling on source pages
