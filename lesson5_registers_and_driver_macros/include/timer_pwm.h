#ifndef LESSON5_TIMER_PWM_H
#define LESSON5_TIMER_PWM_H

/**
 * @file timer_pwm.h
 * @brief Public API for a simple timer-based PWM driver.
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

#include "timer_regs.h"

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
 * @addtogroup lesson5_timer
 * @{
 */

/**
 * @brief Status codes returned by the timer PWM driver.
 */
typedef enum
{
  TIMER_PWM_STATUS_OK = 0,      /**< Operation completed successfully. */
  TIMER_PWM_STATUS_INVALID_ARG, /**< One or more arguments were invalid. */
  TIMER_PWM_STATUS_NOT_READY    /**< Driver was used before initialization. */
} timer_pwm_status_t;

// +--------------------------------------------------------------------------+
// |                                                                          |
// |                                  Public                                  |
// |                            Typedefs / Structs                            |
// |                                                                          |
// +--------------------------------------------------------------------------+

/**
 * @brief Configuration used to initialize a timer PWM channel.
 */
typedef struct
{
  timer_instance_t instance; /**< Timer instance to configure. */
  uint8_t clock_divider;     /**< Clock-divider value encoded into the CTRL register. */
  uint16_t period_ticks;     /**< PWM period in timer ticks. */
  uint16_t duty_ticks;       /**< PWM high time in timer ticks. */
} timer_pwm_config_t;

// +--------------------------------------------------------------------------+
// |                                                                          |
// |                                  Public                                  |
// |                                Functions                                 |
// |                                                                          |
// +--------------------------------------------------------------------------+

/** @name Public Driver API */
/** @{ */

/**
 * @brief Initializes one timer instance for PWM output.
 * @param[in] config Pointer to the configuration to apply.
 * @retval TIMER_PWM_STATUS_OK Timer configured successfully.
 * @retval TIMER_PWM_STATUS_INVALID_ARG `config` was `NULL` or the timing values were invalid.
 * @post The timer registers are configured but the timer is not running yet.
 */
timer_pwm_status_t timer_pwm_init(const timer_pwm_config_t *config);

/**
 * @brief Starts PWM generation on a timer instance.
 * @param[in] instance Timer instance to start.
 * @retval TIMER_PWM_STATUS_OK Timer started successfully.
 * @retval TIMER_PWM_STATUS_NOT_READY The timer instance has not been initialized.
 * @pre `timer_pwm_init()` must have completed successfully for the selected instance.
 */
timer_pwm_status_t timer_pwm_start(timer_instance_t instance);

/**
 * @brief Stops PWM generation on a timer instance.
 * @param[in] instance Timer instance to stop.
 * @retval TIMER_PWM_STATUS_OK Timer stopped successfully.
 * @retval TIMER_PWM_STATUS_NOT_READY The timer instance has not been initialized.
 * @pre `timer_pwm_init()` must have completed successfully for the selected instance.
 */
timer_pwm_status_t timer_pwm_stop(timer_instance_t instance);

/**
 * @brief Reports whether an overflow event is latched in the timer status register.
 * @param[in] instance Timer instance to query.
 * @param[out] is_set Pointer that receives the overflow flag state.
 * @retval TIMER_PWM_STATUS_OK Overflow state returned successfully.
 * @retval TIMER_PWM_STATUS_INVALID_ARG `is_set` was `NULL`.
 * @retval TIMER_PWM_STATUS_NOT_READY The timer instance has not been initialized.
 */
timer_pwm_status_t timer_pwm_is_overflow_set(timer_instance_t instance, bool *is_set);

/**
 * @brief Clears the overflow flag in the timer status register.
 * @param[in] instance Timer instance to update.
 * @retval TIMER_PWM_STATUS_OK Overflow flag cleared successfully.
 * @retval TIMER_PWM_STATUS_NOT_READY The timer instance has not been initialized.
 */
timer_pwm_status_t timer_pwm_clear_overflow(timer_instance_t instance);

/** @} */

/** @} */

// +--------------------------------------------------------------------------+
// |                                                                          |
// |                          Static inline Functions                         |
// |                                                                          |
// +--------------------------------------------------------------------------+

#ifdef __cplusplus
}
#endif

#endif  // LESSON5_TIMER_PWM_H
