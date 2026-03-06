#ifndef LESSON14_WATCHDOG_RECOVERY_H
#define LESSON14_WATCHDOG_RECOVERY_H

/**
 * @file watchdog_recovery.h
 * @brief Public API for a simple watchdog-recovery sample.
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
 * @defgroup lesson14_watchdog Watchdog Recovery
 * @brief Example module used to inspect the upstream doxygen-awesome theme.
 * @{
 */

#define WATCHDOG_RECOVERY_MAX_RETRIES (2U) /**< Maximum allowed recovery attempts before fault lockout. */

// +--------------------------------------------------------------------------+
// |                                                                          |
// |                                  Public                                  |
// |                            Typedefs / Enums                              |
// |                                                                          |
// +--------------------------------------------------------------------------+

/**
 * @brief Status codes returned by the watchdog-recovery module.
 */
typedef enum
{
  WATCHDOG_RECOVERY_STATUS_OK = 0,      /**< Operation completed successfully. */
  WATCHDOG_RECOVERY_STATUS_INVALID_ARG, /**< One or more arguments were invalid. */
  WATCHDOG_RECOVERY_STATUS_INVALID_STATE, /**< The request is not valid in the current state. */
  WATCHDOG_RECOVERY_STATUS_NOT_READY    /**< The module has not been initialized. */
} watchdog_recovery_status_t;

/**
 * @brief Reset reasons handled by the recovery logic.
 */
typedef enum
{
  WATCHDOG_RESET_REASON_TIMEOUT = 0, /**< Watchdog timeout reset. */
  WATCHDOG_RESET_REASON_WINDOW,      /**< Watchdog window violation reset. */
  WATCHDOG_RESET_REASON_CLOCK        /**< Clock failure related reset. */
} watchdog_reset_reason_t;

/**
 * @brief Recovery states used by the sample logic.
 */
typedef enum
{
  WATCHDOG_RECOVERY_STATE_IDLE = 0, /**< No recovery action is currently armed. */
  WATCHDOG_RECOVERY_STATE_ARMED,    /**< Recovery action has been armed. */
  WATCHDOG_RECOVERY_STATE_FAULT     /**< Recovery action failed and locked out. */
} watchdog_recovery_state_t;

// +--------------------------------------------------------------------------+
// |                                                                          |
// |                                  Public                                  |
// |                            Typedefs / Structs                            |
// |                                                                          |
// +--------------------------------------------------------------------------+

/**
 * @brief Snapshot of the current recovery state.
 */
typedef struct
{
  watchdog_reset_reason_t last_reason; /**< Most recent reset reason seen by the module. */
  uint8_t retry_count;                 /**< Number of recovery attempts already used. */
  bool safe_mode_requested;            /**< Indicates whether safe mode was requested. */
} watchdog_snapshot_t;

// +--------------------------------------------------------------------------+
// |                                                                          |
// |                                  Public                                  |
// |                                Functions                                 |
// |                                                                          |
// +--------------------------------------------------------------------------+

/** @name Public Watchdog Recovery API */
/** @{ */

/**
 * @brief Initializes the watchdog-recovery module.
 * @retval WATCHDOG_RECOVERY_STATUS_OK Initialization completed successfully.
 * @post @ref WATCHDOG_RECOVERY_STATE_IDLE becomes the active state.
 */
watchdog_recovery_status_t watchdog_recovery_init(void);

/**
 * @brief Arms the watchdog-recovery logic for the next restart.
 * @retval WATCHDOG_RECOVERY_STATUS_OK Recovery logic armed successfully.
 * @retval WATCHDOG_RECOVERY_STATUS_NOT_READY The module has not been initialized.
 * @retval WATCHDOG_RECOVERY_STATUS_INVALID_STATE Recovery logic is already armed.
 * @note In a real system, this state would usually be mirrored into retention RAM.
 */
watchdog_recovery_status_t watchdog_recovery_arm(void);

/**
 * @brief Handles a reset event and updates the recovery snapshot.
 * @param[in] reason Reset reason reported by the hardware.
 * @retval WATCHDOG_RECOVERY_STATUS_OK Reset reason handled successfully.
 * @retval WATCHDOG_RECOVERY_STATUS_INVALID_ARG `reason` was invalid.
 * @retval WATCHDOG_RECOVERY_STATUS_NOT_READY The module has not been initialized.
 * @warning This sample assumes the caller has already captured the hardware reset cause
 * before other startup code clears it.
 * @todo Persist the snapshot into non-initialized RAM in a production design.
 */
watchdog_recovery_status_t watchdog_recovery_handle_reset(watchdog_reset_reason_t reason);

/**
 * @brief Returns the current recovery snapshot.
 * @param[out] snapshot Pointer that receives the snapshot.
 * @retval WATCHDOG_RECOVERY_STATUS_OK Snapshot returned successfully.
 * @retval WATCHDOG_RECOVERY_STATUS_INVALID_ARG `snapshot` was `NULL`.
 * @retval WATCHDOG_RECOVERY_STATUS_NOT_READY The module has not been initialized.
 */
watchdog_recovery_status_t watchdog_recovery_get_snapshot(watchdog_snapshot_t *snapshot);

/**
 * @brief Legacy name for arming the watchdog recovery logic.
 * @deprecated Use @ref watchdog_recovery_arm instead.
 */
watchdog_recovery_status_t watchdog_recovery_enable(void);

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

#endif  // LESSON14_WATCHDOG_RECOVERY_H
