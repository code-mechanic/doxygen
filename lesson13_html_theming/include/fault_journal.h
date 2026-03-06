#ifndef LESSON13_FAULT_JOURNAL_H
#define LESSON13_FAULT_JOURNAL_H

/**
 * @file fault_journal.h
 * @brief Public API for a simple fault-journal sample.
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
 * @defgroup lesson13_fault_journal Fault Journal
 * @brief Example module used to inspect themed Doxygen HTML pages.
 * @{
 */

#define FAULT_JOURNAL_MAX_RECORDS (12U) /**< Maximum number of records stored in the journal. */

// +--------------------------------------------------------------------------+
// |                                                                          |
// |                                  Public                                  |
// |                            Typedefs / Enums                              |
// |                                                                          |
// +--------------------------------------------------------------------------+

/**
 * @brief Status codes returned by the fault-journal module.
 */
typedef enum
{
  FAULT_JOURNAL_STATUS_OK = 0,      /**< Operation completed successfully. */
  FAULT_JOURNAL_STATUS_INVALID_ARG, /**< One or more arguments were invalid. */
  FAULT_JOURNAL_STATUS_NOT_READY,   /**< The journal has not been initialized. */
  FAULT_JOURNAL_STATUS_FULL         /**< The journal is already full. */
} fault_journal_status_t;

/**
 * @brief Fault sources tracked by the journal.
 */
typedef enum
{
  FAULT_SOURCE_POWER = 0, /**< Power-path related fault. */
  FAULT_SOURCE_THERMAL,   /**< Thermal or temperature related fault. */
  FAULT_SOURCE_COMM       /**< Communication related fault. */
} fault_source_t;

// +--------------------------------------------------------------------------+
// |                                                                          |
// |                                  Public                                  |
// |                            Typedefs / Structs                            |
// |                                                                          |
// +--------------------------------------------------------------------------+

/**
 * @brief One stored fault record.
 */
typedef struct
{
  fault_source_t source; /**< Fault source that raised the event. */
  uint16_t code;         /**< Fault code value. */
  uint32_t tick;         /**< Tick count captured when the fault was recorded. */
} fault_record_t;

// +--------------------------------------------------------------------------+
// |                                                                          |
// |                                  Public                                  |
// |                                Functions                                 |
// |                                                                          |
// +--------------------------------------------------------------------------+

/** @name Public Fault Journal API */
/** @{ */

/**
 * @brief Initializes the fault journal.
 * @retval FAULT_JOURNAL_STATUS_OK Initialization completed successfully.
 */
fault_journal_status_t fault_journal_init(void);

/**
 * @brief Appends a new fault record to the journal.
 * @param[in] source Fault source to store.
 * @param[in] code Fault code to store.
 * @retval FAULT_JOURNAL_STATUS_OK Record stored successfully.
 * @retval FAULT_JOURNAL_STATUS_INVALID_ARG `source` was invalid.
 * @retval FAULT_JOURNAL_STATUS_NOT_READY The journal has not been initialized.
 * @retval FAULT_JOURNAL_STATUS_FULL The journal is already full.
 * @see @ref lesson13_theme "HTML Theme Overview"
 */
fault_journal_status_t fault_journal_record(fault_source_t source, uint16_t code);

/**
 * @brief Clears all stored journal records.
 * @retval FAULT_JOURNAL_STATUS_OK Journal cleared successfully.
 * @retval FAULT_JOURNAL_STATUS_NOT_READY The journal has not been initialized.
 */
fault_journal_status_t fault_journal_clear(void);

/**
 * @brief Returns the total number of stored records.
 * @return Number of stored records.
 */
uint16_t fault_journal_get_count(void);

/**
 * @brief Returns whether the journal currently contains any entry for a source.
 * @param[in] source Fault source to query.
 * @retval true The source has at least one stored entry.
 * @retval false The source is absent or invalid.
 */
bool fault_journal_has_source(fault_source_t source);

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

#endif  // LESSON13_FAULT_JOURNAL_H
