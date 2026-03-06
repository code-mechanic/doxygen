#ifndef LESSON2_ADC_SAMPLER_H
#define LESSON2_ADC_SAMPLER_H

/**
 * @file adc_sampler.h
 * @brief Public API for a simple ADC sampler module.
 */

#ifdef __cplusplus
extern "C" {
#endif

// +--------------------------------------------------------------------------+
// |                                                                          |
// |                                 Includes                                 |
// |                                                                          |
// +--------------------------------------------------------------------------+

#include <stdbool.h>
#include <stdint.h>

// +--------------------------------------------------------------------------+
// |                                                                          |
// |                                  Public                                  |
// |                          Definitions and Macros                          |
// |                                                                          |
// +--------------------------------------------------------------------------+

// +--------------------------------------------------------------------------+
// |                                                                          |
// |                                  Public                                  |
// |                            Typedefs / Enums                              |
// |                                                                          |
// +--------------------------------------------------------------------------+

/**
 * @brief Status codes returned by the ADC sampler module.
 */
typedef enum
{
  ADC_SAMPLER_STATUS_OK = 0,      /**< Operation completed successfully. */
  ADC_SAMPLER_STATUS_INVALID_ARG, /**< One or more arguments were invalid. */
  ADC_SAMPLER_STATUS_NOT_READY    /**< Module was used before initialization. */
} adc_sampler_status_t;

// +--------------------------------------------------------------------------+
// |                                                                          |
// |                                  Public                                  |
// |                            Typedefs / Structs                            |
// |                                                                          |
// +--------------------------------------------------------------------------+

/**
 * @brief Configuration used when the ADC sampler is initialized.
 */
typedef struct
{
  uint8_t channel;           /**< ADC channel number to sample. */
  uint16_t reference_mv;     /**< ADC reference voltage in millivolts. */
  uint16_t startup_delay_us; /**< Delay before the first conversion is allowed. */
} adc_sampler_config_t;

// +--------------------------------------------------------------------------+
// |                                                                          |
// |                                  Public                                  |
// |                                Functions                                 |
// |                                                                          |
// +--------------------------------------------------------------------------+

/**
 * @brief Initializes the ADC sampler module.
 * @param[in] config Pointer to the configuration to apply.
 * @retval ADC_SAMPLER_STATUS_OK Initialization completed successfully.
 * @retval ADC_SAMPLER_STATUS_INVALID_ARG `config` was `NULL` or the channel was invalid.
 * @post The module is ready to accept read requests.
 */
adc_sampler_status_t adc_sampler_init(const adc_sampler_config_t *config);

/**
 * @brief Returns whether the ADC sampler is ready to be used.
 * @retval true Module is initialized.
 * @retval false Module is not initialized.
 */
bool adc_sampler_is_ready(void);

/**
 * @brief Reads one ADC sample and converts it to millivolts.
 * @param[out] sample_mv Pointer that receives the converted sample.
 * @retval ADC_SAMPLER_STATUS_OK Sample was read successfully.
 * @retval ADC_SAMPLER_STATUS_INVALID_ARG `sample_mv` was `NULL`.
 * @retval ADC_SAMPLER_STATUS_NOT_READY The module has not been initialized.
 * @pre `adc_sampler_init()` must have completed successfully.
 * @note In a real driver this function may block until the conversion completes.
 */
adc_sampler_status_t adc_sampler_read_once(uint16_t *sample_mv);

/**
 * @brief Reads several samples and returns the average in millivolts.
 * @param[in] sample_count Number of samples to average.
 * @param[out] average_mv Pointer that receives the averaged value.
 * @retval ADC_SAMPLER_STATUS_OK Average was computed successfully.
 * @retval ADC_SAMPLER_STATUS_INVALID_ARG `sample_count` was zero or `average_mv` was `NULL`.
 * @retval ADC_SAMPLER_STATUS_NOT_READY The module has not been initialized.
 * @pre `adc_sampler_init()` must have completed successfully.
 */
adc_sampler_status_t adc_sampler_read_average(uint8_t sample_count,
                                              uint16_t *average_mv);

/**
 * @brief Converts a raw ADC code to millivolts.
 * @param[in] raw_code Raw ADC conversion result.
 * @param[in] reference_mv ADC reference voltage in millivolts.
 * @return Converted sample in millivolts.
 * @note This conversion assumes a 12-bit ADC range of 0 to 4095.
 */
uint16_t adc_sampler_raw_to_mv(uint16_t raw_code, uint16_t reference_mv);

/**
 * @brief Shuts down the ADC sampler module.
 * @post The module is no longer ready for sampling.
 */
void adc_sampler_deinit(void);

// +--------------------------------------------------------------------------+
// |                                                                          |
// |                          Static inline Functions                         |
// |                                                                          |
// +--------------------------------------------------------------------------+

#ifdef __cplusplus
}
#endif

#endif  // LESSON2_ADC_SAMPLER_H
