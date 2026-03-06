#include "boot_control.h"

void boot_control_example(void)
{
//! [basic_usage]
  boot_image_info_t image =
  {
    0x0102UL,
    0xA5A5A5A5UL,
    16384UL,
    false
  };

  (void)boot_control_init();
  (void)boot_control_request_boot(BOOT_SLOT_B);
  (void)boot_control_confirm_running_image(&image);
//! [basic_usage]
}
