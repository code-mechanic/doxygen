#include "diag_log.h"

void diag_log_example(void)
{
//! [basic_usage]
  (void)diag_log_init();
  (void)diag_log_push(DIAG_CHANNEL_POWER, 0x0101U);
  (void)diag_log_push(DIAG_CHANNEL_COMM, 0x0202U);
//! [basic_usage]
}
