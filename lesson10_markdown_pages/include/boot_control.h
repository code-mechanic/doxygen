#ifndef LESSON10_BOOT_CONTROL_H
#define LESSON10_BOOT_CONTROL_H

/**
 * @file boot_control.h
 * @brief Public API for a simple dual-slot boot-control sample.
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
 * @defgroup lesson10_boot_control Boot Control
 * @brief Example module used to explore Markdown-based Doxygen pages.
 * @{
 */

#define BOOT_CONTROL_MAX_RETRY_COUNT (3U) /**< Maximum allowed failed boots before rollback logic triggers. */

// +--------------------------------------------------------------------------+
// |                                                                          |
// |                                  Public                                  |
// |                            Typedefs / Enums                              |
// |                                                                          |
// +--------------------------------------------------------------------------+

/**
 * @brief Status codes returned by the boot-control module.
 */
typedef enum
{
  BOOT_CONTROL_STATUS_OK = 0,      /**< Operation completed successfully. */
  BOOT_CONTROL_STATUS_INVALID_ARG, /**< One or more arguments were invalid. */
  BOOT_CONTROL_STATUS_INVALID_STATE, /**< The request is not valid in the current state. */
  BOOT_CONTROL_STATUS_VERIFY_FAILED /**< The image could not be confirmed. */
} boot_control_status_t;

/**
 * @brief Boot slots managed by the sample module.
 */
typedef enum
{
  BOOT_SLOT_A = 0, /**< Primary application slot. */
  BOOT_SLOT_B      /**< Alternate application slot. */
} boot_slot_t;

/**
 * @brief States used by the boot-control flow.
 */
typedef enum
{
  BOOT_CONTROL_STATE_IDLE = 0, /**< No pending slot switch exists. */
  BOOT_CONTROL_STATE_PENDING,  /**< A slot switch is pending confirmation. */
  BOOT_CONTROL_STATE_CONFIRMED, /**< The new slot has been confirmed. */
  BOOT_CONTROL_STATE_FAULT     /**< Confirmation failed and rollback is required. */
} boot_control_state_t;

// +--------------------------------------------------------------------------+
// |                                                                          |
// |                                  Public                                  |
// |                            Typedefs / Structs                            |
// |                                                                          |
// +--------------------------------------------------------------------------+

/**
 * @brief Metadata describing the image that just booted.
 */
typedef struct
{
  uint32_t version;          /**< Application version number. */
  uint32_t image_crc32;      /**< CRC32 value measured by the bootloader or application. */
  uint32_t image_size_bytes; /**< Size of the application image in bytes. */
  bool is_confirmed;         /**< Set to `true` once the image is accepted as valid. */
} boot_image_info_t;

// +--------------------------------------------------------------------------+
// |                                                                          |
// |                                  Public                                  |
// |                                Functions                                 |
// |                                                                          |
// +--------------------------------------------------------------------------+

/** @name Public Boot Control API */
/** @{ */

/**
 * @brief Resets the boot-control module to a known baseline.
 * @retval BOOT_CONTROL_STATUS_OK Reset completed successfully.
 * @post @ref BOOT_CONTROL_STATE_IDLE becomes the active state.
 */
boot_control_status_t boot_control_init(void);

/**
 * @brief Requests that the next boot attempt use the selected slot.
 * @param[in] slot Slot to mark as pending.
 * @retval BOOT_CONTROL_STATUS_OK Slot request accepted successfully.
 * @retval BOOT_CONTROL_STATUS_INVALID_ARG `slot` was invalid.
 * @retval BOOT_CONTROL_STATUS_INVALID_STATE A slot switch is already pending.
 * @see @ref lesson10_overview "Boot Control Overview"
 */
boot_control_status_t boot_control_request_boot(boot_slot_t slot);

/**
 * @brief Confirms that the currently running image is valid.
 * @param[in] image Metadata describing the image that just booted.
 * @retval BOOT_CONTROL_STATUS_OK Image confirmation completed successfully.
 * @retval BOOT_CONTROL_STATUS_INVALID_ARG `image` was `NULL` or incomplete.
 * @retval BOOT_CONTROL_STATUS_INVALID_STATE No slot switch is pending.
 * @retval BOOT_CONTROL_STATUS_VERIFY_FAILED The supplied image metadata failed validation.
 * @pre @ref BOOT_CONTROL_STATE_PENDING must be the current state.
 * @note See the example snippet on @ref lesson10_overview "Boot Control Overview".
 */
boot_control_status_t boot_control_confirm_running_image(const boot_image_info_t *image);

/**
 * @brief Handles a failed boot and rolls control back to the last known-good slot.
 * @retval BOOT_CONTROL_STATUS_OK Rollback handling completed successfully.
 * @retval BOOT_CONTROL_STATUS_INVALID_STATE No pending slot switch exists.
 * @post The state becomes @ref BOOT_CONTROL_STATE_FAULT.
 * @see @ref lesson10_integration "Integration Notes"
 */
boot_control_status_t boot_control_handle_failed_boot(void);

/**
 * @brief Returns the currently active slot.
 * @return Current active slot.
 */
boot_slot_t boot_control_get_active_slot(void);

/**
 * @brief Returns the current boot-control state.
 * @return Current state of the boot-control module.
 */
boot_control_state_t boot_control_get_state(void);

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

#endif  // LESSON10_BOOT_CONTROL_H
