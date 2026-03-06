#ifndef LESSON4_UART_CONSOLE_INTERNAL_H
#define LESSON4_UART_CONSOLE_INTERNAL_H

/**
 * @file uart_console_internal.h
 * @brief Internal types and helpers for the UART console module.
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

#include "uart_console.h"

// +--------------------------------------------------------------------------+
// |                                                                          |
// |                                 Private                                  |
// |                          Definitions and Macros                          |
// |                                                                          |
// +--------------------------------------------------------------------------+

/**
 * @brief Maximum number of bytes supported by the lesson TX buffer.
 */
#define UART_CONSOLE_MAX_BUFFER_SIZE (64U)

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

/**
 * @addtogroup lesson4_console
 * @{
 */

/** @name Internal Types */
/** @{ */

/**
 * @brief Private runtime state for the UART console module.
 */
typedef struct
{
  uint8_t tx_buffer[UART_CONSOLE_MAX_BUFFER_SIZE]; /**< Circular transmit buffer. */
  uint16_t head;                                   /**< Next write position. */
  uint16_t tail;                                   /**< Next read position. */
  uint16_t count;                                  /**< Number of queued bytes. */
  uint16_t capacity;                               /**< Active capacity selected at init time. */
  bool is_initialized;                             /**< Indicates whether init completed. */
  uint32_t baud_rate;                              /**< Cached UART baud rate. */
} uart_console_state_t;

/** @} */

// +--------------------------------------------------------------------------+
// |                                                                          |
// |                                 Private                                  |
// |                           Function Prototypes                            |
// |                                                                          |
// +--------------------------------------------------------------------------+

/** @name Internal Functions */
/** @{ */

/**
 * @brief Resets the private runtime state to defaults.
 * @param[out] state State object to reset.
 */
void uart_console_state_reset(uart_console_state_t *state);

/** @} */

/** @} */

#ifdef __cplusplus
}
#endif

#endif  // LESSON4_UART_CONSOLE_INTERNAL_H
