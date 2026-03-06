#ifndef LESSON4_UART_CONSOLE_H
#define LESSON4_UART_CONSOLE_H

/**
 * @file uart_console.h
 * @brief Public API for a buffered UART console module.
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

// +--------------------------------------------------------------------------+
// |                                                                          |
// |                                  Public                                  |
// |                            Typedefs / Enums                              |
// |                                                                          |
// +--------------------------------------------------------------------------+

/**
 * @defgroup lesson4_console UART Console
 * @brief Buffered UART console service for embedded firmware.
 * @{
 */

/**
 * @brief Status codes returned by the UART console module.
 */
typedef enum
{
  UART_CONSOLE_STATUS_OK = 0,      /**< Operation completed successfully. */
  UART_CONSOLE_STATUS_INVALID_ARG, /**< One or more arguments were invalid. */
  UART_CONSOLE_STATUS_NOT_READY,   /**< Module was used before initialization. */
  UART_CONSOLE_STATUS_BUFFER_FULL  /**< The transmit buffer could not accept more data. */
} uart_console_status_t;

// +--------------------------------------------------------------------------+
// |                                                                          |
// |                                  Public                                  |
// |                            Typedefs / Structs                            |
// |                                                                          |
// +--------------------------------------------------------------------------+

/**
 * @brief Configuration applied during UART console initialization.
 */
typedef struct
{
  uint32_t baud_rate;      /**< UART baud rate used for transmissions. */
  uint16_t tx_buffer_size; /**< Number of bytes available in the software TX buffer. */
} uart_console_config_t;

// +--------------------------------------------------------------------------+
// |                                                                          |
// |                                  Public                                  |
// |                                Functions                                 |
// |                                                                          |
// +--------------------------------------------------------------------------+

/** @name Public Functions */
/** @{ */

/**
 * @brief Initializes the UART console module.
 * @param[in] config Pointer to the configuration to apply.
 * @retval UART_CONSOLE_STATUS_OK Initialization completed successfully.
 * @retval UART_CONSOLE_STATUS_INVALID_ARG `config` was `NULL` or invalid.
 * @post The module is ready to queue bytes for transmission.
 * @internal
 * The lesson implementation uses a software ring buffer only. Real firmware
 * would normally connect this API to a UART interrupt or DMA transfer path.
 * @endinternal
 */
uart_console_status_t uart_console_init(const uart_console_config_t *config);

/**
 * @brief Queues raw bytes for transmission.
 * @param[in] data Pointer to the bytes to enqueue.
 * @param[in] length Number of bytes to enqueue.
 * @retval UART_CONSOLE_STATUS_OK Data queued successfully.
 * @retval UART_CONSOLE_STATUS_INVALID_ARG `data` was `NULL` while `length` was non-zero.
 * @retval UART_CONSOLE_STATUS_NOT_READY The module has not been initialized.
 * @retval UART_CONSOLE_STATUS_BUFFER_FULL The transmit buffer ran out of space.
 * @pre `uart_console_init()` must have completed successfully.
 */
uart_console_status_t uart_console_write(const uint8_t *data, uint16_t length);

/**
 * @brief Queues a 16-bit value as four hexadecimal ASCII characters.
 * @param[in] value Value to convert and queue.
 * @retval UART_CONSOLE_STATUS_OK Value queued successfully.
 * @retval UART_CONSOLE_STATUS_NOT_READY The module has not been initialized.
 * @retval UART_CONSOLE_STATUS_BUFFER_FULL The transmit buffer ran out of space.
 * @pre `uart_console_init()` must have completed successfully.
 */
uart_console_status_t uart_console_write_hex_u16(uint16_t value);

/**
 * @brief Empties the software transmit buffer.
 * @retval UART_CONSOLE_STATUS_OK Buffer flushed successfully.
 * @retval UART_CONSOLE_STATUS_NOT_READY The module has not been initialized.
 * @post No queued bytes remain in the transmit buffer.
 */
uart_console_status_t uart_console_flush(void);

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

#endif  // LESSON4_UART_CONSOLE_H
