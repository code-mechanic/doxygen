#include "firmware_update.h"

static const uint8_t k_demo_image[] =
{
  0x10U,
  0x20U,
  0x30U,
  0x40U
};

void firmware_update_example(void)
{
//! [basic_usage]
  fw_image_header_t header =
  {
    FW_UPDATE_FORMAT_VERSION,
    0U,
    sizeof(k_demo_image),
    0xFFFFFFFFUL
  };

  (void)fw_update_init();
  (void)fw_update_begin(&header);
  (void)fw_update_write_chunk(k_demo_image, sizeof(k_demo_image));
  (void)fw_update_finalize();
//! [basic_usage]
}
