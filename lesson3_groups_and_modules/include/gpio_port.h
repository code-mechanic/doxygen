#ifndef LESSON3_GPIO_PORT_H
#define LESSON3_GPIO_PORT_H

/**
 * @file gpio_port.h
 * @brief Public API for a simple GPIO port driver.
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
 * @defgroup lesson3_gpio GPIO Port
 * @ingroup lesson3_board_support
 * @brief GPIO abstraction used by board-level services.
 * @{
 */

/**
 * @brief Status codes returned by the GPIO port module.
 */
typedef enum
{
  GPIO_PORT_STATUS_OK = 0,      /**< Operation completed successfully. */
  GPIO_PORT_STATUS_INVALID_ARG, /**< One or more arguments were invalid. */
  GPIO_PORT_STATUS_NOT_READY    /**< Module was used before initialization. */
} gpio_port_status_t;

/**
 * @brief Logical level driven on or read from a pin.
 */
typedef enum
{
  GPIO_PORT_LEVEL_LOW = 0, /**< Pin is at logic low. */
  GPIO_PORT_LEVEL_HIGH    /**< Pin is at logic high. */
} gpio_port_level_t;

// +--------------------------------------------------------------------------+
// |                                                                          |
// |                                  Public                                  |
// |                            Typedefs / Structs                            |
// |                                                                          |
// +--------------------------------------------------------------------------+

/**
 * @brief Identifies one GPIO pin on the board.
 */
typedef struct
{
  uint8_t port_index; /**< Zero-based port number. */
  uint8_t pin_index;  /**< Zero-based pin number within the port. */
} gpio_port_pin_t;

// +--------------------------------------------------------------------------+
// |                                                                          |
// |                                  Public                                  |
// |                                Functions                                 |
// |                                                                          |
// +--------------------------------------------------------------------------+

/** @name Public Init / Deinit Functions */
/** @{ */

/**
 * @brief Initializes the GPIO module.
 * @retval GPIO_PORT_STATUS_OK Module initialized successfully.
 * @post GPIO APIs may now be called safely.
 */
gpio_port_status_t gpio_port_init(void);

/**
 * @brief Deinitializes the GPIO module.
 * @retval GPIO_PORT_STATUS_OK Module deinitialized successfully.
 */
gpio_port_status_t gpio_port_deinit(void);

/** @} */

/** @name Public Run-time Functions */
/** @{ */

/**
 * @brief Drives a pin to the requested level.
 * @param[in] pin Pin to update.
 * @param[in] level Logic level to drive on the pin.
 * @retval GPIO_PORT_STATUS_OK Pin updated successfully.
 * @retval GPIO_PORT_STATUS_NOT_READY The module has not been initialized.
 * @pre `gpio_port_init()` must have completed successfully.
 */
gpio_port_status_t gpio_port_write(gpio_port_pin_t pin, gpio_port_level_t level);

/**
 * @brief Reads the current level of a pin.
 * @param[in] pin Pin to sample.
 * @param[out] level Pointer that receives the current level.
 * @retval GPIO_PORT_STATUS_OK Pin level read successfully.
 * @retval GPIO_PORT_STATUS_INVALID_ARG `level` was `NULL`.
 * @retval GPIO_PORT_STATUS_NOT_READY The module has not been initialized.
 * @pre `gpio_port_init()` must have completed successfully.
 */
gpio_port_status_t gpio_port_read(gpio_port_pin_t pin, gpio_port_level_t *level);

/**
 * @brief Toggles the current state of a pin.
 * @param[in] pin Pin to toggle.
 * @retval GPIO_PORT_STATUS_OK Pin toggled successfully.
 * @retval GPIO_PORT_STATUS_NOT_READY The module has not been initialized.
 * @pre `gpio_port_init()` must have completed successfully.
 */
gpio_port_status_t gpio_port_toggle(gpio_port_pin_t pin);

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

#endif  // LESSON3_GPIO_PORT_H
