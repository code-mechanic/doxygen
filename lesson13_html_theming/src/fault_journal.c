/**
 * @file fault_journal.c
 * @brief Implementation of the fault-journal sample.
 */

// +--------------------------------------------------------------------------+
// |                                                                          |
// |                                 Includes                                 |
// |                                                                          |
// +--------------------------------------------------------------------------+

#include "fault_journal.h"

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
 * @brief Internal runtime state for the fault-journal sample.
 */
typedef struct
{
  bool is_initialized; /**< Indicates whether the module has been initialized. */
  uint16_t total_records; /**< Total number of stored records. */
  uint32_t tick; /**< Tick value assigned to the next record. */
} fault_journal_runtime_t;

// +--------------------------------------------------------------------------+
// |                                                                          |
// |                                 Private                                  |
// |                           Function Prototypes                            |
// |                                                                          |
// +--------------------------------------------------------------------------+

/**
 * @brief Returns whether the supplied source is valid.
 * @param[in] source Source to validate.
 * @retval true Source is valid.
 * @retval false Source is invalid.
 */
static bool fault_journal_source_is_valid(fault_source_t source);

/**
 * @brief Increments the per-source count for a stored record.
 * @param[in] source Source that should be incremented.
 */
static void fault_journal_increment_source_count(fault_source_t source);

// +--------------------------------------------------------------------------+
// |                                                                          |
// |                                 Private                                  |
// |                          Variable Declarations                           |
// |                                                                          |
// +--------------------------------------------------------------------------+

/** Global runtime instance used by the fault-journal sample. */
static fault_journal_runtime_t g_fault_journal_runtime =
{
  false,
  0U,
  0UL
};

/** Stored per-source counts indexed by `fault_source_t`. */
static uint16_t g_fault_source_counts[FAULT_SOURCE_COMM + 1] =
{
  0U,
  0U,
  0U
};

// +--------------------------------------------------------------------------+
// |                                                                          |
// |                                  Public                                  |
// |                                Functions                                 |
// |                                                                          |
// +--------------------------------------------------------------------------+

fault_journal_status_t fault_journal_init(void)
{
  uint16_t index = 0U;

  g_fault_journal_runtime.is_initialized = true;
  g_fault_journal_runtime.total_records = 0U;
  g_fault_journal_runtime.tick = 0UL;

  while (index <= FAULT_SOURCE_COMM)
  {
    g_fault_source_counts[index] = 0U;
    index++;
  }

  return FAULT_JOURNAL_STATUS_OK;
}

fault_journal_status_t fault_journal_record(fault_source_t source, uint16_t code)
{
  (void)code;

  if (!fault_journal_source_is_valid(source))
  {
    return FAULT_JOURNAL_STATUS_INVALID_ARG;
  }

  if (!g_fault_journal_runtime.is_initialized)
  {
    return FAULT_JOURNAL_STATUS_NOT_READY;
  }

  if (g_fault_journal_runtime.total_records >= FAULT_JOURNAL_MAX_RECORDS)
  {
    return FAULT_JOURNAL_STATUS_FULL;
  }

  fault_journal_increment_source_count(source);
  g_fault_journal_runtime.total_records++;
  g_fault_journal_runtime.tick++;

  return FAULT_JOURNAL_STATUS_OK;
}

fault_journal_status_t fault_journal_clear(void)
{
  uint16_t index = 0U;

  if (!g_fault_journal_runtime.is_initialized)
  {
    return FAULT_JOURNAL_STATUS_NOT_READY;
  }

  g_fault_journal_runtime.total_records = 0U;

  while (index <= FAULT_SOURCE_COMM)
  {
    g_fault_source_counts[index] = 0U;
    index++;
  }

  return FAULT_JOURNAL_STATUS_OK;
}

uint16_t fault_journal_get_count(void)
{
  return g_fault_journal_runtime.total_records;
}

bool fault_journal_has_source(fault_source_t source)
{
  if (!fault_journal_source_is_valid(source))
  {
    return false;
  }

  return (g_fault_source_counts[source] > 0U);
}

// +--------------------------------------------------------------------------+
// |                                                                          |
// |                                 Private                                  |
// |                                Functions                                 |
// |                                                                          |
// +--------------------------------------------------------------------------+

static bool fault_journal_source_is_valid(fault_source_t source)
{
  return (source >= FAULT_SOURCE_POWER) && (source <= FAULT_SOURCE_COMM);
}

static void fault_journal_increment_source_count(fault_source_t source)
{
  g_fault_source_counts[source]++;
}
