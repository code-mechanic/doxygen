#include "fault_journal.h"

void fault_journal_example(void)
{
//! [basic_usage]
  (void)fault_journal_init();
  (void)fault_journal_record(FAULT_SOURCE_POWER, 0x1001U);
  (void)fault_journal_record(FAULT_SOURCE_THERMAL, 0x2002U);
//! [basic_usage]
}
