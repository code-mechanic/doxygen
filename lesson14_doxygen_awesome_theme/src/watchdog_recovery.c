/**
 * @file watchdog_recovery.c
 * @brief Implementation of the watchdog-recovery sample.
 */

// +--------------------------------------------------------------------------+
// |                                                                          |
// |                                 Includes                                 |
// |                                                                          |
// +--------------------------------------------------------------------------+

#include "watchdog_recovery.h"

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

/**
 * @brief Internal runtime state for the watchdog-recovery sample.
 */
typedef struct
{
  bool is_initialized;                 /**< Indicates whether the module has been initialized. */
  watchdog_recovery_state_t state;     /**< Current recovery state. */
  watchdog_snapshot_t snapshot;        /**< Stored runtime snapshot. */
} watchdog_recovery_runtime_t;

// +--------------------------------------------------------------------------+
// |                                                                          |
// |                                 Private                                  |
// |                           Function Prototypes                            |
// |                                                                          |
// +--------------------------------------------------------------------------+

/**
 * @brief Returns whether the supplied reset reason is valid.
 * @param[in] reason Reset reason to validate.
 * @retval true Reason is valid.
 * @retval false Reason is invalid.
 */
static bool watchdog_recovery_reason_is_valid(watchdog_reset_reason_t reason);

/**
 * @brief Requests safe mode after repeated recovery failures.
 */
static void watchdog_recovery_request_safe_mode(void);

// +--------------------------------------------------------------------------+
// |                                                                          |
// |                                 Private                                  |
// |                          Variable Declarations                           |
// |                                                                          |
// +--------------------------------------------------------------------------+

/** Global runtime instance used by the watchdog-recovery sample. */
static watchdog_recovery_runtime_t g_watchdog_recovery_runtime =
{
  false,
  WATCHDOG_RECOVERY_STATE_IDLE,
  {
    WATCHDOG_RESET_REASON_TIMEOUT,
    0U,
    false
  }
};

// +--------------------------------------------------------------------------+
// |                                                                          |
// |                                  Public                                  |
// |                                Functions                                 |
// |                                                                          |
// +--------------------------------------------------------------------------+

watchdog_recovery_status_t watchdog_recovery_init(void)
{
  g_watchdog_recovery_runtime.is_initialized = true;
  g_watchdog_recovery_runtime.state = WATCHDOG_RECOVERY_STATE_IDLE;
  g_watchdog_recovery_runtime.snapshot.last_reason = WATCHDOG_RESET_REASON_TIMEOUT;
  g_watchdog_recovery_runtime.snapshot.retry_count = 0U;
  g_watchdog_recovery_runtime.snapshot.safe_mode_requested = false;

  return WATCHDOG_RECOVERY_STATUS_OK;
}

watchdog_recovery_status_t watchdog_recovery_arm(void)
{
  if (!g_watchdog_recovery_runtime.is_initialized)
  {
    return WATCHDOG_RECOVERY_STATUS_NOT_READY;
  }

  if (g_watchdog_recovery_runtime.state == WATCHDOG_RECOVERY_STATE_ARMED)
  {
    return WATCHDOG_RECOVERY_STATUS_INVALID_STATE;
  }

  g_watchdog_recovery_runtime.state = WATCHDOG_RECOVERY_STATE_ARMED;
  return WATCHDOG_RECOVERY_STATUS_OK;
}

watchdog_recovery_status_t watchdog_recovery_handle_reset(watchdog_reset_reason_t reason)
{
  if (!watchdog_recovery_reason_is_valid(reason))
  {
    return WATCHDOG_RECOVERY_STATUS_INVALID_ARG;
  }

  if (!g_watchdog_recovery_runtime.is_initialized)
  {
    return WATCHDOG_RECOVERY_STATUS_NOT_READY;
  }

  g_watchdog_recovery_runtime.snapshot.last_reason = reason;

  if (g_watchdog_recovery_runtime.snapshot.retry_count >= WATCHDOG_RECOVERY_MAX_RETRIES)
  {
    g_watchdog_recovery_runtime.state = WATCHDOG_RECOVERY_STATE_FAULT;
    watchdog_recovery_request_safe_mode();
    return WATCHDOG_RECOVERY_STATUS_OK;
  }

  g_watchdog_recovery_runtime.snapshot.retry_count++;
  g_watchdog_recovery_runtime.state = WATCHDOG_RECOVERY_STATE_ARMED;
  return WATCHDOG_RECOVERY_STATUS_OK;
}

watchdog_recovery_status_t watchdog_recovery_get_snapshot(watchdog_snapshot_t *snapshot)
{
  if (snapshot == 0)
  {
    return WATCHDOG_RECOVERY_STATUS_INVALID_ARG;
  }

  if (!g_watchdog_recovery_runtime.is_initialized)
  {
    return WATCHDOG_RECOVERY_STATUS_NOT_READY;
  }

  *snapshot = g_watchdog_recovery_runtime.snapshot;
  return WATCHDOG_RECOVERY_STATUS_OK;
}

watchdog_recovery_status_t watchdog_recovery_enable(void)
{
  return watchdog_recovery_arm();
}

// +--------------------------------------------------------------------------+
// |                                                                          |
// |                                 Private                                  |
// |                                Functions                                 |
// |                                                                          |
// +--------------------------------------------------------------------------+

static bool watchdog_recovery_reason_is_valid(watchdog_reset_reason_t reason)
{
  return (reason >= WATCHDOG_RESET_REASON_TIMEOUT) && (reason <= WATCHDOG_RESET_REASON_CLOCK);
}

static void watchdog_recovery_request_safe_mode(void)
{
  g_watchdog_recovery_runtime.snapshot.safe_mode_requested = true;
}
