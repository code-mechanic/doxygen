#ifndef LESSON1_MATH_UTILS_H
#define LESSON1_MATH_UTILS_H

/**
 * @file math_utils.h
 * @brief Small C utilities used in the first Doxygen lesson.
 */

#include <stdint.h>

/**
 * @brief Status codes returned by math utility functions.
 */
typedef enum
{
  MATH_UTILS_OK = 0,          /**< Operation completed successfully. */
  MATH_UTILS_DIVIDE_BY_ZERO   /**< Division was requested with a zero divisor. */
} math_utils_status_t;

/**
 * @brief Tracks a running sum of ADC samples.
 *
 * This is a plain C struct so you can see how Doxygen renders data types
 * commonly used in embedded software.
 */
typedef struct
{
  uint32_t sum;   /**< Sum of all samples added so far. */
  uint16_t count; /**< Number of samples added so far. */
} sample_accumulator_t;

/**
 * @brief Adds two 16-bit values.
 * @param left First input value.
 * @param right Second input value.
 * @return Sum of the two inputs.
 */
uint16_t add_u16(uint16_t left, uint16_t right);

/**
 * @brief Divides one 16-bit value by another.
 * @param numerator Value to divide.
 * @param denominator Value to divide by.
 * @param result Output pointer that receives the quotient.
 * @return `MATH_UTILS_OK` on success or `MATH_UTILS_DIVIDE_BY_ZERO` on error.
 * @warning `result` must point to valid writable memory.
 */
math_utils_status_t divide_u16(uint16_t numerator,
                               uint16_t denominator,
                               uint16_t *result);

/**
 * @brief Initializes an accumulator before first use.
 * @param accumulator Accumulator instance to initialize.
 */
void sample_accumulator_init(sample_accumulator_t *accumulator);

/**
 * @brief Adds one ADC sample to the accumulator.
 * @param accumulator Accumulator instance to update.
 * @param sample New sample value.
 */
void sample_accumulator_add(sample_accumulator_t *accumulator, uint16_t sample);

/**
 * @brief Computes the average of all added samples.
 * @param accumulator Accumulator instance to read.
 * @return Average sample value, or `0` if no samples were added.
 */
uint16_t sample_accumulator_average(const sample_accumulator_t *accumulator);

/**
 * @brief Scales a raw ADC reading to a smaller range.
 * @param raw_value Raw ADC sample.
 * @return Scaled value.
 */
uint16_t scale_raw_adc(uint16_t raw_value);

#endif  // LESSON1_MATH_UTILS_H
