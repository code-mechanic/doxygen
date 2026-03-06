#include "watchdog_recovery.h"

void watchdog_recovery_example(void)
{
//! [basic_usage]
  (void)watchdog_recovery_init();
  (void)watchdog_recovery_arm();
  (void)watchdog_recovery_handle_reset(WATCHDOG_RESET_REASON_TIMEOUT);
//! [basic_usage]
}
