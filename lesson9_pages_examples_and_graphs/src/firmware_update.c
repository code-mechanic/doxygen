/**
 * @file firmware_update.c
 * @brief Implementation of the firmware-update sample.
 */

// +--------------------------------------------------------------------------+
// |                                                                          |
// |                                 Includes                                 |
// |                                                                          |
// +--------------------------------------------------------------------------+

#include "firmware_update.h"

// +--------------------------------------------------------------------------+
// |                                                                          |
// |                                 Private                                  |
// |                          Definitions and Macros                          |
// |                                                                          |
// +--------------------------------------------------------------------------+

/** Initial CRC seed used by the sample CRC32 implementation. */
#define FW_UPDATE_CRC32_INIT (0xFFFFFFFFUL)

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
 * @brief Internal runtime state for the firmware-update sample.
 */
typedef struct
{
  fw_update_state_t state;        /**< Current state of the update state machine. */
  fw_image_header_t active_header; /**< Header captured at the start of the transfer. */
  uint32_t bytes_received;        /**< Total payload bytes accepted so far. */
  uint32_t running_crc32;         /**< Running CRC accumulator over the received payload. */
} fw_update_runtime_t;

// +--------------------------------------------------------------------------+
// |                                                                          |
// |                                 Private                                  |
// |                           Function Prototypes                            |
// |                                                                          |
// +--------------------------------------------------------------------------+

/**
 * @brief Returns whether the supplied image header is acceptable.
 * @param[in] header Header to validate.
 * @retval true Header is valid for this sample.
 * @retval false Header is invalid.
 */
static bool fw_update_header_is_valid(const fw_image_header_t *header);

/**
 * @brief Updates the running CRC with one incoming byte.
 * @param[in] crc Current CRC accumulator value.
 * @param[in] data Next incoming byte.
 * @return Updated CRC accumulator value.
 */
static uint32_t fw_update_crc32_step(uint32_t crc, uint8_t data);

/**
 * @brief Validates the completed image against the expected header values.
 * @retval FW_UPDATE_STATUS_OK Validation completed successfully.
 * @retval FW_UPDATE_STATUS_CRC_ERROR The CRC or image length did not match.
 * @retval FW_UPDATE_STATUS_INVALID_STATE Validation was attempted too early.
 */
static fw_update_status_t fw_update_validate_image(void);

/**
 * @brief Moves the module into the fault state.
 */
static void fw_update_mark_fault(void);

// +--------------------------------------------------------------------------+
// |                                                                          |
// |                                 Private                                  |
// |                          Variable Declarations                           |
// |                                                                          |
// +--------------------------------------------------------------------------+

/** Global runtime instance used by the sample firmware-update state machine. */
static fw_update_runtime_t g_fw_update_runtime =
{
  FW_UPDATE_STATE_IDLE,
  {
    0U,
    0U,
    0UL,
    0UL
  },
  0UL,
  FW_UPDATE_CRC32_INIT
};

// +--------------------------------------------------------------------------+
// |                                                                          |
// |                                  Public                                  |
// |                                Functions                                 |
// |                                                                          |
// +--------------------------------------------------------------------------+

fw_update_status_t fw_update_init(void)
{
  g_fw_update_runtime.state = FW_UPDATE_STATE_IDLE;
  g_fw_update_runtime.active_header.format_version = 0U;
  g_fw_update_runtime.active_header.reserved = 0U;
  g_fw_update_runtime.active_header.image_size_bytes = 0UL;
  g_fw_update_runtime.active_header.expected_crc32 = 0UL;
  g_fw_update_runtime.bytes_received = 0UL;
  g_fw_update_runtime.running_crc32 = FW_UPDATE_CRC32_INIT;

  return FW_UPDATE_STATUS_OK;
}

fw_update_status_t fw_update_begin(const fw_image_header_t *header)
{
  if (!fw_update_header_is_valid(header))
  {
    return FW_UPDATE_STATUS_INVALID_ARG;
  }

  if (g_fw_update_runtime.state != FW_UPDATE_STATE_IDLE)
  {
    return FW_UPDATE_STATUS_INVALID_STATE;
  }

  g_fw_update_runtime.active_header = *header;
  g_fw_update_runtime.bytes_received = 0UL;
  g_fw_update_runtime.running_crc32 = FW_UPDATE_CRC32_INIT;
  g_fw_update_runtime.state = FW_UPDATE_STATE_RECEIVING;

  return FW_UPDATE_STATUS_OK;
}

fw_update_status_t fw_update_write_chunk(const uint8_t *data, uint16_t length)
{
  uint16_t index = 0U;

  if ((data == 0) && (length > 0U))
  {
    return FW_UPDATE_STATUS_INVALID_ARG;
  }

  if (g_fw_update_runtime.state != FW_UPDATE_STATE_RECEIVING)
  {
    return FW_UPDATE_STATUS_INVALID_STATE;
  }

  if ((g_fw_update_runtime.bytes_received + length) > g_fw_update_runtime.active_header.image_size_bytes)
  {
    fw_update_mark_fault();
    return FW_UPDATE_STATUS_STORAGE_ERROR;
  }

  while (index < length)
  {
    g_fw_update_runtime.running_crc32 =
      fw_update_crc32_step(g_fw_update_runtime.running_crc32, data[index]);
    g_fw_update_runtime.bytes_received++;
    index++;
  }

  return FW_UPDATE_STATUS_OK;
}

fw_update_status_t fw_update_finalize(void)
{
  fw_update_status_t status;

  if (g_fw_update_runtime.state != FW_UPDATE_STATE_RECEIVING)
  {
    return FW_UPDATE_STATUS_INVALID_STATE;
  }

  g_fw_update_runtime.state = FW_UPDATE_STATE_VALIDATING;
  status = fw_update_validate_image();

  if (status == FW_UPDATE_STATUS_OK)
  {
    g_fw_update_runtime.state = FW_UPDATE_STATE_READY;
    return status;
  }

  fw_update_mark_fault();
  return status;
}

fw_update_state_t fw_update_get_state(void)
{
  return g_fw_update_runtime.state;
}

// +--------------------------------------------------------------------------+
// |                                                                          |
// |                                 Private                                  |
// |                                Functions                                 |
// |                                                                          |
// +--------------------------------------------------------------------------+

static bool fw_update_header_is_valid(const fw_image_header_t *header)
{
  if (header == 0)
  {
    return false;
  }

  if (header->format_version != FW_UPDATE_FORMAT_VERSION)
  {
    return false;
  }

  if ((header->image_size_bytes == 0UL) || (header->image_size_bytes > FW_UPDATE_MAX_IMAGE_SIZE))
  {
    return false;
  }

  return true;
}

static uint32_t fw_update_crc32_step(uint32_t crc, uint8_t data)
{
  uint8_t bit = 0U;

  crc ^= data;

  while (bit < 8U)
  {
    if ((crc & 1UL) != 0UL)
    {
      crc = (crc >> 1U) ^ 0xEDB88320UL;
    }
    else
    {
      crc >>= 1U;
    }

    bit++;
  }

  return crc;
}

static fw_update_status_t fw_update_validate_image(void)
{
  if (g_fw_update_runtime.state != FW_UPDATE_STATE_VALIDATING)
  {
    return FW_UPDATE_STATUS_INVALID_STATE;
  }

  if (g_fw_update_runtime.bytes_received != g_fw_update_runtime.active_header.image_size_bytes)
  {
    return FW_UPDATE_STATUS_CRC_ERROR;
  }

  if (g_fw_update_runtime.running_crc32 != g_fw_update_runtime.active_header.expected_crc32)
  {
    return FW_UPDATE_STATUS_CRC_ERROR;
  }

  return FW_UPDATE_STATUS_OK;
}

static void fw_update_mark_fault(void)
{
  g_fw_update_runtime.state = FW_UPDATE_STATE_FAULT;
}
