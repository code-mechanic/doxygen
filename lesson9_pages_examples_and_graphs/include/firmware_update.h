#ifndef LESSON9_FIRMWARE_UPDATE_H
#define LESSON9_FIRMWARE_UPDATE_H

/**
 * @file firmware_update.h
 * @brief Public API for a simple firmware-update state machine.
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
 * @defgroup lesson9_fw_update Firmware Update
 * @brief Example module used to explore Doxygen pages, examples, and graphs.
 * @{
 */

#define FW_UPDATE_FORMAT_VERSION (1U) /**< Supported image-header format version. */
#define FW_UPDATE_MAX_IMAGE_SIZE (256U) /**< Maximum image size accepted by the sample. */

// +--------------------------------------------------------------------------+
// |                                                                          |
// |                                  Public                                  |
// |                            Typedefs / Enums                              |
// |                                                                          |
// +--------------------------------------------------------------------------+

/**
 * @brief Status codes returned by the firmware-update module.
 */
typedef enum
{
  FW_UPDATE_STATUS_OK = 0,      /**< Operation completed successfully. */
  FW_UPDATE_STATUS_INVALID_ARG, /**< A pointer or length argument was invalid. */
  FW_UPDATE_STATUS_INVALID_STATE, /**< The requested operation was not valid for the current state. */
  FW_UPDATE_STATUS_CRC_ERROR,   /**< The received image failed validation. */
  FW_UPDATE_STATUS_STORAGE_ERROR /**< The incoming image exceeded the mock storage limit. */
} fw_update_status_t;

/**
 * @brief States used by the firmware-update flow.
 */
typedef enum
{
  FW_UPDATE_STATE_IDLE = 0,  /**< No update is in progress. */
  FW_UPDATE_STATE_RECEIVING, /**< Chunks are being accepted. */
  FW_UPDATE_STATE_VALIDATING, /**< Final validation is running. */
  FW_UPDATE_STATE_READY,     /**< Image validated successfully. */
  FW_UPDATE_STATE_FAULT      /**< Update failed and must be restarted. */
} fw_update_state_t;

// +--------------------------------------------------------------------------+
// |                                                                          |
// |                                  Public                                  |
// |                            Typedefs / Structs                            |
// |                                                                          |
// +--------------------------------------------------------------------------+

/**
 * @brief Fixed header placed in front of an incoming firmware image.
 */
typedef struct
{
  uint16_t format_version;   /**< Header format version expected by the bootloader. */
  uint16_t reserved;         /**< Reserved for alignment or future flags. */
  uint32_t image_size_bytes; /**< Total image size expected during transfer. */
  uint32_t expected_crc32;   /**< Expected CRC value for the completed image. */
} fw_image_header_t;

// +--------------------------------------------------------------------------+
// |                                                                          |
// |                                  Public                                  |
// |                                Functions                                 |
// |                                                                          |
// +--------------------------------------------------------------------------+

/** @name Public Firmware Update API */
/** @{ */

/**
 * @brief Resets the firmware-update module to the idle state.
 * @retval FW_UPDATE_STATUS_OK Module reset completed successfully.
 * @post @ref FW_UPDATE_STATE_IDLE becomes the active state.
 */
fw_update_status_t fw_update_init(void);

/**
 * @brief Starts a new firmware transfer.
 * @param[in] header Pointer to the image header supplied by the sender.
 * @retval FW_UPDATE_STATUS_OK Transfer started successfully.
 * @retval FW_UPDATE_STATUS_INVALID_ARG `header` was `NULL` or invalid.
 * @retval FW_UPDATE_STATUS_INVALID_STATE A transfer was already active.
 * @pre @ref FW_UPDATE_STATE_IDLE must be the current state.
 * @see @ref lesson9_architecture "Architecture and Flow"
 */
fw_update_status_t fw_update_begin(const fw_image_header_t *header);

/**
 * @brief Appends a chunk of firmware data to the current transfer.
 * @param[in] data Pointer to the incoming bytes.
 * @param[in] length Number of bytes in the chunk.
 * @retval FW_UPDATE_STATUS_OK Chunk accepted successfully.
 * @retval FW_UPDATE_STATUS_INVALID_ARG `data` was `NULL` while `length` was non-zero.
 * @retval FW_UPDATE_STATUS_INVALID_STATE No transfer is currently active.
 * @retval FW_UPDATE_STATUS_STORAGE_ERROR The chunk would exceed the configured storage limit.
 * @pre @ref FW_UPDATE_STATE_RECEIVING must be the current state.
 * @note See the example snippet on @ref lesson9_architecture "Architecture and Flow".
 */
fw_update_status_t fw_update_write_chunk(const uint8_t *data, uint16_t length);

/**
 * @brief Finalizes the current transfer and validates the received image.
 * @retval FW_UPDATE_STATUS_OK Image validation completed successfully.
 * @retval FW_UPDATE_STATUS_INVALID_STATE No transfer is currently active.
 * @retval FW_UPDATE_STATUS_CRC_ERROR The final CRC did not match the expected value.
 * @post The state becomes @ref FW_UPDATE_STATE_READY on success or
 * @ref FW_UPDATE_STATE_FAULT on failure.
 * @see @ref lesson9_architecture "Architecture and Flow"
 */
fw_update_status_t fw_update_finalize(void);

/**
 * @brief Returns the current update state.
 * @return Current state of the firmware-update module.
 */
fw_update_state_t fw_update_get_state(void);

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

#endif  // LESSON9_FIRMWARE_UPDATE_H
