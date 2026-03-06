#include "math_utils.h"

uint16_t add_u16(uint16_t left, uint16_t right)
{
  return (uint16_t)(left + right);
}

math_utils_status_t divide_u16(uint16_t numerator,
                               uint16_t denominator,
                               uint16_t *result)
{
  if (denominator == 0U)
  {
    return MATH_UTILS_DIVIDE_BY_ZERO;
  }

  *result = (uint16_t)(numerator / denominator);
  return MATH_UTILS_OK;
}

void sample_accumulator_init(sample_accumulator_t *accumulator)
{
  accumulator->sum = 0U;
  accumulator->count = 0U;
}

void sample_accumulator_add(sample_accumulator_t *accumulator, uint16_t sample)
{
  accumulator->sum += sample;
  accumulator->count++;
}

uint16_t sample_accumulator_average(const sample_accumulator_t *accumulator)
{
  if (accumulator->count == 0U)
  {
    return 0U;
  }

  return (uint16_t)(accumulator->sum / accumulator->count);
}

uint16_t scale_raw_adc(uint16_t raw_value)
{
  return (uint16_t)(raw_value / 4U);
}
