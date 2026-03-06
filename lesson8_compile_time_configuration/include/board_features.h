#ifndef LESSON8_BOARD_FEATURES_H
#define LESSON8_BOARD_FEATURES_H

/**
 * @file board_features.h
 * @brief Board-specific compile-time configuration for the telemetry UART module.
 */

#ifdef __cplusplus
extern "C" {
#endif

// +--------------------------------------------------------------------------+
// |                                                                          |
// |                                 Includes                                 |
// |                                                                          |
// +--------------------------------------------------------------------------+

#include <stdint.h>

// +--------------------------------------------------------------------------+
// |                                                                          |
// |                                  Public                                  |
// |                          Definitions and Macros                          |
// |                                                                          |
// +--------------------------------------------------------------------------+

/**
 * @defgroup lesson8_uart_cfg Telemetry UART Config
 * @brief Example module showing how to document compile-time variants.
 *
 * @par Variant Behavior
 * - When `BOARD_UART_HAS_RS485 == 1`, the RS-485 direction-control API is available.
 * - When `BOARD_UART_HAS_DMA == 1`, transmit behavior is documented as DMA-backed.
 * - Pin mappings, instance selection, and buffer sizes are fixed at compile time.
 * @{
 */

/** @name Board Feature Flags */
/** @{ */

#define BOARD_UART_HAS_RS485 (1U) /**< Enables RS-485 direction-control support. */
#define BOARD_UART_HAS_DMA   (0U) /**< Enables DMA-backed transmit behavior when set. */

/** @} */

/** @name Board Selection */
/** @{ */

#define BOARD_UART_INSTANCE     (2U) /**< UART peripheral instance used for telemetry output. */
#define BOARD_UART_DEFAULT_BAUD (115200UL) /**< Default UART baud rate for this board. */

/** @} */

/** @name Board Pin Mapping */
/** @{ */

#define BOARD_UART_TX_PIN (14U) /**< Physical TX pin used by the telemetry UART. */
#define BOARD_UART_RX_PIN (15U) /**< Physical RX pin used by the telemetry UART. */

#if BOARD_UART_HAS_RS485
#define BOARD_UART_RS485_DE_PIN (9U) /**< Driver-enable pin used for RS-485 direction control. */
#endif

/** @} */

/** @name Buffer Configuration */
/** @{ */

#define TELEMETRY_UART_TX_BUFFER_SIZE (64U) /**< Number of bytes available in the software TX buffer. */

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

#endif  // LESSON8_BOARD_FEATURES_H
