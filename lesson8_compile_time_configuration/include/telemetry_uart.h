#ifndef LESSON8_TELEMETRY_UART_H
#define LESSON8_TELEMETRY_UART_H

/**
 * @file telemetry_uart.h
 * @brief Public API for a board-configured telemetry UART driver.
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

#include "board_features.h"

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
 * @addtogroup lesson8_uart_cfg
 * @{
 */

/**
 * @brief Status codes returned by the telemetry UART driver.
 */
typedef enum
{
  TELEMETRY_UART_STATUS_OK = 0,      /**< Operation completed successfully. */
  TELEMETRY_UART_STATUS_INVALID_ARG, /**< One or more arguments were invalid. */
  TELEMETRY_UART_STATUS_INVALID_STATE, /**< The requested operation is not valid in the current variant or state. */
  TELEMETRY_UART_STATUS_NOT_READY    /**< Driver was used before initialization. */
} telemetry_uart_status_t;

// +--------------------------------------------------------------------------+
// |                                                                          |
// |                                  Public                                  |
// |                            Typedefs / Structs                            |
// |                                                                          |
// +--------------------------------------------------------------------------+

/**
 * @brief Compile-time information exposed by the telemetry UART driver.
 */
typedef struct
{
  uint8_t instance;      /**< UART instance selected by the board configuration. */
  uint32_t baud_rate;    /**< Default baud rate selected by the board configuration. */
  uint8_t tx_pin;        /**< TX pin selected by the board configuration. */
  uint8_t rx_pin;        /**< RX pin selected by the board configuration. */
  bool has_rs485;        /**< Indicates whether the RS-485 variant is compiled in. */
  bool uses_dma;         /**< Indicates whether the DMA-backed variant is compiled in. */
  uint16_t tx_capacity;  /**< Compile-time TX buffer size. */
} telemetry_uart_info_t;

// +--------------------------------------------------------------------------+
// |                                                                          |
// |                                  Public                                  |
// |                                Functions                                 |
// |                                                                          |
// +--------------------------------------------------------------------------+

/** @name Public Driver API */
/** @{ */

/**
 * @brief Initializes the telemetry UART driver using board-selected defaults.
 * @retval TELEMETRY_UART_STATUS_OK Driver initialized successfully.
 * @post The module is ready for transmit requests.
 * @note No runtime configuration is accepted because instance, pins, and baud rate
 * are fixed by [board_features.h](/home/mohan/Experiments/doxygen/lesson8_compile_time_configuration/include/board_features.h).
 */
telemetry_uart_status_t telemetry_uart_init(void);

/**
 * @brief Queues telemetry bytes for transmission.
 * @param[in] data Pointer to the data to send.
 * @param[in] length Number of bytes to send.
 * @retval TELEMETRY_UART_STATUS_OK Data accepted for transmission.
 * @retval TELEMETRY_UART_STATUS_INVALID_ARG `data` was `NULL` while `length` was non-zero.
 * @retval TELEMETRY_UART_STATUS_INVALID_STATE RS-485 support was enabled but TX mode was not selected.
 * @retval TELEMETRY_UART_STATUS_NOT_READY The driver has not been initialized.
 * @pre `telemetry_uart_init()` must have completed successfully.
 * @par Variant Behavior
 * - When `BOARD_UART_HAS_DMA == 1`, the transmit path is expected to hand off to DMA.
 * - When `BOARD_UART_HAS_DMA == 0`, the transmit path is expected to use a software copy loop.
 */
telemetry_uart_status_t telemetry_uart_send(const uint8_t *data, uint16_t length);

/**
 * @brief Returns compile-time board and variant information.
 * @param[out] info Pointer that receives the current build-time configuration.
 * @retval TELEMETRY_UART_STATUS_OK Information returned successfully.
 * @retval TELEMETRY_UART_STATUS_INVALID_ARG `info` was `NULL`.
 * @retval TELEMETRY_UART_STATUS_NOT_READY The driver has not been initialized.
 */
telemetry_uart_status_t telemetry_uart_get_info(telemetry_uart_info_t *info);

#if BOARD_UART_HAS_RS485
/**
 * @brief Selects RS-485 transmit or receive direction.
 * @param[in] is_tx `true` to enable transmit direction, `false` to enable receive direction.
 * @retval TELEMETRY_UART_STATUS_OK Direction updated successfully.
 * @retval TELEMETRY_UART_STATUS_NOT_READY The driver has not been initialized.
 * @note This API exists only when `BOARD_UART_HAS_RS485 == 1`.
 * @par Variant
 * This function is compiled in because [board_features.h](/home/mohan/Experiments/doxygen/lesson8_compile_time_configuration/include/board_features.h#L35)
 * enables the RS-485 board feature.
 */
telemetry_uart_status_t telemetry_uart_set_rs485_tx_mode(bool is_tx);
#endif

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

#endif  // LESSON8_TELEMETRY_UART_H
