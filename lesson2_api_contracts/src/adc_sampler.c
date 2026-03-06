// +--------------------------------------------------------------------------+
// |                                                                          |
// |                                 Includes                                 |
// |                                                                          |
// +--------------------------------------------------------------------------+

#include "adc_sampler.h"

// +--------------------------------------------------------------------------+
// |                                                                          |
// |                                 Private                                  |
// |                          Definitions and Macros                          |
// |                                                                          |
// +--------------------------------------------------------------------------+

// +--------------------------------------------------------------------------+
// |                                                                          |
// |                                 Private                                  |
// |                            Typedefs / Enums                              |
// |                                                                          |
// +--------------------------------------------------------------------------+

// +--------------------------------------------------------------------------+
// |                                                                          |
// |                                 Private                                  |
// |                            Typedefs / Structs                            |
// |                                                                          |
// +--------------------------------------------------------------------------+

// +--------------------------------------------------------------------------+
// |                                                                          |
// |                                 Private                                  |
// |                           Function Prototypes                            |
// |                                                                          |
// +--------------------------------------------------------------------------+

// +--------------------------------------------------------------------------+
// |                                                                          |
// |                                 Private                                  |
// |                          Variable Declarations                           |
// |                                                                          |
// +--------------------------------------------------------------------------+

static bool g_is_ready = false;
static adc_sampler_config_t g_config = {0U, 0U, 0U};
static uint16_t g_next_raw_sample = 512U;

// +--------------------------------------------------------------------------+
// |                                                                          |
// |                                  Public                                  |
// |                                Functions                                 |
// |                                                                          |
// +--------------------------------------------------------------------------+

adc_sampler_status_t adc_sampler_init(const adc_sampler_config_t *config)
{
  if ((config == 0) || (config->channel > 15U))
  {
    return ADC_SAMPLER_STATUS_INVALID_ARG;
  }

  g_config = *config;
  g_is_ready = true;
  return ADC_SAMPLER_STATUS_OK;
}

bool adc_sampler_is_ready(void)
{
  return g_is_ready;
}

adc_sampler_status_t adc_sampler_read_once(uint16_t *sample_mv)
{
  if (sample_mv == 0)
  {
    return ADC_SAMPLER_STATUS_INVALID_ARG;
  }

  if (!g_is_ready)
  {
    return ADC_SAMPLER_STATUS_NOT_READY;
  }

  *sample_mv = adc_sampler_raw_to_mv(g_next_raw_sample, g_config.reference_mv);
  g_next_raw_sample = (uint16_t)((g_next_raw_sample + 137U) & 0x0FFFU);
  return ADC_SAMPLER_STATUS_OK;
}

adc_sampler_status_t adc_sampler_read_average(uint8_t sample_count,
                                              uint16_t *average_mv)
{
  uint32_t total = 0U;
  uint8_t index = 0U;
  uint16_t sample_mv = 0U;

  if ((sample_count == 0U) || (average_mv == 0))
  {
    return ADC_SAMPLER_STATUS_INVALID_ARG;
  }

  if (!g_is_ready)
  {
    return ADC_SAMPLER_STATUS_NOT_READY;
  }

  while (index < sample_count)
  {
    if (adc_sampler_read_once(&sample_mv) != ADC_SAMPLER_STATUS_OK)
    {
      return ADC_SAMPLER_STATUS_NOT_READY;
    }

    total += sample_mv;
    index++;
  }

  *average_mv = (uint16_t)(total / sample_count);
  return ADC_SAMPLER_STATUS_OK;
}

uint16_t adc_sampler_raw_to_mv(uint16_t raw_code, uint16_t reference_mv)
{
  return (uint16_t)(((uint32_t)raw_code * reference_mv) / 4095U);
}

void adc_sampler_deinit(void)
{
  g_is_ready = false;
}

// +--------------------------------------------------------------------------+
// |                                                                          |
// |                                 Private                                  |
// |                                Functions                                 |
// |                                                                          |
// +--------------------------------------------------------------------------+

