# Lesson 2

This lesson focuses on documenting API contracts in embedded C:

- `@param[in]` and `@param[out]`
- `@retval` for status-based APIs
- `@pre` and `@post` for call ordering and state changes
- `@note` for timing or hardware-related behavior

After you generate the docs, compare these APIs:

- `adc_sampler_read_once()` uses explicit direction annotations
- `adc_sampler_raw_to_mv()` uses plain `@param`

That contrast is intentional for the exercise at the end.
