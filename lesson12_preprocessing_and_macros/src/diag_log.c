/**
 * @file diag_log.c
 * @brief Implementation of the diagnostic-log sample.
 */

// +--------------------------------------------------------------------------+
// |                                                                          |
// |                                 Includes                                 |
// |                                                                          |
// +--------------------------------------------------------------------------+

#include "diag_log.h"

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
 * @brief Internal runtime state for the diagnostic-log sample.
 */
typedef struct
{
  bool is_initialized;   /**< Indicates whether the module has been initialized. */
  uint16_t total_entries; /**< Total number of stored entries. */
  uint32_t current_tick; /**< Tick value associated with newly logged events. */
} diag_log_runtime_t;

// +--------------------------------------------------------------------------+
// |                                                                          |
// |                                 Private                                  |
// |                           Function Prototypes                            |
// |                                                                          |
// +--------------------------------------------------------------------------+

/**
 * @brief Returns whether the supplied channel is valid.
 * @param[in] channel Channel to validate.
 * @retval true Channel is valid.
 * @retval false Channel is invalid.
 */
static bool diag_log_channel_is_valid(diag_channel_t channel);

/**
 * @brief Increments the stored count for a channel.
 * @param[in] channel Channel whose count should be incremented.
 */
static void diag_log_increment_channel_count(diag_channel_t channel);

// +--------------------------------------------------------------------------+
// |                                                                          |
// |                                 Private                                  |
// |                          Variable Declarations                           |
// |                                                                          |
// +--------------------------------------------------------------------------+

/** Global runtime instance used by the diagnostic-log sample. */
static diag_log_runtime_t g_diag_log_runtime =
{
  false,
  0U,
  0UL
};

/** Per-channel entry counters indexed by `diag_channel_t`. */
static uint16_t g_diag_channel_counts[DIAG_CHANNEL_COUNT] =
{
  0U,
  0U,
  0U
};

/** Human-readable labels indexed by `diag_channel_t`. */
static const char *const g_diag_channel_labels[DIAG_CHANNEL_COUNT] =
{
  DIAG_CHANNEL_LABEL_ITEMS
};

// +--------------------------------------------------------------------------+
// |                                                                          |
// |                                  Public                                  |
// |                                Functions                                 |
// |                                                                          |
// +--------------------------------------------------------------------------+

diag_log_status_t diag_log_init(void)
{
  uint16_t index = 0U;

  g_diag_log_runtime.is_initialized = true;
  g_diag_log_runtime.total_entries = 0U;
  g_diag_log_runtime.current_tick = 0UL;

  while (index < DIAG_CHANNEL_COUNT)
  {
    g_diag_channel_counts[index] = 0U;
    index++;
  }

  return DIAG_LOG_STATUS_OK;
}

diag_log_status_t diag_log_push(diag_channel_t channel, uint16_t code)
{
  (void)code;

  if (!diag_log_channel_is_valid(channel))
  {
    return DIAG_LOG_STATUS_INVALID_ARG;
  }

  if (!g_diag_log_runtime.is_initialized)
  {
    return DIAG_LOG_STATUS_NOT_READY;
  }

  if (g_diag_log_runtime.total_entries >= DIAG_LOG_MAX_ENTRIES)
  {
    return DIAG_LOG_STATUS_STORAGE_FULL;
  }

  diag_log_increment_channel_count(channel);
  g_diag_log_runtime.total_entries++;

  return DIAG_LOG_STATUS_OK;
}

uint16_t diag_log_get_count(void)
{
  return g_diag_log_runtime.total_entries;
}

bool diag_log_has_channel(diag_channel_t channel)
{
  if (!diag_log_channel_is_valid(channel))
  {
    return false;
  }

  return (g_diag_channel_counts[channel] > 0U);
}

const char *diag_log_get_channel_label(diag_channel_t channel)
{
  if (!diag_log_channel_is_valid(channel))
  {
    return "invalid";
  }

  return g_diag_channel_labels[channel];
}

#if DIAG_LOG_TARGET_EXT_FLASH
diag_log_status_t diag_log_flush_to_ext_flash(void)
{
  if (!g_diag_log_runtime.is_initialized)
  {
    return DIAG_LOG_STATUS_NOT_READY;
  }

  g_diag_log_runtime.total_entries = 0U;
  return DIAG_LOG_STATUS_OK;
}
#endif

#if DIAG_ENABLE_ISR_STAMP
void diag_log_stamp_from_isr(uint32_t tick)
{
  g_diag_log_runtime.current_tick = tick;
}
#endif

// +--------------------------------------------------------------------------+
// |                                                                          |
// |                                 Private                                  |
// |                                Functions                                 |
// |                                                                          |
// +--------------------------------------------------------------------------+

static bool diag_log_channel_is_valid(diag_channel_t channel)
{
  return (channel >= DIAG_CHANNEL_POWER) && (channel < DIAG_CHANNEL_COUNT);
}

static void diag_log_increment_channel_count(diag_channel_t channel)
{
  g_diag_channel_counts[channel]++;
}
