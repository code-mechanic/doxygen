// +--------------------------------------------------------------------------+
// |                                                                          |
// |                                 Includes                                 |
// |                                                                          |
// +--------------------------------------------------------------------------+

#include "power_sequencer.h"

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

static void power_seq_enter_fault(power_seq_fault_t fault);

// +--------------------------------------------------------------------------+
// |                                                                          |
// |                                 Private                                  |
// |                          Variable Declarations                           |
// |                                                                          |
// +--------------------------------------------------------------------------+

static struct
{
  bool is_initialized;
  power_seq_state_t state;
  power_seq_fault_t fault;
  uint16_t timer_ticks;
  power_seq_config_t config;
} g_power_seq_runtime = {false, POWER_SEQ_STATE_OFF, POWER_SEQ_FAULT_NONE, 0U, {0U, 0U}};

// +--------------------------------------------------------------------------+
// |                                                                          |
// |                                  Public                                  |
// |                                Functions                                 |
// |                                                                          |
// +--------------------------------------------------------------------------+

power_seq_status_t power_seq_init(const power_seq_config_t *config)
{
  if ((config == 0) ||
      (config->startup_timeout_ticks == 0U) ||
      (config->recovery_delay_ticks == 0U))
  {
    return POWER_SEQ_STATUS_INVALID_ARG;
  }

  g_power_seq_runtime.is_initialized = true;
  g_power_seq_runtime.state = POWER_SEQ_STATE_OFF;
  g_power_seq_runtime.fault = POWER_SEQ_FAULT_NONE;
  g_power_seq_runtime.timer_ticks = 0U;
  g_power_seq_runtime.config = *config;

  return POWER_SEQ_STATUS_OK;
}

power_seq_status_t power_seq_request_enable(void)
{
  if (!g_power_seq_runtime.is_initialized)
  {
    return POWER_SEQ_STATUS_NOT_READY;
  }

  if (g_power_seq_runtime.state != POWER_SEQ_STATE_OFF)
  {
    return POWER_SEQ_STATUS_INVALID_STATE;
  }

  g_power_seq_runtime.state = POWER_SEQ_STATE_STARTING;
  g_power_seq_runtime.fault = POWER_SEQ_FAULT_NONE;
  g_power_seq_runtime.timer_ticks = 0U;
  return POWER_SEQ_STATUS_OK;
}

power_seq_status_t power_seq_request_disable(void)
{
  if (!g_power_seq_runtime.is_initialized)
  {
    return POWER_SEQ_STATUS_NOT_READY;
  }

  if (g_power_seq_runtime.state != POWER_SEQ_STATE_ON)
  {
    return POWER_SEQ_STATUS_INVALID_STATE;
  }

  g_power_seq_runtime.state = POWER_SEQ_STATE_OFF;
  g_power_seq_runtime.timer_ticks = 0U;
  return POWER_SEQ_STATUS_OK;
}

power_seq_status_t power_seq_process_tick(bool power_good, bool overcurrent)
{
  if (!g_power_seq_runtime.is_initialized)
  {
    return POWER_SEQ_STATUS_NOT_READY;
  }

  if ((g_power_seq_runtime.state == POWER_SEQ_STATE_STARTING) ||
      (g_power_seq_runtime.state == POWER_SEQ_STATE_RECOVERING))
  {
    g_power_seq_runtime.timer_ticks++;
  }

  if ((g_power_seq_runtime.state == POWER_SEQ_STATE_STARTING) && power_good)
  {
    g_power_seq_runtime.state = POWER_SEQ_STATE_ON;
    g_power_seq_runtime.timer_ticks = 0U;
    return POWER_SEQ_STATUS_OK;
  }

  if ((g_power_seq_runtime.state == POWER_SEQ_STATE_STARTING) &&
      (g_power_seq_runtime.timer_ticks >= g_power_seq_runtime.config.startup_timeout_ticks))
  {
    power_seq_enter_fault(POWER_SEQ_FAULT_STARTUP_TIMEOUT);
    return POWER_SEQ_STATUS_OK;
  }

  if ((g_power_seq_runtime.state == POWER_SEQ_STATE_ON) && overcurrent)
  {
    power_seq_enter_fault(POWER_SEQ_FAULT_OVERCURRENT);
    return POWER_SEQ_STATUS_OK;
  }

  if ((g_power_seq_runtime.state == POWER_SEQ_STATE_RECOVERING) &&
      (g_power_seq_runtime.timer_ticks >= g_power_seq_runtime.config.recovery_delay_ticks))
  {
    g_power_seq_runtime.state = POWER_SEQ_STATE_OFF;
    g_power_seq_runtime.timer_ticks = 0U;
  }

  return POWER_SEQ_STATUS_OK;
}

power_seq_status_t power_seq_request_recover(void)
{
  if (!g_power_seq_runtime.is_initialized)
  {
    return POWER_SEQ_STATUS_NOT_READY;
  }

  if (g_power_seq_runtime.state != POWER_SEQ_STATE_FAULT)
  {
    return POWER_SEQ_STATUS_INVALID_STATE;
  }

  g_power_seq_runtime.state = POWER_SEQ_STATE_RECOVERING;
  g_power_seq_runtime.timer_ticks = 0U;
  return POWER_SEQ_STATUS_OK;
}

power_seq_status_t power_seq_get_snapshot(power_seq_snapshot_t *snapshot)
{
  if (snapshot == 0)
  {
    return POWER_SEQ_STATUS_INVALID_ARG;
  }

  if (!g_power_seq_runtime.is_initialized)
  {
    return POWER_SEQ_STATUS_NOT_READY;
  }

  snapshot->state = g_power_seq_runtime.state;
  snapshot->fault = g_power_seq_runtime.fault;
  snapshot->timer_ticks = g_power_seq_runtime.timer_ticks;
  return POWER_SEQ_STATUS_OK;
}

// +--------------------------------------------------------------------------+
// |                                                                          |
// |                                 Private                                  |
// |                                Functions                                 |
// |                                                                          |
// +--------------------------------------------------------------------------+

static void power_seq_enter_fault(power_seq_fault_t fault)
{
  g_power_seq_runtime.state = POWER_SEQ_STATE_FAULT;
  g_power_seq_runtime.fault = fault;
  g_power_seq_runtime.timer_ticks = 0U;
}
