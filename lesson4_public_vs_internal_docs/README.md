# Lesson 4

This lesson focuses on separating public API documentation from maintainer-only
documentation in embedded C.

- Public docs describe what application code is allowed to call.
- Internal docs describe private state, helper functions, and implementation notes.
- `@internal` and `@endinternal` hide paragraphs from the public manual.
- `EXTRACT_STATIC` controls whether documented `static` helpers appear.

This lesson ships with two Doxygen configurations:

- `Doxyfile.public` builds the user-facing manual
- `Doxyfile.internal` builds the maintainer manual

It also includes `pdf` targets so you can generate HTML and a PDF from the same
source comments.
