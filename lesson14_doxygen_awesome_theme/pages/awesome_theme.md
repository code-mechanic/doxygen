@page lesson14_theme Using doxygen-awesome-css

# Using doxygen-awesome-css

This lesson switches from a local hand-authored theme to the upstream
`doxygen-awesome-css` stylesheet.

@tableofcontents

## Core Doxyfile Change

The key switch is in [Doxyfile](/home/mohan/Experiments/doxygen/lesson14_doxygen_awesome_theme/Doxyfile):

```text
HTML_EXTRA_STYLESHEET = doxygen-awesome.css
```

That means Doxygen keeps generating its normal HTML, but the upstream stylesheet
redefines the presentation.

## Why This Theme Is Useful

The theme improves:

- typography
- spacing
- API block styling
- sidebar and navigation polish
- note/warning/todo/deprecated boxes

## Where to Look

This lesson's sample API intentionally includes:

- a `@warning`
- a `@note`
- a `@todo`
- a `@deprecated`

Open the module page for @ref lesson14_watchdog and compare those boxes with
what you saw in earlier lessons.

## Example Snippet

@snippet watchdog_recovery_example.c basic_usage
