# Lesson 3

This lesson focuses on organizing larger embedded C APIs with Doxygen groups:

- `@defgroup` to define a module page
- `@ingroup` to place a module under a parent module
- `@{` and `@}` to collect related items into one group
- `@name` to create sub-sections inside a module

After you generate the docs, open the modules page first and then compare the
GPIO and status LED group pages. The goal is to see how Doxygen stops being
"just function comments" and starts becoming API navigation for a real firmware
codebase.
