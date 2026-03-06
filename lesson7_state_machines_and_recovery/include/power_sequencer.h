#ifndef LESSON7_POWER_SEQUENCER_H
#define LESSON7_POWER_SEQUENCER_H

/**
 * @file power_sequencer.h
 * @brief Public API for a simple power-sequencer state machine.
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
 * @defgroup lesson7_power_seq Power Sequencer
 * @brief Example module showing how to document states, transitions, and recovery.
 *
 * @par States
 * - `POWER_SEQ_STATE_OFF`: Output rail is disabled.
 * - `POWER_SEQ_STATE_STARTING`: Enable request was accepted and the driver is
 *   waiting for the power-good signal.
 * - `POWER_SEQ_STATE_ON`: Output rail is enabled and stable.
 * - `POWER_SEQ_STATE_FAULT`: A fault was detected and output power is disabled.
 * - `POWER_SEQ_STATE_RECOVERING`: Recovery delay is active before the next
 *   enable attempt is allowed.
 *
 * @par Recovery Flow
 * A startup timeout or overcurrent event moves the module to
 * `POWER_SEQ_STATE_FAULT`. The application must call
 * `power_seq_request_recover()` to begin the timed recovery phase. After the
 * configured delay expires, the module returns to `POWER_SEQ_STATE_OFF`.
 * @{
 */

// +--------------------------------------------------------------------------+
// |                                                                          |
// |                                  Public                                  |
// |                            Typedefs / Enums                              |
// |                                                                          |
// +--------------------------------------------------------------------------+

/**
 * @brief Status codes returned by the power-sequencer driver.
 */
typedef enum
{
  POWER_SEQ_STATUS_OK = 0,      /**< Operation completed successfully. */
  POWER_SEQ_STATUS_INVALID_ARG, /**< One or more arguments were invalid. */
  POWER_SEQ_STATUS_INVALID_STATE, /**< The requested operation is not allowed in the current state. */
  POWER_SEQ_STATUS_NOT_READY    /**< Driver was used before initialization. */
} power_seq_status_t;

/**
 * @brief States used by the power-sequencer state machine.
 */
typedef enum
{
  POWER_SEQ_STATE_OFF = 0, /**< Output rail is disabled. */
  POWER_SEQ_STATE_STARTING, /**< Driver is waiting for power-good after enable. */
  POWER_SEQ_STATE_ON, /**< Output rail is enabled and stable. */
  POWER_SEQ_STATE_FAULT, /**< Output rail is disabled because a fault occurred. */
  POWER_SEQ_STATE_RECOVERING /**< Recovery timer is active after a fault. */
} power_seq_state_t;

/**
 * @brief Fault reasons recorded by the power-sequencer driver.
 */
typedef enum
{
  POWER_SEQ_FAULT_NONE = 0, /**< No fault is currently latched. */
  POWER_SEQ_FAULT_STARTUP_TIMEOUT, /**< Power-good did not arrive before the startup timeout. */
  POWER_SEQ_FAULT_OVERCURRENT /**< Output current exceeded the allowed limit. */
} power_seq_fault_t;

// +--------------------------------------------------------------------------+
// |                                                                          |
// |                                  Public                                  |
// |                            Typedefs / Structs                            |
// |                                                                          |
// +--------------------------------------------------------------------------+

/**
 * @brief Configuration used to initialize the power-sequencer driver.
 */
typedef struct
{
  uint16_t startup_timeout_ticks; /**< Maximum ticks to wait for power-good after enable. */
  uint16_t recovery_delay_ticks;  /**< Minimum ticks to remain in recovery before returning to OFF. */
} power_seq_config_t;

/**
 * @brief Snapshot of the state machine for diagnostics or logging.
 */
typedef struct
{
  power_seq_state_t state; /**< Current state of the sequencer. */
  power_seq_fault_t fault; /**< Currently latched fault reason. */
  uint16_t timer_ticks;    /**< Ticks elapsed in the current timed state. */
} power_seq_snapshot_t;

// +--------------------------------------------------------------------------+
// |                                                                          |
// |                                  Public                                  |
// |                                Functions                                 |
// |                                                                          |
// +--------------------------------------------------------------------------+

/** @name Public State Machine API */
/** @{ */

/**
 * @brief Initializes the power-sequencer driver.
 * @param[in] config Pointer to the configuration to apply.
 * @retval POWER_SEQ_STATUS_OK Driver initialized successfully.
 * @retval POWER_SEQ_STATUS_INVALID_ARG `config` was `NULL` or one of the delays was zero.
 * @post The driver enters `POWER_SEQ_STATE_OFF`.
 */
power_seq_status_t power_seq_init(const power_seq_config_t *config);

/**
 * @brief Requests that the output rail be enabled.
 * @retval POWER_SEQ_STATUS_OK Enable request accepted.
 * @retval POWER_SEQ_STATUS_INVALID_STATE The current state was not `POWER_SEQ_STATE_OFF`.
 * @retval POWER_SEQ_STATUS_NOT_READY The driver has not been initialized.
 * @post The driver enters `POWER_SEQ_STATE_STARTING`.
 * @par Transition
 * `POWER_SEQ_STATE_OFF` -> `POWER_SEQ_STATE_STARTING`
 */
power_seq_status_t power_seq_request_enable(void);

/**
 * @brief Requests that the output rail be disabled.
 * @retval POWER_SEQ_STATUS_OK Disable request accepted.
 * @retval POWER_SEQ_STATUS_INVALID_STATE The current state was not `POWER_SEQ_STATE_ON`.
 * @retval POWER_SEQ_STATUS_NOT_READY The driver has not been initialized.
 * @post The driver enters `POWER_SEQ_STATE_OFF`.
 * @par Transition
 * `POWER_SEQ_STATE_ON` -> `POWER_SEQ_STATE_OFF`
 */
power_seq_status_t power_seq_request_disable(void);

/**
 * @brief Processes one periodic state-machine tick.
 * @param[in] power_good Current sampled state of the power-good input.
 * @param[in] overcurrent `true` when an overcurrent condition is present.
 * @retval POWER_SEQ_STATUS_OK Tick processed successfully.
 * @retval POWER_SEQ_STATUS_NOT_READY The driver has not been initialized.
 * @note This function is intended for a fixed-rate scheduler tick.
 * @par State Behavior
 * - In `POWER_SEQ_STATE_STARTING`, `power_good == true` moves the driver to `POWER_SEQ_STATE_ON`.
 * - In `POWER_SEQ_STATE_STARTING`, timeout moves the driver to `POWER_SEQ_STATE_FAULT`.
 * - In `POWER_SEQ_STATE_ON`, `overcurrent == true` moves the driver to `POWER_SEQ_STATE_FAULT`.
 * - In `POWER_SEQ_STATE_RECOVERING`, expiry of the recovery delay moves the driver to `POWER_SEQ_STATE_OFF`.
 */
power_seq_status_t power_seq_process_tick(bool power_good, bool overcurrent);

/**
 * @brief Requests recovery after a latched fault.
 * @retval POWER_SEQ_STATUS_OK Recovery request accepted.
 * @retval POWER_SEQ_STATUS_INVALID_STATE The current state was not `POWER_SEQ_STATE_FAULT`.
 * @retval POWER_SEQ_STATUS_NOT_READY The driver has not been initialized.
 * @post The driver enters `POWER_SEQ_STATE_RECOVERING`.
 * @par Transition
 * `POWER_SEQ_STATE_FAULT` -> `POWER_SEQ_STATE_RECOVERING`
 */
power_seq_status_t power_seq_request_recover(void);

/**
 * @brief Returns a snapshot of the current state machine state.
 * @param[out] snapshot Pointer that receives the current snapshot.
 * @retval POWER_SEQ_STATUS_OK Snapshot returned successfully.
 * @retval POWER_SEQ_STATUS_INVALID_ARG `snapshot` was `NULL`.
 * @retval POWER_SEQ_STATUS_NOT_READY The driver has not been initialized.
 */
power_seq_status_t power_seq_get_snapshot(power_seq_snapshot_t *snapshot);

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

#endif  // LESSON7_POWER_SEQUENCER_H
