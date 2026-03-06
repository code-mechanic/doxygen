// +--------------------------------------------------------------------------+
// |                                                                          |
// |                                 Includes                                 |
// |                                                                          |
// +--------------------------------------------------------------------------+

#include "capture_input.h"

// +--------------------------------------------------------------------------+
// |                                                                          |
// |                                 Private                                  |
// |                          Definitions and Macros                          |
// |                                                                          |
// +--------------------------------------------------------------------------+

#define CAPTURE_INPUT_MAX_CHANNEL (3U)

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

static bool capture_input_is_valid_channel(uint8_t channel);

// +--------------------------------------------------------------------------+
// |                                                                          |
// |                                 Private                                  |
// |                          Variable Declarations                           |
// |                                                                          |
// +--------------------------------------------------------------------------+

static struct
{
  bool is_initialized;
  volatile bool irq_enabled;
  bool report_overflow;
  uint8_t channel;
  volatile uint16_t event_count;
  capture_input_callback_t callback;
  void *callback_context;
} g_capture_runtime = {false, false, false, 0U, 0U, 0, 0};

// +--------------------------------------------------------------------------+
// |                                                                          |
// |                                  Public                                  |
// |                                Functions                                 |
// |                                                                          |
// +--------------------------------------------------------------------------+

capture_input_status_t capture_input_init(const capture_input_config_t *config)
{
  if ((config == 0) || !capture_input_is_valid_channel(config->channel))
  {
    return CAPTURE_INPUT_STATUS_INVALID_ARG;
  }

  g_capture_runtime.is_initialized = true;
  g_capture_runtime.irq_enabled = false;
  g_capture_runtime.report_overflow = config->report_overflow;
  g_capture_runtime.channel = config->channel;
  g_capture_runtime.event_count = 0U;
  g_capture_runtime.callback = 0;
  g_capture_runtime.callback_context = 0;

  return CAPTURE_INPUT_STATUS_OK;
}

capture_input_status_t capture_input_register_callback(capture_input_callback_t callback,
                                                       void *context)
{
  if (!g_capture_runtime.is_initialized)
  {
    return CAPTURE_INPUT_STATUS_NOT_READY;
  }

  g_capture_runtime.callback = callback;
  g_capture_runtime.callback_context = context;
  return CAPTURE_INPUT_STATUS_OK;
}

capture_input_status_t capture_input_enable_irq(void)
{
  if (!g_capture_runtime.is_initialized)
  {
    return CAPTURE_INPUT_STATUS_NOT_READY;
  }

  g_capture_runtime.irq_enabled = true;
  return CAPTURE_INPUT_STATUS_OK;
}

capture_input_status_t capture_input_disable_irq(void)
{
  if (!g_capture_runtime.is_initialized)
  {
    return CAPTURE_INPUT_STATUS_NOT_READY;
  }

  g_capture_runtime.irq_enabled = false;
  return CAPTURE_INPUT_STATUS_OK;
}

capture_input_status_t capture_input_service_irq(uint32_t captured_ticks, bool overflowed)
{
  capture_input_event_t event;

  if (!g_capture_runtime.is_initialized || !g_capture_runtime.irq_enabled)
  {
    return CAPTURE_INPUT_STATUS_NOT_READY;
  }

  if (overflowed && !g_capture_runtime.report_overflow)
  {
    return CAPTURE_INPUT_STATUS_OK;
  }

  g_capture_runtime.event_count++;

  event.channel = g_capture_runtime.channel;
  event.timestamp_ticks = captured_ticks;
  event.overflowed = overflowed;

  if (g_capture_runtime.callback != 0)
  {
    g_capture_runtime.callback(&event, g_capture_runtime.callback_context);
  }

  return CAPTURE_INPUT_STATUS_OK;
}

capture_input_status_t capture_input_get_event_count(uint16_t *event_count)
{
  if (event_count == 0)
  {
    return CAPTURE_INPUT_STATUS_INVALID_ARG;
  }

  if (!g_capture_runtime.is_initialized)
  {
    return CAPTURE_INPUT_STATUS_NOT_READY;
  }

  *event_count = g_capture_runtime.event_count;
  return CAPTURE_INPUT_STATUS_OK;
}

// +--------------------------------------------------------------------------+
// |                                                                          |
// |                                 Private                                  |
// |                                Functions                                 |
// |                                                                          |
// +--------------------------------------------------------------------------+

static bool capture_input_is_valid_channel(uint8_t channel)
{
  return (channel <= CAPTURE_INPUT_MAX_CHANNEL);
}
