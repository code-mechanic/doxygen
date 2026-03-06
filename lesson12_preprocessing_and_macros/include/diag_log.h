#ifndef LESSON12_DIAG_LOG_H
#define LESSON12_DIAG_LOG_H

/**
 * @file diag_log.h
 * @brief Public API for a macro-aware diagnostic logging sample.
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

// +--------------------------------------------------------------------------+
// |                                                                          |
// |                                  Public                                  |
// |                          Definitions and Macros                          |
// |                                                                          |
// +--------------------------------------------------------------------------+

/**
 * @defgroup lesson12_diag_log Diagnostic Log
 * @brief Example module used to explore Doxygen preprocessing behavior.
 * @{
 */

#define DIAG_LOG_MAX_ENTRIES (16U) /**< Maximum number of entries kept in the in-memory log. */

/**
 * @brief Macro list used to build the diagnostic channel enum.
 * @note Lesson 12 expands this macro in Doxygen with `EXPAND_AS_DEFINED`.
 */
#define DIAG_CHANNEL_ENUM_ITEMS \
  DIAG_CHANNEL_POWER,   \
  DIAG_CHANNEL_THERMAL, \
  DIAG_CHANNEL_COMM

/**
 * @brief Macro list used to build the diagnostic channel label table.
 * @note Lesson 12 expands this macro in Doxygen with `EXPAND_AS_DEFINED`.
 */
#define DIAG_CHANNEL_LABEL_ITEMS \
  "power",   \
  "thermal", \
  "comm"

// +--------------------------------------------------------------------------+
// |                                                                          |
// |                                  Public                                  |
// |                            Typedefs / Enums                              |
// |                                                                          |
// +--------------------------------------------------------------------------+

/**
 * @brief Status codes returned by the diagnostic-log module.
 */
typedef enum
{
  DIAG_LOG_STATUS_OK = 0,      /**< Operation completed successfully. */
  DIAG_LOG_STATUS_INVALID_ARG, /**< A pointer or enum argument was invalid. */
  DIAG_LOG_STATUS_NOT_READY,   /**< The log module has not been initialized. */
  DIAG_LOG_STATUS_STORAGE_FULL /**< The in-memory log is already full. */
} diag_log_status_t;

/**
 * @brief Channels that can own a diagnostic log entry.
 */
typedef enum
{
  DIAG_CHANNEL_ENUM_ITEMS,
  DIAG_CHANNEL_COUNT
} diag_channel_t;

// +--------------------------------------------------------------------------+
// |                                                                          |
// |                                  Public                                  |
// |                            Typedefs / Structs                            |
// |                                                                          |
// +--------------------------------------------------------------------------+

/**
 * @brief One diagnostic log entry.
 */
typedef struct
{
  diag_channel_t channel; /**< Channel that produced the log entry. */
  uint16_t code;          /**< Event or fault code. */
  uint32_t tick;          /**< Tick count associated with the event. */
} diag_log_entry_t;

// +--------------------------------------------------------------------------+
// |                                                                          |
// |                                  Public                                  |
// |                                Functions                                 |
// |                                                                          |
// +--------------------------------------------------------------------------+

/** @name Public Diagnostic Log API */
/** @{ */

/**
 * @brief Initializes the diagnostic-log module.
 * @retval DIAG_LOG_STATUS_OK Initialization completed successfully.
 */
diag_log_status_t diag_log_init(void);

/**
 * @brief Pushes a new event into the in-memory log.
 * @param[in] channel Channel that owns the event.
 * @param[in] code Event or fault code to store.
 * @retval DIAG_LOG_STATUS_OK Entry stored successfully.
 * @retval DIAG_LOG_STATUS_INVALID_ARG `channel` was invalid.
 * @retval DIAG_LOG_STATUS_NOT_READY The log module has not been initialized.
 * @retval DIAG_LOG_STATUS_STORAGE_FULL The log already contains the maximum number of entries.
 * @see @ref lesson12_preprocessing "Preprocessing and Macro Expansion"
 */
diag_log_status_t diag_log_push(diag_channel_t channel, uint16_t code);

/**
 * @brief Returns the total number of stored entries.
 * @return Number of entries currently stored in memory.
 */
uint16_t diag_log_get_count(void);

/**
 * @brief Returns whether a channel currently has at least one stored entry.
 * @param[in] channel Channel to query.
 * @retval true The channel has one or more stored entries.
 * @retval false The channel is empty or invalid.
 */
bool diag_log_has_channel(diag_channel_t channel);

/**
 * @brief Returns the human-readable label for a channel.
 * @param[in] channel Channel to query.
 * @return Pointer to a constant label string.
 */
const char *diag_log_get_channel_label(diag_channel_t channel);

#if DIAG_LOG_TARGET_EXT_FLASH
/**
 * @brief Flushes the in-memory log to an external flash target.
 * @retval DIAG_LOG_STATUS_OK Flush completed successfully.
 * @retval DIAG_LOG_STATUS_NOT_READY The log module has not been initialized.
 * @note This API is present only because `PREDEFINED` enables
 * `DIAG_LOG_TARGET_EXT_FLASH=1` for the documentation build.
 * @see @ref lesson12_build_flags "Build Flags and PREDEFINED"
 */
diag_log_status_t diag_log_flush_to_ext_flash(void);
#endif

#if DIAG_ENABLE_ISR_STAMP
/**
 * @brief Updates the tick source from interrupt context.
 * @param[in] tick Current system tick count.
 * @note This API is present only because `PREDEFINED` enables
 * `DIAG_ENABLE_ISR_STAMP=1` for the documentation build.
 * @warning Intended for ISR context only.
 */
void diag_log_stamp_from_isr(uint32_t tick);
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

#endif  // LESSON12_DIAG_LOG_H
