#ifndef LESSON3_STATUS_LED_H
#define LESSON3_STATUS_LED_H

/**
 * @file status_led.h
 * @brief Public API for controlling board status LEDs.
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

#include "gpio_port.h"

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
 * @defgroup lesson3_status_led Status LED
 * @ingroup lesson3_board_support
 * @brief LED service built on top of the GPIO port module.
 * @{
 */

/**
 * @brief IDs for the LEDs managed by the status LED service.
 */
typedef enum
{
  STATUS_LED_GREEN = 0, /**< Green LED used for heartbeat indication. */
  STATUS_LED_RED        /**< Red LED used for error indication. */
} status_led_id_t;

// +--------------------------------------------------------------------------+
// |                                                                          |
// |                                  Public                                  |
// |                            Typedefs / Structs                            |
// |                                                                          |
// +--------------------------------------------------------------------------+

// +--------------------------------------------------------------------------+
// |                                                                          |
// |                                  Public                                  |
// |                                Functions                                 |
// |                                                                          |
// +--------------------------------------------------------------------------+

/** @name Public Init / Deinit Functions */
/** @{ */

/**
 * @brief Initializes the status LED service.
 * @retval GPIO_PORT_STATUS_OK LED service initialized successfully.
 * @retval GPIO_PORT_STATUS_NOT_READY The underlying GPIO module is not ready.
 * @pre `gpio_port_init()` must have completed successfully.
 * @post LED APIs may now be called safely.
 */
gpio_port_status_t status_led_init(void);

/** @} */

/** @name Public Run-time Functions */
/** @{ */

/**
 * @brief Turns a status LED on or off.
 * @param[in] led LED to control.
 * @param[in] is_on `true` to turn the LED on, `false` to turn it off.
 * @retval GPIO_PORT_STATUS_OK LED state updated successfully.
 * @retval GPIO_PORT_STATUS_NOT_READY The GPIO module is not ready.
 * @pre `status_led_init()` must have completed successfully.
 */
gpio_port_status_t status_led_set(status_led_id_t led, bool is_on);

/**
 * @brief Toggles a status LED.
 * @param[in] led LED to toggle.
 * @retval GPIO_PORT_STATUS_OK LED toggled successfully.
 * @retval GPIO_PORT_STATUS_NOT_READY The GPIO module is not ready.
 * @pre `status_led_init()` must have completed successfully.
 */
gpio_port_status_t status_led_toggle(status_led_id_t led);

/**
 * @brief Returns the GPIO pin used by a status LED.
 * @param[in] led LED to query.
 * @param[out] pin Pointer that receives the mapped GPIO pin.
 * @retval GPIO_PORT_STATUS_OK Mapping returned successfully.
 * @retval GPIO_PORT_STATUS_INVALID_ARG `pin` was `NULL`.
 * @note This is useful when application code needs to inspect board mappings.
 */
gpio_port_status_t status_led_get_pin(status_led_id_t led, gpio_port_pin_t *pin);

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

#endif  // LESSON3_STATUS_LED_H
