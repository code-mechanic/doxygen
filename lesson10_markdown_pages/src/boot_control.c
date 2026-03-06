/**
 * @file boot_control.c
 * @brief Implementation of the boot-control sample.
 */

// +--------------------------------------------------------------------------+
// |                                                                          |
// |                                 Includes                                 |
// |                                                                          |
// +--------------------------------------------------------------------------+

#include "boot_control.h"

// +--------------------------------------------------------------------------+
// |                                                                          |
// |                                 Private                                  |
// |                          Definitions and Macros                          |
// |                                                                          |
// +--------------------------------------------------------------------------+

/** Value used to represent an invalid CRC in the sample metadata checks. */
#define BOOT_CONTROL_INVALID_CRC32 (0UL)

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
 * @brief Internal runtime state for the boot-control sample.
 */
typedef struct
{
  boot_slot_t active_slot;         /**< Slot currently considered bootable and active. */
  boot_slot_t pending_slot;        /**< Slot waiting to be confirmed. */
  boot_control_state_t state;      /**< Current boot-control state. */
  uint8_t retry_count;             /**< Remaining retries before rollback logic is applied. */
  boot_image_info_t last_image;    /**< Most recently confirmed image metadata. */
} boot_control_runtime_t;

// +--------------------------------------------------------------------------+
// |                                                                          |
// |                                 Private                                  |
// |                           Function Prototypes                            |
// |                                                                          |
// +--------------------------------------------------------------------------+

/**
 * @brief Returns whether the supplied slot identifier is valid.
 * @param[in] slot Slot to check.
 * @retval true Slot is valid.
 * @retval false Slot is invalid.
 */
static bool boot_control_slot_is_valid(boot_slot_t slot);

/**
 * @brief Returns whether the supplied image metadata is complete enough to confirm.
 * @param[in] image Image metadata to validate.
 * @retval true Metadata is acceptable.
 * @retval false Metadata is not acceptable.
 */
static bool boot_control_image_is_valid(const boot_image_info_t *image);

/**
 * @brief Commits the pending slot as the active slot.
 * @param[in] image Metadata to store as the confirmed image.
 */
static void boot_control_commit_pending_slot(const boot_image_info_t *image);

/**
 * @brief Restores the last known-good slot after a failed boot attempt.
 */
static void boot_control_fallback_to_safe_slot(void);

// +--------------------------------------------------------------------------+
// |                                                                          |
// |                                 Private                                  |
// |                          Variable Declarations                           |
// |                                                                          |
// +--------------------------------------------------------------------------+

/** Global runtime instance used by the boot-control sample. */
static boot_control_runtime_t g_boot_control_runtime =
{
  BOOT_SLOT_A,
  BOOT_SLOT_A,
  BOOT_CONTROL_STATE_IDLE,
  0U,
  {
    0UL,
    0UL,
    0UL,
    false
  }
};

// +--------------------------------------------------------------------------+
// |                                                                          |
// |                                  Public                                  |
// |                                Functions                                 |
// |                                                                          |
// +--------------------------------------------------------------------------+

boot_control_status_t boot_control_init(void)
{
  g_boot_control_runtime.active_slot = BOOT_SLOT_A;
  g_boot_control_runtime.pending_slot = BOOT_SLOT_A;
  g_boot_control_runtime.state = BOOT_CONTROL_STATE_IDLE;
  g_boot_control_runtime.retry_count = 0U;
  g_boot_control_runtime.last_image.version = 0UL;
  g_boot_control_runtime.last_image.image_crc32 = 0UL;
  g_boot_control_runtime.last_image.image_size_bytes = 0UL;
  g_boot_control_runtime.last_image.is_confirmed = false;

  return BOOT_CONTROL_STATUS_OK;
}

boot_control_status_t boot_control_request_boot(boot_slot_t slot)
{
  if (!boot_control_slot_is_valid(slot))
  {
    return BOOT_CONTROL_STATUS_INVALID_ARG;
  }

  if (g_boot_control_runtime.state == BOOT_CONTROL_STATE_PENDING)
  {
    return BOOT_CONTROL_STATUS_INVALID_STATE;
  }

  g_boot_control_runtime.pending_slot = slot;
  g_boot_control_runtime.retry_count = BOOT_CONTROL_MAX_RETRY_COUNT;
  g_boot_control_runtime.state = BOOT_CONTROL_STATE_PENDING;

  return BOOT_CONTROL_STATUS_OK;
}

boot_control_status_t boot_control_confirm_running_image(const boot_image_info_t *image)
{
  if (g_boot_control_runtime.state != BOOT_CONTROL_STATE_PENDING)
  {
    return BOOT_CONTROL_STATUS_INVALID_STATE;
  }

  if (!boot_control_image_is_valid(image))
  {
    boot_control_fallback_to_safe_slot();
    return BOOT_CONTROL_STATUS_VERIFY_FAILED;
  }

  boot_control_commit_pending_slot(image);
  return BOOT_CONTROL_STATUS_OK;
}

boot_control_status_t boot_control_handle_failed_boot(void)
{
  if (g_boot_control_runtime.state != BOOT_CONTROL_STATE_PENDING)
  {
    return BOOT_CONTROL_STATUS_INVALID_STATE;
  }

  if (g_boot_control_runtime.retry_count > 0U)
  {
    g_boot_control_runtime.retry_count--;
  }

  boot_control_fallback_to_safe_slot();
  return BOOT_CONTROL_STATUS_OK;
}

boot_slot_t boot_control_get_active_slot(void)
{
  return g_boot_control_runtime.active_slot;
}

boot_control_state_t boot_control_get_state(void)
{
  return g_boot_control_runtime.state;
}

// +--------------------------------------------------------------------------+
// |                                                                          |
// |                                 Private                                  |
// |                                Functions                                 |
// |                                                                          |
// +--------------------------------------------------------------------------+

static bool boot_control_slot_is_valid(boot_slot_t slot)
{
  return (slot == BOOT_SLOT_A) || (slot == BOOT_SLOT_B);
}

static bool boot_control_image_is_valid(const boot_image_info_t *image)
{
  if (image == 0)
  {
    return false;
  }

  if (image->version == 0UL)
  {
    return false;
  }

  if (image->image_crc32 == BOOT_CONTROL_INVALID_CRC32)
  {
    return false;
  }

  if (image->image_size_bytes == 0UL)
  {
    return false;
  }

  return true;
}

static void boot_control_commit_pending_slot(const boot_image_info_t *image)
{
  g_boot_control_runtime.active_slot = g_boot_control_runtime.pending_slot;
  g_boot_control_runtime.state = BOOT_CONTROL_STATE_CONFIRMED;
  g_boot_control_runtime.retry_count = 0U;
  g_boot_control_runtime.last_image = *image;
  g_boot_control_runtime.last_image.is_confirmed = true;
}

static void boot_control_fallback_to_safe_slot(void)
{
  g_boot_control_runtime.pending_slot = g_boot_control_runtime.active_slot;
  g_boot_control_runtime.state = BOOT_CONTROL_STATE_FAULT;
}
