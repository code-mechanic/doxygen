@page lesson14_vendor Vendoring and Attribution

# Vendoring and Attribution

When you use a third-party Doxygen theme, treat it like a vendored dependency.

@tableofcontents

## Files in This Lesson

Lesson 14 includes:

- [doxygen-awesome.css](/home/mohan/Experiments/doxygen/lesson14_doxygen_awesome_theme/doxygen-awesome.css)
- [THIRD_PARTY_LICENSE.md](/home/mohan/Experiments/doxygen/lesson14_doxygen_awesome_theme/THIRD_PARTY_LICENSE.md)

## Why Keep the License Nearby

If someone copies this lesson into another project, the theme attribution should
travel with it. Keeping the upstream source and license in the lesson directory
makes that easier.

## Practical Update Workflow

If you want to upgrade the theme later:

1. replace the vendored `doxygen-awesome.css` file with a newer upstream version,
2. keep your Doxyfile stable,
3. add a separate custom stylesheet only if you need local overrides.

## HTML Versus PDF

This theme affects HTML output only. The PDF build still uses Doxygen's LaTeX
pipeline and will look like the earlier lessons.
