#ifndef LESSON6_CAPTURE_INPUT_H
#define LESSON6_CAPTURE_INPUT_H

/**
 * @file capture_input.h
 * @brief Public API for a simple capture-input driver.
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
 * @defgroup lesson6_capture Capture Input
 * @brief Example driver showing how to document callbacks and ISR-facing APIs.
 * @{
 */

// +--------------------------------------------------------------------------+
// |                                                                          |
// |                                  Public                                  |
// |                            Typedefs / Enums                              |
// |                                                                          |
// +--------------------------------------------------------------------------+

/**
 * @brief Status codes returned by the capture-input driver.
 */
typedef enum
{
  CAPTURE_INPUT_STATUS_OK = 0,      /**< Operation completed successfully. */
  CAPTURE_INPUT_STATUS_INVALID_ARG, /**< One or more arguments were invalid. */
  CAPTURE_INPUT_STATUS_NOT_READY    /**< Driver was used before initialization or IRQ enable. */
} capture_input_status_t;

// +--------------------------------------------------------------------------+
// |                                                                          |
// |                                  Public                                  |
// |                            Typedefs / Structs                            |
// |                                                                          |
// +--------------------------------------------------------------------------+

/**
 * @brief Configuration used when the capture-input driver is initialized.
 */
typedef struct
{
  uint8_t channel;          /**< Hardware capture channel used by the driver. */
  bool report_overflow;     /**< Indicates whether overflow events should be reported. */
} capture_input_config_t;

/**
 * @brief Event data delivered to the application callback.
 */
typedef struct
{
  uint8_t channel;          /**< Capture channel that produced the event. */
  uint32_t timestamp_ticks; /**< Captured timer value in hardware ticks. */
  bool overflowed;          /**< Indicates whether the event was associated with an overflow. */
} capture_input_event_t;

// +--------------------------------------------------------------------------+
// |                                                                          |
// |                                  Public                                  |
// |                             Callback Typedefs                            |
// |                                                                          |
// +--------------------------------------------------------------------------+

/**
 * @brief Application callback invoked when a capture event is latched.
 * @param[in] event Pointer to event data valid only for the duration of the call.
 * @param[in] context User pointer supplied during callback registration.
 * @warning This callback executes in interrupt context.
 * @par Context
 * Interrupt context only.
 * @par Concurrency
 * The callback may preempt thread-level code. Shared data should be protected
 * with a critical section or transferred through lock-free primitives.
 * @note The callback may be `NULL` when the application wants polling only.
 */
typedef void (*capture_input_callback_t)(const capture_input_event_t *event, void *context);

// +--------------------------------------------------------------------------+
// |                                                                          |
// |                                  Public                                  |
// |                                Functions                                 |
// |                                                                          |
// +--------------------------------------------------------------------------+

/** @name Public Driver API */
/** @{ */

/**
 * @brief Initializes the capture-input driver.
 * @param[in] config Pointer to the configuration to apply.
 * @retval CAPTURE_INPUT_STATUS_OK Driver initialized successfully.
 * @retval CAPTURE_INPUT_STATUS_INVALID_ARG `config` was `NULL` or the channel was invalid.
 * @post The driver is ready for callback registration and IRQ enable.
 */
capture_input_status_t capture_input_init(const capture_input_config_t *config);

/**
 * @brief Registers or unregisters the application callback.
 * @param[in] callback Callback to invoke from interrupt context, or `NULL` to unregister.
 * @param[in] context User pointer passed back to the callback.
 * @retval CAPTURE_INPUT_STATUS_OK Callback configuration updated successfully.
 * @retval CAPTURE_INPUT_STATUS_NOT_READY The driver has not been initialized.
 * @note This function is intended for thread context before interrupts are enabled.
 */
capture_input_status_t capture_input_register_callback(capture_input_callback_t callback,
                                                       void *context);

/**
 * @brief Enables capture interrupts.
 * @retval CAPTURE_INPUT_STATUS_OK Interrupt delivery enabled successfully.
 * @retval CAPTURE_INPUT_STATUS_NOT_READY The driver has not been initialized.
 * @post `capture_input_service_irq()` may now be called by the IRQ handler.
 */
capture_input_status_t capture_input_enable_irq(void);

/**
 * @brief Disables capture interrupts.
 * @retval CAPTURE_INPUT_STATUS_OK Interrupt delivery disabled successfully.
 * @retval CAPTURE_INPUT_STATUS_NOT_READY The driver has not been initialized.
 */
capture_input_status_t capture_input_disable_irq(void);

/**
 * @brief Services one capture interrupt event.
 * @param[in] captured_ticks Captured hardware timestamp in timer ticks.
 * @param[in] overflowed `true` when the event was associated with an overflow condition.
 * @retval CAPTURE_INPUT_STATUS_OK Event processed successfully.
 * @retval CAPTURE_INPUT_STATUS_NOT_READY The driver has not been initialized or IRQ is disabled.
 * @warning This function is intended to be called from the real IRQ handler only.
 * @par Context
 * Interrupt context only.
 * @par Concurrency
 * This function updates state that may also be observed by thread-level getter
 * functions. Those getters can see a changing value unless the application uses
 * a critical section.
 */
capture_input_status_t capture_input_service_irq(uint32_t captured_ticks, bool overflowed);

/**
 * @brief Returns the number of events seen by the driver.
 * @param[out] event_count Pointer that receives the current event count.
 * @retval CAPTURE_INPUT_STATUS_OK Event count returned successfully.
 * @retval CAPTURE_INPUT_STATUS_INVALID_ARG `event_count` was `NULL`.
 * @retval CAPTURE_INPUT_STATUS_NOT_READY The driver has not been initialized.
 * @par Concurrency
 * The returned value may change concurrently if `capture_input_service_irq()` is
 * running on another context.
 */
capture_input_status_t capture_input_get_event_count(uint16_t *event_count);

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

#endif  // LESSON6_CAPTURE_INPUT_H
