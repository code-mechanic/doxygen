// +--------------------------------------------------------------------------+
// |                                                                          |
// |                                 Includes                                 |
// |                                                                          |
// +--------------------------------------------------------------------------+

#include "uart_console.h"
#include "uart_console_internal.h"

// +--------------------------------------------------------------------------+
// |                                                                          |
// |                                 Private                                  |
// |                          Definitions and Macros                          |
// |                                                                          |
// +--------------------------------------------------------------------------+

#define UART_CONSOLE_HEX_DIGIT_COUNT (4U)

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

static uart_console_status_t uart_console_push_byte(uint8_t byte);
static uint8_t uart_console_nibble_to_hex(uint8_t nibble);

// +--------------------------------------------------------------------------+
// |                                                                          |
// |                                 Private                                  |
// |                          Variable Declarations                           |
// |                                                                          |
// +--------------------------------------------------------------------------+

static uart_console_state_t g_uart_console_state;

// +--------------------------------------------------------------------------+
// |                                                                          |
// |                                  Public                                  |
// |                                Functions                                 |
// |                                                                          |
// +--------------------------------------------------------------------------+

uart_console_status_t uart_console_init(const uart_console_config_t *config)
{
  if ((config == 0) ||
      (config->baud_rate == 0UL) ||
      (config->tx_buffer_size == 0U) ||
      (config->tx_buffer_size > UART_CONSOLE_MAX_BUFFER_SIZE))
  {
    return UART_CONSOLE_STATUS_INVALID_ARG;
  }

  uart_console_state_reset(&g_uart_console_state);
  g_uart_console_state.capacity = config->tx_buffer_size;
  g_uart_console_state.baud_rate = config->baud_rate;
  g_uart_console_state.is_initialized = true;

  return UART_CONSOLE_STATUS_OK;
}

uart_console_status_t uart_console_write(const uint8_t *data, uint16_t length)
{
  uint16_t index = 0U;

  if ((data == 0) && (length > 0U))
  {
    return UART_CONSOLE_STATUS_INVALID_ARG;
  }

  if (!g_uart_console_state.is_initialized)
  {
    return UART_CONSOLE_STATUS_NOT_READY;
  }

  while (index < length)
  {
    uart_console_status_t status = uart_console_push_byte(data[index]);
    if (status != UART_CONSOLE_STATUS_OK)
    {
      return status;
    }

    index++;
  }

  return UART_CONSOLE_STATUS_OK;
}

uart_console_status_t uart_console_write_hex_u16(uint16_t value)
{
  uint8_t shift = 0U;

  if (!g_uart_console_state.is_initialized)
  {
    return UART_CONSOLE_STATUS_NOT_READY;
  }

  shift = (uint8_t)((UART_CONSOLE_HEX_DIGIT_COUNT - 1U) * 4U);
  while (true)
  {
    uart_console_status_t status =
      uart_console_push_byte(uart_console_nibble_to_hex((uint8_t)((value >> shift) & 0x0FU)));

    if (status != UART_CONSOLE_STATUS_OK)
    {
      return status;
    }

    if (shift == 0U)
    {
      break;
    }

    shift = (uint8_t)(shift - 4U);
  }

  return UART_CONSOLE_STATUS_OK;
}

uart_console_status_t uart_console_flush(void)
{
  if (!g_uart_console_state.is_initialized)
  {
    return UART_CONSOLE_STATUS_NOT_READY;
  }

  g_uart_console_state.head = 0U;
  g_uart_console_state.tail = 0U;
  g_uart_console_state.count = 0U;
  return UART_CONSOLE_STATUS_OK;
}

// +--------------------------------------------------------------------------+
// |                                                                          |
// |                                 Private                                  |
// |                                Functions                                 |
// |                                                                          |
// +--------------------------------------------------------------------------+

void uart_console_state_reset(uart_console_state_t *state)
{
  uint16_t index = 0U;

  while (index < UART_CONSOLE_MAX_BUFFER_SIZE)
  {
    state->tx_buffer[index] = 0U;
    index++;
  }

  state->head = 0U;
  state->tail = 0U;
  state->count = 0U;
  state->capacity = UART_CONSOLE_MAX_BUFFER_SIZE;
  state->is_initialized = false;
  state->baud_rate = 0UL;
}

/**
 * @brief Enqueues one byte into the private transmit buffer.
 * @param[in] byte Byte to enqueue.
 * @retval UART_CONSOLE_STATUS_OK Byte queued successfully.
 * @retval UART_CONSOLE_STATUS_BUFFER_FULL No more space remained in the buffer.
 * @pre `g_uart_console_state.is_initialized` must be `true`.
 */
static uart_console_status_t uart_console_push_byte(uint8_t byte)
{
  if (g_uart_console_state.count >= g_uart_console_state.capacity)
  {
    return UART_CONSOLE_STATUS_BUFFER_FULL;
  }

  g_uart_console_state.tx_buffer[g_uart_console_state.head] = byte;
  g_uart_console_state.head = (uint16_t)((g_uart_console_state.head + 1U) %
                                         g_uart_console_state.capacity);
  g_uart_console_state.count++;
  return UART_CONSOLE_STATUS_OK;
}

/**
 * @brief Converts a 4-bit value into an uppercase hexadecimal ASCII byte.
 * @param[in] nibble Value in the range `0` to `15`.
 * @return ASCII representation of the nibble.
 */
static uint8_t uart_console_nibble_to_hex(uint8_t nibble)
{
  if (nibble < 10U)
  {
    return (uint8_t)('0' + nibble);
  }

  return (uint8_t)('A' + (nibble - 10U));
}
