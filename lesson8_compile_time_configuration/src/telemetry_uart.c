// +--------------------------------------------------------------------------+
// |                                                                          |
// |                                 Includes                                 |
// |                                                                          |
// +--------------------------------------------------------------------------+

#include "telemetry_uart.h"

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

static void telemetry_uart_fill_info(telemetry_uart_info_t *info);

// +--------------------------------------------------------------------------+
// |                                                                          |
// |                                 Private                                  |
// |                          Variable Declarations                           |
// |                                                                          |
// +--------------------------------------------------------------------------+

static struct
{
  bool is_initialized;
  uint16_t queued_bytes;
#if BOARD_UART_HAS_RS485
  bool rs485_tx_mode;
#endif
} g_telemetry_uart_runtime =
{
  false,
  0U,
#if BOARD_UART_HAS_RS485
  false,
#endif
};

// +--------------------------------------------------------------------------+
// |                                                                          |
// |                                  Public                                  |
// |                                Functions                                 |
// |                                                                          |
// +--------------------------------------------------------------------------+

telemetry_uart_status_t telemetry_uart_init(void)
{
  g_telemetry_uart_runtime.is_initialized = true;
  g_telemetry_uart_runtime.queued_bytes = 0U;
#if BOARD_UART_HAS_RS485
  g_telemetry_uart_runtime.rs485_tx_mode = false;
#endif
  return TELEMETRY_UART_STATUS_OK;
}

telemetry_uart_status_t telemetry_uart_send(const uint8_t *data, uint16_t length)
{
  uint16_t index = 0U;

  if ((data == 0) && (length > 0U))
  {
    return TELEMETRY_UART_STATUS_INVALID_ARG;
  }

  if (!g_telemetry_uart_runtime.is_initialized)
  {
    return TELEMETRY_UART_STATUS_NOT_READY;
  }

#if BOARD_UART_HAS_RS485
  if ((length > 0U) && !g_telemetry_uart_runtime.rs485_tx_mode)
  {
    return TELEMETRY_UART_STATUS_INVALID_STATE;
  }
#endif

  while (index < length)
  {
    if (g_telemetry_uart_runtime.queued_bytes >= TELEMETRY_UART_TX_BUFFER_SIZE)
    {
      break;
    }

    (void)data[index];
    g_telemetry_uart_runtime.queued_bytes++;
    index++;
  }

  return TELEMETRY_UART_STATUS_OK;
}

telemetry_uart_status_t telemetry_uart_get_info(telemetry_uart_info_t *info)
{
  if (info == 0)
  {
    return TELEMETRY_UART_STATUS_INVALID_ARG;
  }

  if (!g_telemetry_uart_runtime.is_initialized)
  {
    return TELEMETRY_UART_STATUS_NOT_READY;
  }

  telemetry_uart_fill_info(info);
  return TELEMETRY_UART_STATUS_OK;
}

#if BOARD_UART_HAS_RS485
telemetry_uart_status_t telemetry_uart_set_rs485_tx_mode(bool is_tx)
{
  if (!g_telemetry_uart_runtime.is_initialized)
  {
    return TELEMETRY_UART_STATUS_NOT_READY;
  }

  g_telemetry_uart_runtime.rs485_tx_mode = is_tx;
  return TELEMETRY_UART_STATUS_OK;
}
#endif

// +--------------------------------------------------------------------------+
// |                                                                          |
// |                                 Private                                  |
// |                                Functions                                 |
// |                                                                          |
// +--------------------------------------------------------------------------+

static void telemetry_uart_fill_info(telemetry_uart_info_t *info)
{
  info->instance = BOARD_UART_INSTANCE;
  info->baud_rate = BOARD_UART_DEFAULT_BAUD;
  info->tx_pin = BOARD_UART_TX_PIN;
  info->rx_pin = BOARD_UART_RX_PIN;
  info->has_rs485 = (BOARD_UART_HAS_RS485 != 0U);
  info->uses_dma = (BOARD_UART_HAS_DMA != 0U);
  info->tx_capacity = TELEMETRY_UART_TX_BUFFER_SIZE;
}
